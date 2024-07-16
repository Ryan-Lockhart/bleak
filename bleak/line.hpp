#pragma once

#include <bleak/typedef.hpp>

#include <bleak/offset.hpp>

namespace bleak {
	struct line_2d_t {
		offset_2d_t start {};
		offset_2d_t end {};

		constexpr line_2d_t() {}

		constexpr line_2d_t(offset_2d_t start, offset_2d_t end) noexcept : start { start }, end { end } {}

		constexpr line_2d_t(ihalf x1, ihalf y1, ihalf x2, ihalf y2) noexcept : start { x1, y1 }, end { x2, y2 } {}

		constexpr usize length() const noexcept { return offset_2d_t::distance<offset_2d_t::product_t>(start, end); }
	};
} // namespace bleak
