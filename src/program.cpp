#include "bleak/extent/extent_2d.hpp"
#include "bleak/typedef.hpp"

#include <cassert>
#include <cstdlib>
#include <format>
#include <iostream>
#include <random>

#include <SDL.h>

#include "bleak/atlas.hpp"
#include "bleak/cardinal.hpp"
#include "bleak/cell.hpp"
#include "bleak/clock.hpp"
#include "bleak/cursor.hpp"
#include "bleak/extent.hpp"
#include "bleak/gamepad.hpp"
#include "bleak/glyph.hpp"
#include "bleak/keyboard.hpp"
#include "bleak/log.hpp"
#include "bleak/mouse.hpp"
#include "bleak/offset.hpp"
#include "bleak/region.hpp"
#include "bleak/renderer.hpp"
#include "bleak/sprite.hpp"
#include "bleak/text.hpp"
#include "bleak/timer.hpp"
#include "bleak/wave.hpp"
#include "bleak/window.hpp"
#include "bleak/zone.hpp"

#include "bleak/constants/bindings.hpp"
#include "bleak/constants/colors.hpp"

using namespace bleak;

namespace Globals {
	constexpr cstr GameName{ "Bleakdepth" };
	constexpr cstr GameVersion{ "0.0.1" };
	constexpr cstr GameAuthor{ "Ryan Lockhart" };

	const std::string GameTitle{ std::format("{} v{} by {}", GameName, GameVersion, GameAuthor) };

	constexpr sdl::window_flags WindowFlags{ static_cast<sdl::window_flags>(SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN) };
	constexpr sdl::renderer_flags RendererFlags{ static_cast<sdl::renderer_flags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };

	constexpr u32 FrameLimit{ 60u };
	constexpr f32 FrameTime{ 1000.0f / FrameLimit };

	constexpr bool UseFrameLimit{ false };

	constexpr extent_2d_t WindowSize{ 640, 480 };
	constexpr extent_2d_t WindowBorder{ 8, 8 };

	constexpr offset_2d_t UniversalOffset{ WindowBorder };

	constexpr offset_2d_t UniversalOrigin{ UniversalOffset - 1 };
	constexpr offset_2d_t UniversalExtent{ UniversalOffset + WindowSize - 1 };

	constexpr extent_2d_t UIGridSize{ WindowSize / 8 };
	constexpr extent_2d_t GameGridSize{ WindowSize / 16 };

	constexpr extent_2d_t AtlasSize{ 16, 16 };

	constexpr extent_2d_t ZoneSize{ 64, 64 };
	constexpr extent_2d_t RegionSize{ 4, 4 };

	constexpr extent_2d_t BorderSize{ 4, 4 };

	constexpr extent_2d_t CellSize{ 16, 16 };

	constexpr offset_2d_t CursorOffset{ UniversalOffset - CellSize / 4 };
} // namespace Globals

struct game_state {
	window_t window;
	renderer_t renderer;

	atlas_t<Globals::AtlasSize> game_atlas;
	atlas_t<Globals::AtlasSize> ui_atlas;

	std::mt19937 random_engine;

	region_t<cell_state_t, Globals::RegionSize, Globals::ZoneSize, Globals::BorderSize> game_map;

	bool gamepad_enabled;

	cptr<gamepad_t> primary_gamepad;

	bool gamepad_active;

	cursor_t cursor;
	grid_cursor_t<Globals::CellSize> grid_cursor;

	bool draw_cursor;

	offset_2d_t camera_position;

	animated_sprite_t<extent_1d_t{ 3 }> player;

	timer_t input_timer;
	timer_t cursor_timer;
	timer_t epoch_timer;
	timer_t animation_timer;

	wave_t sine_wave;

	cardinal_t direction_test;

	game_state() noexcept :
		window{ Globals::GameTitle.c_str(), Globals::WindowSize + Globals::WindowBorder * 2, Globals::WindowFlags },
		renderer{ window, Globals::RendererFlags },
		game_atlas{ renderer, "res\\glyphs_16x16.png", Globals::UniversalOffset },
		ui_atlas{ renderer, "res\\glyphs_8x8.png", Globals::UniversalOffset },
		random_engine{ std::random_device{}() },
		game_map{},
		gamepad_enabled{ true },
		primary_gamepad{ nullptr },
		gamepad_active{ false },
		cursor{ renderer, "res\\cursor.png", Colors::White },
		grid_cursor{ renderer, "res\\grid_cursor.png", Colors::Metals::Gold, game_map.region_origin, offset_2d_t{ game_map.region_size * Globals::ZoneSize } - 1 },
		draw_cursor{ true },
		camera_position{ 0 },
		player{ animated_glyph_t<extent_1d_t{ 3 }>{ animated_glyph_t<extent_1d_t{ 3 }>::generate_contiguous_indices<0xB0, 0xB2>(), Colors::Green }, offset_2d_t{ 0 } },
		input_timer{ 125.0 },
		cursor_timer{ 125.0 },
		epoch_timer{ 250.0 },
		animation_timer{ 1000.0 / 3 },
		sine_wave{ 1.0, 0.5, 1.0 },
		direction_test{ cardinal_t::Central } {}
} static game_state{};

