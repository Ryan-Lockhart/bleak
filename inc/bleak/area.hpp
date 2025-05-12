#pragma once

#include <bleak/typedef.hpp>

#include <cmath>
#include <queue>
#include <random>
#include <unordered_set>
#include <vector>

#include <bleak/arc.hpp>
#include <bleak/circle.hpp>
#include <bleak/concepts.hpp>
#include <bleak/extent.hpp>
#include <bleak/octant.hpp>
#include <bleak/offset.hpp>
#include <bleak/zone.hpp>

#include <bleak/constants/numeric.hpp>
#include <bleak/constants/octants.hpp>

namespace bleak {
	struct area_t : public std::unordered_set<offset_t, offset_t::hasher> {
		using underlying_t = std::unordered_set<offset_t, offset_t::hasher>;

		inline area_t() noexcept {}

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false> inline ref<area_t> collect(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value) {
			if constexpr (!Defer) {
				clear();
			}

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

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> collect(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value) {
			if constexpr (!Defer) {
				clear();
			}

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

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false> inline ref<area_t> flood(cref<zone_t<T, Size, BorderSize>> zone, offset_t position, cref<T> value, bool inclusive = false) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			std::queue<offset_t> frontier{};

			frontier.push(position);
			insert(position);

			while (!frontier.empty()) {
				const offset_t current{ frontier.front() };
				frontier.pop();

				for (offset_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_t neighbour{ current.x + x, current.y + y };

						if (!zone.template within<zone_region_t::All>(neighbour) || contains(neighbour)) {
							continue;
						}

						if (inclusive && zone[neighbour] != value) {
							insert(neighbour);
							continue;
						} else if (zone[neighbour] != value) {
							continue;
						}

						frontier.push(neighbour);
						insert(current);
					}
				}
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> flood(cref<zone_t<T, Size, BorderSize>> zone, offset_t position, cref<U> value, bool inclusive = false) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			std::queue<offset_t> frontier{};

			frontier.push(position);
			insert(position);

			while (!frontier.empty()) {
				const offset_t current{ std::move(frontier.front()) };
				frontier.pop();

				for (offset_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_t neighbour{ current.x + x, current.y + y };

						if (!zone.template within<zone_region_t::All>(neighbour) || contains(neighbour)) {
							continue;
						}

						if (inclusive && zone[neighbour] != value) {
							insert(neighbour);
							continue;
						} else if (zone[neighbour] != value) {
							continue;
						}

						frontier.push(neighbour);
						insert(neighbour);
					}
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false>
		inline ref<area_t> flood(cref<zone_t<T, Size, BorderSize>> zone, offset_t position, cref<T> value, cref<extent_t::product_t> distance, bool inclusive = false) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			std::queue<creeper_t<f32>> frontier{};

			frontier.push({ position, 0.0f });
			insert(position);

			while (!frontier.empty()) {
				const creeper_t current{ frontier.front() };
				frontier.pop();

				for (offset_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_t neighbour{ current.position.x + x, current.position.y + y };

						if (!zone.template within<zone_region_t::All>(neighbour) || contains(neighbour) || current.distance > distance) {
							continue;
						}

						if (inclusive && zone[neighbour] != value) {
							insert(neighbour);
							continue;
						} else if (zone[neighbour] != value) {
							continue;
						}

						frontier.push({ neighbour, x != 0 && y != 0 ? current.distance + PlanarDiagonalDistance<extent_t::product_t> : current.distance + PlanarDistance<extent_t::product_t> });
						insert(neighbour);
					}
				}
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> flood(cref<zone_t<T, Size, BorderSize>> zone, offset_t position, cref<U> value, cref<extent_t::product_t> distance, bool inclusive = false) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			std::queue<creeper_t<f32>> frontier{};

			frontier.push({ position, 0.0f });
			insert(position);

			while (!frontier.empty()) {
				const creeper_t current{ frontier.front() };
				frontier.pop();

				for (offset_t::scalar_t y{ -1 }; y <= 1; ++y) {
					for (offset_t::scalar_t x{ -1 }; x <= 1; ++x) {
						if (x == 0 && y == 0) {
							continue;
						}

						const offset_t neighbour{ current.position.x + x, current.position.y + y };

						if (!zone.template within<zone_region_t::All>(neighbour) || contains(neighbour) || current.distance > distance) {
							continue;
						}

						if (inclusive && zone[neighbour] != value) {
							insert(neighbour);
							continue;
						} else if (zone[neighbour] != value) {
							continue;
						}

						frontier.push({ neighbour, x != 0 && y != 0 ? current.distance + PlanarDiagonalDistance<extent_t::product_t> : current.distance + PlanarDistance<extent_t::product_t> });
						insert(neighbour);
					}
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false> inline ref<area_t> cast(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value, offset_t position, u32 radius, bool inclusive = false) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			if (radius == 0) {
				insert(position);
				return *this;
			}

			if (inclusive) {
				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ position + offset };

						if (zone.template within<zone_region_t::All>(neighbour)) {
							insert(neighbour);
						}
					}
				}
			} else {
				insert(position);
			}

			if (radius == 1) {
				return *this;
			}

			for (cref<octant_t> octant : Octants) {
				shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius);
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> cast(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value, offset_t position, u32 radius, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			if (radius == 0) {
				insert(position);
				return *this;
			}

			if (inclusive) {
				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ position + offset };

						if (zone.template within<zone_region_t::All>(neighbour)) {
							insert(neighbour);
						}
					}
				}
			} else {
				insert(position);
			}

