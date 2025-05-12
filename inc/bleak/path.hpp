#pragma once

#include <bleak/typedef.hpp>

#include <queue>
#include <stack>
#include <unordered_map>
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
	  public:
		using creep_t = memory_creeper_t<offset_t::product_t>;

		using frontier_t = std::priority_queue<creep_t, std::vector<creep_t>, creep_t::less>;
		using sparse_t = std::unordered_set<offset_t, offset_t::hasher>;
		using trail_t = std::unordered_map<offset_t, rememberance_t<offset_t>, offset_t::hasher>;

		#define dense_t zone_t<T, Size, BorderSize>
		#define dense_args typename T, extent_t Size, extent_t BorderSize

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
				if (pos != line.start) {
					points.push(pos);
				}

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

		template<zone_region_t Region, dense_args>
		inline ref<path_t> generate(cref<line_t> line, cref<dense_t> zone, cref<T> value) {
			if (!empty()) {
				clear();
			}

			if (!is_valid(line.start, line.end, zone, value) || line.start == line.end) {
				return *this;
			}

			offset_t pos{ line.start };

			offset_t delta{ std::abs(line.end.x - line.start.x), std::abs(line.end.y - line.start.y) };

			offset_t step{ line.start.x < line.end.x ? 1 : -1, line.start.y < line.end.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			for (;;) {
				if (pos == line.end) {
					points.push(pos);
					break;
				}

				if (!is_valid<Region>(pos, zone, value)) {
					clear();
					return *this;
				}

				if (pos != line.start) {
					points.push(pos);
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

		template<zone_region_t Region, dense_args, typename U>
			requires is_equatable<T, U>::value
		inline ref<path_t> generate(cref<line_t> line, cref<dense_t> zone, cref<U> value) {
			if (!empty()) {
				clear();
			}

			if (!is_valid<Region>(line.start, line.end, zone, value) || line.start == line.end) {
				return *this;
			}

			offset_t pos{ line.start };

			offset_t delta{ std::abs(line.end.x - line.start.x), std::abs(line.end.y - line.start.y) };

			offset_t step{ line.start.x < line.end.x ? 1 : -1, line.start.y < line.end.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			for (;;) {
				if (pos == line.end) {
					points.push(pos);
					break;
				}

				if (!is_valid<Region>(pos, zone, value)) {
					clear();
					return *this;
				}
				
				if (pos != line.start) {
					points.push(pos);
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

		template<zone_region_t Region, bool Inclusive = false, dense_args>
		inline ref<path_t> generate(cref<line_t> line, cref<dense_t> zone, cref<T> value, cref<sparse_t> sparse_blockage) {
			if (!empty()) {
				clear();
			}

			if constexpr (Inclusive) {				
				if (!is_valid<Region>(line.start, line.end, zone, value) || line.start == line.end) {
					return *this;
				}
			} else {
				if (!is_valid<Region>(line.start, line.end, zone, value, sparse_blockage) || line.start == line.end) {
					return *this;
				}
			}

			offset_t pos{ line.start };

			offset_t delta{ std::abs(line.end.x - line.start.x), std::abs(line.end.y - line.start.y) };

			offset_t step{ line.start.x < line.end.x ? 1 : -1, line.start.y < line.end.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			for (;;) {
				if (pos == line.end) {
					points.push(pos);
					break;
				}

				if (pos != line.start && !is_valid<Region>(pos, zone, value, sparse_blockage)) {
					clear();
					return *this;
				}
				
				if (pos != line.start) {
					points.push(pos);
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

		template<zone_region_t Region, bool Inclusive = false, dense_args, typename U>
			requires is_equatable<T, U>::value
		inline ref<path_t> generate(cref<line_t> line, cref<dense_t> zone, cref<U> value, cref<sparse_t> sparse_blockage) {
			if (!empty()) {
				clear();
			}

			if constexpr (Inclusive) {				
				if (!is_valid<Region>(line.start, line.end, zone, value) || line.start == line.end) {
					return *this;
				}
			} else {
				if (!is_valid<Region>(line.start, line.end, zone, value, sparse_blockage) || line.start == line.end) {
					return *this;
				}
			}

			offset_t pos{ line.start };

			offset_t delta{ std::abs(line.end.x - line.start.x), std::abs(line.end.y - line.start.y) };

			offset_t step{ line.start.x < line.end.x ? 1 : -1, line.start.y < line.end.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			for (;;) {
				if (pos == line.end) {
					points.push(pos);
					break;
				}

				if (pos != line.start && !is_valid<Region>(pos, zone, value, sparse_blockage)) {
					clear();
					return *this;
				}
				
				if (pos != line.start) {
					points.push(pos);
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

		template<zone_region_t Region, distance_function_t Distance, dense_args>
		inline ref<path_t> generate(offset_t origin, offset_t destination, cref<dense_t> zone, cref<T> value) {
			if (!empty()) {
				clear();
			}

			if (!is_valid<Region>(origin, destination, zone, value)) {
				return *this;
			}

			frontier_t frontier{};
			sparse_t visited{};

			frontier.emplace(origin, origin, offset_t::distance<Distance>(origin, destination));
			visited.insert(origin);

			trail_t trail{};

			trail.emplace(origin, origin);

			while (!frontier.empty()) {
				const creep_t current_creeper{ frontier.top() };
				frontier.pop();

				visited.insert(current_creeper.position.current);

				if (current_creeper.position.current == destination) {
					unwind(origin, destination, trail);
					return *this;
				}

				for (crauto offset : neighbourhood_offsets<Distance>) {
					const offset_t neighbour{ current_creeper.position.current + offset };
					cauto distance{ offset_t::distance<Distance>(neighbour, destination) };

					if (neighbour != destination && (!is_valid<Region>(neighbour, zone, value, visited) || distance > current_creeper.distance)) {
						continue;
					}

					const rememberance_t<offset_t> memory{ neighbour, current_creeper.position.current };

					frontier.emplace(memory, distance);
					trail.emplace(neighbour, memory);
				}
			}

			return *this;
		}

		template<zone_region_t Region, distance_function_t Distance, dense_args, typename U>
			requires is_equatable<T, U>::value
		inline ref<path_t> generate(offset_t origin, offset_t destination, cref<dense_t> zone, cref<U> value) {
			if (!empty()) {
				clear();
			}
			
			if (!is_valid<Region>(origin, destination, zone, value)) {
				return *this;
			}

			frontier_t frontier{};
			sparse_t visited{};

			frontier.emplace(origin, origin, offset_t::distance<Distance>(origin, destination));
			visited.insert(origin);

			trail_t trail{};

			trail.emplace(origin, origin);

			while (!frontier.empty()) {
				const creep_t current_creeper{ frontier.top() };
				frontier.pop();

				visited.insert(current_creeper.position.current);

				if (current_creeper.position.current == destination) {
					unwind(origin, destination, trail);
					return *this;
				}

				for (crauto offset : neighbourhood_offsets<Distance>) {
					const offset_t neighbour{ current_creeper.position.current + offset };
					cauto distance{ offset_t::distance<Distance>(neighbour, destination) };

					if (neighbour != destination && (!is_valid<Region>(neighbour, zone, value, visited) || distance > current_creeper.distance)) {
						continue;
					}

					const rememberance_t<offset_t> memory{ neighbour, current_creeper.position.current };

					frontier.emplace(memory, distance);
					trail.emplace(neighbour, memory);
				}
			}

			return *this;
		}

		template<zone_region_t Region, distance_function_t Distance, bool Inclusive = false, dense_args>
		inline ref<path_t> generate(offset_t origin, offset_t destination, cref<dense_t> zone, cref<T> value, cref<sparse_t> sparse_blockage) {
			if (!empty()) {
				clear();
			}

			if constexpr (Inclusive) {				
				if (!is_valid<Region>(origin, destination, zone, value)) {
					return *this;
				}
			} else {
				if (!is_valid<Region>(origin, destination, zone, value, sparse_blockage)) {
					return *this;
				}
			}

			frontier_t frontier{};
			sparse_t visited{};

			frontier.emplace(origin, origin, offset_t::distance<Distance>(origin, destination));
			visited.insert(origin);

			trail_t trail{};

			trail.emplace(origin, origin);

			while (!frontier.empty()) {
				const creep_t current_creeper{ frontier.top() };
				frontier.pop();

				visited.insert(current_creeper.position.current);

				if (current_creeper.position.current == destination) {
					unwind(origin, destination, trail);
					return *this;
				}

				for (crauto offset : neighbourhood_offsets<Distance>) {
					const offset_t neighbour{ current_creeper.position.current + offset };
					cauto distance{ offset_t::distance<Distance>(neighbour, destination) };

					if (neighbour != destination && (!is_valid<Region>(neighbour, zone, value, visited, sparse_blockage) || distance > current_creeper.distance)) {
						continue;
					}

					const rememberance_t<offset_t> memory{ neighbour, current_creeper.position.current };

					frontier.emplace(memory, distance);
					trail.emplace(neighbour, memory);
				}
			}

			return *this;
		}

		template<zone_region_t Region, distance_function_t Distance, bool Inclusive = false, dense_args, typename U>
			requires is_equatable<T, U>::value
		inline ref<path_t> generate(offset_t origin, offset_t destination, cref<dense_t> zone, cref<U> value, cref<sparse_t> sparse_blockage) {
			if (!empty()) {
				clear();
			}

			if constexpr (Inclusive) {				
				if (!is_valid<Region>(origin, destination, zone, value)) {
					return *this;
				}
			} else {
				if (!is_valid<Region>(origin, destination, zone, value, sparse_blockage)) {
					return *this;
				}
			}

			frontier_t frontier{};
			sparse_t visited{};

			frontier.emplace(origin, origin, offset_t::distance<Distance>(origin, destination));
			visited.insert(origin);

			trail_t trail{};

			trail.emplace(origin, origin);

			while (!frontier.empty()) {
				const creep_t current_creeper{ frontier.top() };
				frontier.pop();

				visited.insert(current_creeper.position.current);

				if (current_creeper.position.current == destination) {
					unwind(origin, destination, trail);
					return *this;
				}

				for (crauto offset : neighbourhood_offsets<Distance>) {
					const offset_t neighbour{ current_creeper.position.current + offset };
					cauto distance{ offset_t::distance<Distance>(neighbour, destination) };

					if (neighbour != destination && (!is_valid<Region>(neighbour, zone, value, visited, sparse_blockage) || distance > current_creeper.distance)) {
						continue;
					}

					const rememberance_t<offset_t> memory{ neighbour, current_creeper.position.current };

					frontier.emplace(memory, distance);
					trail.emplace(neighbour, memory);
				}
			}

			return *this;
		}

		inline bool empty() const { return points.empty(); }

		inline usize size() const { return points.size(); }

		inline offset_t top() const { return points.top(); }

		inline void push(offset_t point) { points.push(point); }

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

		template<extent_t AtlasSize> inline void draw(cref<atlas_t<AtlasSize>> atlas, glyph_t glyph, offset_t offset) const {
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

	  private:
		std::stack<offset_t> points;

		template<zone_region_t Region, dense_args>
		inline bool is_valid(offset_t origin, offset_t destination, cref<dense_t> zone, cref<T> value) const {
			if (!zone.template within<Region>(origin) || !zone.template within<Region>(destination)) {
				return false;
			}

			if (zone[origin] != value || zone[destination] != value) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline bool is_valid(offset_t origin, offset_t destination, cref<dense_t> zone, cref<U> value) const {
			if (!zone.template within<Region>(origin) || !zone.template within<Region>(destination)) {
				return false;
			}

			if (zone[origin] != value || zone[destination] != value) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, dense_args>
		inline bool is_valid(offset_t origin, offset_t destination, cref<dense_t> zone, cref<T> value, cref<sparse_t> blockage) const {
			if (!zone.template within<Region>(origin) || !zone.template within<Region>(destination)) {
				return false;
			}

			if (zone[origin] != value || zone[destination] != value) {
				return false;
			}

			if (blockage.contains(origin) || blockage.contains(destination)) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, typename T, typename U, extent_t Size, extent_t BorderSize>
			requires is_equatable<T, U>::value
		inline bool is_valid(offset_t origin, offset_t destination, cref<dense_t> zone, cref<U> value, cref<sparse_t> blockage) const {
			if (!zone.template within<Region>(origin) || !zone.template within<Region>(destination)) {
				return false;
			}

			if (zone[origin] != value || zone[destination] != value) {
				return false;
			}

			if (blockage.contains(origin) || blockage.contains(destination)) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, dense_args>
		inline bool is_valid(offset_t position, cref<dense_t> zone, cref<T> value) const {
			if (!zone.template within<Region>(position)) {
				return false;
			}

			if (zone[position] != value) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, dense_args, typename U>
			requires is_equatable<T, U>::value
		inline bool is_valid(offset_t position, cref<dense_t> zone, cref<U> value) const {
			if (!zone.template within<Region>(position)) {
				return false;
			}

			if (zone[position] != value) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, dense_args>
		inline bool is_valid(offset_t position, cref<dense_t> zone, cref<T> value, cref<sparse_t> visited) const {
			if (!zone.template within<Region>(position)) {
				return false;
			}

			if (zone[position] != value) {
				return false;
			}

			if (visited.contains(position)) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, dense_args, typename U>
			requires is_equatable<T, U>::value
		inline bool is_valid(offset_t position, cref<dense_t> zone, cref<U> value, cref<sparse_t> visited) const {
			if (!zone.template within<Region>(position)) {
				return false;
			}

			if (zone[position] != value) {
				return false;
			}

			if (visited.contains(position)) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, dense_args>
		inline bool is_valid(offset_t position, cref<dense_t> zone, cref<T> value, cref<sparse_t> visited, cref<sparse_t> blockage) const {
			if (!zone.template within<Region>(position)) {
				return false;
			}

			if (zone[position] != value) {
				return false;
			}

			if (visited.contains(position)) {
				return false;
			}

			if (blockage.contains(position)) {
				return false;
			}

			return true;
		}

		template<zone_region_t Region, dense_args, typename U>
			requires is_equatable<T, U>::value
		inline bool is_valid(offset_t position, cref<dense_t> zone, cref<U> value, cref<sparse_t> visited, cref<sparse_t> blockage) const {
			if (!zone.template within<Region>(position)) {
				return false;
			}

			if (zone[position] != value) {
				return false;
			}

			if (visited.contains(position)) {
				return false;
			}

			if (blockage.contains(position)) {
				return false;
			}

			return true;
		}

		inline void unwind(offset_t origin, offset_t destination, cref<trail_t> trail) noexcept {
			rememberance_t<offset_t> unwind{ trail.at(destination) };

			points.push(destination);

			while (unwind.current != origin) {
				points.push(unwind.current);
				unwind = trail.at(unwind.previous);
			}
		}

		#undef dense_t
		#undef dense_args
	};
} // namespace bleak
