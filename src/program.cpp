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

constexpr cstr GAME_NAME = "Bleakdepth";
constexpr cstr GAME_VERSION = "0.0.1";
constexpr cstr GAME_AUTHOR = "Ryan Lockhart";

const std::string GAME_TITLE{ std::format("{} v{} by {}", GAME_NAME, GAME_VERSION, GAME_AUTHOR) };

constexpr sdl::window_flags WINDOW_FLAGS{ static_cast<sdl::window_flags>(SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN) };
constexpr sdl::renderer_flags RENDERER_FLAGS{ static_cast<sdl::renderer_flags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };

constexpr u32 FRAME_LIMIT = 60u;
constexpr f32 FRAME_TIME = 1000.0f / FRAME_LIMIT;

constexpr extent_2d_t WINDOW_SIZE{ 640, 480 };
constexpr extent_2d_t WINDOW_PADDING{ 8, 24 };

constexpr offset_2d_t UNIVERSAL_OFFSET{ WINDOW_PADDING };

constexpr extent_2d_t GAME_GRID_SIZE{ WINDOW_SIZE / 16 };
constexpr extent_2d_t UI_GRID_SIZE{ WINDOW_SIZE / 8 };

static window_t window{ GAME_TITLE.c_str(), WINDOW_SIZE + WINDOW_PADDING * 2, WINDOW_FLAGS };
static renderer_t renderer{ window, RENDERER_FLAGS };

constexpr extent_2d_t ATLAS_SIZE{ 16, 16 };

static atlas_t<ATLAS_SIZE> game_atlas{ renderer, "res\\glyphs_16x16.png", UNIVERSAL_OFFSET };
static atlas_t<ATLAS_SIZE> ui_atlas{ renderer, "res\\glyphs_8x8.png", UNIVERSAL_OFFSET };

static std::minstd_rand random_engine{ std::random_device{}() };

constexpr extent_2d_t ZONE_SIZE{ GAME_GRID_SIZE / 2 };
constexpr extent_2d_t REGION_SIZE{ 2, 2 };

constexpr extent_2d_t BORDER_SIZE{ 2, 2 };

static_assert((REGION_SIZE * ZONE_SIZE).area() == GAME_GRID_SIZE.area(), "region and zone sizes do not match game grid size");

// static zone_t<zone_SIZE, { 2, 2 }> game_map{};
static region_t<cell_state_t, REGION_SIZE, ZONE_SIZE, BORDER_SIZE> game_map{};

constexpr extent_2d_t CELL_SIZE{ 16, 16 };

constexpr offset_2d_t CURSOR_OFFSET{ UNIVERSAL_OFFSET - CELL_SIZE / 4 };

static bool gamepad_enabled{ true };

static void primary_gamepad_disconnected();
static void primary_gamepad_reconnected(cptr<gamepad_t> gamepad);

static cptr<gamepad_t> primary_gamepad{ nullptr };

static void primary_gamepad_disconnected() {
	gamepad_enabled = false;
	primary_gamepad = nullptr;
}

static void primary_gamepad_reconnected(cptr<gamepad_t> gamepad) {
	gamepad_enabled = true;
	primary_gamepad = gamepad;
}

static cursor_t cursor{ renderer, "res\\cursor.png", Colors::White };
static grid_cursor_t<CELL_SIZE> grid_cursor{ renderer, "res\\grid_cursor.png", Colors::Metals::Gold, game_map.origin, game_map.extent };

static bool draw_cursor{ true };

static offset_2d_t camera_position{ 0 };

static animated_sprite_t<3> player{ { animated_glyph_t<3>::generate_contiguous_indices<0xB0, 0xB2>(), Colors::Green }, { 0 } };

static timer_t input_timer{ 250.0 };
static timer_t epoch_timer{ 250.0 };
static timer_t animation_timer{ 1000.0 / 3 };

static wave_t sine_wave{ 1.0, 0.5, 1.0 };

static cardinal_t direction_test{ cardinal_t::Central };

#define main SDL_main

