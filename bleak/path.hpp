#pragma once

#include <bleak/typedef.hpp>

#include <list>
#include <stdexcept>
#include <vector>

#include <bleak/line.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	class path_t {
	  private:
		std::list<offset_2d_t> points;

	  public:
		inline path_t() noexcept : points{} {}

		inline path_t(cref<std::list<offset_2d_t>> points) : points{ points } {}

		inline path_t(cref<line_2d_t> line) : points{} {
			if (line.start == line.end) {
				throw std::invalid_argument("path cannot be comprised of a single point");
			}

			offset_2d_t pos{ line.start };

			offset_2d_t delta{ std::abs(line.end.x - line.start.x), std::abs(line.end.y - line.start.y) };

			offset_2d_t step{ line.start.x < line.end.x ? 1 : -1, line.start.y < line.end.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			for (;;) {
				points.push_back(line.start);

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
		}

		inline path_t(cref<std::vector<line_2d_t>> lines) {
			if (lines.size() < 2) {
				throw std::invalid_argument("path cannot be comprised of a single point");
			}

			points.push_back(lines.front().start);

			for (usize i = 0; i < lines.size() - 1; ++i) {
				if (lines[i].end != lines[i + 1].start) {
					throw std::invalid_argument("path is discontiguous");
				}

				points.push_back(lines[i].end);
			}

			points.push_back(lines.back().end);
		}

		inline path_t(cref<path_t> path) noexcept : points{ path.points } {}

		inline path_t(rval<path_t> path) noexcept : points{ std::move(path.points) } {}

		inline ~path_t() = default;

		inline path_t& operator=(cref<path_t> path) noexcept {
			points = path.points;
			return *this;
		}

		inline path_t& operator=(rval<path_t> path) noexcept {
			points = std::move(path.points);
			return *this;
		}

		inline bool empty() const { return points.empty(); }

		inline usize size() const { return points.size(); }

		inline cref<offset_2d_t> front() const { return points.front(); }

		inline cref<offset_2d_t> back() const { return points.back(); }

		inline cref<offset_2d_t> operator[](usize index) const { return *std::next(points.begin(), index); }

		inline void push_front(cref<offset_2d_t> point) { points.push_front(point); }

		inline void push_back(cref<offset_2d_t> point) { points.push_back(point); }

		inline void pop_front() { points.pop_front(); }

		inline void pop_back() { points.pop_back(); }

		inline void emplace_front(rval<offset_2d_t> point) { points.emplace_front(point); }

		inline void emplace_back(rval<offset_2d_t> point) { points.emplace_back(point); }

		inline rval<offset_2d_t> extract_front() {
			rval<offset_2d_t> p = std::move(points.front());
			points.pop_front();
			return std::move(p);
		}

		inline rval<offset_2d_t> extract_back() {
			rval<offset_2d_t> p = std::move(points.back());
			points.pop_back();
			return std::move(p);
		}

		inline void clear() { points.clear(); }

		inline void insert(usize index, cref<offset_2d_t> point) { points.insert(std::next(points.begin(), index), point); }

		inline void insert(usize index, cref<path_t> path) { points.insert(std::next(points.begin(), index), path.points.begin(), path.points.end()); }

		inline void erase(usize index) { points.erase(std::next(points.begin(), index)); }

		inline void append(cref<path_t> path) { points.insert(points.end(), path.points.begin(), path.points.end()); }

		inline void reverse() { points.reverse(); }
	};
} // namespace bleak
