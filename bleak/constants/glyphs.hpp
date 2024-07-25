#pragma once

#include <bleak/typedef.hpp>

#include <bleak/cardinal.hpp>
#include <bleak/glyph.hpp>

#include <bleak/constants/characters.hpp>
#include <bleak/constants/colors.hpp>

namespace bleak {
	namespace Glyphs {
		constexpr const glyph_t Empty { characters::Empty, colors::Transparent };

		constexpr const glyph_t Error { characters::Error, colors::Red };

		constexpr const glyph_t Wall { characters::Wall, colors::Marble };
		constexpr const glyph_t Floor { characters::Floor, colors::light::Charcoal };
		constexpr const glyph_t Obstacle { characters::Obstacle, colors::light::Intrite };

		constexpr const glyph_t Player { characters::Entity[cardinal_t::Central], colors::Green };
		constexpr const glyph_t Enemy { characters::Entity[cardinal_t::Central], colors::Red };
		constexpr const glyph_t Ally { characters::Entity[cardinal_t::Central], colors::Cyan };
		constexpr const glyph_t Neutral { characters::Entity[cardinal_t::Central], colors::Yellow };
	} // namespace Glyphs
} // namespace Bleakdepth
