#pragma once

#include <bleak/typedef.hpp>

#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>

#include <bleak/atlas.hpp>
#include <bleak/concepts.hpp>
#include <bleak/creeper.hpp>
#include <bleak/extent.hpp>
#include <bleak/glyph.hpp>
#include <bleak/line.hpp>
#include <bleak/memory.hpp>
#include <bleak/offset.hpp>
#include <bleak/zone.hpp>

namespace bleak {
	class path_t {
	  private:
		std::stack<offset_t> points;

	  public:
		using creep_t = memory_creeper_t<offset_t::product_t>;

		inline path_t() noexcept : points{} {}

		inline ref<path_t> generate(cref<line_t> line) {
			if (!empty()) {
				clear();
			}

			if (line.start == line.end) {
				points.push(line.start);
				return *this;
			}

			offset_t pos{ line.start };

			offset_t delta{ std::abs(line.end.x - line.start.x), std::abs(line.end.y - line.start.y) };

			offset_t step{ line.start.x < line.end.x ? 1 : -1, line.start.y < line.end.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			for (;;) {
				points.push(line.start);

				if (pos == line.end) {
					break;
				}

				i32 e2 = 2 * err;

				if (e2 > -delta.y) {
					err -= delta.y;
					pos.x += step.x;
				}

				if (e2 < delta.x) {
					err += delta.x;
					pos.y += step.y;
				}
			}

			return *this;
		}

		template<typename T, extent_t Size, extent_t BorderSize> inline ref<path_t> generate(cref<line_t> line, cref<zone_t<T, Size, BorderSize>> zone, cref<T> value) {
			if (!empty()) {
				clear();
			}

			if (!zone.within<zone_region_t::All>(line.start) || zone[line.start] != value || !zone.within<zone_region_t::All>(line.end) || zone[line.end] != value || line.start == line.end) {
				return *this;
			}

			offset_t pos{ line.start };

			offset_t delta{ std::abs(line.end.x - line.start.x), std::abs(line.end.y - line.start.y) };

			offset_t step{ line.start.x < line.end.x ? 1 : -1, line.start.y < line.end.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			for (;;) {
				if (!zone.within<zone_region_t::All>(pos) || zone[pos] != value) {
					clear();
					return *this;
				}

				points.push(line.start);

				if (pos == line.end) {
					break;
				}

				i32 e2 = 2 * err;

				if (e2 > -delta.y) {
					err -= delta.y;
					pos.x += step.x;
				}

				if (e2 < delta.x) {
					err += delta.x;
					pos.y += step.y;
				}
			}

			return *this;
		}

		template<typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline ref<path_t> generate(cref<line_t> line, cref<zone_t<T, Size, BorderSize>> zone, cref<U> value) {
			if (!empty()) {
				clear();
			}

			if (!zone.within<zone_region_t::All>(line.start) || zone[line.start] != value || !zone.within<zone_region_t::All>(line.end) || zone[line.end] != value || line.start == line.end) {
				return *this;
			}

			offset_t pos{ line.start };

			offset_t delta{ std::abs(line.end.x - line.start.x), std::abs(line.end.y - line.start.y) };

			offset_t step{ line.start.x < line.end.x ? 1 : -1, line.start.y < line.end.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			for (;;) {
				if (!zone.within<zone_region_t::All>(pos) || zone[pos] != value) {
					clear();
					return *this;
				}

				points.push(line.start);

				if (pos == line.end) {
					break;
				}

				i32 e2 = 2 * err;

				if (e2 > -delta.y) {
					err -= delta.y;
					pos.x += step.x;
				}

				if (e2 < delta.x) {
					err += delta.x;
					pos.y += step.y;
				}
			}

			return *this;
		}

		template<distance_function_t Distance, typename T, extent_t Size, extent_t BorderSize> inline ref<path_t> generate(cref<offset_t> origin, cref<offset_t> destination, cref<zone_t<T, Size, BorderSize>> zone, cref<T> value) {
			if (!empty()) {
				clear();
			}

			if (!zone.within<zone_region_t::All>(origin) || zone[origin] != value || !zone.within<zone_region_t::All>(destination) || zone[destination] != value || origin == destination) {
				return *this;
			}

			std::priority_queue<creep_t, std::vector<creep_t>, creep_t::less> frontier{};
			std::unordered_set<offset_t, offset_t::hasher> visited{};

			frontier.push(creep_t{ origin, origin, offset_t::distance<Distance>(origin, destination) });
			visited.insert(origin);

			std::unordered_map<offset_t, rememberance_t<offset_t>, offset_t::hasher> trail{};

			while (!frontier.empty()) {
				const creep_t current{ frontier.top() };
				frontier.pop();

				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						if (offs_x == 0 && offs_y == 0) {
							continue;
						}

						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ current.position.current + offset };
						cauto distance{ offset_t::distance<Distance>(neighbour, destination) };

						if (!zone.within<zone_region_t::All>(neighbour) || zone[neighbour] != value || distance > current.distance || !visited.insert(neighbour).second) {
							continue;
						}

						if (neighbour == destination) {
							rememberance_t<offset_t> unwind{ trail[destination] };

							while (unwind.current != origin) {
								points.push(unwind.current);
								unwind = trail[unwind.previous];
							}

							points.push(origin);

							return *this;
						}

						frontier.push(creep_t{ neighbour, current.position.current, distance });
						trail.emplace(neighbour, current.position.perpetuate(neighbour));
					}
				}
			}

			return *this;
		}

