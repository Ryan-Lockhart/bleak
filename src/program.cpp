#include <bleak/typedef.hpp>

#include <cassert>
#include <cstdlib>
#include <format>
#include <iostream>
#include <string>

#include <SDL.h>

#include <bleak/area.hpp>
#include <bleak/atlas.hpp>
#include <bleak/camera.hpp>
#include <bleak/cardinal.hpp>
#include <bleak/cell.hpp>
#include <bleak/clock.hpp>
#include <bleak/cursor.hpp>
#include <bleak/extent.hpp>
#include <bleak/gamepad.hpp>
#include <bleak/glyph.hpp>
#include <bleak/keyboard.hpp>
#include <bleak/log.hpp>
#include <bleak/mouse.hpp>
#include <bleak/offset.hpp>
#include <bleak/random.hpp>
#include <bleak/region.hpp>
#include <bleak/renderer.hpp>
#include <bleak/sprite.hpp>
#include <bleak/text.hpp>
#include <bleak/timer.hpp>
#include <bleak/wave.hpp>
#include <bleak/window.hpp>
#include <bleak/zone.hpp>

#include <bleak/constants/bindings.hpp>
#include <bleak/constants/colors.hpp>
#include <bleak/constants/glyphs.hpp>

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

	constexpr bool UseFrameLimit{ true };

	constexpr extent_t WindowSize{ 640, 480 };
	constexpr extent_t WindowBorder{ 8, 8 };

	constexpr offset_t UniversalOffset{ WindowBorder };

	constexpr offset_t UniversalOrigin{ UniversalOffset - 1 };
	constexpr offset_t UniversalExtent{ UniversalOffset + WindowSize - 1 };

	constexpr extent_t UIGridSize{ WindowSize / 8 };
	constexpr extent_t GameGridSize{ WindowSize / 16 };

	constexpr extent_t GlyphsetSize{ 16, 16 };
	constexpr extent_t TilesetSize{ 16, 5 };

	constexpr extent_t ZoneSize{ 64, 64 };
	constexpr extent_t RegionSize{ 4, 4 };

	constexpr extent_t MapSize{ RegionSize * ZoneSize };

	constexpr extent_t BorderSize{ 4, 4 };

	constexpr extent_t CellSize{ 16, 16 };

	constexpr offset_t CursorOffset{ UniversalOffset - CellSize / 4 };

	constexpr extent_t CameraExtent{ MapSize - Globals::GameGridSize };
} // namespace Globals

struct game_state {
	window_t window{ Globals::GameTitle.c_str(), Globals::WindowSize + Globals::WindowBorder * 2, Globals::WindowFlags };
	renderer_t renderer{ window, Globals::RendererFlags };

	atlas_t<Globals::TilesetSize> game_atlas{ renderer, "res\\tiles\\tileset_16x16.png", Globals::UniversalOffset };
	atlas_t<Globals::GlyphsetSize> ui_atlas{ renderer, "res\\glyphs\\glyphs_8x8.png", Globals::UniversalOffset };

	std::mt19937 random_engine{ std::random_device{}() };

	zone_t<cell_state_t, Globals::RegionSize * Globals::ZoneSize, Globals::BorderSize> game_map{ "res\\maps\\test_region.map" };
	zone_t<glyph_t, Globals::GameGridSize> glyph_map{};

	bool gamepad_enabled{ true };

	cptr<gamepad_t> primary_gamepad{ nullptr };

	bool gamepad_active{ false };

	cursor_t cursor{ renderer, "res\\sprites\\cursor.png", Colors::White };
	grid_cursor_t<Globals::CellSize> grid_cursor{ renderer, "res\\sprites\\grid_cursor.png", Colors::Metals::Gold, game_map.zone_origin, game_map.zone_extent };

	bool draw_cursor{ true };

	sprite_t player{ Glyphs::Player };
	area_t player_fov{};

	camera_t camera{ Globals::GameGridSize, extent_t::Zero, Globals::CameraExtent };
	bool camera_locked{ true };

