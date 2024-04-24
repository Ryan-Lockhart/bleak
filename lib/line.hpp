#pragma once

#include "typedef.hpp"

#include "point.hpp"

namespace Bleakdepth {
	template<typename T = i32> struct line_t {
		point_t<T> start;
		point_t<T> end;

		constexpr line_t() = delete;

		constexpr line_t(T x1, T y1, T x2, T y2) noexcept : start(x1, y1), end(x2, y2) {}

		constexpr line_t(cref<point_t<T>> start, cref<point_t<T>> end) noexcept : start(start), end(end) {}

		constexpr line_t(rval<point_t<T>> start, rval<point_t<T>> end) noexcept :
			start(std::move(start)),
			end(std::move(end)) {}

		constexpr line_t(cref<line_t> line) noexcept : start(line.start), end(line.end) {}

		constexpr line_t(rval<line_t> line) noexcept : start(std::move(line.start)), end(std::move(line.end)) {}

		constexpr ~line_t() = default;

		constexpr ref<line_t> operator=(cref<line_t> line) noexcept {
			start = line.start;
			end = line.end;

			return *this;
		}

		constexpr ref<line_t> operator=(rval<line_t> line) noexcept {
			start = std::move(line.start);
			end = std::move(line.end);

			return *this;
		}
	};
} // namespace Bleakdepth
