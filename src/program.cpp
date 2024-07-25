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
#include <bleak/clip_pool.hpp>
#include <bleak/clock.hpp>
#include <bleak/cursor.hpp>
#include <bleak/extent.hpp>
#include <bleak/gamepad.hpp>
#include <bleak/glyph.hpp>
#include <bleak/keyboard.hpp>
#include <bleak/log.hpp>
#include <bleak/mixer.hpp>
#include <bleak/mouse.hpp>
#include <bleak/music.hpp>
#include <bleak/offset.hpp>
#include <bleak/random.hpp>
#include <bleak/region.hpp>
#include <bleak/renderer.hpp>
#include <bleak/sound.hpp>
#include <bleak/sprite.hpp>
#include <bleak/text.hpp>
#include <bleak/timer.hpp>
#include <bleak/wave.hpp>
#include <bleak/window.hpp>
#include <bleak/zone.hpp>
#include <bleak/applicator.hpp>

#include <bleak/constants/bindings.hpp>
#include <bleak/constants/colors.hpp>
#include <bleak/constants/glyphs.hpp>
#include <bleak/constants/numeric.hpp>

using namespace bleak;

namespace globals {
	constexpr cstr GameName{ "Bleakdepth" };
	constexpr cstr GameVersion{ "0.0.1" };
	constexpr cstr GameAuthor{ "Ryan Lockhart" };

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

	constexpr extent_t ZoneSize{ 32, 32 };
	constexpr extent_t RegionSize{ 16, 16 };

	constexpr extent_t MapSize{ RegionSize * ZoneSize };

	constexpr extent_t BorderSize{ 4, 4 };

	constexpr extent_t CellSize{ 16, 16 };

	constexpr offset_t CursorOffset{ UniversalOffset - CellSize / 4 };

	constexpr extent_t CameraExtent{ MapSize - globals::GameGridSize };

	constexpr f64 FillPercent{ 0.425 };
	constexpr u32 AutomotaIterations{ 5 };
	constexpr u32 AutomotaThreshold{ 4 };

	constexpr u32 ViewDistance{ 8 };
	constexpr f64 ViewSpan{ 135.0 };
} // namespace globals

struct game_state {
	subsystem_s subsystem{};

	window_t window{ globals::GameTitle.c_str(), globals::WindowSize + globals::WindowBorder * 2, globals::WindowFlags };
	renderer_t renderer{ window, globals::RendererFlags };

	atlas_t<globals::TilesetSize> game_atlas{ renderer, "res\\tiles\\tileset_16x16.png", globals::UniversalOffset };
	atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res\\glyphs\\glyphs_8x8.png", globals::UniversalOffset };

	std::mt19937 random_engine{ std::random_device{}() };

	zone_t<cell_state_t, globals::RegionSize * globals::ZoneSize, globals::BorderSize> game_map{};
	zone_t<glyph_t, globals::GameGridSize> glyph_map{};

	bool gamepad_enabled{ true };

	cptr<gamepad_t> primary_gamepad{ nullptr };

	bool gamepad_active{ false };

	cursor_t cursor{ renderer, "res\\sprites\\cursor.png", colors::White };
	grid_cursor_t<globals::CellSize> grid_cursor{ renderer, "res\\sprites\\grid_cursor.png", colors::metals::Gold, game_map.zone_origin, game_map.zone_extent };

	bool draw_cursor{ true };

	area_t player_fov{};
	sprite_t player{ Glyphs::Player };

	camera_t camera{ globals::GameGridSize, extent_t::Zero, globals::CameraExtent };
	bool camera_locked{ true };

	timer_t input_timer{ 125.0 };
	timer_t cursor_timer{ 125.0 };
	timer_t epoch_timer{ 250.0 };
	timer_t animation_timer{ 1000.0 / 3 };

	wave_t sine_wave{ 1.0, 0.5, 1.0 };

	mixer_s mixer{ 44100, MIX_DEFAULT_FORMAT, channel_t::Stereo, 2048 };

