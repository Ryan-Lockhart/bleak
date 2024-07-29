#pragma once

#include <bleak/typedef.hpp>

#include <bleak/offset.hpp>

namespace bleak {
	struct line_t {
		offset_t start {};
		offset_t end {};

		constexpr line_t() {}

		constexpr line_t(offset_t start, offset_t end) noexcept : start { start }, end { end } {}

		constexpr line_t(ihalf x1, ihalf y1, ihalf x2, ihalf y2) noexcept : start { x1, y1 }, end { x2, y2 } {}

		constexpr usize length() const noexcept { return offset_t::distance<distance_function_t::Chebyshev>(start, end); }
	};
} // namespace bleak