	timer_t input_timer{ 125.0 };
	timer_t cursor_timer{ 125.0 };
	timer_t epoch_timer{ 250.0 };
	timer_t animation_timer{ 1000.0 / 3 };

	wave_t sine_wave{ 1.0, 0.5, 1.0 };
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

bool update_camera() {
	if (game_state.camera_locked) {
		return game_state.camera.center_on(game_state.player.position);
	}

	offset_t direction{};

	if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::North])) {
		--direction.y;
	} if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::South])) {
		++direction.y;
	} if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::West])) {
		--direction.x;
	} if (Keyboard::is_key_pressed(Bindings::CameraMovement[cardinal_t::East])) {
		++direction.x;
	}

	if (direction != offset_t::Zero) {
		game_state.input_timer.record();
		mouse_keyboard_active();

		return game_state.camera.move(direction * 2);
	}

	if (game_state.gamepad_enabled && direction == offset_t::Zero) {
		direction = static_cast<offset_t>(game_state.primary_gamepad->right_stick.current_state);
	}

	if (direction != offset_t::Zero) {
		game_state.input_timer.record();
		primary_gamepad_active();

		return game_state.camera.move(direction * 2);
	}

	return false;
}

bool character_movement() {
	if (Keyboard::is_key_pressed(Bindings::Wait)) {
		game_state.input_timer.record();
		game_state.epoch_timer.record();

		return true;
	}

	offset_t direction{};

	if (Keyboard::any_keys_pressed(Bindings::CharacterMovement[cardinal_t::North])) {
		--direction.y;
	} if (Keyboard::any_keys_pressed(Bindings::CharacterMovement[cardinal_t::South])) {
		++direction.y;
	} if (Keyboard::any_keys_pressed(Bindings::CharacterMovement[cardinal_t::West])) {
		--direction.x;
	} if (Keyboard::any_keys_pressed(Bindings::CharacterMovement[cardinal_t::East])) {
		++direction.x;
	}

	if (direction != offset_t::Zero) {
		offset_t target_position{ game_state.player.position + direction };

		if (!game_state.game_map.within<zone_region_t::Interior>(target_position)) {
			return false;
		}

		if (game_state.game_map[target_position].solid) {
			return false;
		}

		game_state.player.position += direction;

		game_state.input_timer.record();
		game_state.epoch_timer.record();

		mouse_keyboard_active();

		return true;
	}

	if (game_state.gamepad_enabled) {
		direction = static_cast<offset_t>(game_state.primary_gamepad->dpad.current_state);
	}

	if (direction != offset_t::Zero) {
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

	Mouse::hide_cursor();

	game_state.primary_gamepad = GamepadManager::lease(0, &primary_gamepad_disconnected, &primary_gamepad_reconnected);
	game_state.gamepad_enabled = game_state.primary_gamepad != nullptr;

	if (game_state.primary_gamepad == nullptr) {
		message_log.add("no gamepad detected\n");
	}

	region_t<cell_state_t, Globals::RegionSize, Globals::ZoneSize, Globals::BorderSize> region{ "res\\maps\\test_region.map" };

	/*constexpr cell_state_t open_state{ cell_trait_t::Open, cell_trait_t::Transperant, cell_trait_t::Seen, cell_trait_t::Explored };
	constexpr cell_state_t closed_state{ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Seen, cell_trait_t::Explored };

	for (extent_t::product_t i{ 0 }; i < region.region_area; ++i) {
		region[offset_1d_t{ i }].set<zone_region_t::Border>(closed_state);
		region[offset_1d_t{ i }].generate<zone_region_t::Interior>(game_state.random_engine, 0.45, 10, 4, closed_state, open_state);
	}

	auto& fill_zone{ region[offset_t{ 0, 0 }] };

	area_t area{ fill_zone, cell_state_t{ cell_trait_t::Open, cell_trait_t::Transperant, cell_trait_t::Seen, cell_trait_t::Explored } };
	area.set(fill_zone, cell_state_t{ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Seen, cell_trait_t::Explored });*/

	region.compile(game_state.game_map);

	game_state.game_map.apply<zone_region_t::All>(cell_trait_t::Unseen, cell_trait_t::Unexplored);

	auto player_position{ game_state.game_map.find_random<zone_region_t::Interior>(game_state.random_engine, cell_trait_t::Open) };

	if (player_position.has_value()) {
		game_state.player.position = player_position.value();
	} else {
		error_log.add("no open cells found for player start position\n", __TIME_FILE_LINE__);
		terminate_prematurely();
	}

	//game_state.player_fov.recalculate(game_state.game_map, game_state.player.position, cell_trait_t::Transperant, 8.0f, true);
	game_state.player_fov.shadow_cast(game_state.game_map, game_state.player.position, cell_trait_t::Transperant, 8.0f,  90.0, 180.0);

	game_state.player_fov.apply(game_state.game_map, cell_trait_t::Seen, cell_trait_t::Explored);

	update_camera();

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
		// game_state.player.glyph.advance();
		game_state.animation_timer.record();
	}

	if (Keyboard::is_key_down(Bindings::Quit)) {
		game_state.window.close();
		return;
	}

	if (game_state.input_timer.ready()) {
		if (game_state.epoch_timer.ready()) {
			if (character_movement()) {
				game_state.player_fov.repeal(game_state.game_map, cell_trait_t::Seen);

				//game_state.player_fov.recalculate(game_state.game_map, game_state.player.position, cell_trait_t::Transperant, 8.0f, true);
				game_state.player_fov.shadow_cast(game_state.game_map, game_state.player.position, cell_trait_t::Transperant, 8.0f,  90.0, 180.0);

				game_state.player_fov.apply(game_state.game_map, cell_trait_t::Seen, cell_trait_t::Explored);
			}
		}

		update_camera();
	}

	game_state.sine_wave.update<wave_type_t::Sine>(Clock::elapsed());

	offset_t mouse_pos{ Mouse::get_position() };

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
			game_state.grid_cursor.update(game_state.camera.get_position());
		}

		game_state.grid_cursor.color.set_alpha(game_state.sine_wave.current_value());
	}
}

