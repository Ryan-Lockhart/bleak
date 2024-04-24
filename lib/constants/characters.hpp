#pragma once

#include "typedef.hpp"

#include "cardinal.hpp"
#include "lut.hpp"

namespace Bleakdepth {
	namespace Characters {
		constexpr const u8 Error { 0x58 };

		constexpr const u8 Empty { 0x00 };
		constexpr const u8 Wall { 0x4F };
		constexpr const u8 Obstacle { 0x4E };
		constexpr const u8 Floor { 0x4D };

		constexpr const lut_t<cardinal_t, u8, 9> Entity {
			pair_t { cardinal_t::Northwest, u8 { 0x48 } },
			pair_t { cardinal_t::North, u8 { 0x41 } },
			pair_t { cardinal_t::Northeast, u8 { 0x42 } },
			pair_t { cardinal_t::West, u8 { 0x47 } },
			pair_t { cardinal_t::Central, u8 { 0x40 } },
			pair_t { cardinal_t::East, u8 { 0x43 } },
			pair_t { cardinal_t::Southwest, u8 { 0x46 } },
			pair_t { cardinal_t::South, u8 { 0x45 } },
			pair_t { cardinal_t::Southeast, u8 { 0x44 } }
		};

		constexpr const u8 Medkit { 0x49 };
		constexpr const u8 Glock { 0x4A };
		constexpr const u8 Ladder { 0x4B };
		constexpr const u8 Corpse { 0x4C };
	} // namespace Characters
} // namespace Bleakdepth
