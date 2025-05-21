#pragma once

#include <bleak/typedef.hpp>

namespace bleak {
	namespace characters {
		constexpr const u8 Floor{ 0x00 };

		constexpr const u8 UnarmoredPlayer{ 0x10 };
		constexpr const u8 ArmoredPlayer{ 0x11 };

		constexpr const u8 FreshSkeleton{ 0x12 };
		constexpr const u8 AnimateSkeleton{ 0x13 };
		constexpr const u8 RottedSkeleton{ 0x14 };

		constexpr const u8 Cultist{ 0x15 };
		constexpr const u8 Bloodhound{ 0x16 };
		
		constexpr const u8 Wraith{ 0x18 };
		constexpr const u8 FleshGolem{ 0x19 };

		constexpr const u8 Adventurer{ 0x1A };
		constexpr const u8 Paladin{ 0x1B };
		constexpr const u8 Mercenary{ 0x1C };
		constexpr const u8 Priest{ 0x1D };

		constexpr const u8 FreshSkull{ 0x1E };
		constexpr const u8 AnimateSkull{ 0x1F };
		constexpr const u8 RottedSkull{ 0x20 };

		constexpr const u8 UpLadder{ 0x21 };
		constexpr const u8 DownLadder{ 0x22 };
		
		constexpr const u8 Shackle{ 0x23 };
	} // namespace characters
} // namespace bleak
