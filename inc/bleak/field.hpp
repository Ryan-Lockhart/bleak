#pragma once

#include <bleak/typedef.hpp>

#include <limits>
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
	template<Numeric D, extent_t ZoneSize, extent_t ZoneBorder> struct field_t {
	  private:
		zone_t<D, ZoneSize, ZoneBorder> distances;
		std::unordered_set<offset_t, offset_t::hasher> goals;

	  public:
		static constexpr D goal_value{ 0 };
		static constexpr D obstacle_value{ std::numeric_limits<D>::max() };

		constexpr field_t() noexcept : distances{}, goals{} { reset<zone_region_t::All>(); }

		template<typename... Goals>
			requires is_homogeneous<offset_t, Goals...>::value
		constexpr field_t(cref<Goals>... goals) noexcept : distances{}, goals{ goals... } {
			reset<zone_region_t::All>();
		}

		template<typename T, typename... Goals>
			requires is_homogeneous<offset_t, Goals...>::value
		constexpr field_t(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, cref<Goals>... goals) noexcept : distances{}, goals{ goals... } {
			recalculate<zone_region_t::All>(zone, value);
		}

		template<typename... Goals>
			requires is_homogeneous<offset_t, Goals...>::value
		constexpr field_t(rval<Goals>... goals) noexcept : distances{}, goals{ (std::move(goals), ...) } {
			reset<zone_region_t::All>();
		}

		template<zone_region_t Region, typename T, typename... Goals>
			requires is_homogeneous<offset_t, Goals...>::value
		constexpr field_t(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, rval<Goals>... goals) noexcept : distances{}, goals{ (std::move(goals), ...) } {
			recalculate<zone_region_t::All>(zone, value);
		}

		template<zone_region_t Region> constexpr ref<field_t<D, ZoneSize, ZoneBorder>> reset() noexcept {
			distances.template set<Region>(obstacle_value);
			return *this;
		}

		constexpr cref<D> operator[](cref<offset_t> position) const noexcept { return distances[position]; }

		template<zone_region_t Region, typename T> constexpr ref<field_t<D, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value) noexcept {
			reset();

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

				for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
					for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
						if (x_offs == 0 && y_offs == 0) {
							continue;
						}

						const offset_t offset{ x_offs, y_offs };
						const offset_t offset_position{ current.position + offset };

						if (!visited.insert(offset_position).second || !zone.template within<Region>(offset_position) || zone[offset_position] != value) {
							continue;
						}

						frontier.emplace(offset_position, D{ current.distance + 1 });
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename T, typename U>
			requires is_equatable<T, U>::value
		constexpr ref<field_t<D, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<U> value) noexcept {
			reset<Region>();

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

				for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
					for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
						if (x_offs == 0 && y_offs == 0) {
							continue;
						}

						const offset_t offset{ x_offs, y_offs };
						const offset_t offset_position{ current.position + offset };

						if (!visited.insert(offset_position).second || !zone.template within<Region>(offset_position) || zone[offset_position] != value) {
							continue;
						}

						frontier.emplace(offset_position, D{ current.distance + 1 });
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename T, SparseBlockage Blockage> constexpr ref<field_t<D, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<T> value, cref<Blockage> sparse_blockage) noexcept {
			reset();

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

				for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
					for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
						if (x_offs == 0 && y_offs == 0) {
							continue;
						}

						const offset_t offset{ x_offs, y_offs };
						const offset_t offset_position{ current.position + offset };

						if (!visited.insert(offset_position).second || !zone.template within<Region>(offset_position) || zone[offset_position] != value || sparse_blockage.contains(offset_position)) {
							continue;
						}

						frontier.emplace(offset_position, D{ current.distance + 1 });
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename T, typename U, SparseBlockage Blockage>
			requires is_equatable<T, U>::value
		constexpr ref<field_t<D, ZoneSize, ZoneBorder>> recalculate(cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<U> value, cref<Blockage> sparse_blockage) noexcept {
			reset<Region>();

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

				for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
					for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
						if (x_offs == 0 && y_offs == 0) {
							continue;
						}

						const offset_t offset{ x_offs, y_offs };
						const offset_t offset_position{ current.position + offset };

						if (!visited.insert(offset_position).second || !zone.template within<Region>(offset_position) || zone[offset_position] != value || sparse_blockage.contains(offset_position)) {
							continue;
						}

						frontier.emplace(offset_position, D{ current.distance + 1 });
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region> constexpr std::optional<offset_t> ascend(cref<offset_t> position) const noexcept {
			if (!distances.template within<Region>(position) || distances[position] == obstacle_value) {
				return std::nullopt;
			}

			offset_t highest{ position };

			for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
				for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
					if (x_offs == 0 && y_offs == 0) {
						continue;
					}

					const offset_t offset{ x_offs, y_offs };
					const offset_t offset_position{ position + offset };

					if (!distances.template within<Region>(offset_position) || distances[offset_position] == obstacle_value || distances[offset_position] <= distances[highest]) {
						continue;
					}

					highest = offset_position;
				}
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<zone_region_t Region, SparseBlockage Blockage> constexpr std::optional<offset_t> ascend(cref<offset_t> position, cref<Blockage> sparse_blockage) const noexcept {
			if (!distances.template within<Region>(position) || distances[position] == obstacle_value) {
				return std::nullopt;
			}

			offset_t highest{ position };

			for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
				for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
					if (x_offs == 0 && y_offs == 0) {
						continue;
					}

					const offset_t offset{ x_offs, y_offs };
					const offset_t offset_position{ position + offset };

					if (!distances.template within<Region>(offset_position) || distances[offset_position] == obstacle_value || distances[offset_position] <= distances[highest] || sparse_blockage.contains(offset_position)) {
						continue;
					}

					highest = offset_position;
				}
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<zone_region_t Region, typename Generator>
			requires is_random_engine<Generator>::value
		constexpr std::optional<offset_t> ascend(cref<offset_t> position, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.template within<Region>(position) || distances[position] == obstacle_value) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t highest{ position };

			for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
				for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
					if (x_offs == 0 && y_offs == 0) {
						continue;
					}

					const offset_t offset{ x_offs, y_offs };
					const offset_t offset_position{ position + offset };

					if (!distances.template within<Region>(offset_position) || distances[offset_position] == obstacle_value || distances[offset_position] < distances[highest]
						|| (distances[offset_position] == distances[highest] && !distribution(generator))) {
						continue;
					}

					highest = offset_position;
				}
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<zone_region_t Region, typename Generator, SparseBlockage Blockage>
			requires is_random_engine<Generator>::value
		constexpr std::optional<offset_t> ascend(cref<offset_t> position, cref<Blockage> sparse_blockage, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.template within<Region>(position) || distances[position] == obstacle_value) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t highest{ position };

			for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
				for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
					if (x_offs == 0 && y_offs == 0) {
						continue;
					}

					const offset_t offset{ x_offs, y_offs };
					const offset_t offset_position{ position + offset };

					if (!distances.template within<Region>(offset_position) || distances[offset_position] == obstacle_value || distances[offset_position] < distances[highest] || sparse_blockage.contains(offset_position)
						|| (distances[offset_position] == distances[highest] && !distribution(generator))) {
						continue;
					}

					highest = offset_position;
				}
			}

			if (highest == position) {
				return std::nullopt;
			}

			return highest;
		}

		template<zone_region_t Region> constexpr std::optional<offset_t> descend(cref<offset_t> position) const noexcept {
			if (!distances.template within<Region>(position) || distances[position] == goal_value) {
				return std::nullopt;
			}

			offset_t lowest{ position };

			for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
				for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
					if (x_offs == 0 && y_offs == 0) {
						continue;
					}

					const offset_t offset{ x_offs, y_offs };
					const offset_t offset_position{ position + offset };

					if (!distances.template within<Region>(offset_position) || distances[offset_position] >= distances[lowest]) {
						continue;
					}

					lowest = offset_position;
				}
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<zone_region_t Region, SparseBlockage Blockage> constexpr std::optional<offset_t> descend(cref<offset_t> position, cref<Blockage> sparse_blockage) const noexcept {
			if (!distances.template within<Region>(position) || distances[position] == goal_value) {
				return std::nullopt;
			}

			offset_t lowest{ position };

			for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
				for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
					if (x_offs == 0 && y_offs == 0) {
						continue;
					}

					const offset_t offset{ x_offs, y_offs };
					const offset_t offset_position{ position + offset };

					if (!distances.template within<Region>(offset_position) || distances[offset_position] >= distances[lowest] || sparse_blockage.contains(offset_position)) {
						continue;
					}

					lowest = offset_position;
				}
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<zone_region_t Region, typename Generator>
			requires is_random_engine<Generator>::value
		constexpr std::optional<offset_t> descend(cref<offset_t> position, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.template within<Region>(position) || distances[position] == goal_value) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t lowest{ position };

			for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
				for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
					if (x_offs == 0 && y_offs == 0) {
						continue;
					}

					const offset_t offset{ x_offs, y_offs };
					const offset_t offset_position{ position + offset };

					if (!distances.template within<Region>(offset_position) || distances[offset_position] > distances[lowest] || (distances[offset_position] == distances[lowest] && !distribution(generator))) {
						continue;
					}

					lowest = offset_position;
				}
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		template<zone_region_t Region, typename Generator, SparseBlockage Blockage>
			requires is_random_engine<Generator>::value
		constexpr std::optional<offset_t> descend(cref<offset_t> position, cref<Blockage> sparse_blockage, ref<Generator> generator, f64 unseat_probability = 0.5) const noexcept {
			if (!distances.template within<Region>(position) || distances[position] == goal_value) {
				return std::nullopt;
			}

			std::bernoulli_distribution distribution{ unseat_probability };

			offset_t lowest{ position };

			for (offset_t::scalar_t y_offs{ -1 }; y_offs <= 1; ++y_offs) {
				for (offset_t::scalar_t x_offs{ -1 }; x_offs <= 1; ++x_offs) {
					if (x_offs == 0 && y_offs == 0) {
						continue;
					}

					const offset_t offset{ x_offs, y_offs };
					const offset_t offset_position{ position + offset };

					if (!distances.template within<Region>(offset_position) || distances[offset_position] > distances[lowest] || sparse_blockage.contains(offset_position)
						|| (distances[offset_position] == distances[lowest] && !distribution(generator))) {
						continue;
					}

					lowest = offset_position;
				}
			}

			if (lowest == position) {
				return std::nullopt;
			}

			return lowest;
		}

		constexpr bool operator+=(cref<offset_t> goal) noexcept {
			if (!distances.template within<zone_region_t::All>(goal)) {
				return false;
			}

			return goals.insert(goal).second;
		}

		constexpr bool operator-=(cref<offset_t> goal) noexcept {
			if (!distances.template within<zone_region_t::All>(goal)) {
				return false;
			}

			return goals.erase(goal);
		}

		constexpr bool update(cref<offset_t> from, cref<offset_t> to) noexcept {
			if (!distances.template within<zone_region_t::All>(from) || !distances.template within<zone_region_t::All>(to)) {
				return false;
			}

			if (goals.erase(from)) {
				return goals.insert(to).second;
			}

			return false;
		}
	};
} // namespace bleak
