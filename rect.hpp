#pragma once

#include "typedef.hpp"

#include "point.hpp"
#include "line.hpp"

#include <utility>

struct Rect
{
	using rect_t = i32;

	rect_t x, y, w, h;

	constexpr Rect() noexcept : x(0), y(0), w(0), h(0) {}
	constexpr Rect(rect_t x, rect_t y, rect_t w, rect_t h) noexcept : x(x), y(y), w(w), h(h) {}
	constexpr Rect(cref<Point> point, cref<Point> size) noexcept : x(point.x), y(point.y), w(size.x), h(size.y) {}

	constexpr Rect(cref<Rect> other) noexcept : x(other.x), y(other.y), w(other.w), h(other.h) {}
	constexpr Rect(rval<Rect> other) noexcept : x(std::move(other.x)), y(std::move(other.y)), w(std::move(other.w)), h(std::move(other.h)) {}

	constexpr Rect& operator=(cref<Rect> other) noexcept
	{
		x = other.x;
		y = other.y;

		w = other.w;
		h = other.h;

		return *this;
	}

	constexpr Rect& operator=(rval<Rect> other) noexcept
	{
		x = std::move(other.x);
		y = std::move(other.y);

		w = std::move(other.w);
		h = std::move(other.h);

		return *this;
	}

	constexpr bool operator==(cref<Rect> other) const noexcept { return x == other.x && y == other.y && w == other.w && h == other.h; }
	constexpr bool operator!=(cref<Rect> other) const noexcept { return x != other.x || y != other.y || w != other.w || h != other.h; }

	constexpr bool operator<(cref<Rect> other) = delete;
	constexpr bool operator<=(cref<Rect> other) = delete;

	constexpr bool operator>(cref<Rect> other) = delete;
	constexpr bool operator>=(cref<Rect> other) = delete;

	// I swear, if this works...

	constexpr ref<Point> position() noexcept { return *(Point*)(&x); }
	constexpr cref<Point> position() const noexcept { return *(Point*)(&x); }

	constexpr ref<Point> size() noexcept { return *(Point*)(&w); }
	constexpr cref<Point> size() const noexcept { return *(Point*)(&w); }

	constexpr Point center() const noexcept { return Point(x + w / 2, y + h / 2); }

	enum class Corner : u8
	{
		TopLeft,
		TopRight,
		BottomRight,
		BottomLeft
	};

	constexpr Point corner(Corner corner) const noexcept
	{
		switch (corner)
		{
			case Corner::TopLeft: return Point(x, y);
			case Corner::TopRight: return Point(x + w, y);
			case Corner::BottomRight: return Point(x + w, y + h);
			case Corner::BottomLeft: return Point(x, y + h);
		}

		return Point();
	}

	constexpr rect_t area() const noexcept { return w * h; }

	constexpr bool contains(rect_t x, rect_t y) const noexcept { return x >= this->x && x < this->x + w && y >= this->y && y < this->y + h; }
	constexpr bool contains(cref<Point> point) const noexcept { return point.x >= x && point.x < x + w && point.y >= y && point.y < y + h; }
	constexpr bool contains(cref<Line> line) const noexcept { return contains(line.start) && contains(line.end); }
};