void primary_gamepad_disconnected() {
	game_state.gamepad_enabled = false;
	game_state.primary_gamepad = nullptr;
}

void primary_gamepad_reconnected(cptr<gamepad_t> gamepad) {
	game_state.gamepad_enabled = true;
	game_state.primary_gamepad = gamepad;
}

void primary_gamepad_active() { game_state.gamepad_active = true; }

void mouse_keyboard_active() { game_state.gamepad_active = false; }

#define main SDL_main

using namespace bleak;

void constrain_camera();

bool camera_movement() {
	offset_2d_t direction{};

	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::North])) {
		--direction.y;
	}
	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::South])) {
		++direction.y;
	}
	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::West])) {
		--direction.x;
	}
	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::East])) {
		++direction.x;
	}

	if (direction != offset_2d_t::zero) {
		game_state.camera_position += direction * 2;

		game_state.input_timer.record();

		mouse_keyboard_active();

		constrain_camera();

		return true;
	}

	if (game_state.gamepad_enabled && direction == offset_2d_t::zero) {
		direction = static_cast<offset_2d_t>(game_state.primary_gamepad->right_stick.current_state);
	}

	if (direction != offset_2d_t::zero) {
		game_state.camera_position += direction * 2;

		game_state.input_timer.record();

		primary_gamepad_active();

		constrain_camera();

		return true;
	}

	return false;
}

void constrain_camera() { game_state.camera_position.clamp(offset_2d_t::zero, offset_2d_t{ game_state.game_map.size } - offset_2d_t{ Globals::GameGridSize }); }

bool character_movement() {
	if (Keyboard::is_key_pressed(Bindings::Wait)) {
		game_state.input_timer.record();
		game_state.epoch_timer.record();

		return true;
	}

	offset_2d_t direction{};

	if (Keyboard::any_keys_pressed(Bindings::CharacterMovement[cardinal_t::North])) {
		--direction.y;
	}
	if (Keyboard::any_keys_pressed(Bindings::CharacterMovement[cardinal_t::South])) {
		++direction.y;
	}
	if (Keyboard::any_keys_pressed(Bindings::CharacterMovement[cardinal_t::West])) {
		--direction.x;
	}
	if (Keyboard::any_keys_pressed(Bindings::CharacterMovement[cardinal_t::East])) {
		++direction.x;
	}

	if (direction != offset_2d_t::zero) {
		game_state.player.position += direction;

		game_state.input_timer.record();
		game_state.epoch_timer.record();

		mouse_keyboard_active();

		return true;
	}

	if (game_state.gamepad_enabled) {
		direction = static_cast<offset_2d_t>(game_state.primary_gamepad->dpad.current_state);
	}

	if (direction != offset_2d_t::zero) {
		game_state.player.position += direction;

		game_state.input_timer.record();
		game_state.epoch_timer.record();

		primary_gamepad_active();

		return true;
	}

	return false;
}

void startup();

void update();
void render();

void shutdown();

void terminate_prematurely();

int main(int argc, char* argv[]) {
	startup();

	do {
		update();
		render();
	} while (game_state.window.is_running());

	shutdown();

	return EXIT_SUCCESS;
}

