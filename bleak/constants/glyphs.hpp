#pragma once

#include <bleak/typedef.hpp>

#include <bleak/cardinal.hpp>
#include <bleak/glyph.hpp>

#include <bleak/constants/characters.hpp>
#include <bleak/constants/colors.hpp>

namespace bleak {
	namespace Glyphs {
		constexpr const glyph_t Empty { Characters::Empty, colors::Transparent };

		constexpr const glyph_t Error { Characters::Error, colors::Red };

		constexpr const glyph_t Wall { Characters::Wall, colors::Marble };
		constexpr const glyph_t Floor { Characters::Floor, colors::light::Charcoal };
		constexpr const glyph_t Obstacle { Characters::Obstacle, colors::light::Intrite };

		constexpr const glyph_t Player { Characters::Entity[cardinal_t::Central], colors::Green };
		constexpr const glyph_t Enemy { Characters::Entity[cardinal_t::Central], colors::Red };
		constexpr const glyph_t Ally { Characters::Entity[cardinal_t::Central], colors::Cyan };
		constexpr const glyph_t Neutral { Characters::Entity[cardinal_t::Central], colors::Yellow };
	} // namespace Glyphs
} // namespace Bleakdepth
