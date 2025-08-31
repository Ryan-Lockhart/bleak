#pragma once

#include <bleak/typedef.hpp>

#include <expected>
#include <optional>
#include <queue>

#include <bleak/concepts.hpp>
#include <bleak/extent.hpp>
#include <bleak/octant.hpp>
#include <bleak/offset.hpp>
#include <bleak/sparse.hpp>
#include <bleak/random.hpp>
#include <bleak/zone.hpp>

namespace bleak {
	enum struct marker_e : u8 {
		Obstacle,
		Goal
	};

	template<Numeric D, distance_function_e DistanceFunction, extent_t ZoneSize, extent_t ZoneBorder> struct field_t {
	  private:
		template<typename T> using zone_t = zone_t<T, ZoneSize, ZoneBorder>;

		zone_t<D> distances;
		sparse_t<D> goals;

		using return_t = std::expected<D, marker_e>;
		using error_t = return_t::unexpected_type;

	  public:
		static constexpr D goal_value{ 0 };
		static constexpr D obstacle_value{ ZoneSize.area() };

		static constexpr D close_to_goal_value{ goal_value + 1 };
		static constexpr D close_to_obstacle_value{ obstacle_value - 1 };

		constexpr bool is_goal(D distance) const noexcept { return distance == goal_value; }
		
		constexpr bool is_obstacle(D distance) const noexcept { return distance == obstacle_value; }

		constexpr bool is_close_to_goal(D distance) const noexcept { return distance == close_to_goal_value; };

		constexpr bool is_close_to_obstacle(D distance) const noexcept { return distance == close_to_obstacle_value; };

		constexpr bool goal_reached(offset_t position) const noexcept { return distances[position] == goal_value; }

		constexpr bool goal_reached(offset_t position, D threshold) const noexcept { return distances[position] <= threshold; }

		constexpr bool obstacle_reached(offset_t position) const noexcept { return distances[position] >= close_to_obstacle_value; }

		constexpr bool obstacle_reached(offset_t position, D threshold) const noexcept { return distances[position] >= close_to_obstacle_value - threshold; }

		constexpr field_t() noexcept : distances{}, goals{} { clear<region_e::All>(); }

		template<typename... Goals>
			requires is_homogeneous<D, Goals...>::value
		constexpr field_t(cref<Goals>... goals) noexcept : distances{}, goals{ goals... } {
			clear<region_e::All>();
		}

		template<typename T, typename... Goals>
			requires is_homogeneous<D, Goals...>::value
		constexpr field_t(cref<zone_t<T>> zone, cref<T> value, cref<Goals>... goals) noexcept : distances{}, goals{ goals... } {
			recalculate<region_e::All>(zone, value);
		}

		template<typename... Goals>
			requires is_homogeneous<D, Goals...>::value
		constexpr field_t(rval<Goals>... goals) noexcept : distances{}, goals{ (std::move(goals), ...) } {
			clear<region_e::All>();
		}

		template<region_e Region, typename T, typename... Goals>
			requires is_homogeneous<D, Goals...>::value
		constexpr field_t(cref<zone_t<T>> zone, cref<T> value, rval<Goals>... goals) noexcept : distances{}, goals{ (std::move(goals), ...) } {
			recalculate<region_e::All>(zone, value);
		}

		template<region_e Region> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> clear() noexcept {
			distances.dependent set<Region>(obstacle_value);

			if constexpr (Region == region_e::All) {
				goals.clear();
			} else {
				for (cauto [g_pos, _] : goals) {
					if (distances.dependent within<Region>(g_pos)) {
						goals.remove(g_pos);
					}
				}
			}

			return *this;
		}

		constexpr D operator[](offset_t position) const noexcept {
			if (!distances.dependent within<region_e::All>(position)) {
				return obstacle_value;
			}

			return distances[position];
		}

		template<region_e Region> constexpr return_t at(offset_t position) const noexcept {
			if (!distances.dependent within<Region>(position)) {
				return error_t{ marker_e::Obstacle };
			}

			const D distance{ distances[position] };

			if (is_goal(distance)) {
				return error_t{ marker_e::Goal };
			} else if (is_obstacle(distance)) {
				return error_t{ marker_e::Obstacle };
			} else {
				return return_t{ distance };
			}
		}

		constexpr return_t at(offset_t position) const noexcept { return at<region_e::All>(position); }

		template<region_e Region, distance_function_e Distance> constexpr D average(offset_t position) const noexcept {
			if (!distances.dependent within<Region>(position)) {
				return obstacle_value;
			}

			const D source_distance{ distances[position] };

			usize count{ source_distance != obstacle_value };
			D distance{ source_distance != obstacle_value ? source_distance : D{ 0 } };

			bool all_obstacles{ true };

			for (cauto offset : neighbourhood_offsets<Distance>) {
				cauto current_position{ position + offset };
				
				if (!distances.dependent within<Region>(current_position)) {
					continue;
				}

				cauto current_distance{ distances[current_position] };

				if (current_distance == obstacle_value) {
					continue;
				}

				all_obstacles = false;

				distance += current_distance;
				++count;
			}

			if (all_obstacles) {
				return obstacle_value;
			} else if (count > D{ 0 }) {
				distance /= count;
			}

			return distance;
		}

		template<region_e Region> constexpr D average(offset_t position) const noexcept { return average<Region, DistanceFunction>(position); }

		constexpr D average(offset_t position) const noexcept { return average<region_e::All>(position); }

		constexpr void homogenize() noexcept {
			for (usize i{ 0 }; i < ZoneSize.area(); ++i) {
				const D distance{ distances[i] };

				if (distance >= 1) {
					continue;
				}

				distances[i] = (distance < 1 && distance > -1) ? 0 : std::abs(distance);
			}
		}

		template<region_e Region, typename T> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T>> zone, cref<T> value) noexcept {
			distances.dependent set<Region>(obstacle_value);

			if (goals.empty()) {
				return *this;
			}

			std::priority_queue<creeper_t<D>, std::vector<creeper_t<D>>, typename creeper_t<D>::less> frontier{};
			gtl::flat_hash_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto [g_pos, g_val] : goals) {
				if (!zone.dependent within<Region>(g_pos) || zone[g_pos] != value) {
					continue;
				}

				frontier.emplace(g_pos, g_val);

				if (g_val < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.top() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (cauto creeper : neighbourhood_creepers<DistanceFunction, D>) {
					const offset_t offset_position{ current.position + creeper.position };

					if (visited.contains(offset_position) || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value) {
						if (!visited.contains(offset_position)) {
							visited.insert(offset_position);
						}

						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + creeper.distance });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<region_e Region, typename T, typename U>
			requires is_equatable<T, U>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T>> zone, cref<U> value) noexcept {
			distances.dependent set<Region>(obstacle_value);

			if (goals.empty()) {
				return *this;
			}

			std::priority_queue<creeper_t<D>, std::vector<creeper_t<D>>, typename creeper_t<D>::less> frontier{};
			gtl::flat_hash_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto [g_pos, g_val] : goals) {
				if (!zone.dependent within<Region>(g_pos) || zone[g_pos] != value) {
					continue;
				}

				frontier.emplace(g_pos, g_val);

				if (g_val < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.top() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (crauto creeper : neighbourhood_creepers<DistanceFunction, D>) {
					cauto offset_position{ current.position + creeper.position };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + creeper.distance });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<region_e Region, typename T, SparseBlockage Blockage> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T>> zone, cref<T> value, cref<Blockage> blockage) noexcept {
			distances.dependent set<Region>(obstacle_value);

			if (goals.empty()) {
				return *this;
			}

			std::priority_queue<creeper_t<D>, std::vector<creeper_t<D>>, typename creeper_t<D>::less> frontier{};
			gtl::flat_hash_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto [g_pos, g_val] : goals) {
				if (!zone.dependent within<Region>(g_pos) || zone[g_pos] != value) {
					continue;
				}

				frontier.emplace(g_pos, g_val);

				if (g_val < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.top() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (crauto creeper : neighbourhood_creepers<DistanceFunction, D>) {
					cauto offset_position{ current.position + creeper.position };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value || blockage.contains(offset_position)) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + creeper.distance });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<region_e Region, typename T, typename U, SparseBlockage Blockage>
			requires is_equatable<T, U>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T>> zone, cref<U> value, cref<Blockage> sparse_blockage) noexcept {
			distances.dependent set<Region>(obstacle_value);

			if (goals.empty()) {
				return *this;
			}

			std::priority_queue<creeper_t<D>, std::vector<creeper_t<D>>, typename creeper_t<D>::less> frontier{};
			gtl::flat_hash_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto [g_pos, g_val] : goals) {
				if (!zone.dependent within<Region>(g_pos) || zone[g_pos] != value) {
					continue;
				}

				frontier.emplace(g_pos, g_val);

				if (g_val < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.top() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (crauto creeper : neighbourhood_creepers<DistanceFunction, D>) {
					cauto offset_position{ current.position + creeper.position };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value || sparse_blockage.contains(offset_position)) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + creeper.distance });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<region_e Region, typename T, SparseBlockage... Blockages>
			requires is_plurary<Blockages...>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T>> zone, cref<T> value, cref<Blockages>... blockages) noexcept {
			distances.dependent set<Region>(obstacle_value);

			if (goals.empty()) {
				return *this;
			}

			std::priority_queue<creeper_t<D>, std::vector<creeper_t<D>>, typename creeper_t<D>::less> frontier{};
			gtl::flat_hash_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto [g_pos, g_val] : goals) {
				if (!zone.dependent within<Region>(g_pos) || zone[g_pos] != value) {
					continue;
				}

				frontier.emplace(g_pos, g_val);

				if (g_val < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.top() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (crauto creeper : neighbourhood_creepers<DistanceFunction, D>) {
					cauto offset_position{ current.position + creeper.position };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value || (blockages.contains(offset_position) || ...)) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + creeper.distance });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<region_e Region, typename T, typename U, SparseBlockage... Blockages>
			requires is_plurary<Blockages...>::value && is_equatable<T, U>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T>> zone, cref<U> value, cref<Blockages>... blockages) noexcept {
			distances.dependent set<Region>(obstacle_value);

			if (goals.empty()) {
				return *this;
			}

			std::priority_queue<creeper_t<D>, std::vector<creeper_t<D>>, typename creeper_t<D>::less> frontier{};
			gtl::flat_hash_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto [g_pos, g_val] : goals) {
				if (!zone.dependent within<Region>(g_pos) || zone[g_pos] != value) {
					continue;
				}

				frontier.emplace(g_pos, g_val);

				if (g_val < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.top() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (crauto creeper : neighbourhood_creepers<DistanceFunction, D>) {
					cauto offset_position{ current.position + creeper.position };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value || (blockages.contains(offset_position) || ...)) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + creeper.distance });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<region_e Region> constexpr std::optional<offset_t> ascend(offset_t position) const noexcept {
			if (!distances.dependent within<Region>(position)) {
				return std::nullopt;
			}

			offset_t highest{ position };
			D highest_distance{ goal_value };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				const D offset_distance{ distances[offset_position] };

				if (!distances.dependent within<Region>(offset_position) || offset_distance == obstacle_value || offset_distance <= highest_distance) {
					continue;
				}

				highest = offset_position;
				highest_distance = offset_distance;
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<region_e Region, SparseBlockage Blockage> constexpr std::optional<offset_t> ascend(offset_t position, cref<Blockage> sparse_blockage) const noexcept {
			if (!distances.dependent within<Region>(position)) {
				return std::nullopt;
			}

			offset_t highest{ position };
			D highest_distance{ goal_value };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				const D offset_distance{ distances[offset_position] };

				if (!distances.dependent within<Region>(offset_position) || offset_distance == obstacle_value || offset_distance <= highest_distance || sparse_blockage.contains(offset_position)) {
					continue;
				}

				highest = offset_position;
				highest_distance = offset_distance;
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<region_e Region, RandomEngine Generator> constexpr std::optional<offset_t> ascend(offset_t position, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.dependent within<Region>(position)) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t highest{ position };
			D highest_distance{ goal_value };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				const D offset_distance{ distances[offset_position] };

				if (!distances.dependent within<Region>(offset_position) || offset_distance == obstacle_value || offset_distance < highest_distance || (offset_distance == highest_distance && !distribution(generator))) {
					continue;
				}

				highest = offset_position;
				highest_distance = offset_distance;
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<region_e Region, RandomEngine Generator, SparseBlockage Blockage> constexpr std::optional<offset_t> ascend(offset_t position, cref<Blockage> sparse_blockage, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.dependent within<Region>(position)) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t highest{ position };
			D highest_distance{ goal_value };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				const D offset_distance{ distances[offset_position] };

				if (!distances.dependent within<Region>(offset_position) || offset_distance == obstacle_value || offset_distance < highest_distance || sparse_blockage.contains(offset_position) || (offset_distance == highest_distance && !distribution(generator))) {
					continue;
				}

				highest = offset_position;
				highest_distance = offset_distance;
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<region_e Region> constexpr std::optional<offset_t> descend(offset_t position) const noexcept {
			if (!distances.dependent within<Region>(position) || goal_reached(position)) {
				return std::nullopt;
			}

			offset_t lowest{ position };
			D lowest_distance{ obstacle_value };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				const D offset_distance{ distances[offset_position] };

				if (!distances.dependent within<Region>(offset_position) || offset_distance >= lowest_distance) {
					continue;
				}

				lowest = offset_position;
				lowest_distance = offset_distance;
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<region_e Region, SparseBlockage Blockage> constexpr std::optional<offset_t> descend(offset_t position, cref<Blockage> sparse_blockage) const noexcept {
			if (!distances.dependent within<Region>(position) || goal_reached(position)) {
				return std::nullopt;
			}

			offset_t lowest{ position };
			D lowest_distance{ obstacle_value };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				const D offset_distance{ distances[offset_position] };

				if (!distances.dependent within<Region>(offset_position) || offset_distance >= lowest_distance || sparse_blockage.contains(offset_position)) {
					continue;
				}

				lowest = offset_position;
				lowest_distance = offset_distance;
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<region_e Region, RandomEngine Generator> constexpr std::optional<offset_t> descend(offset_t position, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.dependent within<Region>(position) || goal_reached(position)) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t lowest{ position };
			D lowest_distance{ obstacle_value };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				const D offset_distance{ distances[offset_position] };

				if (!distances.dependent within<Region>(offset_position) || offset_distance > lowest_distance || (offset_distance == lowest_distance && !distribution(generator))) {
					continue;
				}

				lowest = offset_position;
				lowest_distance = offset_distance;
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<region_e Region, RandomEngine Generator, SparseBlockage Blockage> constexpr std::optional<offset_t> descend(offset_t position, cref<Blockage> sparse_blockage, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.dependent within<Region>(position) || goal_reached(position)) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t lowest{ position };
			D lowest_distance{ obstacle_value };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				const D offset_distance{ distances[offset_position] };
				
				if (!distances.dependent within<Region>(offset_position) || offset_distance > lowest_distance || sparse_blockage.contains(offset_position) || (offset_distance == lowest_distance && !distribution(generator))) {
					continue;
				}

				lowest = offset_position;
				lowest_distance = offset_distance;
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<region_e Region, typename Randomizer, typename T, SparseBlockage Blockage>
		constexpr std::optional<offset_t> find_random(cref<zone_t<T>> zone, ref<Randomizer> generator, cref<T> value, cref<Blockage> sparse_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == region_e::None) {
				return *this;
			}

			if constexpr (Region == region_e::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == region_e::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == region_e::Border) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.border_size.h * 2 - 1 };

				for (extent_t::product_t i{ 0 }; i < zone.border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < zone.border_size.w ? x : zone.zone_extent.x - x;
					y = y < zone.border_size.h ? y : zone.zone_extent.y - y;

					const offset_t pos{ x, y };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			}

			return std::nullopt;
		}

		template<region_e Region, typename Randomizer, typename T, typename U, SparseBlockage Blockage>
			requires is_random_engine<Randomizer>::value && is_equatable<T, U>::value
		constexpr std::optional<offset_t> find_random(cref<zone_t<T>> zone, ref<Randomizer> generator, cref<U> value, cref<Blockage> sparse_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == region_e::None) {
				return *this;
			}

			if constexpr (Region == region_e::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == region_e::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == region_e::Border) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.border_size.h * 2 - 1 };

				for (extent_t::product_t i{ 0 }; i < zone.border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < zone.border_size.w ? x : zone.zone_extent.x - x;
					y = y < zone.border_size.h ? y : zone.zone_extent.y - y;

					const offset_t pos{ x, y };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			}

			return std::nullopt;
		}

		template<region_e Region, typename Randomizer, typename T, SparseBlockage EntityBlockage, SparseBlockage ObjectBlockage>
		constexpr std::optional<offset_t> find_random(cref<zone_t<T>> zone, ref<Randomizer> generator, cref<T> value, cref<EntityBlockage> entity_blockage, cref<ObjectBlockage> object_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == region_e::None) {
				return *this;
			}

			if constexpr (Region == region_e::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == region_e::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == region_e::Border) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.border_size.h * 2 - 1 };

				for (extent_t::product_t i{ 0 }; i < zone.border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < zone.border_size.w ? x : zone.zone_extent.x - x;
					y = y < zone.border_size.h ? y : zone.zone_extent.y - y;

					const offset_t pos{ x, y };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			}

			return std::nullopt;
		}

		template<region_e Region, typename Randomizer, typename T, typename U, SparseBlockage EntityBlockage, SparseBlockage ObjectBlockage>
			requires is_random_engine<Randomizer>::value && is_equatable<T, U>::value
		constexpr std::optional<offset_t> find_random(cref<zone_t<T>> zone, ref<Randomizer> generator, cref<U> value, cref<EntityBlockage> entity_blockage, cref<ObjectBlockage> object_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == region_e::None) {
				return *this;
			}

			if constexpr (Region == region_e::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == region_e::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == region_e::Border) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.border_size.h * 2 - 1 };

				for (extent_t::product_t i{ 0 }; i < zone.border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < zone.border_size.w ? x : zone.zone_extent.x - x;
					y = y < zone.border_size.h ? y : zone.zone_extent.y - y;

					const offset_t pos{ x, y };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			}

			return std::nullopt;
		}

		constexpr bool add(offset_t goal) noexcept {
			if (!distances.dependent within<region_e::All>(goal)) {
				return false;
			}

			return goals.add(goal, goal_value);
		}

		template<region_e Region> constexpr bool add(offset_t goal) noexcept {
			if (!distances.dependent within<Region>(goal)) {
				return false;
			}

			return goals.add(goal, goal_value);
		}

		constexpr bool add(offset_t goal, D value) noexcept {
			if (!distances.dependent within<region_e::All>(goal) || value > goal_value) {
				return false;
			}

			return goals.add(goal, value);
		}

		template<region_e Region> constexpr bool add(offset_t goal, D value) noexcept {
			if (!distances.dependent within<Region>(goal) || value > goal_value) {
				return false;
			}

			return goals.add(goal, value);
		}

		constexpr bool remove(offset_t goal) noexcept {
			if (!distances.dependent within<region_e::All>(goal)) {
				return false;
			}

			return goals.remove(goal);
		}

		template<region_e Region> constexpr bool remove(offset_t goal) noexcept {
			if (!distances.dependent within<Region>(goal)) {
				return false;
			}

			return goals.remove(goal);
		}

		constexpr bool update(offset_t from, offset_t to) noexcept {
			if (!distances.dependent within<region_e::All>(from) || !distances.dependent within<region_e::All>(to)) {
				return false;
			}

			return goals.update(from, to);
		}

		template<region_e Region> constexpr bool update(offset_t from, offset_t to) noexcept {
			if (!distances.dependent within<Region>(from) || !distances.dependent within<Region>(to)) {
				return false;
			}

			return goals.update(from, to);
		}
	};

	template struct field_t<f32, distance_function_e::Octile, extent_t{ 32, 32 }, extent_t{ 4, 4 }>;
} // namespace bleak
