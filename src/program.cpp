#include "typedef.hpp"

#include <cassert>
#include <cstdlib>
#include <format>
#include <random>

#include <SDL.h>

#include "atlas.hpp"
#include "cardinal.hpp"
#include "clock.hpp"
#include "cursor.hpp"
#include "gamepad.hpp"
#include "glyph.hpp"
#include "keyboard.hpp"
#include "log.hpp"
#include "map.hpp"
#include "mouse.hpp"
#include "point.hpp"
#include "renderer.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include "timer.hpp"
#include "wave.hpp"
#include "window.hpp"

#include "constants/bindings.hpp"
#include "constants/colors.hpp"

namespace Bleakdepth {
	constexpr cstr GAME_NAME = "Bleakdepth";
	constexpr cstr GAME_VERSION = "0.0.1";
	constexpr cstr GAME_AUTHOR = "Ryan Lockhart";

	const std::string GAME_TITLE { std::format("{} v{} by {}", GAME_NAME, GAME_VERSION, GAME_AUTHOR) };

	constexpr u32 WINDOW_FLAGS = SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN;
	constexpr u32 RENDERER_FLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	constexpr u32 FRAME_LIMIT = 60u;
	constexpr f32 FRAME_TIME = 1000.0f / FRAME_LIMIT;

	constexpr size_t<i32> WINDOW_SIZE { 640, 480 };
	constexpr size_t<i32> WINDOW_PADDING { 8, 8 };

	constexpr size_t<i32> GAME_GRID_SIZE { WINDOW_SIZE / 16 };
	constexpr size_t<i32> UI_GRID_SIZE { WINDOW_SIZE / 8 };

	static window_t window { GAME_TITLE.c_str(), WINDOW_SIZE + WINDOW_PADDING, WINDOW_FLAGS };
	static renderer_t renderer { window, RENDERER_FLAGS };

	static atlas_t<16, 16> game_atlas { { renderer, "res\\glyphs_16x16.png" } };
	static atlas_t<16, 16> ui_atlas { { renderer, "res\\glyphs_8x8.png" } };

	static std::minstd_rand random_engine { std::random_device {}() };

	static map_t<GAME_GRID_SIZE.x, GAME_GRID_SIZE.y, 1, 1> game_map {};

	static bool gamepad_enabled { true };

	static void primary_gamepad_disconnected();
	static void primary_gamepad_reconnected(cptr<gamepad_t> gamepad);

	static cptr<gamepad_t> primary_gamepad { GamepadManager::lease(0, &primary_gamepad_disconnected, &primary_gamepad_reconnected) };

	static void primary_gamepad_disconnected() {
		gamepad_enabled = false;
		primary_gamepad = nullptr;
	}

	static void primary_gamepad_reconnected(cptr<gamepad_t> gamepad) {
		gamepad_enabled = true;
		primary_gamepad = gamepad;
	}

	static cursor_t cursor { { renderer, "res\\cursor.png" }, Colors::White };
	static grid_cursor_t<16, 16> grid_cursor { { renderer, "res\\grid_cursor.png" }, { 0 }, Colors::Metals::Gold, { 0, 0 }, GAME_GRID_SIZE - 1 };

	static point_t<i32> camera_position { 0 };

	static animated_sprite_t<3> player { { animated_glyph_t<3>::generate_contiguous_indices<0xB0, 0xB2>(), Colors::Green }, { 0 } };

	static timer_t input_timer { 125 };
	static timer_t epoch_timer { 250 };
	static timer_t animation_timer { 1000.0 / 3 };

	static wave_t sine_wave { 1.0, 0.5, 1.0 };

} // namespace Bleakdepth

#define main SDL_main

using namespace Bleakdepth;

bool camera_movement() {
	point_t<> direction { 0, 0 };

	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::North])) {
		++direction.y;
	}
	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::South])) {
		--direction.y;
	}
	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::West])) {
		++direction.x;
	}
	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::East])) {
		--direction.x;
	}

	if (gamepad_enabled && direction == point_t<>::Zero) {
		direction = primary_gamepad->right_stick.current_state;
	}

	if (direction != point_t<>::Zero) {
		camera_position -= direction;

		input_timer.record();

		return true;
	}

	return false;
}

