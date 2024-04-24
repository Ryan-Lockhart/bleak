#include "typedef.hpp"

#include <SDL_scancode.h>
#include <cstdlib>
#include <iostream>

#include <SDL.h>

#include "clock.hpp"
#include "renderer.hpp"
#include "subsystem.hpp"
#include "window.hpp"

#include "log.hpp"

#include "keyboard.hpp"
#include "mouse.hpp"

#include "atlas.hpp"
#include "timer.hpp"

#include <format>

namespace Bleakdepth {
	constexpr cstr GameName = "Bleakdepth";
	constexpr cstr GameVersion = "0.0.1";
	constexpr cstr GameAuthor = "Ryan Lockhart";

	const std::string GameTitle { std::format("{} v{} by {}", GameName, GameVersion, GameAuthor) };

	constexpr u32 WindowFlags = SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS;
	constexpr u32 RendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	constexpr size_t<i32> WindowSize { 800, 600 };

	static window_t Window { GameTitle.c_str(), WindowSize, WindowFlags };
	static renderer_t Renderer { Window, RendererFlags };

	static atlas_t<16, 16> GameAtlas { { Renderer, "resources\\glyphs_16x16.png" } };
	static atlas_t<16, 16> UIAtlas { { Renderer, "resources\\glyphs_8x8.png" } };

	static texture_t CursorTexture { Renderer, "resources\\cursor.png" };

	static timer_t InputTimer { 0.25 };
	static timer_t EpochTimer { 1.00 };

	static log_t MessageLog;
	static log_t ErrorLog;

} // namespace Bleakdepth

using namespace Bleakdepth;

#define main SDL_main

#include "constants/colors.hpp"

int main(int argv, char** args) {
	while (!Window.isClosing()) {
		Window.pollEvents();

		if (Keyboard.IsKeyDown(SDL_SCANCODE_ESCAPE)) {
			Window.close();
		}

		Renderer.clear(Colors::Black);

		Renderer.present();
	}

	return EXIT_SUCCESS;
}
