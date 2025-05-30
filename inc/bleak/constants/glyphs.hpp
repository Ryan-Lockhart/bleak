#pragma once

#include <bleak/typedef.hpp>

#include <bleak/glyph.hpp>

#include <bleak/constants/characters.hpp>
#include <bleak/constants/colors.hpp>

namespace bleak {
	namespace icons {
		constexpr const glyph_t RandomWarp{ 0x00, colors::White };
		constexpr const glyph_t TargetWarp{ 0x01, colors::White };

		constexpr const glyph_t CalciticInvocation{ 0x02, colors::White };
		constexpr const glyph_t SpectralInvocation{ 0x03, colors::White };
		constexpr const glyph_t SanguineInvocation{ 0x04, colors::White };
		constexpr const glyph_t GalvanicInvocation{ 0x05, colors::White };

		constexpr const glyph_t NecromanticAscendance{ 0x06, colors::White };
	} // namespace icons

	namespace glyphs {
		constexpr const glyph_t Floor{ characters::Floor, colors::Transparent };

		constexpr const glyph_t UnarmoredPlayer{ characters::UnarmoredPlayer, colors::White };
		constexpr const glyph_t ArmoredPlayer{ characters::ArmoredPlayer, colors::White };

		constexpr const glyph_t FreshSkeleton{ characters::FreshSkeleton, colors::White };
		constexpr const glyph_t AnimateSkeleton{ characters::AnimateSkeleton, colors::White };
		constexpr const glyph_t RottedSkeleton{ characters::RottedSkeleton, colors::White };

		constexpr const glyph_t TwistedSuit{ characters::TwistedSuit, colors::White };
		constexpr const glyph_t ShimmeringSuit{ characters::ShimmeringSuit, colors::White };
		constexpr const glyph_t WrigglingSuit{ characters::WrigglingSuit, colors::White };
		constexpr const glyph_t WrithingSuit{ characters::WrithingSuit, colors::White };

		constexpr const glyph_t Cultist{ characters::Cultist, colors::White };
		constexpr const glyph_t Bloodhound{ characters::Bloodhound, colors::White };

		constexpr const glyph_t Wraith{ characters::Wraith, colors::White };
		constexpr const glyph_t FleshGolem{ characters::FleshGolem, colors::White };

		constexpr const glyph_t TwistedKnight{ characters::TwistedKnight, colors::White };
		constexpr const glyph_t ShimmeringKnight{ characters::ShimmeringKnight, colors::White };
		constexpr const glyph_t WrigglingKnight{ characters::WrigglingKnight, colors::White };
		constexpr const glyph_t WrithingKnight{ characters::WrithingKnight, colors::White };

		constexpr const glyph_t Adventurer{ characters::Adventurer, colors::White };
		constexpr const glyph_t Mercenary{ characters::Mercenary, colors::White };
		constexpr const glyph_t Paladin{ characters::Paladin, colors::White };
		constexpr const glyph_t Priest{ characters::Priest, colors::White };

		constexpr const glyph_t FreshSkull{ characters::FreshSkull, colors::White };
		constexpr const glyph_t AnimateSkull{ characters::AnimateSkull, colors::White };
		constexpr const glyph_t RottedSkull{ characters::RottedSkull, colors::White };

		constexpr const glyph_t TwistedMetal{ characters::TwistedMetal, colors::White };
		constexpr const glyph_t ShimmeringMetal{ characters::ShimmeringMetal, colors::White };
		constexpr const glyph_t WrigglingMetal{ characters::WrigglingMetal, colors::White };
		constexpr const glyph_t WrithingMetal{ characters::WrithingMetal, colors::White };

		constexpr const glyph_t UpLadder{ characters::UpLadder, colors::White };
		constexpr const glyph_t DownLadder{ characters::DownLadder, colors::White };
		
		constexpr const glyph_t Shackle{ characters::Shackle, colors::White };

		constexpr const glyph_t RightPatch{ characters::RightPatch, colors::White };
		constexpr const glyph_t LeftPatch{ characters::LeftPatch, colors::White };

		constexpr const glyph_t AltRightPatch{ characters::AltRightPatch, colors::White };
		constexpr const glyph_t AltLeftPatch{ characters::AltLeftPatch, colors::White };
	} // namespace glyphs
} // namespace bleak
