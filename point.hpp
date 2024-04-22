#pragma once

#include "typedef.hpp"

#include <utility>
#include <string>

struct Point
{
	using point_t = i32;

	point_t x, y;

	static const Point Zero;

	constexpr Point() noexcept : x(Zero.x), y(Zero.y) {}
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

	constexpr operator std::string() const noexcept { return "[" + std::to_string(x) + ", " + std::to_string(y) + "]"; }
};

constexpr Point Point::Zero = { 0, 0 };
