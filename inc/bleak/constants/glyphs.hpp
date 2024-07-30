#pragma once

#include <bleak/typedef.hpp>

#include <bleak/cardinal.hpp>
#include <bleak/glyph.hpp>

#include <bleak/constants/characters.hpp>
#include <bleak/constants/colors.hpp>

namespace bleak {
	namespace glyphs {
		constexpr const glyph_t Empty{ characters::Empty, colors::Transparent };

		constexpr const glyph_t Error{ characters::Error, colors::Red };

		constexpr const glyph_t Wall{ characters::Wall, colors::Marble };
		constexpr const glyph_t Floor{ characters::Floor, colors::light::Charcoal };
		constexpr const glyph_t Obstacle{ characters::Obstacle, colors::light::Intrite };

		constexpr const glyph_t Player{ characters::Asperand, colors::Green };
		constexpr const glyph_t Enemy{ characters::Asperand, colors::Red };
		constexpr const glyph_t Ally{ characters::Asperand, colors::Cyan };
		constexpr const glyph_t Neutral{ characters::Asperand, colors::Yellow };

		constexpr const glyph_t PathSegment{ characters::Pound, colors::Blue };
	} // namespace glyphs
} // namespace bleak
