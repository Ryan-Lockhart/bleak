#pragma once

#include <bleak/typedef.hpp>

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

		constexpr const u8 Empty{ 0x00 };

		constexpr const u8 UnarmoredPlayer{ 0x40 };
		constexpr const u8 ArmoredPlayer{ 0x41 };

		constexpr const u8 Skeleton{ 0x42 };
		constexpr const u8 Wraith{ 0x43 };
		constexpr const u8 FleshGolem{ 0x44 };

		constexpr const u8 FreshSkull{ 0x45 };
		constexpr const u8 AnimateSkull{ 0x46 };

		constexpr const u8 UpLadder{ 0x47 };
		constexpr const u8 DownLadder{ 0x48 };
		
		constexpr const u8 Shackle{ 0x49 };

		constexpr const u8 Adventurer{ 0x4A };
		constexpr const u8 Paladin{ 0x4B };
		constexpr const u8 Priest{ 0x4C };

		constexpr const u8 Energy{ 0x4D };
		constexpr const u8 Armor{ 0x4E };

		constexpr const u8 Floor{ 0x4F };
	} // namespace characters
} // namespace bleak
