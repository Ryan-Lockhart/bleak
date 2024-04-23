#pragma once

#include "typedef.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <fmt/core.h>

#include "window.hpp"
#include "color.hpp"
#include "point.hpp"
#include "line.hpp"
#include "rect.hpp"

class Renderer
{
private:
	ptr<SDL_Window> window;
	ptr<SDL_Renderer> renderer;

public:
	inline Renderer(ptr<SDL_Window> window, u32 flags) : window(window), renderer(nullptr)
	{
		if (!window) throw std::runtime_error("window is nullptr");

		renderer = SDL_CreateRenderer(window, -1, flags);

		if (!renderer) throw std::runtime_error(fmt::format("failed to create renderer: {}", SDL_GetError()));
	}

	inline ~Renderer() { if (renderer) SDL_DestroyRenderer(renderer); }

	constexpr ptr<SDL_Renderer> handle() noexcept { return renderer; }
	constexpr cptr<SDL_Renderer> handle() const noexcept { return renderer; }

	inline void setDrawColor(u8 r, u8 g, u8 b, u8 a) noexcept { SDL_SetRenderDrawColor(renderer, r, g, b, a); }
	inline void setDrawColor(cref<Color> color) noexcept { SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); }

	inline void clear() noexcept { SDL_RenderClear(renderer); }
	inline void clear(cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderClear(renderer);
	}

	inline void present() noexcept { SDL_RenderPresent(renderer); }

	inline void drawPoint(i32 x, i32 y) noexcept { SDL_RenderDrawPoint(renderer, x, y); }
	inline void drawPoint(i32 x, i32 y, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderDrawPoint(renderer, x, y);
	}

	inline void drawPoint(cref<Point> point) noexcept { SDL_RenderDrawPoint(renderer, point.x, point.y); }
	inline void drawPoint(cref<Point> point, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderDrawPoint(renderer, point.x, point.y);
	}

	inline void drawLine(i32 startX, i32 startY, i32 endX, i32 endY) noexcept { SDL_RenderDrawLine(renderer, startX, startY, endX, endY); }
	inline void drawLine(i32 startX, i32 startY, i32 endX, i32 endY, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
	}

	inline void drawLine(cref<Point> start, cref<Point> end) noexcept { SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y); }
	inline void drawLine(cref<Point> start, cref<Point> end, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
	}

	inline void drawLine(cref<Line> line) noexcept { SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y); }
	inline void drawLine(cref<Line> line, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);
	}

	inline void drawRect(cref<SDL_Rect> rect) noexcept { SDL_RenderDrawRect(renderer, &rect); }
	inline void drawRect(cref<SDL_Rect> rect, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderDrawRect(renderer, &rect);
	}

	inline void drawRect(cref<Rect> rect) noexcept { SDL_RenderDrawRect(renderer, (ptr<SDL_Rect>)&rect); }
	inline void drawRect(cref<Rect> rect, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderDrawRect(renderer, (ptr<SDL_Rect>)&rect);
	}

	inline void fillRect(cref<SDL_Rect> rect) noexcept { SDL_RenderFillRect(renderer, &rect); }
	inline void fillRect(cref<SDL_Rect> rect, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderFillRect(renderer, &rect);
	}

	inline void fillRect(cref<Rect> rect) noexcept { SDL_RenderDrawRect(renderer, (ptr<SDL_Rect>)&rect); }
	inline void fillRect(cref<Rect> rect, cref<Color> color) noexcept
	{
		setDrawColor(color);
		SDL_RenderFillRect(renderer, (ptr<SDL_Rect>)&rect);
	}

	inline void drawRect(cref<Rect> rect, cref<Color> fill, cref<Color> outline) noexcept
	{
		fillRect(rect, fill);
		drawRect(rect, outline);
	}

	inline void drawRect(cref<Rect> rect, cref<Color> fill, cref<Color> outline, i32 outlineThickness) noexcept
	{
		fillRect(rect, fill);
		drawRect(rect, outline);
	}
};
