#include "typedef.hpp"

#include <cstdlib>
#include <iostream>

#include <SDL.h>

#include "subsystem.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "delta.hpp"

#include "log.hpp"

#include "keyboard.hpp"
#include "mouse.hpp"

#include "timer.hpp"
#include "atlas.hpp"

constexpr cstr GAME_TITLE = "Bleakdepth";
constexpr cstr GAME_VERSION = "0.0.1";

constexpr u32 WINDOW_FLAGS = SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS;
constexpr u32 RENDERER_FLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

constexpr i32 WINDOW_WIDTH = 1280;
constexpr i32 WINDOW_HEIGHT = 480;

static Log MESSAGES;
static Log ERRORS;

static Subsystem SUBSYSTEM{ };

static Window WINDOW{ GAME_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS };
static Renderer RENDERER{ WINDOW, RENDERER_FLAGS };

static DeltaClock CLOCK{ };

static Atlas<16, 16> GAME_GLYPHS{ Texture{ RENDERER, "Assets\\glyphs_16x16.png" } };
static Atlas<16, 16> UI_GLYPHS{ Texture{ RENDERER, "Assets\\glyphs_8x8.png" } };

static Texture CURSOR{ RENDERER, "Assets\\cursor.png" };

static Timer INPUT_TIMER{ 0.25 };
static Timer EPOCH_TIMER{ 1.00 };

#define main SDL_main

int main(int argv, char** args)
{
	std::cout << "Hello, world!\n";

	return EXIT_SUCCESS;
}