void render() {
	game_state.renderer.clear(Colors::Black);

	game_state.game_map.draw(game_state.game_atlas, -game_state.camera.get_position());

	game_state.player.draw(game_state.game_atlas, -game_state.camera.get_position());

	if (game_state.draw_cursor) {
		game_state.cursor.draw();
	} else {
		game_state.grid_cursor.draw(Globals::CursorOffset - game_state.camera.get_position() * Globals::CellSize);
	}

	runes_t fps_text{ std::format("FPS:{:4}", static_cast<u32>(Clock::frame_time())), Colors::Green };

	game_state.ui_atlas.draw(fps_text, offset_t{ Globals::UIGridSize.w - 1, 1 }, cardinal_t::West, offset_t{ 0, -4 });

	runes_t title_text{ Globals::GameTitle, Colors::Marble };

	game_state.ui_atlas.draw_label(game_state.renderer, title_text, offset_t{ Globals::UIGridSize.w / 2, 1 }, cardinal_t::Central, offset_t{ 0, -4 }, Colors::Black, Colors::White);

	game_state.renderer.draw_outline_rect(offset_t::Zero, Globals::WindowSize + Globals::WindowBorder * 2, Globals::BorderSize, Colors::Black);
	game_state.renderer.draw_outline_rect(offset_t::Zero, Globals::WindowSize + Globals::WindowBorder * 2, Colors::White);

	game_state.renderer.present();
}

void shutdown() {
	if (game_state.primary_gamepad != nullptr) {
		GamepadManager::release(0);
	}

	GamepadManager::terminate();

	Keyboard::terminate();
	Mouse::terminate();

	Subsystem::terminate();

	message_log.flush_to_file();
	error_log.flush_to_file();

	// game_state.game_map.serialize("res\\maps\\test_region.map");
}

void terminate_prematurely() {
	std::cout << "Message Log:\n";
	message_log.flush_to_console(std::cout);

	std::cerr << "\nError Log:\n";
	error_log.flush_to_console(std::cerr);

	shutdown();
	exit(EXIT_FAILURE);
}
