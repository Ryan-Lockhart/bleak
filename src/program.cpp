#include "point.hpp"
#include "texture.hpp"
#include "typedef.hpp"

#include <SDL_scancode.h>
#include <SDL_video.h>
#include <cstdlib>

#include <SDL.h>

#include "clock.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include "log.hpp"

#include "keyboard.hpp"
#include "mouse.hpp"

#include "atlas.hpp"
#include "cursor.hpp"
#include "timer.hpp"

#include <format>
#include <iostream>

#include "constants/glyphs.hpp"

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

	static grid_cursor_t<16, 16> Cursor { { Renderer, "resources\\grid_cursor.png" }, { -4, -4 } };

	static point_t<i32> PlayerPosition { 0, 0 };

	static timer_t InputTimer { 100 };
	static timer_t EpochTimer { 250 };

	static log_t MessageLog;
	static log_t ErrorLog;

} // namespace Bleakdepth

#define main SDL_main

int main(int argc, char* argv[]) {
	for (int i { 0 }; i < argc; ++i) {
		Bleakdepth::MessageLog.add("{}", argv[i]);
	}

	std::cout << (std::string)Bleakdepth::MessageLog << std::endl;

	Bleakdepth::Mouse.HideCursor();

	Bleakdepth::Clock.tick();

	Bleakdepth::InputTimer.reset();
	Bleakdepth::EpochTimer.reset();

	while (!Bleakdepth::Window.isClosing()) {

		Bleakdepth::Clock.tick(Bleakdepth::FrameTime);

		Bleakdepth::Window.pollEvents();

		if (Bleakdepth::Keyboard.IsKeyDown(SDL_SCANCODE_ESCAPE)) {
			Bleakdepth::Window.close();
			Bleakdepth::InputTimer.record();
		}

		if (Bleakdepth::InputTimer.ready() && Bleakdepth::EpochTimer.ready()) {
			Bleakdepth::point_t<> direction { 0, 0 };

			if (Bleakdepth::Keyboard.IsKeyPressed(SDL_SCANCODE_W)) {
				--direction.y;
			}
			if (Bleakdepth::Keyboard.IsKeyPressed(SDL_SCANCODE_S)) {
				++direction.y;
			}
			if (Bleakdepth::Keyboard.IsKeyPressed(SDL_SCANCODE_A)) {
				--direction.x;
			}
			if (Bleakdepth::Keyboard.IsKeyPressed(SDL_SCANCODE_D)) {
				++direction.x;
			}

			if (direction != Bleakdepth::point_t<>::Zero) {
				Bleakdepth::InputTimer.record();
				Bleakdepth::EpochTimer.record();

				Bleakdepth::PlayerPosition += direction;
			}
		}

		Bleakdepth::Cursor.update();

		Bleakdepth::Renderer.clear(Bleakdepth::Colors::Black);

		Bleakdepth::GameAtlas.draw(Bleakdepth::Glyphs::Player, Bleakdepth::PlayerPosition);
		Bleakdepth::Cursor.draw();

		Bleakdepth::Renderer.present();
	}

	return EXIT_SUCCESS;
}
