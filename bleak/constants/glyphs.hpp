#pragma once

#include "bleak/cardinal.hpp"
#include "bleak/glyph.hpp"

#include "bleak/constants/characters.hpp"
#include "bleak/constants/colors.hpp"

namespace bleak {
	namespace Glyphs {
		constexpr const glyph_t Empty { Characters::Empty, Colors::Transparent };

		constexpr const glyph_t Error { Characters::Error, Colors::Red };

		constexpr const glyph_t Wall { Characters::Wall, Colors::Marble };
		constexpr const glyph_t Floor { Characters::Floor, Colors::Light::Charcoal };
		constexpr const glyph_t Obstacle { Characters::Obstacle, Colors::Light::Intrite };

		constexpr const glyph_t Player { Characters::Entity[cardinal_t::Central], Colors::Green };
		constexpr const glyph_t Enemy { Characters::Entity[cardinal_t::Central], Colors::Red };
		constexpr const glyph_t Ally { Characters::Entity[cardinal_t::Central], Colors::Cyan };
		constexpr const glyph_t Neutral { Characters::Entity[cardinal_t::Central], Colors::Yellow };
	} // namespace Glyphs
} // namespace Bleakdepth
