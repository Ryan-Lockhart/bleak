#pragma once
#include <bleak/typedef.hpp>

#include <cmath>
#include <queue>
#include <unordered_set>

#include <bleak/concepts.hpp>
#include <bleak/octant.hpp>
#include <bleak/offset.hpp>
#include <bleak/zone.hpp>

#include <bleak/constants/numeric.hpp>
#include <bleak/constants/octants.hpp>

namespace bleak {
	struct area_t : public std::unordered_set<offset_t, offset_t::hasher> {
		using underlying_t = std::unordered_set<offset_t, offset_t::hasher>;

		inline area_t() noexcept {}

		template<typename T, extent_t Size, extent_t BorderSize> inline area_t(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value) { recalculate(zone, value); }

		template<typename T, extent_t Size, extent_t BorderSize> inline area_t(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<T> value) { recalculate(zone, position, value); }

		template<typename T, extent_t Size, extent_t BorderSize> inline area_t(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<T> value, offset_t::product_t distance) { recalculate(zone, position, value, distance); }

		template<typename T, extent_t Size, extent_t BorderSize> inline area_t(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<T> value, f32 distance) { recalculate(zone, position, value, distance); }

		template<typename T, extent_t Size, extent_t BorderSize> inline ref<area_t> recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value) {
			clear();

			for (extent_t::scalar_t y{ 0 }; y < Size.h; ++y) {
				for (extent_t::scalar_t x{ 0 }; x < Size.w; ++x) {
					if (zone[x, y] != value) {
						continue;
					}

					emplace(x, y);
				}
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline ref<area_t> recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value) {
			clear();

			for (extent_t::scalar_t y{ 0 }; y < Size.h; ++y) {
				for (extent_t::scalar_t x{ 0 }; x < Size.w; ++x) {
					if (zone[x, y] != value) {
						continue;
					}

					emplace(x, y);
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize> inline ref<area_t> recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<T> value, bool inclusive = false) {
			clear();

			if (!zone.within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			std::queue<offset_t> frontier{};
			frontier.push(position);

			while (!frontier.empty()) {
				const offset_t current{ frontier.front() };
				frontier.pop();

				insert(current);

				for (offset_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_t neighbour{ current.x + x, current.y + y };

						if (!zone.within<zone_region_t::All>(neighbour) || contains(neighbour)) {
							continue;
						}

						if (inclusive && zone[neighbour] != value) {
							insert(neighbour);
							continue;
						}

						frontier.push(neighbour);
					}
				}
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline ref<area_t> recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<U> value, bool inclusive = false) {
			clear();

			if (!zone.within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			std::queue<offset_t> frontier{};
			frontier.push(position);

			while (!frontier.empty()) {
				const offset_t current{ frontier.front() };
				frontier.pop();

				insert(current);

				for (offset_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_t neighbour{ current.x + x, current.y + y };

						if (!zone.within<zone_region_t::All>(neighbour) || contains(neighbour)) {
							continue;
						}

						if (inclusive && zone[neighbour] != value) {
							insert(neighbour);
							continue;
						}

						frontier.push(neighbour);
					}
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize> inline ref<area_t> recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<T> value, cref<extent_t::product_t> distance, bool inclusive = false) {
			clear();

			if (!zone.within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			std::queue<creeper_t<f32>> frontier{};
			frontier.push({ position, 0.0f });

			while (!frontier.empty()) {
				const creeper_t current{ frontier.front() };
				frontier.pop();

				insert(current.position);

				for (offset_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_t neighbour{ current.position.x + x, current.position.y + y };

						if (!zone.within<zone_region_t::All>(neighbour) || contains(neighbour) || current.distance >= distance) {
							continue;
						}

						if (inclusive && zone[neighbour] != value) {
							insert(neighbour);
							continue;
						}

						frontier.push({ neighbour, x != 0 && y != 0 ? current.distance + PlanarDiagonalDistance<extent_t::product_t> : current.distance + PlanarDistance<extent_t::product_t> });
					}
				}
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline ref<area_t> recalculate(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<U> value, cref<extent_t::product_t> distance, bool inclusive = false) {
			clear();

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			std::queue<creeper_t<f32>> frontier{};
			frontier.push({ position, 0.0f });

			while (!frontier.empty()) {
				const creeper_t current{ frontier.front() };
				frontier.pop();

				insert(current.position);

				for (offset_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_t neighbour{ current.position.x + x, current.position.y + y };

						if (!zone.template within<zone_region_t::All>(neighbour) || contains(neighbour) || current.distance >= distance) {
							continue;
						}

						if (inclusive && zone[neighbour] != value) {
							insert(neighbour);
							continue;
						}

						frontier.push({ neighbour, x != 0 && y != 0 ? current.distance + PlanarDiagonalDistance<extent_t::product_t> : current.distance + PlanarDistance<extent_t::product_t> });
					}
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize> inline ref<area_t> shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<T> value, f64 radius) {
			clear();

			if (!zone.within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
				for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
					const offset_t neighbour{ position.x + offs_x, position.y + offs_y };

					if (zone.within<zone_region_t::All>(neighbour) && zone[neighbour] == value) {
						insert(neighbour);
					}
				}
			}

			for (cref<octant_t> octant : Octants) {
				shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius);
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline ref<area_t> shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<U> value, f64 radius) {
			clear();

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
				for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
					const offset_t neighbour{ position.x + offs_x, position.y + offs_y };

					if (zone.template within<zone_region_t::All>(neighbour) && zone[neighbour] == value) {
						insert(neighbour);
					}
				}
			}

			for (cref<octant_t> octant : Octants) {
				shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize> inline ref<area_t> shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<T> value, f64 radius, f64 angle, f64 span) {
			clear();

			if (!zone.within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			bool is_nan = std::isnan(angle) || std::isnan(span);

			if (!is_nan) {
				angle = (angle > 360.0 || angle < 0.0 ? wrap(angle, 360.0) : angle) * PercentOfCircle;
				span *= PercentOfCircle;
			}

			for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
				for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
					const offset_t neighbour{ position.x + offs_x, position.y + offs_y };

					if (zone.within<zone_region_t::All>(neighbour) && zone[neighbour] == value) {
						insert(neighbour);
					}
				}
			}

			for (cref<octant_t> octant : Octants) {
				is_nan ? shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius) : shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius, angle, span);
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline ref<area_t> shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> position, cref<U> value, f64 radius, f64 angle, f64 span) {
			clear();

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			bool is_nan = std::isnan(angle) || std::isnan(span);

			if (!is_nan) {
				angle = (angle > 360.0 || angle < 0.0 ? wrap(angle, 360.0) : angle) * PercentOfCircle;
				span *= PercentOfCircle;
			}

			for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
				for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
					const offset_t neighbour{ position.x + offs_x, position.y + offs_y };

					if (zone.template within<zone_region_t::All>(neighbour) && zone[neighbour] == value) {
						insert(neighbour);
					}
				}
			}

			for (cref<octant_t> octant : Octants) {
				is_nan ? shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius) : shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius, angle, span);
			}

			return *this;
		}

		inline bool contains(cref<offset_t> position) const noexcept { return find(position) != end(); }

		template<typename T, extent_t Size, extent_t BorderSize> inline cref<area_t> set(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (cref<offset_t> position : *this) {
				zone[position] = value;
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires std::is_assignable<T, U>::value
		inline cref<area_t> set(ref<zone_t<T, Size, BorderSize>> zone, cref<U> value) const noexcept {
			for (cref<offset_t> position : *this) {
				zone[position] = value;
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize>
			requires is_operable_unary<T, operator_t::Addition>::value
		inline cref<area_t> apply(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (cref<offset_t> position : *this) {
				zone[position] += value;
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_operable<T, U, operator_t::Addition>::value
		inline cref<area_t> apply(ref<zone_t<T, Size, BorderSize>> zone, cref<U> value) const noexcept {
			for (cref<offset_t> position : *this) {
				zone[position] += value;
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, typename... Params>
			requires(is_operable<T, Params, operator_t::Addition>::value, ...) && is_plurary<Params...>::value
		inline cref<area_t> apply(ref<zone_t<T, Size, BorderSize>> zone, cref<Params>... values) const noexcept {
			for (cref<offset_t> position : *this) {
				for (crauto value : { values... }) {
					zone[position] += value;
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize>
			requires is_operable_unary<T, operator_t::Subtraction>::value
		inline cref<area_t> repeal(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (cref<offset_t> position : *this) {
				zone[position] -= value;
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_operable<T, U, operator_t::Subtraction>::value
		inline cref<area_t> repeal(ref<zone_t<T, Size, BorderSize>> zone, cref<U> value) const noexcept {
			for (cref<offset_t> position : *this) {
				zone[position] -= value;
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, typename... Params>
			requires(is_operable<T, Params, operator_t::Subtraction>::value, ...) && is_plurary<Params...>::value
		inline cref<area_t> repeal(ref<zone_t<T, Size, BorderSize>> zone, cref<Params>... values) const noexcept {
			for (cref<offset_t> position : *this) {
				for (crauto value : { values... }) {
					zone[position] -= value;
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, typename Generator>
		inline cref<area_t> randomize(ref<zone_t<T, Size, BorderSize>> zone, ref<Generator> generator, f64 probability, cref<binary_applicator_t<T>> applicator) const noexcept
			requires is_random_engine<Generator>::value
		{
			std::bernoulli_distribution dis{ probability };

			for (cref<offset_t> position : *this) {
				zone[position] = applicator(generator, dis);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, typename Generator>
		inline cref<area_t> randomize(ref<zone_t<T, Size, BorderSize>> zone, ref<Generator> generator, f64 probability, cref<T> true_value, cref<T> false_value) const noexcept
			requires is_random_engine<Generator>::value
		{
			std::bernoulli_distribution dis{ probability };

			for (cref<offset_t> position : *this) {
				zone[position] = dis(generator) ? true_value : false_value;
			}

			return *this;
		}

	  private:
		template<typename T, extent_t Size, extent_t BorderSize> inline void shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> origin, cref<T> value, i32 row, f64 start, f64 end, cref<octant_t> octant, f64 radius) {
			if (start < end) {
				return;
			}

			f64 new_start{ 0.0 };

			bool blocked{ false };

			for (f64 distance{ static_cast<f64>(row) }; distance <= radius && distance < zone.zone_area && !blocked; ++distance) {
				f64 dy = -distance;

				for (f64 dx = -distance; dx <= 0.0; ++dx) {
					const offset_t delta{ dx, dy };

					const offset_t position{ origin.x + delta.x * octant.position.x + delta.y * octant.delta.x, origin.y + delta.x * octant.position.y + delta.y * octant.delta.y };

					f64 left_slope{ (dx - 0.5) / (dy + 0.5) };
					f64 right_slope{ (dx + 0.5) / (dy - 0.5) };

					if (!zone.within<zone_region_t::All>(position) || start < right_slope) {
						continue;
					}

					if (end > left_slope) {
						break;
					}

					const f64 delta_radius{ std::sqrt(dx * dx + dy * dy) };

					if (delta_radius <= radius) {
						insert(position);
					}

					if (blocked) {
						if (zone[position] != value) {
							new_start = right_slope;
						} else {
							blocked = false;
							start = new_start;
						}
					} else if (zone[position] != value && distance < radius) {
						blocked = true;

						shadow_cast(zone, origin, value, static_cast<i32>(distance) + 1, start, left_slope, octant, radius);

						new_start = right_slope;
					}
				}
			}
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline void shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> origin, cref<U> value, i32 row, f64 start, f64 end, cref<octant_t> octant, f64 radius) {
			if (start < end) {
				return;
			}

			f64 new_start{ 0.0 };

			bool blocked{ false };

			for (f64 distance{ static_cast<f64>(row) }; distance <= radius && distance < zone.zone_area && !blocked; ++distance) {
				f64 dy = -distance;

				for (f64 dx = -distance; dx <= 0.0; ++dx) {
					const offset_t delta{ dx, dy };

					const offset_t position{ origin.x + delta.x * octant.position.x + delta.y * octant.delta.x, origin.y + delta.x * octant.position.y + delta.y * octant.delta.y };

					f64 left_slope{ (dx - 0.5) / (dy + 0.5) };
					f64 right_slope{ (dx + 0.5) / (dy - 0.5) };

					if (!zone.template within<zone_region_t::All>(position) || start < right_slope) {
						continue;
					}

					if (end > left_slope) {
						break;
					}

					const f64 delta_radius{ std::sqrt(dx * dx + dy * dy) };

					if (delta_radius <= radius) {
						insert(position);
					}

					if (blocked) {
						if (zone[position] != value) {
							new_start = right_slope;
						} else {
							blocked = false;
							start = new_start;
						}
					} else if (zone[position] != value && distance < radius) {
						blocked = true;

						shadow_cast(zone, origin, value, static_cast<i32>(distance) + 1, start, left_slope, octant, radius);

						new_start = right_slope;
					}
				}
			}

			return;
		}

		template<typename T, extent_t Size, extent_t BorderSize>
		inline void shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> origin, cref<T> value, i32 row, f64 start, f64 end, cref<octant_t> octant, f64 radius, f64 angle, f64 span) {
			if (start < end) {
				return;
			}

			f64 new_start{ 0.0 };

			bool blocked{ false };

			for (f64 distance{ static_cast<f64>(row) }; distance <= radius && distance < zone.zone_area && !blocked; ++distance) {
				f64 dy = -distance;

				for (f64 dx = -distance; dx <= 0.0; ++dx) {
					const offset_t delta{ dx, dy };

					const offset_t position{ origin.x + delta.x * octant.position.x + delta.y * octant.delta.x, origin.y + delta.x * octant.position.y + delta.y * octant.delta.y };

					f64 left_slope{ (dx - 0.5) / (dy + 0.5) };
					f64 right_slope{ (dx + 0.5) / (dy - 0.5) };

					if (!zone.within<zone_region_t::All>(position) || start < right_slope) {
						continue;
					}

					if (end > left_slope) {
						break;
					}

					const f64 delta_radius{ std::sqrt(dx * dx + dy * dy) };
					const f64 result{ std::abs(angle - atan2<f64>(delta.y, delta.x)) };

					if (delta_radius <= radius && (result <= span * 0.5 || result >= 1.0 - span * 0.5)) {
						insert(position);
					}

					if (blocked) {
						if (zone[position] != value) {
							new_start = right_slope;
						} else {
							blocked = false;
							start = new_start;
						}
					} else if (zone[position] != value && distance < radius) {
						blocked = true;

						shadow_cast(zone, origin, value, static_cast<i32>(distance) + 1, start, left_slope, octant, radius, angle, span);

						new_start = right_slope;
					}
				}
			}

			return;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline void shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<offset_t> origin, cref<U> value, i32 row, f64 start, f64 end, cref<octant_t> octant, f64 radius, f64 angle, f64 span) {
			if (start < end) {
				return;
			}

			f64 new_start{ 0.0 };

			bool blocked{ false };

			for (f64 distance{ static_cast<f64>(row) }; distance <= radius && distance < zone.zone_area && !blocked; ++distance) {
				f64 dy = -distance;

				for (f64 dx = -distance; dx <= 0.0; ++dx) {
					const offset_t delta{ dx, dy };

					const offset_t position{ origin.x + delta.x * octant.position.x + delta.y * octant.delta.x, origin.y + delta.x * octant.position.y + delta.y * octant.delta.y };

					f64 left_slope{ (dx - 0.5) / (dy + 0.5) };
					f64 right_slope{ (dx + 0.5) / (dy - 0.5) };

					if (!zone.template within<zone_region_t::All>(position) || start < right_slope) {
						continue;
					}

					if (end > left_slope) {
						break;
					}

					const f64 delta_radius{ std::sqrt(dx * dx + dy * dy) };
					const f64 result{ std::abs(angle - atan2<f64>(delta.y, delta.x)) };

					if (delta_radius <= radius && (result <= span * 0.5 || result >= 1.0 - span * 0.5)) {
						insert(position);
					}

					if (blocked) {
						if (zone[position] != value) {
							new_start = right_slope;
						} else {
							blocked = false;
							start = new_start;
						}
					} else if (zone[position] != value && distance < radius) {
						blocked = true;

						shadow_cast(zone, origin, value, static_cast<i32>(distance) + 1, start, left_slope, octant, radius, angle, span);

						new_start = right_slope;
					}
				}
			}

			return;
		}
	};
} // namespace bleak
