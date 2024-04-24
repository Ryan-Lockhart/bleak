#pragma once

#include "typedef.hpp"

#include "color.hpp"

namespace Bleakdepth {
	struct glyph_t {
		color_t color;
		u8 index;

		constexpr glyph_t() noexcept : color {}, index {} {}

		constexpr glyph_t(u8 index, color_t color) noexcept : color { color }, index { index } {}
	};
} // namespace Bleakdepth
