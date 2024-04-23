#pragma once

#include "typedef.hpp"

#include <utility>
#include <string>

#include <SDL2\SDL.h>

struct Point
{
	using point_t = i32;

	union {
		// logical name of the first component of a point
		point_t x;
		// logical name of the first component of a size
		point_t w;
		// logical name of the first component of an index
		point_t i;
	};

	union {
		// logical name of the second component of a point
		point_t y;
		// logical name of the second component of a size
		point_t h;
		// logical name of the second component of an index
		point_t j;
	};

	static const Point zero;

	constexpr Point() noexcept : x(zero.x), y(zero.y) {}
	constexpr Point(point_t x, point_t y) noexcept : x(x), y(y) {}

	constexpr Point(cref<Point> other) noexcept : x(other.x), y(other.y) {}
	constexpr Point(rval<Point> other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {}

	constexpr ~Point() = default;

	constexpr ref<Point> operator=(cref<Point> other) noexcept
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	constexpr ref<Point> operator=(rval<Point> other) noexcept
	{
		x = std::move(other.x);
		y = std::move(other.y);

		return *this;
	}

	constexpr usize size() const noexcept { return (usize)x * y; }

	constexpr bool operator==(cref<Point> other) const noexcept { return x == other.x && y == other.y; }
	constexpr bool operator!=(cref<Point> other) const noexcept { return x != other.x || y != other.y; }

	constexpr bool operator<(cref<Point> other) const = delete;
	constexpr bool operator>(cref<Point> other) const = delete;

	constexpr bool operator<=(cref<Point> other) const = delete;
	constexpr bool operator>=(cref<Point> other) const = delete;

	constexpr Point operator-() const noexcept { return { -x, -y }; }

	constexpr Point operator+(cref<Point> other) const noexcept { return { x + other.x, y + other.y }; }
	constexpr Point operator-(cref<Point> other) const noexcept { return { x - other.x, y - other.y }; }

	constexpr Point operator*(cref<Point> other) const noexcept { return { x * other.x, y * other.y }; }
	constexpr Point operator/(cref<Point> other) const noexcept { return { x / other.x, y / other.y }; }
	constexpr Point operator%(cref<Point> other) const noexcept { return { x % other.x, y % other.y }; }

	constexpr Point operator+(point_t scalar) const noexcept { return { x + scalar, y + scalar }; }
	constexpr Point operator-(point_t scalar) const noexcept { return { x - scalar, y - scalar }; }

	constexpr Point operator*(point_t scalar) const noexcept { return { x * scalar, y * scalar }; }
	constexpr Point operator/(point_t scalar) const noexcept { return { x / scalar, y / scalar }; }
	constexpr Point operator%(point_t scalar) const noexcept { return { x % scalar, y % scalar }; }

	constexpr ref<Point> operator-() noexcept { x = -x; y = -y; return *this; }

	constexpr ref<Point> operator+=(cref<Point> other) noexcept { x += other.x; y += other.y; return *this; }
	constexpr ref<Point> operator-=(cref<Point> other) noexcept { x -= other.x; y -= other.y; return *this; }

	constexpr ref<Point> operator*=(cref<Point> other) noexcept { x *= other.x; y *= other.y; return *this; }
	constexpr ref<Point> operator/=(cref<Point> other) noexcept { x /= other.x; y /= other.y; return *this; }
	constexpr ref<Point> operator%=(cref<Point> other) noexcept { x %= other.x; y %= other.y; return *this; }

	constexpr ref<Point> operator+=(point_t scalar) noexcept { x += scalar; y += scalar; return *this; }
	constexpr ref<Point> operator-=(point_t scalar) noexcept { x -= scalar; y -= scalar; return *this; }

	constexpr ref<Point> operator*=(point_t scalar) noexcept { x *= scalar; y *= scalar; return *this; }
	constexpr ref<Point> operator/=(point_t scalar) noexcept { x /= scalar; y /= scalar; return *this; }
	constexpr ref<Point> operator%=(point_t scalar) noexcept { x %= scalar; y %= scalar; return *this; }

	constexpr operator SDL_Point() const noexcept { return { x, y }; }
	constexpr explicit operator SDL_FPoint() const noexcept { return { static_cast<f32>(x), static_cast<f32>(y) }; }

	inline operator std::string() const noexcept { return "[" + std::to_string(x) + ", " + std::to_string(y) + "]"; }
};

using Size = Point;

constexpr Point Point::zero = { 0, 0 };