void startup() {
	Subsystem::initialize();

	Mouse::initialize();
	Keyboard::initialize();

	GamepadManager::initialize();

	game_state.primary_gamepad = GamepadManager::lease(0, &primary_gamepad_disconnected, &primary_gamepad_reconnected);
	game_state.gamepad_enabled = game_state.primary_gamepad != nullptr;

	if (game_state.primary_gamepad == nullptr) {
		message_log.add("no gamepad detected\n");
	}

	constexpr cell_state_t open_state{ cell_trait_t::Open, cell_trait_t::Transperant, cell_trait_t::Seen, cell_trait_t::Explored };
	constexpr cell_state_t closed_state{ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Seen, cell_trait_t::Explored };

	for (extent_2d_t::product_t i{ 0 }; i < game_state.game_map.region_area; ++i) {
		game_state.game_map[offset_1d_t{ i }].set<zone_region_t::Border>(closed_state);
		game_state.game_map[offset_1d_t{ i }].generate<zone_region_t::Interior>(game_state.random_engine, 0.45, 10, 4, closed_state, open_state);
	}

	auto player_position{ game_state.game_map[offset_1d_t{ 0 }].find_random<zone_region_t::Interior>(game_state.random_engine, cell_trait_t::Open) };

	if (player_position.has_value()) {
		game_state.player.position = player_position.value();
	} else {
		error_log.add("no open cells found for player start position\n", __TIME_FILE_LINE__);
		terminate_prematurely();
	}

	Mouse::hide_cursor();

	Clock::tick();

	game_state.input_timer.reset();
	game_state.cursor_timer.reset();
	game_state.epoch_timer.reset();

	game_state.animation_timer.reset();

	message_log.flush_to_console(std::cout);
	error_log.flush_to_console(std::cerr);
}

void update() {
	if constexpr (Globals::UseFrameLimit) {
		Clock::tick(Globals::FrameTime);
	} else {
		Clock::tick();
	}

	game_state.window.poll_events();

	if (game_state.animation_timer.ready()) {
		game_state.player.glyph.advance();
		game_state.animation_timer.record();
	}

	if (Keyboard::is_key_down(Bindings::Quit)) {
		game_state.window.close();
		return;
	}

	if (game_state.input_timer.ready()) {
		camera_movement();

		if (game_state.epoch_timer.ready()) {
			character_movement();
		}
	}

	game_state.sine_wave.update<wave_type_t::Sine>(Clock::elapsed());

	offset_2d_t mouse_pos{ Mouse::get_position() };

	game_state.draw_cursor = mouse_pos.x < Globals::UniversalOrigin.x || mouse_pos.y < Globals::UniversalOrigin.y || mouse_pos.x > Globals::UniversalExtent.x || mouse_pos.y > Globals::UniversalExtent.y;

	if (game_state.draw_cursor) {
		game_state.cursor.update();
	} else {
		if (game_state.gamepad_enabled && game_state.gamepad_active) {
			if (game_state.primary_gamepad->left_stick.current_state != cardinal_t::Central && game_state.cursor_timer.ready()) {
				game_state.grid_cursor.update(game_state.primary_gamepad->left_stick.current_state);

				game_state.cursor_timer.record();
			}
		} else {
			game_state.grid_cursor.update(game_state.camera_position);
		}

		game_state.grid_cursor.color.set_alpha(game_state.sine_wave.current_value());
	}
}

void render() {
	game_state.renderer.clear(Colors::Black);

	game_state.game_map.draw(game_state.renderer, game_state.game_atlas, -game_state.camera_position);

	game_state.player.draw(game_state.renderer, game_state.game_atlas, -game_state.camera_position);

	if (game_state.draw_cursor) {
		game_state.cursor.draw(game_state.renderer);
	} else {
		game_state.grid_cursor.draw(game_state.renderer, Globals::CursorOffset - game_state.camera_position * Globals::CellSize);
	}

	runes_t fps_text{ std::format("FPS: {}", static_cast<u32>(Clock::frame_time())), Colors::Green };

	game_state.ui_atlas.draw(game_state.renderer, fps_text, offset_2d_t{ Globals::UIGridSize.w - 1, 1 }, cardinal_t::West, offset_2d_t{ 0, -4 });

	runes_t title_text{ Globals::GameTitle, Colors::Marble };

	game_state.ui_atlas.draw(game_state.renderer, title_text, offset_2d_t{ Globals::UIGridSize.w / 2, 1 }, cardinal_t::Central, offset_2d_t{ 0, -4 });

	game_state.renderer.draw_outline_rect(offset_2d_t::zero, Globals::WindowSize + Globals::WindowBorder * 2, Globals::BorderSize, Colors::Black);
	game_state.renderer.draw_outline_rect(offset_2d_t::zero, Globals::WindowSize + Globals::WindowBorder * 2, Colors::White);

	game_state.renderer.present();
}

void shutdown() {
	if (game_state.primary_gamepad != nullptr) {
		GamepadManager::release(0);
	}

	message_log.flush_to_file();
	error_log.flush_to_file();

	// game_map.serialize("res", "test");
}

void terminate_prematurely() {
	shutdown();
	exit(EXIT_FAILURE);
}
