#pragma once

#include <bleak.hpp>

#include <necrowarp/globals.hpp>

namespace necrowarp {
	using namespace bleak;

	static inline subsystem_s subsystem{};

	static inline window_t window{ globals::GameTitle.c_str(), globals::WindowSize + globals::WindowBorder * 2, globals::WindowFlags };
	static inline renderer_t renderer{ window, globals::RendererFlags };

	static inline atlas_t<globals::TilesetSize> game_atlas{ renderer, "res\\tiles\\tileset_16x16.png", globals::UniversalOffset };
	static inline atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res\\glyphs\\glyphs_8x8.png", globals::UniversalOffset };

	static inline std::mt19937 random_engine{ std::random_device{}() };

	static inline zone_t<cell_state_t, globals::MapSize, globals::BorderSize> game_map{};

	static inline bool gamepad_enabled{ true };

	static inline cptr<gamepad_t> primary_gamepad{ nullptr };

	static inline bool gamepad_active{ false };

	static inline cursor_t cursor{ renderer, "res\\sprites\\cursor.png", colors::White };
	static inline grid_cursor_t<globals::CellSize> grid_cursor{ renderer, "res\\sprites\\grid_cursor.png", colors::metals::Gold, game_map.zone_origin, game_map.zone_extent };

	static inline bool draw_cursor{ true };

	static inline field_t<offset_t::product_t, globals::MapSize, globals::BorderSize> adventurer_goal_map{};
	static inline field_t<offset_t::product_t, globals::MapSize, globals::BorderSize> skeleton_goal_map{};

	static inline camera_t camera{ globals::GameGridSize, extent_t::Zero, globals::CameraExtent };
	static inline bool camera_locked{ true };

	static inline timer_t input_timer{ 125.0 };
	static inline timer_t cursor_timer{ 125.0 };
	static inline timer_t epoch_timer{ 250.0 };
	static inline timer_t animation_timer{ 1000.0 / 3 };

	static inline wave_t sine_wave{ 1.0, 0.5, 1.0 };

	static inline mixer_s mixer{};

	static inline path_t test_path{};

	static inline bool player_acted{ false };

	static inline i16 total_kills{ 0 };
} // namespace necrowarp
