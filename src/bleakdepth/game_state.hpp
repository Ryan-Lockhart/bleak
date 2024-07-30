#pragma once

#include <bleak.hpp>

#include <bleakdepth/globals.hpp>

namespace bleakdepth {
	using namespace bleak;

	inline subsystem_s subsystem{};

	inline window_t window{ globals::GameTitle.c_str(), globals::WindowSize + globals::WindowBorder * 2, globals::WindowFlags };
	inline renderer_t renderer{ window, globals::RendererFlags };

	inline atlas_t<globals::TilesetSize> game_atlas{ renderer, "res\\tiles\\tileset_16x16_without-floor.png", globals::UniversalOffset };
	inline atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res\\glyphs\\glyphs_8x8.png", globals::UniversalOffset };

	inline std::mt19937 random_engine{ std::random_device{}() };

	inline zone_t<cell_state_t, globals::MapSize, globals::BorderSize> game_map{};

	inline bool gamepad_enabled{ true };

	inline cptr<gamepad_t> primary_gamepad{ nullptr };

	inline bool gamepad_active{ false };

	inline cursor_t cursor{ renderer, "res\\sprites\\cursor.png", colors::White };
	inline grid_cursor_t<globals::CellSize> grid_cursor{ renderer, "res\\sprites\\grid_cursor.png", colors::metals::Gold, game_map.zone_origin, game_map.zone_extent };

	inline bool draw_cursor{ true };

	inline sprite_t player{ glyphs::Player };

	inline std::vector<sprite_t> adventurers{ globals::AdventurerPopulation, glyphs::Enemy };
	inline std::vector<sprite_t> skeletons{};

	inline std::unordered_set<offset_t, offset_t::hasher> occupation_map{};

	inline field_t<offset_t::product_t, globals::MapSize, globals::BorderSize> adventurer_goal_map{};
	inline field_t<offset_t::product_t, globals::MapSize, globals::BorderSize> skeleton_goal_map{};

	inline camera_t camera{ globals::GameGridSize, extent_t::Zero, globals::CameraExtent };
	inline bool camera_locked{ true };

	inline timer_t input_timer{ 125.0 };
	inline timer_t cursor_timer{ 125.0 };
	inline timer_t epoch_timer{ 250.0 };
	inline timer_t animation_timer{ 1000.0 / 3 };

	inline wave_t sine_wave{ 1.0, 0.5, 1.0 };

	inline mixer_s mixer{};

	inline path_t test_path{};
} // namespace bleakdepth