	music_t music{ "res\\audio\\music\\hall_of_the_mountain_king.wav" };
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

inline bool update_camera() {
	if (game_state.camera_locked) {
		return game_state.camera.center_on(game_state.player.position);
	}

	offset_t direction{};

	if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_t::North])) {
		--direction.y;
	}
	if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_t::South])) {
		++direction.y;
	}
	if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_t::West])) {
		--direction.x;
	}
	if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_t::East])) {
		++direction.x;
	}

	if (game_state.gamepad_enabled && direction == offset_t::Zero) {
		direction = static_cast<offset_t>(game_state.primary_gamepad->right_stick.current_state);
	}

	if (direction != offset_t::Zero) {
		game_state.input_timer.record();
		return game_state.camera.move(direction * 2);
	}

	return false;
}

inline void recalculate_fov(cref<offset_t> position, cardinal_t direction) {
	game_state.player_fov.repeal(game_state.game_map, cell_trait_t::Seen);
	game_state.player_fov.cast(game_state.game_map, position, cell_trait_t::Transperant, globals::ViewDistance, direction_to_angle(direction), globals::ViewSpan, true);
	game_state.player_fov.apply(game_state.game_map, cell_trait_t::Seen, cell_trait_t::Explored);
}

inline void recalculate_fov() {
	game_state.player_fov.repeal(game_state.game_map, cell_trait_t::Seen);
	game_state.player_fov.cast(game_state.game_map, game_state.player.position, cell_trait_t::Transperant, globals::ViewDistance / 2, true);
	game_state.player_fov.apply(game_state.game_map, cell_trait_t::Seen, cell_trait_t::Explored);
}

inline void recalculate_fov(cardinal_t direction) { recalculate_fov(game_state.player.position, direction); }

inline bool character_movement() {
	if (Keyboard::any_keys_pressed(bindings::Wait)) {
		game_state.input_timer.record();
		game_state.epoch_timer.record();

		game_state.player.glyph.index = characters::Entity[cardinal_t::Central];

		recalculate_fov();

		return true;
	}

	offset_t direction{};

	if (Keyboard::any_keys_pressed(bindings::CharacterMovement[cardinal_t::North])) {
		--direction.y;
	}
	if (Keyboard::any_keys_pressed(bindings::CharacterMovement[cardinal_t::South])) {
		++direction.y;
	}
	if (Keyboard::any_keys_pressed(bindings::CharacterMovement[cardinal_t::West])) {
		--direction.x;
	}
	if (Keyboard::any_keys_pressed(bindings::CharacterMovement[cardinal_t::East])) {
		++direction.x;
	}

	if (game_state.gamepad_enabled && direction == offset_t::Zero) {
		direction = static_cast<offset_t>(game_state.primary_gamepad->dpad.current_state);
	}

	if (direction != offset_t::Zero) {
		game_state.player.glyph.index = characters::Entity[direction];

		offset_t target_position{ game_state.player.position + direction };

		if (!game_state.game_map.within<zone_region_t::Interior>(target_position) || game_state.game_map[target_position].solid) {
			recalculate_fov(direction);
			return false;
		}

		game_state.player.position += direction;

		recalculate_fov(direction);

		game_state.input_timer.record();
		game_state.epoch_timer.record();

		return true;
	}

	return false;
}

inline void startup();

inline void update();
inline void render();

inline void shutdown();

inline void terminate_prematurely();

int main(int argc, char* argv[]) {
	startup();

	do {
		update();
		render();
	} while (game_state.window.is_running());

	shutdown();

	return EXIT_SUCCESS;
}

