#pragma once

#include <bleak/primitive.hpp>

#include <cassert>

#include <bleak/array.hpp>
#include <bleak/color.hpp>
#include <bleak/concepts.hpp>
#include <bleak/hash.hpp>

namespace bleak {
	struct glyph_t {
		using index_t = u16;

		color_t color;
		index_t index;

		constexpr glyph_t() noexcept : color{}, index{} {}

		constexpr glyph_t(index_t index, color_t color) noexcept : color{ color }, index{ index } {}

		struct hasher {
			static constexpr size_t operator()(glyph_t glyph) noexcept { return hash_combine(glyph.color, glyph.index); }
		};
	};
} // namespace bleak