		template< distance_function_t Distance, typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline ref<path_t> generate(cref<offset_t> origin, cref<offset_t> destination, cref<zone_t<T, Size, BorderSize>> zone, cref<U> value) {
			if (!empty()) {
				clear();
			}

			if (!zone.template within<zone_region_t::All>(origin) || zone[origin] != value || !zone.template within<zone_region_t::All>(destination) || zone[destination] != value || origin == destination) {
				return *this;
			}

			std::priority_queue<creep_t, std::vector<creep_t>, creep_t::less> frontier{};
			std::unordered_set<offset_t, offset_t::hasher> visited{};

			frontier.emplace(origin, origin, offset_t::distance<Distance>(origin, destination));
			visited.insert(origin);

			std::unordered_map<offset_t, rememberance_t<offset_t>, offset_t::hasher> trail{};

			trail.emplace(origin, origin);

			while (!frontier.empty()) {
				const creep_t current{ frontier.top() };
				frontier.pop();

				for (offset_t::scalar_t offs_y{ -1 }; offs_y <= 1; ++offs_y) {
					for (offset_t::scalar_t offs_x{ -1 }; offs_x <= 1; ++offs_x) {
						if (offs_x == 0 && offs_y == 0) {
							continue;
						}

						const offset_t offset{ offs_x, offs_y };
						const offset_t neighbour{ current.position.current + offset };
						const offset_t::product_t distance{ offset_t::distance<Distance>(neighbour, destination) };

						if (!zone.template within<zone_region_t::All>(neighbour) || zone[neighbour] != value || distance > current.distance || visited.contains(neighbour)) {
							visited.insert(neighbour);
							continue;
						}

						if (neighbour == destination) {
							rememberance_t<offset_t> unwind{ trail[current.position.current] };

							points.push(destination);

							while (unwind.current != origin) {
								points.push(unwind.current);
								unwind = trail[unwind.previous];
							}

							points.push(origin);

							return *this;
						}

						const rememberance_t<offset_t> memory{ neighbour, current.position.current };

						frontier.emplace(memory, distance);
						trail.emplace(neighbour, memory);
						visited.insert(neighbour);
					}
				}
			}

			return *this;
		}

		inline bool empty() const { return points.empty(); }

		inline usize size() const { return points.size(); }

		inline cref<offset_t> top() const { return points.top(); }

		inline void push(cref<offset_t> point) { points.push(point); }

		inline void push(rval<offset_t> point) { points.push(std::move(point)); }

		inline void pop() { points.pop(); }

		inline void emplace(offset_t::scalar_t x, offset_t::scalar_t y) { points.emplace(x, y); }

		inline rval<offset_t> extract() {
			rval<offset_t> point = std::move(points.top());
			points.pop();
			return std::move(point);
		}

		inline void clear() {
			while (points.size() > 0) {
				points.pop();
			}
		}

		inline void reverse() {
			std::stack<offset_t> reversed;

			while (points.size() > 0) {
				reversed.push(points.top());
				points.pop();
			}

			std::swap(points, reversed);
		}

		template<extent_t AtlasSize> inline void draw(cref<atlas_t<AtlasSize>> atlas, cref<glyph_t> glyph, cref<offset_t> offset) const {
			if (points.empty()) {
				return;
			}

			std::stack<offset_t> copy{ points };

			while (copy.size() > 0) {
				const offset_t point{ std::move(copy.top()) };

				atlas.draw(glyph, point + offset);

				copy.pop();
			}
		}
	};
} // namespace bleak
