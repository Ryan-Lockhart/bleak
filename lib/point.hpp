#pragma once

#include "typedef.hpp"

#include <SDL_rect.h>
#include <utility>
#include <string>
#include <cmath>
#include <format>

#include <SDL.h>

#include "cardinal.hpp"

template<typename T = i32> struct Point
{
	union { T x, w;	};
	union { T y, h;	};

	static const Point Zero;

	static const Point North;
	static const Point South;
	static const Point East;
	static const Point West;

	static const Point Northeast;
	static const Point Northwest;

	static const Point Southeast;
	static const Point Southwest;

	static const Point Central;

	constexpr Point() noexcept : x(Zero.x), y(Zero.y) {}
	constexpr Point(T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr Point(T x, T y) noexcept : x(x), y(y) {}

	constexpr Point(cref<Point> other) noexcept : x(other.x), y(other.y) {}
	constexpr Point(rval<Point> other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {}

	constexpr Point(cref<SDL_Point> other) noexcept : x(other.x), y(other.y) {}
	constexpr Point(rval<SDL_Point> other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {}

	constexpr explicit Point(cref<SDL_FPoint> other) noexcept : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}
	constexpr explicit Point(rval<SDL_FPoint> other) noexcept : x(static_cast<T>(std::move(other.x))), y(static_cast<T>(std::move(other.y))) {}

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

	constexpr ref<Point> operator=(cref<SDL_Point> other) noexcept
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	constexpr ref<Point> operator=(rval<SDL_Point> other) noexcept
	{
		x = std::move(other.x);
		y = std::move(other.y);

		return *this;
	}

	constexpr ref<Point> operator=(cref<SDL_FPoint> other) noexcept
	{
		x = static_cast<T>(other.x);
		y = static_cast<T>(other.y);

		return *this;
	}

	constexpr ref<Point> operator=(rval<SDL_FPoint> other) noexcept
	{
		x = static_cast<T>(std::move(other.x));
		y = static_cast<T>(std::move(other.y));

		return *this;
	}

	constexpr usize size() const noexcept { return (usize)x * y; }
	constexpr usize dot() const noexcept { return (usize)x * x + (usize)y * y; }

	template<typename L, typename = f32>
	constexpr L length() const noexcept;

	template<> constexpr f32 length() const noexcept { return std::sqrt(static_cast<f32>(dot())); }
	template<> constexpr f64 length() const noexcept { return std::sqrt(static_cast<f64>(dot())); }
	template<> constexpr i32 length() const noexcept
	{
		union { f32 f; i32 i; } v;

		v.f = (f32)dot();

		v.i -= 1 << 23;
		v.i >>= 1;
		v.i += 1 << 29;

		return (i32)((f32)v.i);
	}

	template<> constexpr i64 length() const noexcept
	{
		union { f64 f; i64 i; } v;

		v.f = (f64)dot();

		v.i -= 1LL << 52;
		v.i >>= 1;
		v.i += 1LL << 58;

		return (i64)((f64)v.i);
	}

	constexpr bool operator==(cref<Point> other) const noexcept { return x == other.x && y == other.y; }
	constexpr bool operator!=(cref<Point> other) const noexcept { return x != other.x || y != other.y; }

	constexpr bool operator<(cref<Point> other) const { return x < other.x && y < other.y; }
	constexpr bool operator>(cref<Point> other) const { return x > other.x && y > other.y; }

	constexpr bool operator<=(cref<Point> other) const { return x <= other.x && y <= other.y; }
	constexpr bool operator>=(cref<Point> other) const { return x >= other.x && y >= other.y; }

	constexpr Point operator-() const noexcept { return { -x, -y }; }

	constexpr Point operator+(cref<Point> other) const noexcept { return { x + other.x, y + other.y }; }
	constexpr Point operator-(cref<Point> other) const noexcept { return { x - other.x, y - other.y }; }

	constexpr Point operator*(cref<Point> other) const noexcept { return { x * other.x, y * other.y }; }
	constexpr Point operator/(cref<Point> other) const noexcept { return { x / other.x, y / other.y }; }
	constexpr Point operator%(cref<Point> other) const noexcept { return { x % other.x, y % other.y }; }

	constexpr Point operator+(T scalar) const noexcept { return { x + scalar, y + scalar }; }
	constexpr Point operator-(T scalar) const noexcept { return { x - scalar, y - scalar }; }

	constexpr Point operator*(T scalar) const noexcept { return { x * scalar, y * scalar }; }
	constexpr Point operator/(T scalar) const noexcept { return { x / scalar, y / scalar }; }
	constexpr Point operator%(T scalar) const noexcept { return { x % scalar, y % scalar }; }

	constexpr ref<Point> operator-() noexcept { x = -x; y = -y; return *this; }

	constexpr ref<Point> operator+=(cref<Point> other) noexcept { x += other.x; y += other.y; return *this; }
	constexpr ref<Point> operator-=(cref<Point> other) noexcept { x -= other.x; y -= other.y; return *this; }

	constexpr ref<Point> operator*=(cref<Point> other) noexcept { x *= other.x; y *= other.y; return *this; }
	constexpr ref<Point> operator/=(cref<Point> other) noexcept { x /= other.x; y /= other.y; return *this; }
	constexpr ref<Point> operator%=(cref<Point> other) noexcept { x %= other.x; y %= other.y; return *this; }

	constexpr ref<Point> operator+=(T scalar) noexcept { x += scalar; y += scalar; return *this; }
	constexpr ref<Point> operator-=(T scalar) noexcept { x -= scalar; y -= scalar; return *this; }

	constexpr ref<Point> operator*=(T scalar) noexcept { x *= scalar; y *= scalar; return *this; }
	constexpr ref<Point> operator/=(T scalar) noexcept { x /= scalar; y /= scalar; return *this; }
	constexpr ref<Point> operator%=(T scalar) noexcept { x %= scalar; y %= scalar; return *this; }

	constexpr operator SDL_Point() const noexcept { return SDL_Point{ x, y }; }
	constexpr explicit operator SDL_FPoint() const noexcept { return SDL_FPoint{ static_cast<f32>(x), static_cast<f32>(y) }; }

	constexpr operator std::string() const noexcept { return std::format("[{}, {}]", x, y); }

	constexpr operator Cardinal() const noexcept
	{
		if (x == 0 && y == 0)
			return Cardinal::Central;
		else
		{
			Cardinal result = Cardinal::Central;

			if (x != 0) result |= x < 0 ? Cardinal::West : Cardinal::East;
			if (y != 0) result |= y < 0 ? Cardinal::North : Cardinal::South;

			return result;
		}
	}
};

template<typename T = i32> using Size = Point<T>;

template<> constexpr const Point Point<i32>::Zero = { 0 };

template<> constexpr const Point Point<i32>::Central = Point::Zero;

template<> constexpr const Point Point<i32>::North = { 0, -1 };
template<> constexpr const Point Point<i32>::South = { 0, 1 };

template<> constexpr const Point Point<i32>::East = { 1, 0 };
template<> constexpr const Point Point<i32>::West = { -1, 0 };

template<> constexpr const Point Point<i32>::Northeast = { 1, -1 };
template<> constexpr const Point Point<i32>::Northwest = { -1, -1 };

template<> constexpr const Point Point<i32>::Southeast = { 1, 1 };
template<> constexpr const Point Point<i32>::Southwest = { -1, 1 };
