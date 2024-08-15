#pragma once

#include <bleak/typedef.hpp>

#include "bleak/cardinal.hpp"
#include "bleak/lut.hpp"

namespace bleak {
	namespace characters {
		constexpr const u8 RecedingJaggedSet{ 0x00 };
		constexpr const u8 ProtrudingJaggedSet{ 0x10 };

		static constexpr u8 jagged_set(u8 index) { return ProtrudingJaggedSet + index; }

		static constexpr u8 jagged_set(bool protrudes, u8 index) { return (protrudes ? ProtrudingJaggedSet : RecedingJaggedSet) + index; }

		constexpr const u8 RecedingSmoothSet{ 0x20 };
		constexpr const u8 ProtrudingSmoothSet{ 0x30 };

		static constexpr u8 smooth_set(u8 index) { return ProtrudingSmoothSet + index; }

		static constexpr u8 smooth_set(bool protrudes, u8 index) { return (protrudes ? ProtrudingSmoothSet : RecedingSmoothSet) + index; }

		static constexpr u8 auto_set(bool smooth, bool protrudes, u8 index) { return smooth ? smooth_set(protrudes, index) : jagged_set(protrudes, index); }

		constexpr const u8 Error{ 0x58 };

		constexpr const u8 Empty{ 0x00 };

		constexpr const lut_t<cardinal_t, u8, 9> Entity{ pair_t<cardinal_t, u8>{ cardinal_t::Northwest, u8{ 0x48 } }, pair_t<cardinal_t, u8>{ cardinal_t::North, u8{ 0x41 } },	 pair_t<cardinal_t, u8>{ cardinal_t::Northeast, u8{ 0x42 } },
														 pair_t<cardinal_t, u8>{ cardinal_t::West, u8{ 0x47 } },	  pair_t<cardinal_t, u8>{ cardinal_t::Central, u8{ 0x40 } }, pair_t<cardinal_t, u8>{ cardinal_t::East, u8{ 0x43 } },
														 pair_t<cardinal_t, u8>{ cardinal_t::Southwest, u8{ 0x46 } }, pair_t<cardinal_t, u8>{ cardinal_t::South, u8{ 0x45 } },	 pair_t<cardinal_t, u8>{ cardinal_t::Southeast, u8{ 0x44 } } };

		constexpr const u8 Ladder{ 0x49 };
		constexpr const u8 Corpse{ 0x4A };
		constexpr const u8 Asperand{ 0x4B };
		constexpr const u8 Pound{ 0x4C };
		constexpr const u8 Floor{ 0x4D };
		constexpr const u8 Obstacle{ 0x4E };
		constexpr const u8 Wall{ 0x4F };
	} // namespace characters
} // namespace bleak
