#include "typedef.hpp"

#include <cstdlib>
#include <format>
#include <initializer_list>
#include <iostream>
#include <random>
#include <utility>

#include <SDL.h>

#include "array.hpp"
#include "atlas.hpp"
#include "cardinal.hpp"
#include "clock.hpp"
#include "cursor.hpp"
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
	constexpr cstr GameName = "Bleakdepth";
	constexpr cstr GameVersion = "0.0.1";
	constexpr cstr GameAuthor = "Ryan Lockhart";

	const std::string GameTitle { std::format("{} v{} by {}", GameName, GameVersion, GameAuthor) };

	constexpr u32 WindowFlags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN;
	constexpr u32 RendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	constexpr u32 FrameLimit = 60u;
	constexpr f32 FrameTime = 1000.0f / FrameLimit;

	constexpr size_t<i32> WindowSize { 640, 480 };

	static window_t Window { GameTitle.c_str(), WindowSize, WindowFlags };
	static renderer_t Renderer { Window, RendererFlags };

	static atlas_t<16, 16> GameAtlas { { Renderer, "resources\\glyphs_16x16.png" } };
	static atlas_t<16, 16> UIAtlas { { Renderer, "resources\\glyphs_8x8.png" } };

	static cursor_t Cursor { { Renderer, "resources\\cursor.png" }, Colors::White };
	static grid_cursor_t<16, 16> GridCursor { { Renderer, "resources\\grid_cursor.png" }, { -4, -4 }, Colors::Metals::Gold };

	static point_t<i32> CameraPosition { 0, 0 };

	static map_t<32, 32> Map {};

	static animated_sprite_t<3> Player { { { u8 { 0xB0 }, u8 { 0xB1 }, u8 { 0xB2 } }, Colors::Green }, { 0, 0 } };

	static timer_t InputTimer { 100 };
	static timer_t EpochTimer { 250 };
	static timer_t AnimationTimer { 1000.0 / 3 };

	static wave_t SineWave { 1.0, 0.5, 1.0 };

	static log_t MessageLog;
	static log_t ErrorLog;

} // namespace Bleakdepth

#define main SDL_main

using namespace Bleakdepth;

void initialize_map() {
	std::mt19937 generator { std::random_device()() };
	std::bernoulli_distribution distribution { 0.5 };

	array_t<bool, Map.width, Map.height> solids {};

	for (usize y { 0 }; y < Map.height; ++y) {
		for (uhalf x { 0 }; x < Map.width; ++x) {
			bool border = x == 0 || x == Map.width - 1 || y == 0 || y == Map.height - 1;
			solids[x, y] = border ? true : distribution(generator);
		}
	}

	auto buffer { solids };

	for (u8 i { 0 }; i < 5; ++i) {
		for (uhalf y { 0 }; y < Map.height; ++y) {
			for (uhalf x { 0 }; x < Map.width; ++x) {
				const point_t<uhalf> pos { x, y };
				bool border = x == 0 || x == Map.width - 1 || y == 0 || y == Map.height - 1;

				if (border) {
					buffer[pos] = true;
					continue;
				}

				u8 neighbours { 0 };

				for (i8 y_offs { -1 }; y_offs <= 1; ++y_offs) {
					for (i8 x_offs { -1 }; x_offs <= 1; ++x_offs) {
						if (x_offs == 0 && y_offs == 0) {
							continue;
						}
						const point_t<uhalf> offs_pos { static_cast<uhalf>(x_offs + pos.x), static_cast<uhalf>(y_offs + pos.y) };

						if (solids.valid(offs_pos) && solids[offs_pos]) {
							++neighbours;
						}
					}
				}

				if (neighbours > 4) {
					buffer[pos] = true;
				} else if (neighbours < 4) {
					buffer[pos] = false;
				} else {
					buffer[pos] = solids[pos];
				}
			}
		}

		std::swap(solids, buffer);
	}

	std::swap(solids, buffer);

	for (usize i { 0 }; i < Map.area; ++i) {
		Map[i].add(cell_state_t::Seen | cell_state_t::Explored);
		if (solids[i]) {
			Map[i].add(cell_state_t::Solid);
		}
	}
}

bool camera_movement() {
	point_t<> direction { 0, 0 };

	if (Keyboard.IsKeyPressed(Bindings::CameraMovement[cardinal_t::North])) {
		++direction.y;
	}
	if (Keyboard.IsKeyPressed(Bindings::CameraMovement[cardinal_t::South])) {
		--direction.y;
	}
	if (Keyboard.IsKeyPressed(Bindings::CameraMovement[cardinal_t::West])) {
		++direction.x;
	}
	if (Keyboard.IsKeyPressed(Bindings::CameraMovement[cardinal_t::East])) {
		--direction.x;
	}

	if (direction != point_t<>::Zero) {
		CameraPosition += direction;

		InputTimer.record();

		return true;
	}

	return false;
}

bool character_movement() {
	point_t<> direction { 0, 0 };

	if (Keyboard.AnyKeysPressed(Bindings::CharacterMovement[cardinal_t::North])) {
		--direction.y;
	}
	if (Keyboard.AnyKeysPressed(Bindings::CharacterMovement[cardinal_t::South])) {
		++direction.y;
	}
	if (Keyboard.AnyKeysPressed(Bindings::CharacterMovement[cardinal_t::West])) {
		--direction.x;
	}
	if (Keyboard.AnyKeysPressed(Bindings::CharacterMovement[cardinal_t::East])) {
		++direction.x;
	}

	if (direction != point_t<>::Zero) {
		Player.position += direction;

		InputTimer.record();
		EpochTimer.record();

		return true;
	}

	return false;
}

int main(int argc, char* argv[]) {
	for (int i { 0 }; i < argc; ++i) {
		MessageLog.add("{}", argv[i]);
	}

	std::cout << (std::string)MessageLog << std::endl;

	initialize_map();

	Mouse.HideCursor();

	Clock.tick();

	InputTimer.reset();
	EpochTimer.reset();

	AnimationTimer.reset();

	while (!Window.isClosing()) {
		Clock.tick(FrameTime);

		Window.pollEvents();

		if (AnimationTimer.ready()) {
			Player.glyph.advance();
			AnimationTimer.record();
		}

		if (Keyboard.IsKeyDown(SDL_SCANCODE_ESCAPE)) {
			Window.close();
			continue;
		}

		if (InputTimer.ready()) {
			camera_movement();

			if (EpochTimer.ready()) {
				character_movement();
			}
		}

		SineWave.update<wave_type_t::Sine>(Clock.elapsed());

		GridCursor.color.setAlpha(SineWave.currentValue());
		GridCursor.update();

		Renderer.clear(Colors::Black);

		Map.draw(GameAtlas, CameraPosition);
		Player.draw(GameAtlas, CameraPosition);

		GridCursor.draw();

		Renderer.present();
	}

	return EXIT_SUCCESS;
}
