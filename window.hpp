#pragma once

#include "typedef.hpp"

#include <stdexcept>
#include <format>

#include <SDL2/SDL.h>

class Window
{
private:
	ptr<SDL_Window> window;

public:
	const cstr title;

	const i32 width;
	const i32 height;
	const u32 flags;

private:
	bool closing = false;

public:

	inline Window() = delete;
	inline Window(cstr title, i32 width, i32 height, u32 flags) : window(nullptr), title(title), width(width), height(height), flags(flags)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

		if (!window) throw std::runtime_error(std::format("failed to create window ({})", SDL_GetError()));

		if (width <= 0) throw std::invalid_argument("width must be greater than zero!");
		if (height <= 0) throw std::invalid_argument("height must be greater than zero!");
	}

	constexpr bool closing() const noexcept { return closing; }
};
