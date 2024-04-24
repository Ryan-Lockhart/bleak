#pragma once

#include "typedef.hpp"

#include "color.hpp"

struct Glyph
{
	Color color;
	u8 index;

    constexpr Glyph() noexcept : color{ }, index{ } { }
    constexpr Glyph(u8 index, Color color) noexcept :
        color{ color },
        index{ index }
    { }
};