inline void startup() {
	Mouse::initialize();
	Keyboard::initialize();

	GamepadManager::initialize();

	Mouse::hide_cursor();

	game_state.music.play();

	game_state.primary_gamepad = GamepadManager::lease(0, &primary_gamepad_disconnected, &primary_gamepad_reconnected);
	game_state.gamepad_enabled = game_state.primary_gamepad != nullptr;

	if (game_state.primary_gamepad == nullptr) {
		message_log.add("no gamepad detected\n");
	}

	region_t<cell_state_t, globals::RegionSize, globals::ZoneSize, globals::BorderSize> region{ "res\\maps\\test_region.map" };

	constexpr cell_state_t open_state{ cell_trait_t::Open, cell_trait_t::Transperant, cell_trait_t::Unseen, cell_trait_t::Unexplored };
	constexpr cell_state_t closed_state{ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Unseen, cell_trait_t::Unexplored };

	constexpr binary_applicator_t<cell_state_t> cell_applicator{ closed_state, open_state };

	for (extent_t::product_t i{ 0 }; i < region.region_area; ++i) {
		region[i].set<zone_region_t::Border>(closed_state);
		region[i].generate<zone_region_t::Interior>(
			game_state.random_engine,
			globals::FillPercent,
			globals::AutomotaIterations,
			globals::AutomotaThreshold,
			cell_applicator
		);
	}

	region.compile(game_state.game_map);

	auto starting_position{ game_state.game_map.find_random<zone_region_t::Interior>(game_state.random_engine, cell_trait_t::Open) };

	if (starting_position.has_value()) {
		game_state.player.position = starting_position.value();
	} else {
		error_log.add("no open cells found for player start position\n", __TIME_FILE_LINE__);
		error_log.add("\ninterior contents:\n open cells: {}\n solid cells: {}\n", game_state.game_map.count<zone_region_t::Interior>(cell_trait_t::Open), game_state.game_map.count<zone_region_t::Interior>(cell_trait_t::Solid));
		terminate_prematurely();
	}

	recalculate_fov();

	update_camera();

	Clock::tick();

	game_state.input_timer.reset();
	game_state.cursor_timer.reset();
	game_state.epoch_timer.reset();

	game_state.animation_timer.reset();

	message_log.flush_to_console(std::cout);
	error_log.flush_to_console(std::cerr);
}

inline void update() {
	if (game_state.window.is_closing()) {
		return;
	}

	if constexpr (globals::UseFrameLimit) {
		Clock::tick(globals::FrameTime);
	} else {
		Clock::tick();
	}

	game_state.window.poll_events();

	if (Keyboard::is_key_down(bindings::Quit)) {
		game_state.window.close();
		return;
	}

	if (game_state.input_timer.ready()) {
		if (game_state.epoch_timer.ready()) {
			character_movement();
		}

		update_camera();
	}

	game_state.sine_wave.update<wave_type_t::Sine>(Clock::elapsed());

	offset_t mouse_pos{ Mouse::get_position() };

	game_state.draw_cursor = mouse_pos.x < globals::UniversalOrigin.x || mouse_pos.y < globals::UniversalOrigin.y || mouse_pos.x > globals::UniversalExtent.x || mouse_pos.y > globals::UniversalExtent.y;

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

inline void render() {
	if (game_state.window.is_closing()) {
		return;
	}

	game_state.renderer.clear(colors::Black);

	game_state.game_map.draw(game_state.game_atlas, -game_state.camera.get_position());

	game_state.player.draw(game_state.game_atlas, -game_state.camera.get_position());

	if (game_state.draw_cursor) {
		game_state.cursor.draw();
	} else {
		game_state.grid_cursor.draw(globals::CursorOffset - game_state.camera.get_position() * globals::CellSize);
	}

	runes_t fps_text{ std::format("FPS:{:4}", static_cast<u32>(Clock::frame_time())), colors::Green };

	game_state.ui_atlas.draw(fps_text, offset_t{ globals::UIGridSize.w - 1, 1 }, cardinal_t::West, offset_t{ 0, -4 });

	runes_t title_text{ globals::GameTitle, colors::Marble };

	game_state.ui_atlas.draw_label(game_state.renderer, title_text, offset_t{ globals::UIGridSize.w / 2, 1 }, cardinal_t::Central, offset_t{ 0, -4 }, colors::Black, colors::White);

	game_state.renderer.draw_outline_rect(offset_t::Zero, globals::WindowSize + globals::WindowBorder * 2, globals::BorderSize, colors::Black);
	game_state.renderer.draw_outline_rect(offset_t::Zero, globals::WindowSize + globals::WindowBorder * 2, colors::White);

	game_state.renderer.present();
}

inline void shutdown() {
	if (game_state.primary_gamepad != nullptr) {
		GamepadManager::release(0);
	}

	GamepadManager::terminate();

	Keyboard::terminate();
	Mouse::terminate();

	message_log.flush_to_file();
	error_log.flush_to_file();
}

inline void terminate_prematurely() {
	std::cout << "Message Log:\n";
	message_log.flush_to_console(std::cout);

	std::cerr << "\nError Log:\n";
	error_log.flush_to_console(std::cerr);

	shutdown();
	exit(EXIT_FAILURE);
}
