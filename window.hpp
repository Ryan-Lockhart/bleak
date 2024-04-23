#pragma once

#include "typedef.hpp"

#include <stdexcept>
#include <format>

#include <SDL2/SDL.h>

#include "point.hpp"
#include "rect.hpp"

class Window
{
private:
	ptr<SDL_Window> window;

public:
	const cstr title;
	const Size size;
	const u32 flags;

private:
	bool closing = false;

public:

	inline Window() = delete;

	inline Window(cstr title, cref<Size> size, u32 flags) : window(nullptr), title(title), size(size), flags(flags)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.w, size.h, flags);

		if (!window) throw std::runtime_error(std::format("failed to create window: {}", SDL_GetError()));

		if (size.w <= 0) throw std::invalid_argument("width must be greater than zero!");
		if (size.h <= 0) throw std::invalid_argument("height must be greater than zero!");
	}

	inline Window(cstr title, cref<Point> position, cref<Size> size, u32 flags) : window(nullptr), title(title), size(size), flags(flags)
	{
		window = SDL_CreateWindow(title, position.x, position.y, size.w, size.h, flags);

		if (!window) throw std::runtime_error(std::format("failed to create window: {}", SDL_GetError()));

		if (size.w <= 0) throw std::invalid_argument("width must be greater than zero!");
		if (size.h <= 0) throw std::invalid_argument("height must be greater than zero!");
	}

	inline Window(cstr title, cref<Rect> transform, u32 flags) : window(nullptr), title(title), size(transform.size), flags(flags)
	{
		window = SDL_CreateWindow(title, transform.position.x, transform.position.y, transform.size.w, transform.size.h, flags);

		if (!window) throw std::runtime_error(std::format("failed to create window: {}", SDL_GetError()));

		if (size.w <= 0) throw std::invalid_argument("width must be greater than zero!");
		if (size.h <= 0) throw std::invalid_argument("height must be greater than zero!");
	}

	inline Window(cstr title, i32 width, i32 height, u32 flags) : window(nullptr), title(title), size(width, height), flags(flags)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

		if (!window) throw std::runtime_error(std::format("failed to create window: {}", SDL_GetError()));

		if (size.w <= 0) throw std::invalid_argument("width must be greater than zero!");
		if (size.h <= 0) throw std::invalid_argument("height must be greater than zero!");
	}

	inline Window(cstr title, i32 x, i32 y, i32 width, i32 height, u32 flags) : window(nullptr), title(title), size(width, height), flags(flags)
	{
		window = SDL_CreateWindow(title, x, y, width, height, flags);

		if (!window) throw std::runtime_error(std::format("failed to create window: {}", SDL_GetError()));

		if (size.w <= 0) throw std::invalid_argument("width must be greater than zero!");
		if (size.h <= 0) throw std::invalid_argument("height must be greater than zero!");
	}

	inline ~Window() { if (window) SDL_DestroyWindow(window); }

	constexpr bool closing() const noexcept { return closing; }
	constexpr void close() noexcept { closing = true; }

	constexpr ptr<SDL_Window> handle() noexcept { return window; }
	constexpr cptr<SDL_Window> handle() const noexcept { return window; }

	constexpr Point origin() const noexcept { return Size::zero; }
	constexpr Point center() const noexcept { return size / 2; }
	constexpr Point extents() const noexcept { return size - 1; }

	constexpr Rect bounds() const noexcept { return { origin(), extents() }; }
};
