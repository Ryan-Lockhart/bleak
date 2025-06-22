#pragma once

#include <bleak/typedef.hpp>

namespace bleak {
	namespace characters {
		constexpr const u8 UnarmoredPlayer{ 0x10 };
		constexpr const u8 ArmoredPlayer{ 0x11 };

		constexpr const u8 FreshSkeleton{ 0x12 };
		constexpr const u8 AnimateSkeleton{ 0x13 };
		constexpr const u8 RottedSkeleton{ 0x14 };

		constexpr const u8 Cultist{ 0x15 };
		constexpr const u8 Bloodhound{ 0x16 };

		constexpr const u8 Bonespur{ 0x17 };
		constexpr const u8 Wraith{ 0x18 };
		constexpr const u8 FleshGolem{ 0x19 };

		constexpr const u8 Adventurer{ 0x1A };
		constexpr const u8 Mercenary{ 0x1B };

		//constexpr const u8 YoungThetwo{ 0x1C };
		//constexpr const u8 AdultThetwo{ 0x1D };
		//constexpr const u8 MatureThetwo{ 0x1E };
		//constexpr const u8 GreatThetwo{ 0x1F };

		constexpr const u8 Ranger{ 0x1C };
		constexpr const u8 Skulker{ 0x1D };
		constexpr const u8 BattleMonk{ 0x1E };

		constexpr const u8 Berserker{ 0x1F };
		constexpr const u8 BloodyBerserker{ 0x20 };
		constexpr const u8 IchorousBerserker{ 0x21 };
		constexpr const u8 BloodyIchorousBerserker{ 0x22 };

		constexpr const u8 Paladin{ 0x23 };

		constexpr const u8 Arrow{ 0x24 };

		constexpr const u8 FreshSkull{ 0x25 };
		constexpr const u8 AnimateSkull{ 0x26 };
		constexpr const u8 RottedSkull{ 0x27 };

		constexpr const u8 Flesh{ 0x28 };

		constexpr const u8 UpLadder{ 0x29 };
		constexpr const u8 DownLadder{ 0x2A };

		constexpr const u8 Floor{ 0x00 };

		constexpr const u8 RightPatch{ 0x2B };
		constexpr const u8 LeftPatch{ 0x2C };

		constexpr const u8 AltRightPatch{ 0x2D };
		constexpr const u8 AltLeftPatch{ 0x2E };
	} // namespace characters
} // namespace bleak
