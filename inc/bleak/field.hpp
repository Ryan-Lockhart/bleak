#pragma once

#include <bleak/typedef.hpp>

#include <optional>
#include <queue>
#include <unordered_set>

#include <bleak/concepts.hpp>
#include <bleak/extent.hpp>
#include <bleak/octant.hpp>
#include <bleak/offset.hpp>
#include <bleak/random.hpp>
#include <bleak/zone.hpp>

namespace bleak {
	template<Numeric D, distance_function_t DistanceFunction, extent_t ZoneSize, extent_t ZoneBorder> struct field_t {
	  private:
		zone_t<D, ZoneSize, ZoneBorder> distances;
		std::unordered_set<offset_t, offset_t::hasher> goals;

	  public:
		static constexpr D goal_value{ 0 };
		static constexpr D obstacle_value{ ZoneSize.area() };

		static constexpr D close_to_obstacle_value{ obstacle_value - 1 };

		constexpr bool goal_reached(offset_t position) const noexcept { return distances[position] == goal_value; }

		constexpr bool goal_reached(offset_t position, D threshold) const noexcept { return distances[position] <= threshold; }

		constexpr bool obstacle_reached(offset_t position) const noexcept { return distances[position] >= close_to_obstacle_value; }

		constexpr bool obstacle_reached(offset_t position, D threshold) const noexcept { return distances[position] >= close_to_obstacle_value - threshold; }

		constexpr field_t() noexcept : distances{}, goals{} { clear<zone_region_t::All>(); }

		template<typename... Goals>
			requires is_homogeneous<offset_t, Goals...>::value
		constexpr field_t(cref<Goals>... goals) noexcept : distances{}, goals{ goals... } {
			clear<zone_region_t::All>();
		}

		template<typename T, typename... Goals>
			requires is_homogeneous<offset_t, Goals...>::value
		constexpr field_t(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, cref<Goals>... goals) noexcept : distances{}, goals{ goals... } {
			recalculate<zone_region_t::All>(zone, value);
		}

		template<typename... Goals>
			requires is_homogeneous<offset_t, Goals...>::value
		constexpr field_t(rval<Goals>... goals) noexcept : distances{}, goals{ (std::move(goals), ...) } {
			clear<zone_region_t::All>();
		}

		template<zone_region_t Region, typename T, typename... Goals>
			requires is_homogeneous<offset_t, Goals...>::value
		constexpr field_t(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, rval<Goals>... goals) noexcept : distances{}, goals{ (std::move(goals), ...) } {
			recalculate<zone_region_t::All>(zone, value);
		}

		template<zone_region_t Region> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> clear() noexcept {
			distances.template set<Region>(obstacle_value);

			return *this;
		}

		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> reset() noexcept {
			clear<zone_region_t::All>();
			goals.clear();

			return *this;
		}

		constexpr D at(offset_t position) const noexcept { return distances[position]; }

		template<zone_region_t Region, typename T> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value) noexcept {
			clear();

			if (goals.empty()) {
				return *this;
			}

			std::queue<creeper_t<T>> frontier{};
			std::unordered_set<offset_t, offset_t::hasher> visited{};

			for (crauto goal : goals) {
				if (!zone.template within<Region>(goal) || zone[goal] != value) {
					continue;
				}

				frontier.emplace(goal, goal_value);
				visited.insert(goal);
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

					if (!visited.insert(offset_position).second || !zone.template within<Region>(offset_position) || zone[offset_position] != value) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + 1 });					
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename T, typename U>
			requires is_equatable<T, U>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<U> value) noexcept {
			clear<Region>();

			if (goals.empty()) {
				return *this;
			}

			std::queue<creeper_t<D>> frontier{};
			std::unordered_set<offset_t, offset_t::hasher> visited{};

			for (crauto goal : goals) {
				if (!zone.template within<Region>(goal) || zone[goal] != value) {
					continue;
				}

				frontier.emplace(goal, goal_value);
				visited.insert(goal);
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

					if (!visited.insert(offset_position).second || !zone.template within<Region>(offset_position) || zone[offset_position] != value) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + 1 });
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename T, SparseBlockage Blockage> constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, cref<Blockage> sparse_blockage) noexcept {
			clear();

			if (goals.empty()) {
				return *this;
			}

			std::queue<creeper_t<T>> frontier{};
			std::unordered_set<offset_t, offset_t::hasher> visited{};

			for (crauto goal : goals) {
				if (!zone.template within<Region>(goal) || zone[goal] != value) {
					continue;
				}

				frontier.emplace(goal, goal_value);
				visited.insert(goal);
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

					if (!visited.insert(offset_position).second || !zone.template within<Region>(offset_position) || zone[offset_position] != value || sparse_blockage.contains(offset_position)) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + 1 });
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename T, typename U, SparseBlockage Blockage>
			requires is_equatable<T, U>::value
		constexpr ref<field_t<D, DistanceFunction, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<U> value, cref<Blockage> sparse_blockage) noexcept {
			clear<Region>();

			if (goals.empty()) {
				return *this;
			}

			std::queue<creeper_t<D>> frontier{};
			std::unordered_set<offset_t, offset_t::hasher> visited{};

			for (crauto goal : goals) {
				if (!zone.template within<Region>(goal) || zone[goal] != value) {
					continue;
				}

				frontier.emplace(goal, goal_value);
				visited.insert(goal);
			}

			if (frontier.empty()) {
				return *this;
			}

			while (!frontier.empty()) {
				const creeper_t<D> current{ frontier.front() };
				frontier.pop();

				distances[current.position] = current.distance;

				for (crauto offset : neighbourhood_creepers<DistanceFunction, D>) {
					cauto offset_position{ current.position + offset.first };

					if (!visited.insert(offset_position).second || !zone.template within<Region>(offset_position) || zone[offset_position] != value || sparse_blockage.contains(offset_position)) {
						continue;
					}

					frontier.emplace(offset_position, D{ current.distance + offset.second });
				}
			}

			return *this;
		}

		template<zone_region_t Region> constexpr std::optional<offset_t> ascend(offset_t position) const noexcept {
			if (!distances.template within<Region>(position)) {
				return std::nullopt;
			}

			offset_t highest{ position };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };

				if (!distances.template within<Region>(offset_position) || distances[offset_position] == obstacle_value || distances[offset_position] <= distances[highest]) {
					continue;
				}

				highest = offset_position;
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<zone_region_t Region, SparseBlockage Blockage> constexpr std::optional<offset_t> ascend(offset_t position, cref<Blockage> sparse_blockage) const noexcept {
			if (!distances.template within<Region>(position)) {
				return std::nullopt;
			}

			offset_t highest{ position };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };

				if (!distances.template within<Region>(offset_position) || distances[offset_position] == obstacle_value || distances[offset_position] <= distances[highest] || sparse_blockage.contains(offset_position)) {
					continue;
				}

				highest = offset_position;
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<zone_region_t Region, typename Generator>
			requires is_random_engine<Generator>::value
		constexpr std::optional<offset_t> ascend(offset_t position, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.template within<Region>(position)) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t highest{ position };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };

				if (!distances.template within<Region>(offset_position) ||
					distances[offset_position] == obstacle_value ||
					distances[offset_position] < distances[highest] ||
					(distances[offset_position] == distances[highest] && !distribution(generator))
				) {
					continue;
				}

				highest = offset_position;
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<zone_region_t Region, typename Generator, SparseBlockage Blockage>
			requires is_random_engine<Generator>::value
		constexpr std::optional<offset_t> ascend(offset_t position, cref<Blockage> sparse_blockage, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.template within<Region>(position)) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t highest{ position };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };

				if (!distances.template within<Region>(offset_position) ||
					distances[offset_position] == obstacle_value ||
					distances[offset_position] < distances[highest] ||
					sparse_blockage.contains(offset_position) ||
					(distances[offset_position] == distances[highest] && !distribution(generator))
				) {
					continue;
				}

				highest = offset_position;
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<zone_region_t Region> constexpr std::optional<offset_t> descend(offset_t position) const noexcept {
			if (!distances.template within<Region>(position) || goal_reached(position)) {
				return std::nullopt;
			}

			offset_t lowest{ position };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };

				if (!distances.template within<Region>(offset_position) || distances[offset_position] >= distances[lowest]) {
					continue;
				}

				lowest = offset_position;
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<zone_region_t Region, SparseBlockage Blockage> constexpr std::optional<offset_t> descend(offset_t position, cref<Blockage> sparse_blockage) const noexcept {
			if (!distances.template within<Region>(position) || goal_reached(position)) {
				return std::nullopt;
			}

			offset_t lowest{ position };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };

				if (!distances.template within<Region>(offset_position) || distances[offset_position] >= distances[lowest] || sparse_blockage.contains(offset_position)) {
					continue;
				}

				lowest = offset_position;
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<zone_region_t Region, typename Generator>
			requires is_random_engine<Generator>::value
		constexpr std::optional<offset_t> descend(offset_t position, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.template within<Region>(position) || goal_reached(position)) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t lowest{ position };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };

				if (!distances.template within<Region>(offset_position) ||
					distances[offset_position] > distances[lowest] ||
					(distances[offset_position] == distances[lowest] && !distribution(generator))
				) {
					continue;
				}

				lowest = offset_position;
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<zone_region_t Region, typename Generator, SparseBlockage Blockage>
			requires is_random_engine<Generator>::value
		constexpr std::optional<offset_t> descend(offset_t position, cref<Blockage> sparse_blockage, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.template within<Region>(position) || goal_reached(position)) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t lowest{ position };

			for (cauto offset : neighbourhood_offsets<DistanceFunction>) {
				const offset_t offset_position{ position + offset };
				
				if (!distances.template within<Region>(offset_position) ||
					distances[offset_position] > distances[lowest] ||
					sparse_blockage.contains(offset_position) ||
					(distances[offset_position] == distances[lowest] && !distribution(generator))
				) {
					continue;
				}

				lowest = offset_position;
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<zone_region_t Region, typename Randomizer, typename T, SparseBlockage Blockage>
		constexpr std::optional<offset_t> find_random(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, ref<Randomizer> generator, cref<T> value, cref<Blockage> sparse_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_t::Border) {
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

		template<zone_region_t Region, typename Randomizer, typename T, typename U, SparseBlockage Blockage>
			requires is_random_engine<Randomizer>::value && is_equatable<T, U>::value
		constexpr std::optional<offset_t> find_random(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, ref<Randomizer> generator, cref<U> value, cref<Blockage> sparse_blockage, cref<D> minimum_distance) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone.zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone.zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ zone.interior_origin.x, zone.interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ zone.interior_origin.y, zone.interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone.interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (zone[pos] != value || sparse_blockage.contains(pos) || distances[pos] < minimum_distance) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_t::Border) {
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

		constexpr bool add(offset_t goal) noexcept {
			if (!distances.template within<zone_region_t::All>(goal)) {
				return false;
			}

			return goals.insert(goal).second;
		}

		template<zone_region_t Region> constexpr bool add(offset_t goal) noexcept {
			if (!distances.template within<Region>(goal)) {
				return false;
			}

			return goals.insert(goal).second;
		}

		constexpr bool remove(offset_t goal) noexcept {
			if (!distances.template within<zone_region_t::All>(goal)) {
				return false;
			}

			return goals.erase(goal);
		}

		template<zone_region_t Region> constexpr bool remove(offset_t goal) noexcept {
			if (!distances.template within<Region>(goal)) {
				return false;
			}

			return goals.erase(goal);
		}

		constexpr bool update(offset_t from, offset_t to) noexcept {
			if (!distances.template within<zone_region_t::All>(from) || !distances.template within<zone_region_t::All>(to)) {
				return false;
			}
			
			cauto iter{ goals.find(from) };

			if (iter == goals.end()) {
				return false;
			}

			auto node{ goals.extract(iter) };

			node.value() = to;

			return goals.insert(std::move(node)).inserted;
		}

		template<zone_region_t Region> constexpr bool update(offset_t from, offset_t to) noexcept {
			if (!distances.template within<Region>(from) || !distances.template within<Region>(to)) {
				return false;
			}
			
			cauto iter{ goals.find(from) };

			if (iter == goals.end()) {
				return false;
			}

			auto node{ goals.extract(iter) };

			node.value() = to;

			return goals.insert(std::move(node)).inserted;
		}
	};
} // namespace bleak