using namespace bleak;

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

	if (gamepad_enabled && direction == offset_2d_t::zero) {
		direction = primary_gamepad->right_stick.current_state;
	}

	// test!
	direction_test = direction;
	return false;

	if (direction != offset_2d_t::zero) {
		camera_position -= direction;

		input_timer.record();

		return true;
	}

	return false;
}

bool character_movement() {
	if (Keyboard::is_key_pressed(Bindings::Wait)) {
		input_timer.record();
		epoch_timer.record();

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

	if (gamepad_enabled && direction == offset_2d_t::zero) {
		direction = primary_gamepad->left_stick.current_state;
	}

	if (direction != offset_2d_t::zero) {
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

void terminate_prematurely();

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
	Subsystem::initialize();

	Mouse::initialize();
	Keyboard::initialize();

	GamepadManager::initialize();

	primary_gamepad = GamepadManager::lease(0, &primary_gamepad_disconnected, &primary_gamepad_reconnected);
	gamepad_enabled = primary_gamepad != nullptr;

	if (primary_gamepad == nullptr) {
		message_log.add("no gamepad detected\n");
	}

	constexpr cell_state_t open_state{ cell_trait_t::Open, cell_trait_t::Transperant, cell_trait_t::Seen, cell_trait_t::Explored };
	constexpr cell_state_t closed_state{ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Seen, cell_trait_t::Explored };

	for (extent_2d_t::product_t i{0}; i < game_map.area; ++i) {
		game_map[i].set<zone_region_t::Border>(closed_state);
		game_map[i].generate<zone_region_t::Interior>(random_engine, 0.425, 100, 4, closed_state, open_state);
	}

	auto player_position{ game_map[0].find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open) };

	if (player_position.has_value()) {
		player.position = player_position.value();
	} else {
		error_log.add("no open cells found for player start position\n", __TIME_FILE_LINE__);
		terminate_prematurely();
	}

	Mouse::hide_cursor();

	Clock::tick();

	input_timer.reset();
	epoch_timer.reset();

	animation_timer.reset();

	message_log.flush_to_console(std::cout);
	error_log.flush_to_console(std::cerr);
}

void update() {
	Clock::tick(FRAME_TIME);
	window.poll_events();

	if (animation_timer.ready()) {
		player.glyph.advance();
		animation_timer.record();
	}

	if (Keyboard::is_key_down(Bindings::Quit)) {
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

	auto mouse_pos{ Mouse::get_position() };

	draw_cursor = mouse_pos.x < WINDOW_PADDING.w || mouse_pos.y < WINDOW_PADDING.h || mouse_pos.x >= WINDOW_SIZE.w + WINDOW_PADDING.w || mouse_pos.y >= WINDOW_SIZE.h + WINDOW_PADDING.h;

	if (draw_cursor) {
		cursor.update();
	} else {
		grid_cursor.update(CURSOR_OFFSET - camera_position);
		grid_cursor.color.set_alpha(sine_wave.current_value());
	}
}

void render() {
	renderer.clear(Colors::Black);

	game_map.draw(renderer, game_atlas, camera_position);

	player.draw(renderer, game_atlas, camera_position);

	if (draw_cursor) {
		cursor.draw(renderer);
	} else {
		grid_cursor.draw(renderer, CURSOR_OFFSET);
	}

	runes_t fps_text{ std::format("FPS: {}", static_cast<u32>(Clock::frame_time())), Colors::Green };

	// extent_2d_t fps_text_size{ Text::calculate_size(fps_text) };
	// offset_2d_t fps_text_pos{ UI_GRID_SIZE.w / 2 - fps_text_size.w / 2, UI_GRID_SIZE.h + 1 };
	offset_2d_t fps_text_pos{ UI_GRID_SIZE.w / 2, UI_GRID_SIZE.h + 1 };
	ui_atlas.draw(renderer, fps_text, fps_text_pos, direction_test);

	runes_t title_text{ GAME_TITLE };

	extent_2d_t title_text_size{ Text::calculate_size(title_text) };
	offset_2d_t title_text_pos{ UI_GRID_SIZE.w / 2 - title_text_size.w / 2, -2 };
	ui_atlas.draw(renderer, title_text, title_text_pos);

	renderer.present();
}

void shutdown() {
	if (primary_gamepad != nullptr) {
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
