#pragma once

#include <bleak/typedef.hpp>

#include <optional>
#include <queue>
#include <unordered_set>

#include <bleak/concepts.hpp>
#include <bleak/extent.hpp>
#include <bleak/octant.hpp>
#include <bleak/offset.hpp>
#include <bleak/sparse.hpp>
#include <bleak/random.hpp>
#include <bleak/zone.hpp>

namespace bleak {
	template<Numeric D> using goal_t = sparseling_t<D>;

	template<Numeric D, distance_function_e DistanceFunction, extent_t ZoneSize, extent_t ZoneBorder> struct field_t {
	  private:
		zone_t<D, ZoneSize, ZoneBorder> distances;
		sparse_t<goal_t<D>> goals;

	  public:
		static constexpr D goal_value{ 0 };
		static constexpr D obstacle_value{ ZoneSize.area() };

		static constexpr D close_to_obstacle_value{ obstacle_value - 1 };

		constexpr bool goal_reached(offset_t position) const noexcept { return distances[position] == goal_value; }

		constexpr bool goal_reached(offset_t position, D threshold) const noexcept { return distances[position] <= threshold; }

		constexpr bool obstacle_reached(offset_t position) const noexcept { return distances[position] >= close_to_obstacle_value; }

		constexpr bool obstacle_reached(offset_t position, D threshold) const noexcept { return distances[position] >= close_to_obstacle_value - threshold; }

		constexpr field_t() noexcept : distances{}, goals{} { clear<zone_region_e::All>(); }

		template<typename... Goals>
			requires is_homogeneous<goal_t<D>, Goals...>::value
		constexpr field_t(cref<Goals>... goals) noexcept : distances{}, goals{ goals... } {
			clear<zone_region_e::All>();
		}

		template<typename T, typename... Goals>
			requires is_homogeneous<goal_t<D>, Goals...>::value
		constexpr field_t(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, cref<Goals>... goals) noexcept : distances{}, goals{ goals... } {
			recalculate<zone_region_e::All>(zone, value);
		}

		template<typename... Goals>
			requires is_homogeneous<goal_t<D>, Goals...>::value
		constexpr field_t(rval<Goals>... goals) noexcept : distances{}, goals{ (std::move(goals), ...) } {
			clear<zone_region_e::All>();
		}

		template<zone_region_e Region, typename T, typename... Goals>
			requires is_homogeneous<goal_t<D>, Goals...>::value
		constexpr field_t(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, rval<Goals>... goals) noexcept : distances{}, goals{ (std::move(goals), ...) } {
			recalculate<zone_region_e::All>(zone, value);
		}

		template<zone_region_e Region> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> clear() noexcept {
			distances.dependent set<Region>(obstacle_value);

			return *this;
		}

		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> reset() noexcept {
			clear<zone_region_e::All>();
			goals.clear();

			return *this;
		}

		constexpr D at(offset_t position) const noexcept { return distances[position]; }

		constexpr void homogenize() noexcept {
			for (usize i{ 0 }; i < ZoneSize.area(); ++i) {
				const D distance{ distances[i] };

				if (distance >= 1) {
					continue;
				}

				distances[i] = (distance < 1 && distance > -1) ? 0 : std::abs(distance);
			}
		}

		template<zone_region_e Region, typename T> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value) noexcept {
			clear();

			if (goals.empty()) {
				return *this;
			}

			std::queue<creeper_t<D>> frontier{};
			std::unordered_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto goal : goals) {
				if (!zone.dependent within<Region>(goal.position) || zone[goal.position] != value) {
					continue;
				}

				frontier.emplace(goal.position, goal.value);
				visited.insert(goal.position);

				if (goal.value < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.front() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
					const offset_t offset_position{ current.position + offset };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + 1 });					
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<zone_region_e Region, typename T, typename U>
			requires is_equatable<T, U>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<U> value) noexcept {
			clear<Region>();

			if (goals.empty()) {
				return *this;
			}

			std::queue<creeper_t<D>> frontier{};
			std::unordered_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto goal : goals) {
				if (!zone.dependent within<Region>(goal.position) || zone[goal.position] != value) {
					continue;
				}

				frontier.emplace(goal.position, goal.value);
				visited.insert(goal.position);

				if (goal.value < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.front() };
				frontier.pop();

				distances[current.position] = current.distance;

				for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
					const offset_t offset_position{ current.position + offset };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + 1 });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<zone_region_e Region, typename T, SparseBlockage Blockage> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, cref<Blockage> blockage) noexcept {
			clear();

			if (goals.empty()) {
				return *this;
			}

			std::queue<creeper_t<D>> frontier{};
			std::unordered_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto goal : goals) {
				if (!zone.dependent within<Region>(goal.position) || zone[goal.position] != value) {
					continue;
				}

				frontier.emplace(goal.position, goal.value);
				visited.insert(goal.position);

				if (goal.value < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.front() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
					const offset_t offset_position{ current.position + offset };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value || blockage.contains(offset_position)) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + 1 });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<zone_region_e Region, typename T, typename U, SparseBlockage Blockage>
			requires is_equatable<T, U>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<U> value, cref<Blockage> sparse_blockage) noexcept {
			clear<Region>();

			if (goals.empty()) {
				return *this;
			}

			std::priority_queue<creeper_t<D>, std::vector<creeper_t<D>>, typename creeper_t<D>::less> frontier{};
			std::unordered_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto goal : goals) {
				if (!zone.dependent within<Region>(goal.position) || zone[goal.position] != value) {
					continue;
				}

				frontier.emplace(goal.position, goal.value);
				visited.insert(goal.position);

				if (goal.value < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.top() };
				frontier.pop();

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

		template<zone_region_e Region, typename T, SparseBlockage... Blockages>
			requires is_plurary<Blockages...>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, cref<Blockages>... blockages) noexcept {
			clear();

			if (goals.empty()) {
				return *this;
			}

			std::queue<creeper_t<D>> frontier{};
			std::unordered_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto goal : goals) {
				if (!zone.dependent within<Region>(goal.position) || zone[goal.position] != value) {
					continue;
				}

				frontier.emplace(goal.position, goal.value);
				visited.insert(goal.position);

				if (goal.value < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.front() };
				frontier.pop();

				visited.insert(current.position);

				distances[current.position] = current.distance;

				for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
					const offset_t offset_position{ current.position + offset };

					if (!visited.insert(offset_position).second || !zone.dependent within<Region>(offset_position) || zone[offset_position] != value || (blockages.contains(offset_position) || ...)) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + 1 });
				}
			}

			if (negative_goal) {
				homogenize();
			}

			return *this;
		}

		template<zone_region_e Region, typename T, typename U, SparseBlockage... Blockages>
			requires is_plurary<Blockages...>::value && is_equatable<T, U>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<U> value, cref<Blockages>... blockages) noexcept {
			clear<Region>();

			if (goals.empty()) {
				return *this;
			}

			std::priority_queue<creeper_t<D>, std::vector<creeper_t<D>>, typename creeper_t<D>::less> frontier{};
			std::unordered_set<offset_t, offset_t::std_hasher> visited{};

			bool negative_goal{ false };

			for (crauto goal : goals) {
				if (!zone.dependent within<Region>(goal.position) || zone[goal.position] != value) {
					continue;
				}

				frontier.emplace(goal.position, goal.value);
				visited.insert(goal.position);

				if (goal.value < 0) {
					negative_goal = true;
				}
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.top() };
				frontier.pop();

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

		template<zone_region_e Region> constexpr std::optional<offset_t> ascend(offset_t position) const noexcept {
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

		template<zone_region_e Region, SparseBlockage Blockage> constexpr std::optional<offset_t> ascend(offset_t position, cref<Blockage> sparse_blockage) const noexcept {
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

		template<zone_region_e Region, RandomEngine Generator> constexpr std::optional<offset_t> ascend(offset_t position, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
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

		template<zone_region_e Region, RandomEngine Generator, SparseBlockage Blockage> constexpr std::optional<offset_t> ascend(offset_t position, cref<Blockage> sparse_blockage, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
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

		template<zone_region_e Region> constexpr std::optional<offset_t> descend(offset_t position) const noexcept {
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

		template<zone_region_e Region, SparseBlockage Blockage> constexpr std::optional<offset_t> descend(offset_t position, cref<Blockage> sparse_blockage) const noexcept {
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

		template<zone_region_e Region, RandomEngine Generator> constexpr std::optional<offset_t> descend(offset_t position, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
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

		template<zone_region_e Region, RandomEngine Generator, SparseBlockage Blockage> constexpr std::optional<offset_t> descend(offset_t position, cref<Blockage> sparse_blockage, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
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

		template<zone_region_e Region, typename Randomizer, typename T, SparseBlockage Blockage>
		constexpr std::optional<offset_t> find_random(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, ref<Randomizer> generator, cref<T> value, cref<Blockage> sparse_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == zone_region_e::None) {
				return *this;
			}

			if constexpr (Region == zone_region_e::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_e::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_e::Border) {
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

		template<zone_region_e Region, typename Randomizer, typename T, typename U, SparseBlockage Blockage>
			requires is_random_engine<Randomizer>::value && is_equatable<T, U>::value
		constexpr std::optional<offset_t> find_random(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, ref<Randomizer> generator, cref<U> value, cref<Blockage> sparse_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == zone_region_e::None) {
				return *this;
			}

			if constexpr (Region == zone_region_e::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_e::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_e::Border) {
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

		template<zone_region_e Region, typename Randomizer, typename T, SparseBlockage EntityBlockage, SparseBlockage ObjectBlockage>
		constexpr std::optional<offset_t> find_random(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, ref<Randomizer> generator, cref<T> value, cref<EntityBlockage> entity_blockage, cref<ObjectBlockage> object_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == zone_region_e::None) {
				return *this;
			}

			if constexpr (Region == zone_region_e::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_e::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_e::Border) {
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

		template<zone_region_e Region, typename Randomizer, typename T, typename U, SparseBlockage EntityBlockage, SparseBlockage ObjectBlockage>
			requires is_random_engine<Randomizer>::value && is_equatable<T, U>::value
		constexpr std::optional<offset_t> find_random(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, ref<Randomizer> generator, cref<U> value, cref<EntityBlockage> entity_blockage, cref<ObjectBlockage> object_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == zone_region_e::None) {
				return *this;
			}

			if constexpr (Region == zone_region_e::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_e::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || entity_blockage.contains(pos) || object_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_e::Border) {
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
			if (!distances.dependent within<zone_region_e::All>(goal)) {
				return false;
			}

			return goals.add(goal_t<D>{ goal_value, goal });
		}

		template<zone_region_e Region> constexpr bool add(offset_t goal) noexcept {
			if (!distances.dependent within<Region>(goal)) {
				return false;
			}

			return goals.add(goal_t<D>{ goal, goal_value });
		}

		constexpr bool add(offset_t goal, D value) noexcept {
			if (!distances.dependent within<zone_region_e::All>(goal) || value > goal_value) {
				return false;
			}

			return goals.add(goal_t<D>{ value, goal });
		}

		template<zone_region_e Region> constexpr bool add(offset_t goal, D value) noexcept {
			if (!distances.dependent within<Region>(goal) || value > goal_value) {
				return false;
			}

			return goals.add(goal_t<D>{ goal, value });
		}

		constexpr bool remove(offset_t goal) noexcept {
			if (!distances.dependent within<zone_region_e::All>(goal)) {
				return false;
			}

			return goals.remove(goal);
		}

		template<zone_region_e Region> constexpr bool remove(offset_t goal) noexcept {
			if (!distances.dependent within<Region>(goal)) {
				return false;
			}

			return goals.remove(goal);
		}

		constexpr bool update(offset_t from, offset_t to) noexcept {
			if (!distances.dependent within<zone_region_e::All>(from) || !distances.dependent within<zone_region_e::All>(to)) {
				return false;
			}

			return goals.move(from, to);
		}

		template<zone_region_e Region> constexpr bool update(offset_t from, offset_t to) noexcept {
			if (!distances.dependent within<Region>(from) || !distances.dependent within<Region>(to)) {
				return false;
			}

			return goals.move(from, to);
		}
	};
} // namespace bleak
