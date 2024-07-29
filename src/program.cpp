#include "bleak/offset.hpp"
#include <bleak/bleak.hpp>

#include <cassert>
#include <cstdlib>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>

using namespace bleak;

namespace globals {
	constexpr std::string GameName{ "Bleakdepth" };
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

	constexpr offset_t CursorOffset{ UniversalOffset - CellSize / 4 };

	constexpr extent_t CameraExtent{ MapSize - globals::GameGridSize };
	constexpr offset_t::scalar_t CameraSpeed{ 2 };

	constexpr f64 FillPercent{ 0.425 };
	constexpr u32 AutomotaIterations{ 10 };
	constexpr u32 AutomotaThreshold{ 4 };

	constexpr u32 ViewDistance{ 8 };
	constexpr f64 ViewSpan{ 135.0 };

	constexpr u32 AdventurerPopulation{ 5 };
} // namespace globals

subsystem_s subsystem{};

window_t window{ globals::GameTitle.c_str(), globals::WindowSize + globals::WindowBorder * 2, globals::WindowFlags };
renderer_t renderer{ window, globals::RendererFlags };

atlas_t<globals::TilesetSize> game_atlas{ renderer, "res\\tiles\\tileset_16x16.png", globals::UniversalOffset };
atlas_t<globals::GlyphsetSize> ui_atlas{ renderer, "res\\glyphs\\glyphs_8x8.png", globals::UniversalOffset };

std::mt19937 random_engine{ std::random_device{}() };

zone_t<cell_state_t, globals::MapSize, globals::BorderSize> game_map{};

bool gamepad_enabled{ true };

cptr<gamepad_t> primary_gamepad{ nullptr };

bool gamepad_active{ false };

cursor_t cursor{ renderer, "res\\sprites\\cursor.png", colors::White };
grid_cursor_t<globals::CellSize> grid_cursor{ renderer, "res\\sprites\\grid_cursor.png", colors::metals::Gold, game_map.zone_origin, game_map.zone_extent };

bool draw_cursor{ true };

sprite_t player{ glyphs::Player };
area_t player_fov{};

std::vector<sprite_t> chasers{ globals::AdventurerPopulation, glyphs::Enemy };

camera_t camera{ globals::GameGridSize, extent_t::Zero, globals::CameraExtent };
bool camera_locked{ true };

timer_t input_timer{ 125.0 };
timer_t cursor_timer{ 125.0 };
timer_t epoch_timer{ 250.0 };
timer_t animation_timer{ 1000.0 / 3 };

wave_t sine_wave{ 1.0, 0.5, 1.0 };

mixer_s mixer{};

path_t test_path{};

field_t<offset_t::product_t, globals::MapSize, globals::BorderSize> goal_map{};

void primary_gamepad_disconnected() noexcept {
	gamepad_enabled = false;
	primary_gamepad = nullptr;
}

void primary_gamepad_reconnected(cptr<gamepad_t> gamepad) noexcept {
	gamepad_enabled = true;
	primary_gamepad = gamepad;
}

void primary_gamepad_active() noexcept { gamepad_active = true; }

void mouse_keyboard_active() noexcept { gamepad_active = false; }

#define main SDL_main

using namespace bleak;

