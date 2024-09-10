#pragma once

#include <bleak.hpp>

namespace necrowarp {
	namespace globals {
		using namespace bleak;

		constexpr std::string GameName{ "Necrowarp" };
		constexpr std::string GameVersion{ "0.0.1" };
		constexpr std::string GameAuthor{ "Ryan Lockhart" };

		const std::string GameTitle{ std::format("{} v{} by {}", GameName, GameVersion, GameAuthor) };

		constexpr sdl::window_flags WindowFlags{ static_cast<sdl::window_flags>(SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN) };
		constexpr sdl::renderer_flags RendererFlags{ static_cast<sdl::renderer_flags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };

		constexpr u32 FrameLimit{ 60u };
		constexpr f32 FrameTime{ 1000.0f / FrameLimit };

		constexpr bool UseFrameLimit{ true };

		constexpr extent_t WindowSize{ 1280, 720 };
		constexpr extent_t WindowBorder{ 8, 8 };

		constexpr offset_t UniversalOffset{ WindowBorder };

		constexpr offset_t UniversalOrigin{ UniversalOffset - 1 };
		constexpr offset_t UniversalExtent{ UniversalOffset + WindowSize - 1 };

		constexpr extent_t UIGridSize{ WindowSize / 8 };
		constexpr extent_t GameGridSize{ WindowSize / 16 };

		constexpr extent_t GlyphsetSize{ 16, 16 };
		constexpr extent_t TilesetSize{ 16, 5 };

		constexpr extent_t ZoneSize{ globals::GameGridSize };
		constexpr extent_t RegionSize{ 1, 1 };

		constexpr extent_t MapSize{ RegionSize * ZoneSize };

		constexpr offset_t MapCenter{ GameGridSize / 2 };

		constexpr extent_t BorderSize{ 4, 4 };

		constexpr extent_t CellSize{ 16, 16 };
		constexpr extent_t GlyphSize{ 8, 8 };

		constexpr offset_t CursorOffset{ UniversalOffset - CellSize / 4 };

		constexpr extent_t CameraExtent{ MapSize - globals::GameGridSize };
		constexpr offset_t::scalar_t CameraSpeed{ 2 };

		constexpr f64 FillPercent{ 0.45 };
		constexpr u32 AutomotaIterations{ 10 };
		constexpr u32 AutomotaThreshold{ 4 };

		constexpr u32 ViewDistance{ 8 };
		constexpr f64 ViewSpan{ 135.0 };

		constexpr u32 AdventurerPopulation{ 3 };
		constexpr u32 SkullDebris{ 5 };

		constexpr u8 StartingEnergy{ 3 };
		constexpr u8 StartingArmor{ 0 };

		constexpr u8 MinimumEnergy{ 4 };
		constexpr u8 MinimumArmor{ 2 };

		constexpr u8 MaximumEnergy{ 32 };
		constexpr u8 MaximumArmor{ 16 };

		constexpr u8 SkullBoon{ 1 };
		constexpr u8 FailedWarpBoon{ 1 };
		
		constexpr u8 AdventurerDeathBoon{ 1 };
		constexpr u8 PaladinDeathBoon{ 2 };
		constexpr u8 PriestDeathBoon{ 4 };

		constexpr u8 RandomWarpCost{ 1 };
		constexpr u8 TargetWarpCost{ 2 };
		constexpr u8 SummonWraithCost{ 4 };
		constexpr u8 GrandSummoningCost{ 8 };

		constexpr u8 AdventurerDamage{ 1 };
		constexpr u8 PaladinDamage{ 2 };
		constexpr u8 PriestDamage{ 4 };

		constexpr glyph_t EnergyGlyph{ 0x44, colors::White };
		constexpr glyph_t ArmorGlyph{ 0x45, colors::White };

		constexpr bool UseSimpleGraphics{ false };
	} // namespace globals
} // namespace necrowarp
