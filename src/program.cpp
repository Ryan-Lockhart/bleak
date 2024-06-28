#include "typedef.hpp"

#include <cassert>
#include <cstdlib>
#include <format>
#include <initializer_list>
#include <iostream>

#include <SDL.h>

#include "atlas.hpp"
#include "cardinal.hpp"
#include "clock.hpp"
#include "cursor.hpp"
#include "keyboard.hpp"
#include "log.hpp"
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
	constexpr size_t<i32> WindowPadding { 8, 8 };

	constexpr size_t<i32> GameGridSize { WindowSize / 16 };
	constexpr size_t<i32> UIGridSize { WindowSize / 8 };

	static window_t Window { GameTitle.c_str(), WindowSize + WindowPadding, WindowFlags };
	static renderer_t Renderer { Window, RendererFlags };

	static atlas_t<16, 16> GameAtlas { { Renderer, "res\\glyphs_16x16.png" } };
	static atlas_t<16, 16> UIAtlas { { Renderer, "res\\glyphs_8x8.png" } };

	static cursor_t Cursor { { Renderer, "res\\cursor.png" }, Colors::White };
	static grid_cursor_t<16, 16> GridCursor { { Renderer, "res\\grid_cursor.png" }, { 0 }, Colors::Metals::Gold, { 0, 0 }, GameGridSize - 1 };

	static point_t<i32> CameraPosition { 0 };

	static animated_sprite_t<3> Player { { { u8 { 0xB0 }, u8 { 0xB1 }, u8 { 0xB2 } }, Colors::Green }, { 0 } };

	static timer_t InputTimer { 100 };
	static timer_t EpochTimer { 250 };
	static timer_t AnimationTimer { 1000.0 / 3 };

	static wave_t SineWave { 1.0, 0.5, 1.0 };

	static log_t MessageLog;
	static log_t ErrorLog;

} // namespace Bleakdepth

#define main SDL_main

using namespace Bleakdepth;

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

	GameAtlas.universal_offset = WindowPadding / 2;
	UIAtlas.universal_offset = WindowPadding / 2;

	GameAtlas.universal_foffset = static_cast<point_t<f32>>(WindowPadding / 2);
	UIAtlas.universal_foffset = static_cast<point_t<f32>>(WindowPadding / 2);

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

		Player.draw(GameAtlas, CameraPosition);

		GridCursor.draw();

		runes_t fpsText { std::format("FPS: {}", static_cast<u32>(Clock.frameTime())), Colors::White };

		UIAtlas.draw(fpsText, point_t<i32> { 0, UIGridSize.h - 1 });

		Renderer.present();
	}

	return EXIT_SUCCESS;
}
