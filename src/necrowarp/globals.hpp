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

		constexpr u32 FrameLimit{ 30u };
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

		constexpr f32 CellToGlyphRatio{ 2.0f };
		constexpr f32 GlyphToCellRatio{ 0.5f };

		constexpr offset_t convert_cell_to_glyph(cref<offset_t> position) noexcept {
			return offset_t{ offset_t::scalar_cast(position.x * CellToGlyphRatio), offset_t::scalar_cast(position.y * CellToGlyphRatio) };
		}

		constexpr offset_t convert_glyph_to_cell(cref<offset_t> position) noexcept {
			return offset_t{ offset_t::scalar_cast(position.x * GlyphToCellRatio), offset_t::scalar_cast(position.y * GlyphToCellRatio) };
		}

		constexpr offset_t CursorOffset{ UniversalOffset - CellSize / 4 };

		constexpr extent_t CameraExtent{ MapSize - globals::GameGridSize };
		constexpr offset_t::scalar_t CameraSpeed{ 2 };

		constexpr f64 FillPercent{ 0.425 };
		constexpr u32 AutomotaIterations{ 128 };
		constexpr u32 AutomotaThreshold{ 4 };

		constexpr u32 ViewDistance{ 8 };
		constexpr f64 ViewSpan{ 135.0 };

		constexpr u32 NumberOfLadders{ 4 };

		constexpr u32 StartingAdventurers{ 4 };
		constexpr u32 StartingSkulls{ 8 };

		constexpr u32 SpawnDistributionLow{ 0 };
		constexpr u32 SpawnDistributionHigh{ 9 };

		constexpr u32 SmallWaveSize{ 16 };
		constexpr u32 MediumWaveSize{ 64 };
		constexpr u32 LargeWaveSize{ 128 };

		constexpr u32 KillsPerPopulation{ 4 };

		constexpr u32 KillsPerEnergySlot{ 16 };
		constexpr u32 KillsPerArmorSlot{ 8 };
	} // namespace globals
} // namespace necrowarp
