#pragma once

#include <bleak/typedef.hpp>

namespace bleak {
	namespace characters {
		constexpr const u8 UnarmoredPlayer{ 0x00 };
		constexpr const u8 ArmoredPlayer{ 0x01 };

		constexpr const u8 FreshSkeleton{ 0x02 };
		constexpr const u8 AnimateSkeleton{ 0x03 };
		constexpr const u8 RottedSkeleton{ 0x04 };

		constexpr const u8 Cultist{ 0x05 };
		constexpr const u8 Bloodhound{ 0x06 };

		constexpr const u8 TwistedSuit{ 0x07 };
		constexpr const u8 ShimmeringSuit{ 0x08 };
		constexpr const u8 WrigglingSuit{ 0x09 };
		constexpr const u8 WrithingSuit{ 0x0A };
		
		constexpr const u8 Bonespur{ 0x0B };
		constexpr const u8 Wraith{ 0x0C };
		constexpr const u8 FleshGolem{ 0x0D };

		constexpr const u8 TwistedKnight{ 0x0E };
		constexpr const u8 ShimmeringKnight{ 0x0F };
		constexpr const u8 WrigglingKnight{ 0x10 };
		constexpr const u8 WrithingKnight{ 0x11 };

		constexpr const u8 Adventurer{ 0x12 };
		constexpr const u8 Mercenary{ 0x13 };
		constexpr const u8 Paladin{ 0x14 };
		constexpr const u8 ReservedOffensiveGood{ 0x15 };

		constexpr const u8 Priest{ 0x16 };
		constexpr const u8 ReservedSupportiveGoodOne{ 0x17 };
		constexpr const u8 ReservedSupportiveGoodTwo{ 0x18 };

		constexpr const u8 FreshSkull{ 0x19 };
		constexpr const u8 AnimateSkull{ 0x1A };
		constexpr const u8 RottedSkull{ 0x1B };

		constexpr const u8 TwistedMetal{ 0x1C };
		constexpr const u8 ShimmeringMetal{ 0x1D };
		constexpr const u8 WrigglingMetal{ 0x1E };
		constexpr const u8 WrithingMetal{ 0x1F };

		constexpr const u8 UpLadder{ 0x20 };
		constexpr const u8 DownLadder{ 0x21 };
		
		constexpr const u8 Unshackled{ 0x22 };
		constexpr const u8 CalciticShackle{ 0x23 };
		constexpr const u8 SpectralShackle{ 0x24 };
		constexpr const u8 SanguineShackle{ 0x25 };
		constexpr const u8 GalvanicShackle{ 0x26 };

		constexpr const u8 CalmPortal{ 0x27 };
		constexpr const u8 VocalPortal{ 0x28 };
		constexpr const u8 TurbulentPortal{ 0x29 };
		constexpr const u8 InsightfulPortal{ 0x2A };
		constexpr const u8 CollapsingPortal{ 0x2B };
		constexpr const u8 YawningPortal{ 0x2C };

		constexpr const u8 Floor{ 0x00 };

		constexpr const u8 RightPatch{ 0x10 };
		constexpr const u8 LeftPatch{ 0x11 };

		constexpr const u8 AltRightPatch{ 0x12 };
		constexpr const u8 AltLeftPatch{ 0x13 };
	} // namespace characters
} // namespace bleak
