#pragma once

#include "typedef.hpp"

#include "lut.hpp"
#include "cardinal.hpp"

namespace Characters
{
    constexpr const u8 Error{ 0x58 };

    constexpr const u8 Empty{ 0x00 };
    constexpr const u8 Wall{ 0x4F };
    constexpr const u8 Obstacle{ 0x4E };
    constexpr const u8 Floor{ 0x4D };

    constexpr const lut<Cardinal, u8, 9> Entity {
        Pair{ Cardinal::Northwest, u8{ 0x48 } },
        Pair{ Cardinal::North, u8{ 0x41 } },
        Pair{ Cardinal::Northeast, u8{ 0x42 } },
        Pair{ Cardinal::West, u8{ 0x47 } },
        Pair{ Cardinal::Central, u8{ 0x40 } },
        Pair{ Cardinal::East, u8{ 0x43 } },
        Pair{ Cardinal::Southwest, u8{ 0x46 } },
        Pair{ Cardinal::South, u8{ 0x45 } },
        Pair{ Cardinal::Southeast, u8{ 0x44 } }
    };

    constexpr const u8 Medkit{ 0x49 };
    constexpr const u8 Glock{ 0x4A };
    constexpr const u8 Ladder{ 0x4B };
    constexpr const u8 Corpse{ 0x4C };
}