bool character_movement() {
	point_t<> direction { 0, 0 };

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

	if (gamepad_enabled && direction == point_t<>::Zero) {
		direction = primary_gamepad->left_stick.current_state;
	}

	if (direction != point_t<>::Zero) {
		player.position += direction;

		input_timer.record();
		epoch_timer.record();

		return true;
	}

	return false;
}

void startup();

void update();
void render();

void shutdown();

int main(int argc, char* argv[]) {
	startup();

	do {
		update();
		render();
	} while (window.is_running());

	shutdown();

	return EXIT_SUCCESS;
}

void startup() {
	if (primary_gamepad == nullptr) {
		message_log.add("no gamepad detected\n");
		gamepad_enabled = false;
	}

	game_atlas.universal_offset = WINDOW_PADDING / 2;
	ui_atlas.universal_offset = WINDOW_PADDING / 2;

	game_atlas.universal_foffset = static_cast<point_t<f32>>(WINDOW_PADDING / 2);
	ui_atlas.universal_foffset = static_cast<point_t<f32>>(WINDOW_PADDING / 2);

	game_map.set<map_region_t::Border>({ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Seen, cell_trait_t::Explored });
	game_map.randomize<map_region_t::Interior>(
		random_engine,
		0.5,
		{ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Seen, cell_trait_t::Explored },
		{ cell_trait_t::Open, cell_trait_t::Transperant, cell_trait_t::Seen, cell_trait_t::Explored }
	);

	player.position = static_cast<point_t<i32>>(game_map.find_random_cell_interior(random_engine, cell_trait_t::Open).value());

	Mouse::hide_cursor();

	Clock::tick();

	input_timer.reset();
	epoch_timer.reset();

	animation_timer.reset();
}

void update() {
	Clock::tick(FRAME_TIME);
	window.poll_events();

	if (animation_timer.ready()) {
		player.glyph.advance();
		animation_timer.record();
	}

	if (Keyboard::is_key_down(SDL_SCANCODE_ESCAPE)) {
		window.close();
		return;
	}

	if (input_timer.ready()) {
		camera_movement();

		if (epoch_timer.ready()) {
			character_movement();
		}
	}

	sine_wave.update<wave_type_t::Sine>(Clock::elapsed());

	grid_cursor.color.set_alpha(sine_wave.current_value());
	grid_cursor.update();
}

void render() {
	renderer.clear(Colors::Black);

	game_map.draw(game_atlas, camera_position);

	player.draw(game_atlas, camera_position);

	grid_cursor.draw();

	runes_t fps_text { std::format("FPS: {}", static_cast<u32>(Clock::frame_time())), Colors::White };
	ui_atlas.draw(fps_text, point_t<i32> { 0, UI_GRID_SIZE.h - 1 });

	runes_t tooltip_text { "waffle\nboarde", Colors::White };
	auto tooltip_size { (point_t<i32>)Text::calculate_size(tooltip_text) };

	auto tooltip_alignment { (cardinal_t)grid_cursor.get_quadrant() };

	auto tooltip_position { grid_cursor.position * game_atlas.glyph_size + grid_cursor.size / 2 + point_t<>{ 4, 4 } };

	renderer.draw_rect(tooltip_position, tooltip_size * ui_atlas.glyph_size, tooltip_alignment, Colors::Black, Colors::White);
	ui_atlas.draw(tooltip_text, grid_cursor.position * game_atlas.glyph_size / ui_atlas.glyph_size, tooltip_alignment, point_t<>{ 4, 4 } + game_atlas.glyph_size / ui_atlas.glyph_size * tooltip_alignment);

	renderer.draw_point(tooltip_position, Colors::Red);
	renderer.draw_point(grid_cursor.position * game_atlas.glyph_size, Colors::Green);

	renderer.present();
}

void shutdown() {
	if (primary_gamepad != nullptr) {
		GamepadManager::release(0);
	}

	message_log.flush_to_file();
	error_log.flush_to_file();
}
