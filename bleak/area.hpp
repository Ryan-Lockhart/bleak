#pragma once

#include "bleak/concepts.hpp"
#include "constants/numeric.hpp"
#include <bleak/typedef.hpp>

#include <queue>
#include <unordered_set>

#include <bleak/offset.hpp>
#include <bleak/zone.hpp>

namespace bleak {
	struct area_t : public std::unordered_set<offset_2d_t, offset_2d_t::hasher> {
		using underlying_t = std::unordered_set<offset_2d_t, offset_2d_t::hasher>;

		inline area_t() noexcept {}

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline area_t(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value) { recalculate(zone, value); }

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline area_t(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_2d_t> position, cref<T> value) { recalculate(zone, position, value); }

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline area_t(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_2d_t> position, cref<T> value, offset_2d_t::product_t distance) {
			recalculate(zone, position, value, distance);
		}

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline area_t(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_2d_t> position, cref<T> value, f32 distance) { recalculate(zone, position, value, distance); }

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline void recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value) {
			clear();

			for (extent_2d_t::scalar_t y{ 0 }; y < Size.h; ++y) {
				for (extent_2d_t::scalar_t x{ 0 }; x < Size.w; ++x) {
					if (zone[x, y] != value) {
						continue;
					}

					insert(offset_2d_t{ x, y });
				}
			}
		}

		template<typename T, typename U, extent_2d_t Size, extent_2d_t BorderSize>
			requires is_equatable<T, U>::value
		inline void recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value) {
			clear();

			for (extent_2d_t::scalar_t y{ 0 }; y < Size.h; ++y) {
				for (extent_2d_t::scalar_t x{ 0 }; x < Size.w; ++x) {
					if (zone[x, y] != value) {
						continue;
					}

					insert(offset_2d_t{ x, y });
				}
			}
		}

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline void recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_2d_t> position, cref<T> value) {
			clear();

			if (!zone.within<zone_region_t::All>(position) || zone[position] != value) {
				return;
			}

			std::queue<offset_2d_t> frontier{};
			frontier.push(position);

			while (!frontier.empty()) {
				const offset_2d_t current{ frontier.front() };
				frontier.pop();

				insert(current);

				for (offset_2d_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_2d_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_2d_t neighbour{ current.x + x, current.y + y };

						if (!zone.within<zone_region_t::All>(neighbour) || zone[neighbour] != value || contains(neighbour)) {
							continue;
						}

						frontier.push(neighbour);
					}
				}
			}
		}

		template<typename T, typename U, extent_2d_t Size, extent_2d_t BorderSize>
			requires is_equatable<T, U>::value
		inline void recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_2d_t> position, cref<U> value) {
			clear();

			if (!zone.within<zone_region_t::All>(position) || zone[position] != value) {
				return;
			}

			std::queue<offset_2d_t> frontier{};
			frontier.push(position);

			while (!frontier.empty()) {
				const offset_2d_t current{ frontier.front() };
				frontier.pop();

				insert(current);

				for (offset_2d_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_2d_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_2d_t neighbour{ current.x + x, current.y + y };

						if (!zone.within<zone_region_t::All>(neighbour) || zone[neighbour] != value || contains(neighbour)) {
							continue;
						}

						frontier.push(neighbour);
					}
				}
			}
		}

		template<typename T, typename D, extent_2d_t Size, extent_2d_t BorderSize>
			requires is_numeric<D>::value
		inline void recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_2d_t> position, cref<T> value, cref<D> distance) {
			clear();

			if (!zone.within<zone_region_t::All>(position) || zone[position] != value) {
				return;
			}

			std::queue<creeper_t<f32>> frontier{};
			frontier.push({ position, 0.0f });

			while (!frontier.empty()) {
				const creeper_t current{ frontier.front() };
				frontier.pop();

				insert(current.position);

				for (offset_2d_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_2d_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_2d_t neighbour{ current.position.x + x, current.position.y + y };

						if (!zone.within<zone_region_t::All>(neighbour) || zone[neighbour] != value || contains(neighbour) || current.distance >= distance) {
							continue;
						}

						frontier.push({ neighbour, x != 0 && y != 0 ? current.distance + PlanarDiagonalDistance<D> : current.distance + PlanarDistance<D> });
					}
				}
			}
		}

		template<typename T, typename U, typename D, extent_2d_t Size, extent_2d_t BorderSize>
			requires is_equatable<T, U>::value && is_numeric<D>::value
		inline void recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_2d_t> position, cref<U> value, cref<D> distance) {
			clear();

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return;
			}

			std::queue<creeper_t<f32>> frontier{};
			frontier.push({ position, 0.0f });

			while (!frontier.empty()) {
				const creeper_t current{ frontier.front() };
				frontier.pop();

				insert(current.position);

				for (offset_2d_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_2d_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_2d_t neighbour{ current.position.x + x, current.position.y + y };

						if (!zone.template within<zone_region_t::All>(neighbour) || zone[neighbour] != value || contains(neighbour) || current.distance >= distance) {
							continue;
						}

						frontier.push({ neighbour, x != 0 && y != 0 ? current.distance + PlanarDiagonalDistance<D> : current.distance + PlanarDistance<D> });
					}
				}
			}
		}

		inline bool contains(cref<offset_2d_t> position) const noexcept { return find(position) != end(); }

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline void set(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (cref<offset_2d_t> position : *this) {
				zone[position] = value;
			}
		}

		template<typename T, typename U, extent_2d_t Size, extent_2d_t BorderSize>
			requires std::is_assignable<T, U>::value
		inline void set(ref<zone_t<T, Size, BorderSize>> zone, cref<U> value) const noexcept {
			for (cref<offset_2d_t> position : *this) {
				zone[position] = value;
			}
		}

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline void apply(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (cref<offset_2d_t> position : *this) {
				zone[position] += value;
			}
		}

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize> inline void repeal(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (cref<offset_2d_t> position : *this) {
				zone[position] -= value;
			}
		}

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize, typename Generator>
		inline void randomize(ref<zone_t<T, Size, BorderSize>> zone, ref<Generator> generator, f64 probability, cref<binary_applicator_t<T>> applicator) const noexcept
			requires is_random_engine<Generator>::value
		{
			std::bernoulli_distribution dis{ probability };

			for (cref<offset_2d_t> position : *this) {
				zone[position] = applicator(generator, dis);
			}
		}

		template<typename T, extent_2d_t Size, extent_2d_t BorderSize, typename Generator>
		inline void randomize(ref<zone_t<T, Size, BorderSize>> zone, ref<Generator> generator, f64 probability, cref<T> true_value, cref<T> false_value) const noexcept
			requires is_random_engine<Generator>::value
		{
			std::bernoulli_distribution dis{ probability };

			for (cref<offset_2d_t> position : *this) {
				zone[position] = dis(generator) ? true_value : false_value;
			}
		}
	};
} // namespace bleak