inline bool update_camera() noexcept {
	if constexpr (globals::MapSize <= globals::GameGridSize) {
		return camera.center_on(globals::MapCenter);
	}

	if (Keyboard::is_key_down(bindings::CameraLock)) {
		camera_locked = !camera_locked;
	}

	if (camera_locked) {
		return camera.center_on(player.position);
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

	if (gamepad_enabled && direction == offset_t::Zero) {
		direction = static_cast<offset_t>(primary_gamepad->right_stick.current_state);
	}

	if (direction != offset_t::Zero) {
		input_timer.record();
		return camera.move(direction * globals::CameraSpeed);
	}

	return false;
}

inline void recalculate_fov(cref<offset_t> position, cardinal_t direction) noexcept {
	player_fov.repeal(game_map, cell_trait_t::Seen);
	player_fov.cast(game_map, cell_trait_t::Transperant, position, globals::ViewDistance, direction_to_angle(direction), globals::ViewSpan, true);
	player_fov.apply(game_map, cell_trait_t::Seen, cell_trait_t::Explored);
}

inline void recalculate_fov() noexcept {
	player_fov.repeal(game_map, cell_trait_t::Seen);
	//player_fov.cast(game_map, cell_trait_t::Transperant, player.position, globals::ViewDistance / 2, true);

	std::vector<circle_t> chaser_fovs{};

	for (crauto chaser : chasers) {
		chaser_fovs.emplace_back(chaser.position, globals::ViewDistance / 2);
	}

	chaser_fovs.emplace_back(player.position, globals::ViewDistance);

	player_fov.multi_cast(game_map, cell_trait_t::Transperant, chaser_fovs, true);

	player_fov.apply(game_map, cell_trait_t::Seen, cell_trait_t::Explored);
}

inline void recalculate_fov(cardinal_t direction) noexcept { recalculate_fov(player.position, direction); }

inline bool any_chaser_at_position(cref<offset_t> position) noexcept {
	for (cref<sprite_t> chaser : chasers) {
		if (chaser.position == position) {
			return true;
		}
	}

	return false;
}

inline bool character_movement() noexcept {
	if (Keyboard::any_keys_pressed(bindings::Wait)) {
		input_timer.record();
		epoch_timer.record();

		player.glyph.index = characters::Entity[cardinal_t::Central];

		//recalculate_fov();

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

	if (gamepad_enabled && direction == offset_t::Zero) {
		direction = static_cast<offset_t>(primary_gamepad->dpad.current_state);
	}

	if (direction != offset_t::Zero) {
		player.glyph.index = characters::Entity[direction];

		offset_t target_position{ player.position + direction };

		if (!game_map.within<zone_region_t::Interior>(target_position) || game_map[target_position].solid || any_chaser_at_position(target_position)) {
			//recalculate_fov(direction);
			return false;
		}

		goal_map.update(player.position, target_position);
		goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open);

		player.position += direction;

		//recalculate_fov(direction);

		input_timer.record();
		epoch_timer.record();

		return true;
	}

	return false;
}

inline bool chaser_movement() noexcept {
	bool any_moved{ false };
	for (rauto chaser : chasers) {
		crauto new_position{ goal_map.descend<zone_region_t::Interior>(chaser.position, random_engine) };

		if (!new_position.has_value() || new_position.value() == player.position || any_chaser_at_position(new_position.value())) {
			continue;
		}

		const cardinal_t chaser_direction{ offset_t::direction(chaser.position, new_position.value()) };

		chaser.position = new_position.value();

		chaser.glyph.index = characters::Entity[chaser_direction];

		any_moved = true;
	}

	recalculate_fov();

	return any_moved;
}

inline void startup() noexcept;

inline void update() noexcept;
inline void render() noexcept;

inline void shutdown() noexcept;

inline void terminate_prematurely() noexcept;

int main(int argc, char* argv[]) {
	startup();

	do {
		update();
		render();
	} while (window.is_running());

	shutdown();

	return EXIT_SUCCESS;
}

inline void startup() noexcept {
	Mouse::initialize();
	Keyboard::initialize();

	GamepadManager::initialize();

	Mouse::hide_cursor();

	primary_gamepad = GamepadManager::lease(0, &primary_gamepad_disconnected, &primary_gamepad_reconnected);
	gamepad_enabled = primary_gamepad != nullptr;

	if (primary_gamepad == nullptr) {
		message_log.add("no gamepad detected\n");
	}

	region_t<cell_state_t, globals::RegionSize, globals::ZoneSize, globals::BorderSize> region{ "res\\maps\\test_region.map" };

	constexpr cell_state_t open_state{ cell_trait_t::Open, cell_trait_t::Transperant, cell_trait_t::Unseen, cell_trait_t::Explored };
	constexpr cell_state_t closed_state{ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Unseen, cell_trait_t::Explored };

	constexpr binary_applicator_t<cell_state_t> cell_applicator{ closed_state, open_state };

	for (extent_t::product_t i{ 0 }; i < region.region_area; ++i) {
		region[i].set<zone_region_t::Border>(closed_state);
		region[i].generate<zone_region_t::Interior>(random_engine, globals::FillPercent, globals::AutomotaIterations, globals::AutomotaThreshold, cell_applicator);
		region[i].collapse<zone_region_t::Interior>(cell_trait_t::Solid, 0x00, cell_trait_t::Open);
	}

	region.compile(game_map);

	std::vector<area_t> areas{ area_t::partition(game_map, cell_trait_t::Open) };

	if (areas.size() > 1) {
		cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

		for (crauto area : areas) {
			if (area != largest_area) {
				area.apply(game_map, cell_trait_t::Solid);
			}
		}
	}

	auto player_start{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open) };

	if (player_start.has_value()) {
		player.position = player_start.value();
	} else {
		error_log.add("no open cells found for player start position\n", __TIME_FILE_LINE__);
		terminate_prematurely();
	}

	goal_map += player.position;
	goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open);

	update_camera();

	for (rauto chaser : chasers) {
		cauto chaser_start{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open) };

		if (chaser_start.has_value()) {
			chaser.position = chaser_start.value();
		} else {
			error_log.add("no open cells found for chaser start position\n", __TIME_FILE_LINE__);
			terminate_prematurely();
		}
	}

	recalculate_fov();

	Clock::tick();

	input_timer.reset();
	cursor_timer.reset();
	epoch_timer.reset();

	animation_timer.reset();

	message_log.flush_to_console(std::cout);
	error_log.flush_to_console(std::cerr);
}