			if (radius == 1) {
				return *this;
			}

			for (cref<octant_t> octant : Octants) {
				shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false> inline ref<area_t> cast(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value, cref<circle_t> circle, bool inclusive = false) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(circle.position) || zone[circle.position] != value) {
				return *this;
			}

			if (circle.radius == 0) {
				insert(circle.position);
				return *this;
			}

			if (inclusive) {
				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ circle.position + offset };

						if (zone.template within<zone_region_t::All>(neighbour)) {
							insert(neighbour);
						}
					}
				}
			} else {
				insert(circle.position);
			}

			if (circle.radius == 1) {
				return *this;
			}

			for (cref<octant_t> octant : Octants) {
				shadow_cast(zone, circle.position, value, 1, 1.0, 0.0, octant, circle.radius);
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> cast(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value, cref<circle_t> circle, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(circle.position) || zone[circle.position] != value) {
				return *this;
			}

			if (circle.radius == 0) {
				insert(circle.position);
				return *this;
			}

			if (inclusive) {
				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ circle.position + offset };

						if (zone.template within<zone_region_t::All>(neighbour)) {
							insert(neighbour);
						}
					}
				}
			} else {
				insert(circle.position);
			}

			if (circle.radius == 1) {
				return *this;
			}

			for (cref<octant_t> octant : Octants) {
				shadow_cast(zone, circle.position, value, 1, 1.0, 0.0, octant, circle.radius);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false> inline ref<area_t> cast(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value, offset_t position, u32 radius, f64 angle, f64 span, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			if (radius == 0) {
				insert(position);
				return *this;
			}

			bool is_nan = std::isnan(angle) || std::isnan(span);

			if (!is_nan) {
				angle -= 90.0;
				angle = (angle > 360.0 || angle < 0.0 ? wrap(angle, 360.0) : angle) * PercentOfCircle;
				span *= PercentOfCircle;
			}

			if (inclusive) {
				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ position + offset };

						if (zone.template within<zone_region_t::All>(neighbour)) {
							insert(neighbour);
						}
					}
				}
			} else {
				insert(position);
			}

			if (radius == 1) {
				return *this;
			}

			for (cref<octant_t> octant : Octants) {
				is_nan ? shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius) : shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius, angle, span);
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> cast(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value, offset_t position, u32 radius, f64 angle, f64 span, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(position) || zone[position] != value) {
				return *this;
			}

			if (radius == 0) {
				insert(position);
				return *this;
			}

			bool is_nan = std::isnan(angle) || std::isnan(span);

			if (!is_nan) {
				angle -= 90.0;
				angle = (angle > 360.0 || angle < 0.0 ? wrap(angle, 360.0) : angle) * PercentOfCircle;
				span *= PercentOfCircle;
			}

			if (inclusive) {
				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ position + offset };

						if (zone.template within<zone_region_t::All>(neighbour)) {
							insert(neighbour);
						}
					}
				}
			} else {
				insert(position);
			}

			if (radius == 1) {
				return *this;
			}

			for (cref<octant_t> octant : Octants) {
				is_nan ? shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius) : shadow_cast(zone, position, value, 1, 1.0, 0.0, octant, radius, angle, span);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false> inline ref<area_t> cast(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value, cref<arc_t> arc, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(arc.position) || zone[arc.position] != value) {
				return *this;
			}

			if (arc.radius == 0) {
				insert(arc.position);
				return *this;
			}

			bool is_nan = std::isnan(arc.angle) || std::isnan(arc.span);

			f64 angle{ arc.angle };
			f64 span{ arc.span };

			if (!is_nan) {
				angle -= 90.0;
				angle = (angle > 360.0 || angle < 0.0 ? wrap(angle, 360.0) : angle) * PercentOfCircle;
				span *= PercentOfCircle;
			}

			if (inclusive) {
				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ arc.position + offset };

						if (zone.template within<zone_region_t::All>(neighbour)) {
							insert(neighbour);
						}
					}
				}
			} else {
				insert(arc.position);
			}

			if (arc.radius == 1) {
				return *this;
			}

			for (cref<octant_t> octant : Octants) {
				is_nan ? shadow_cast(zone, arc.position, value, 1, 1.0, 0.0, octant, arc.radius) : shadow_cast(zone, arc.position, value, 1, 1.0, 0.0, octant, arc.radius, angle, span);
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> cast(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value, cref<arc_t> arc, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(arc.position) || zone[arc.position] != value) {
				return *this;
			}

			if (arc.radius == 0) {
				insert(arc.position);
				return *this;
			}

			bool is_nan = std::isnan(arc.angle) || std::isnan(arc.span);

			f64 angle{ arc.angle };
			f64 span{ arc.span };

			if (!is_nan) {
				angle -= 90.0;
				angle = (angle > 360.0 || angle < 0.0 ? wrap(angle, 360.0) : angle) * PercentOfCircle;
				span *= PercentOfCircle;
			}

			if (inclusive) {
				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ arc.position + offset };

						if (zone.template within<zone_region_t::All>(neighbour)) {
							insert(neighbour);
						}
					}
				}
			} else {
				insert(arc.position);
			}

			if (arc.radius == 1) {
				return *this;
			}

			for (cref<octant_t> octant : Octants) {
				is_nan ? shadow_cast(zone, arc.position, value, 1, 1.0, 0.0, octant, arc.radius) : shadow_cast(zone, arc.position, value, 1, 1.0, 0.0, octant, arc.radius, angle, span);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false> inline ref<area_t> multi_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value, cref<std::vector<circle_t>> circles, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			for (crauto circle : circles) {
				cast<T, Size, BorderSize, true>(zone, value, circle, inclusive);
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> multi_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value, cref<std::vector<circle_t>> circles, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			for (crauto circle : circles) {
				cast<T, U, Size, BorderSize, true>(zone, value, circle, inclusive);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, bool Defer = false> inline ref<area_t> multi_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value, cref<std::vector<arc_t>> arcs, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			for (crauto arc : arcs) {
				cast<T, Size, BorderSize, true>(zone, value, arc, inclusive);
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize, bool Defer = false>
			requires is_equatable<T, U>::value
		inline ref<area_t> multi_cast(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value, cref<std::vector<arc_t>> arcs, bool inclusive) {
			if constexpr (!Defer) {
				clear();
			}

			for (crauto arc : arcs) {
				cast<T, U, Size, BorderSize, true>(zone, value, arc, inclusive);
			}

			return *this;
		}

		inline bool contains(offset_t position) const noexcept { return find(position) != end(); }

		template<typename T, extent_t Size, extent_t BorderSize> inline cref<area_t> set(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (offset_t position : *this) {
				zone[position] = value;
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires std::is_assignable<T, U>::value
		inline cref<area_t> set(ref<zone_t<T, Size, BorderSize>> zone, cref<U> value) const noexcept {
			for (offset_t position : *this) {
				zone[position] = value;
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize>
			requires is_operable_unary<T, operator_t::Addition>::value
		inline cref<area_t> apply(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (offset_t position : *this) {
				zone[position] += value;
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_operable<T, U, operator_t::Addition>::value
		inline cref<area_t> apply(ref<zone_t<T, Size, BorderSize>> zone, cref<U> value) const noexcept {
			for (offset_t position : *this) {
				zone[position] += value;
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, typename... Params>
			requires(is_operable<T, Params, operator_t::Addition>::value, ...) && is_plurary<Params...>::value
		inline cref<area_t> apply(ref<zone_t<T, Size, BorderSize>> zone, cref<Params>... values) const noexcept {
			for (offset_t position : *this) {
				for (crauto value : { values... }) {
					zone[position] += value;
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize>
			requires is_operable_unary<T, operator_t::Subtraction>::value
		inline cref<area_t> repeal(ref<zone_t<T, Size, BorderSize>> zone, cref<T> value) const noexcept {
			for (offset_t position : *this) {
				zone[position] -= value;
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_operable<T, U, operator_t::Subtraction>::value
		inline cref<area_t> repeal(ref<zone_t<T, Size, BorderSize>> zone, cref<U> value) const noexcept {
			for (offset_t position : *this) {
				zone[position] -= value;
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, typename... Params>
			requires(is_operable<T, Params, operator_t::Subtraction>::value, ...) && is_plurary<Params...>::value
		inline cref<area_t> repeal(ref<zone_t<T, Size, BorderSize>> zone, cref<Params>... values) const noexcept {
			for (offset_t position : *this) {
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

			for (offset_t position : *this) {
				zone[position] = applicator(generator, dis);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize, typename Generator>
		inline cref<area_t> randomize(ref<zone_t<T, Size, BorderSize>> zone, ref<Generator> generator, f64 probability, cref<T> true_value, cref<T> false_value) const noexcept
			requires is_random_engine<Generator>::value
		{
			std::bernoulli_distribution dis{ probability };

			for (offset_t position : *this) {
				zone[position] = dis(generator) ? true_value : false_value;
			}

			return *this;
		}

		inline ref<area_t> add(cref<area_t> other) {
			for (offset_t position : other) {
				insert(position);
			}

			return *this;
		}

		inline ref<area_t> remove(cref<area_t> other) {
			for (offset_t position : other) {
				erase(position);
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize> static std::vector<area_t> partition(cref<zone_t<T, Size, BorderSize>> zone, cref<T> value) {
			std::vector<area_t> partitions{};

			area_t values{};
			values.collect(zone, value);

			while (!values.empty()) {
				area_t partition{};

				partition.flood(zone, *values.begin(), value);
				values.remove(partition);

				partitions.push_back(std::move(partition));
			}

			return partitions;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		static std::vector<area_t> partition(cref<zone_t<T, Size, BorderSize>> zone, cref<U> value) {
			std::vector<area_t> partitions{};

			area_t values{};
			values.collect(zone, value);

			while (!values.empty()) {
				area_t partition{};

				partition.flood(zone, *values.begin(), value);
				values.remove(partition);

				partitions.push_back(std::move(partition));
			}

			return partitions;
		}

	  private:
		template<typename T, extent_t Size, extent_t BorderSize> inline void shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, offset_t origin, cref<T> value, i32 row, f64 start, f64 end, cref<octant_t> octant, f64 radius) {
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
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline void shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, offset_t origin, cref<U> value, i32 row, f64 start, f64 end, cref<octant_t> octant, f64 radius) {
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
		inline void shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, offset_t origin, cref<T> value, i32 row, f64 start, f64 end, cref<octant_t> octant, f64 radius, f64 angle, f64 span) {
			if (start < end) {
				return;
			}

			f64 new_start{ 0.0 };

			bool blocked{ false };

			for (f64 distance{ static_cast<f64>(row) }; distance <= radius && distance < zone.zone_area && !blocked; ++distance) {
				f64 dy = -distance;

				for (f64 dx = -distance; dx <= 0.0; ++dx) {
					const offset_t position{ origin.x + dx * octant.position.x + dy * octant.delta.x, origin.y + dx * octant.position.y + dy * octant.delta.y };
					const offset_t delta{ position - origin };

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

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline void shadow_cast(cref<zone_t<T, Size, BorderSize>> zone, offset_t origin, cref<U> value, i32 row, f64 start, f64 end, cref<octant_t> octant, f64 radius, f64 angle, f64 span) {
			if (start < end) {
				return;
			}

			f64 new_start{ 0.0 };

			bool blocked{ false };

			for (f64 distance{ static_cast<f64>(row) }; distance <= radius && distance < zone.zone_area && !blocked; ++distance) {
				f64 dy = -distance;

				for (f64 dx = -distance; dx <= 0.0; ++dx) {
					const offset_t position{ origin.x + dx * octant.position.x + dy * octant.delta.x, origin.y + dx * octant.position.y + dy * octant.delta.y };
					const offset_t delta{ position - origin };

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
