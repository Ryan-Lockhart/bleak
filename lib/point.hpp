#pragma once

#include "typedef.hpp"

#include <SDL_rect.h>
#include <cmath>
#include <format>
#include <string>
#include <utility>

#include <SDL.h>

#include "cardinal.hpp"

namespace Bleakdepth {
	template<typename T = i32> struct point_t {
		union {
			T x, w;
		};

		union {
			T y, h;
		};

		static const point_t Zero;

		static const point_t North;
		static const point_t South;
		static const point_t East;
		static const point_t West;

		static const point_t Northeast;
		static const point_t Northwest;

		static const point_t Southeast;
		static const point_t Southwest;

		static const point_t Central;

		constexpr point_t() noexcept : x(Zero.x), y(Zero.y) {}

		constexpr point_t(T scalar) noexcept : x(scalar), y(scalar) {}

		constexpr point_t(T x, T y) noexcept : x(x), y(y) {}

		constexpr point_t(cref<point_t> other) noexcept : x(other.x), y(other.y) {}

		constexpr point_t(rval<point_t> other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {}

		constexpr point_t(cref<SDL_Point> other) noexcept : x(other.x), y(other.y) {}

		constexpr point_t(rval<SDL_Point> other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {}

		constexpr explicit point_t(cref<SDL_FPoint> other) noexcept :
			x(static_cast<T>(other.x)),
			y(static_cast<T>(other.y)) {}

		constexpr explicit point_t(rval<SDL_FPoint> other) noexcept :
			x(static_cast<T>(std::move(other.x))),
			y(static_cast<T>(std::move(other.y))) {}

		constexpr ~point_t() = default;

		constexpr ref<point_t> operator=(cref<point_t> other) noexcept {
			x = other.x;
			y = other.y;

			return *this;
		}

		constexpr ref<point_t> operator=(rval<point_t> other) noexcept {
			x = std::move(other.x);
			y = std::move(other.y);

			return *this;
		}

		constexpr ref<point_t> operator=(cref<SDL_Point> other) noexcept {
			x = other.x;
			y = other.y;

			return *this;
		}

		constexpr ref<point_t> operator=(rval<SDL_Point> other) noexcept {
			x = std::move(other.x);
			y = std::move(other.y);

			return *this;
		}

		constexpr ref<point_t> operator=(cref<SDL_FPoint> other) noexcept {
			x = static_cast<T>(other.x);
			y = static_cast<T>(other.y);

			return *this;
		}

		constexpr ref<point_t> operator=(rval<SDL_FPoint> other) noexcept {
			x = static_cast<T>(std::move(other.x));
			y = static_cast<T>(std::move(other.y));

			return *this;
		}

		constexpr usize size() const noexcept { return (usize)x * y; }

		constexpr usize dot() const noexcept { return (usize)x * x + (usize)y * y; }

		template<typename L, typename = f32> constexpr L length() const noexcept;

		template<> constexpr f32 length() const noexcept { return std::sqrt(static_cast<f32>(dot())); }

		template<> constexpr f64 length() const noexcept { return std::sqrt(static_cast<f64>(dot())); }

		template<> constexpr i32 length() const noexcept {
			union {
				f32 f;
				i32 i;
			} v;

			v.f = (f32)dot();

			v.i -= 1 << 23;
			v.i >>= 1;
			v.i += 1 << 29;

			return (i32)((f32)v.i);
		}

		template<> constexpr i64 length() const noexcept {
			union {
				f64 f;
				i64 i;
			} v;

			v.f = (f64)dot();

			v.i -= 1LL << 52;
			v.i >>= 1;
			v.i += 1LL << 58;

			return (i64)((f64)v.i);
		}

		constexpr bool operator==(cref<point_t> other) const noexcept { return x == other.x && y == other.y; }

		constexpr bool operator!=(cref<point_t> other) const noexcept { return x != other.x || y != other.y; }

		constexpr bool operator<(cref<point_t> other) const { return x < other.x && y < other.y; }

		constexpr bool operator>(cref<point_t> other) const { return x > other.x && y > other.y; }

		constexpr bool operator<=(cref<point_t> other) const { return x <= other.x && y <= other.y; }

		constexpr bool operator>=(cref<point_t> other) const { return x >= other.x && y >= other.y; }

		constexpr point_t operator-() const noexcept { return { -x, -y }; }

		constexpr point_t operator+(cref<point_t> other) const noexcept { return { x + other.x, y + other.y }; }

		constexpr point_t operator-(cref<point_t> other) const noexcept { return { x - other.x, y - other.y }; }

		constexpr point_t operator*(cref<point_t> other) const noexcept { return { x * other.x, y * other.y }; }

		constexpr point_t operator/(cref<point_t> other) const noexcept { return { x / other.x, y / other.y }; }

		constexpr point_t operator%(cref<point_t> other) const noexcept { return { x % other.x, y % other.y }; }

		constexpr point_t operator+(T scalar) const noexcept { return { x + scalar, y + scalar }; }

		constexpr point_t operator-(T scalar) const noexcept { return { x - scalar, y - scalar }; }

		constexpr point_t operator*(T scalar) const noexcept { return { x * scalar, y * scalar }; }

		constexpr point_t operator/(T scalar) const noexcept { return { x / scalar, y / scalar }; }

		constexpr point_t operator%(T scalar) const noexcept { return { x % scalar, y % scalar }; }

		constexpr ref<point_t> operator-() noexcept {
			x = -x;
			y = -y;
			return *this;
		}

		constexpr ref<point_t> operator+=(cref<point_t> other) noexcept {
			x += other.x;
			y += other.y;
			return *this;
		}

		constexpr ref<point_t> operator-=(cref<point_t> other) noexcept {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		constexpr ref<point_t> operator*=(cref<point_t> other) noexcept {
			x *= other.x;
			y *= other.y;
			return *this;
		}

		constexpr ref<point_t> operator/=(cref<point_t> other) noexcept {
			x /= other.x;
			y /= other.y;
			return *this;
		}

		constexpr ref<point_t> operator%=(cref<point_t> other) noexcept {
			x %= other.x;
			y %= other.y;
			return *this;
		}

		constexpr ref<point_t> operator+=(T scalar) noexcept {
			x += scalar;
			y += scalar;
			return *this;
		}

		constexpr ref<point_t> operator-=(T scalar) noexcept {
			x -= scalar;
			y -= scalar;
			return *this;
		}

		constexpr ref<point_t> operator*=(T scalar) noexcept {
			x *= scalar;
			y *= scalar;
			return *this;
		}

		constexpr ref<point_t> operator/=(T scalar) noexcept {
			x /= scalar;
			y /= scalar;
			return *this;
		}

		constexpr ref<point_t> operator%=(T scalar) noexcept {
			x %= scalar;
			y %= scalar;
			return *this;
		}

		constexpr operator SDL_Point() const noexcept { return SDL_Point { x, y }; }

		constexpr explicit operator SDL_FPoint() const noexcept {
			return SDL_FPoint { static_cast<f32>(x), static_cast<f32>(y) };
		}

		constexpr operator std::string() const noexcept { return std::format("[{}, {}]", x, y); }

		constexpr operator cardinal_t() const noexcept {
			if (x == 0 && y == 0) {
				return cardinal_t::Central;
			} else {
				cardinal_t result = cardinal_t::Central;

				if (x != 0) {
					result |= x < 0 ? cardinal_t::West : cardinal_t::East;
				}
				if (y != 0) {
					result |= y < 0 ? cardinal_t::North : cardinal_t::South;
				}

				return result;
			}
		}
	};

	template<typename T = i32> using size_t = point_t<T>;

	template<> constexpr const point_t point_t<i32>::Zero = { 0 };

	template<> constexpr const point_t point_t<i32>::Central = point_t::Zero;

	template<> constexpr const point_t point_t<i32>::North = { 0, -1 };
	template<> constexpr const point_t point_t<i32>::South = { 0, 1 };

	template<> constexpr const point_t point_t<i32>::East = { 1, 0 };
	template<> constexpr const point_t point_t<i32>::West = { -1, 0 };

	template<> constexpr const point_t point_t<i32>::Northeast = { 1, -1 };
	template<> constexpr const point_t point_t<i32>::Northwest = { -1, -1 };

	template<> constexpr const point_t point_t<i32>::Southeast = { 1, 1 };
	template<> constexpr const point_t point_t<i32>::Southwest = { -1, 1 };
} // namespace Bleakdepth