inline void update() noexcept {
	if (window.is_closing()) {
		return;
	}

	if constexpr (globals::UseFrameLimit) {
		Clock::tick(globals::FrameTime);
	} else {
		Clock::tick();
	}

	window.poll_events();

	if (Keyboard::is_key_down(bindings::Quit)) {
		window.close();
		return;
	}

	if (Keyboard::is_key_down(bindings::RevealMap)) {
		game_map.apply<zone_region_t::All>(cell_trait_t::Explored);
	}

	bool turn_taken{ false };

	if (input_timer.ready()) {
		if (epoch_timer.ready()) {
			turn_taken = character_movement();
		}

		update_camera();
	}

	if (turn_taken) {
		chaser_movement();
	}

	sine_wave.update<wave_type_t::Sine>(Clock::elapsed());

	offset_t mouse_pos{ Mouse::get_position() };

	draw_cursor = mouse_pos.x < globals::UniversalOrigin.x || mouse_pos.y < globals::UniversalOrigin.y || mouse_pos.x > globals::UniversalExtent.x || mouse_pos.y > globals::UniversalExtent.y;

	if (draw_cursor) {
		cursor.update();
	} else {
		if (gamepad_enabled && gamepad_active) {
			if (primary_gamepad->left_stick.current_state != cardinal_t::Central && cursor_timer.ready()) {
				grid_cursor.update(primary_gamepad->left_stick.current_state);

				cursor_timer.record();
			}
		} else {
			grid_cursor.update(camera.get_position());
		}

		grid_cursor.color.set_alpha(sine_wave.current_value());
	}

	test_path.generate<distance_function_t::Octile>(player.position, grid_cursor.get_position(), game_map, cell_trait_t::Open);
}

inline void render() noexcept {
	if (window.is_closing()) {
		return;
	}

	renderer.clear(colors::Black);

	game_map.draw(game_atlas, camera);

	test_path.draw(game_atlas, { 0x4D, colors::Blue }, -camera.get_position());

	for (crauto chaser : chasers) {
		if (!player_fov.contains(chaser.position)) {
			continue;
		}
		
		chaser.draw(game_atlas, -camera.get_position());
	}

	player.draw(game_atlas, -camera.get_position());

	renderer.draw_outline_rect(offset_t::Zero, globals::WindowSize + globals::WindowBorder * 2, globals::BorderSize, colors::Black);
	renderer.draw_outline_rect(offset_t::Zero, globals::WindowSize + globals::WindowBorder * 2, colors::White);
	renderer.draw_outline_rect(globals::UniversalOffset, globals::WindowSize, colors::White);

	runes_t fps_text{ std::format("FPS:{:4}", static_cast<u32>(Clock::frame_time())), colors::Green };

	ui_atlas.draw_label(renderer, fps_text, offset_t{ globals::UIGridSize.w - 1, 0 }, cardinal_t::Southwest, extent_t{ 1, 1 }, colors::Black, colors::White);

	runes_t title_text{ globals::GameTitle, colors::Marble };

	ui_atlas.draw_label(renderer, title_text, offset_t{ globals::UIGridSize.w / 2, 0 }, cardinal_t::South, extent_t{ 1, 1 }, colors::Black, colors::White);

	runes_t tooltip_text{ game_map[grid_cursor.get_position()].to_tooltip(), colors::White };

	ui_atlas.draw_label(renderer, tooltip_text, offset_t{ globals::UIGridSize.w - 1, globals::UIGridSize.h - 1 }, cardinal_t::Northwest, extent_t{ 1, 1 }, colors::Black, colors::White);

	if (draw_cursor) {
		cursor.draw();
	} else {
		grid_cursor.draw(globals::CursorOffset - camera.get_position() * globals::CellSize);
	}

	renderer.present();
}

inline void shutdown() noexcept {
	if (primary_gamepad != nullptr) {
		GamepadManager::release(0);
	}

	GamepadManager::terminate();

	Keyboard::terminate();
	Mouse::terminate();

	message_log.flush_to_file();
	error_log.flush_to_file();
}

inline void terminate_prematurely() noexcept {
	std::cout << "Message Log:\n";
	message_log.flush_to_console(std::cout);

	std::cerr << "\nError Log:\n";
	error_log.flush_to_console(std::cerr);

	shutdown();
	exit(EXIT_FAILURE);
}
