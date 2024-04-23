#pragma once

#include "typedef.hpp"

#include <utility>

#include <SDL2/SDL.h>

#include "point.hpp"
#include "line.hpp"

struct Rect
{
	using rect_t = Point::point_t;

	Point position;
	Size size;

	constexpr Rect() noexcept : position(), size() {}
	constexpr Rect(rect_t x, rect_t y, rect_t w, rect_t h) noexcept : position(x, y), size(w, h) {}
	constexpr Rect(cref<Point> position, cref<Size> size) noexcept : position(position), size(size) {}

	constexpr Rect(cref<Rect> other) noexcept : position(other.position), size(other.size) {}
	constexpr Rect(rval<Rect> other) noexcept : position(std::move(other.position)), size(std::move(other.size)) {}

	constexpr ref<Rect> operator=(cref<Rect> other) noexcept
	{
		position = other.position;
		size = other.size;

		return *this;
	}

	constexpr ref<Rect> operator=(rval<Rect> other) noexcept
	{
		position = std::move(other.position);
		size = std::move(other.size);

		return *this;
	}

	constexpr bool operator==(cref<Rect> other) const noexcept { return position == other.position && size == other.size; }
	constexpr bool operator!=(cref<Rect> other) const noexcept { return position != other.position || size != other.size; }

	constexpr bool operator<(cref<Rect> other) = delete;
	constexpr bool operator<=(cref<Rect> other) = delete;

	constexpr bool operator>(cref<Rect> other) = delete;
	constexpr bool operator>=(cref<Rect> other) = delete;

	constexpr Point center() const noexcept { return position + size / 2; }

	constexpr rect_t area() const noexcept { return size.size(); }

	constexpr bool contains(rect_t x, rect_t y) const noexcept { return x >= position.x && x < position.x + size.w && y >= position.y && y < position.y + size.h; }
	constexpr bool contains(cref<Point> point) const noexcept { return point.x >= position.x && point.x < position.x + size.w && point.y >= position.y && point.y < position.y + size.h; }
	constexpr bool contains(cref<Line> line) const noexcept { return contains(line.start) && contains(line.end); }
	constexpr bool contains(cref<Rect> other) const noexcept
	{
		return position.x <= other.position.x && position.y <= other.position.y && position.x + size.w >= other.position.x + other.size.w && position.y + size.h >= other.position.y + other.size.h;
	}

	constexpr bool intersects(cref<Rect> other) const noexcept
	{
		return position.x < other.position.x + other.size.w && position.x + size.w > other.position.x && position.y < other.position.y + other.size.h && position.y + size.h > other.position.y;
	}

	constexpr operator SDL_Rect() const noexcept { return SDL_Rect{ position.x, position.y, size.w, size.h }; }
	constexpr explicit operator SDL_FRect() const noexcept { return SDL_FRect{ (f32)position.x, (f32)position.y, (f32)size.w, (f32)size.h }; }
};
