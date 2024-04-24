#pragma once

#include "glyph.hpp"

#include "constants/characters.hpp"
#include "constants/colors.hpp"

namespace Glyphs
{
    constexpr const Glyph Empty{ Characters::Empty, Colors::Transparent };

    constexpr const Glyph Error{ Characters::Error, Colors::Red };

    constexpr const Glyph Wall{ Characters::Wall, Colors::Marble };
    constexpr const Glyph Floor{ Characters::Floor, Colors::LightCharcoal };
    constexpr const Glyph Obstacle{ Characters::Obstacle, Colors::LightIntrite };

    constexpr const Glyph Player{ Characters::Entity[Cardinal::Central], Colors::Green };
    constexpr const Glyph Enemy{ Characters::Entity[Cardinal::Central], Colors::Red };
    constexpr const Glyph Ally{ Characters::Entity[Cardinal::Central], Colors::Cyan };
    constexpr const Glyph Neutral{ Characters::Entity[Cardinal::Central], Colors::Yellow };
}
