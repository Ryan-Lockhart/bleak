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
		constexpr const glyph_t NecromanticAscendance{ 0x05, colors::White };
	} // namespace icons
	namespace glyphs {
		constexpr const glyph_t Empty{ characters::Empty, colors::Transparent };

		constexpr const glyph_t UnarmoredPlayer{ characters::UnarmoredPlayer, colors::White };
		constexpr const glyph_t ArmoredPlayer{ characters::ArmoredPlayer, colors::White };

		constexpr const glyph_t Skeleton{ characters::Skeleton, colors::White };
		constexpr const glyph_t Wraith{ characters::Wraith, colors::White };
		constexpr const glyph_t FleshGolem{ characters::FleshGolem, colors::White };

		constexpr const glyph_t FreshSkull{ characters::FreshSkull, colors::White };
		constexpr const glyph_t AnimateSkull{ characters::AnimateSkull, colors::White };

		constexpr const glyph_t UpLadder{ characters::UpLadder, colors::White };
		constexpr const glyph_t DownLadder{ characters::DownLadder, colors::White };
		
		constexpr const glyph_t Shackle{ characters::Shackle, colors::White };

		constexpr const glyph_t Adventurer{ characters::Adventurer, colors::White };
		constexpr const glyph_t Paladin{ characters::Paladin, colors::White };
		constexpr const glyph_t Priest{ characters::Priest, colors::White };

		constexpr const glyph_t Energy{ characters::Energy, colors::White };
		constexpr const glyph_t Armor{ characters::Armor, colors::White };

		constexpr const glyph_t Floor{ characters::Floor, colors::White };
	} // namespace glyphs
} // namespace bleak
