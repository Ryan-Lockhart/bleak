#pragma once

#include "typedef.hpp"

#include <cmath>
#include <format>
#include <string>
#include <utility>

#include <SDL.h>

#include "cardinal.hpp"
#include "point.hpp"

namespace Bleakdepth {
	template<typename T = i32> struct coord_t {
		union {
			T x, w;
		};

		union {
			T y, h;
		};

		union {
			T z, d;
		};

		static const coord_t Zero;

		struct Horizontal {
			static const coord_t North;
			static const coord_t South;
			static const coord_t East;
			static const coord_t West;

			static const coord_t Northeast;
			static const coord_t Northwest;

			static const coord_t Southeast;
			static const coord_t Southwest;

			static const coord_t Central;
		};

		struct Vertical {
			struct Up {
				static const coord_t North;
				static const coord_t South;
				static const coord_t East;
				static const coord_t West;

				static const coord_t Northeast;
				static const coord_t Northwest;

				static const coord_t Southeast;
				static const coord_t Southwest;

				static const coord_t Central;
			};

			struct Down {
				static const coord_t North;
				static const coord_t South;
				static const coord_t East;
				static const coord_t West;

				static const coord_t Northeast;
				static const coord_t Northwest;

				static const coord_t Southeast;
				static const coord_t Southwest;

				static const coord_t Central;
			};
		};

		constexpr coord_t() noexcept : x { Zero.x }, y { Zero.y }, z { Zero.z } {}

		constexpr coord_t(T scalar) noexcept : x { scalar }, y { scalar }, z { scalar } {}

		constexpr coord_t(T x, T y, T z) noexcept : x { x }, y { y }, z { z } {}

		constexpr coord_t(cref<coord_t> other) noexcept : x { other.x }, y { other.y }, z { other.z } {}

		constexpr coord_t(rval<coord_t> other) noexcept :
			x { std::move(other.x) },
			y { std::move(other.y) },
			z { std::move(other.z) } {}

		constexpr ~coord_t() = default;

		constexpr ref<coord_t> operator=(cref<coord_t> other) noexcept {
			x = other.x;
			y = other.y;
			z = other.z;

			return *this;
		}

		constexpr ref<coord_t> operator=(rval<coord_t> other) noexcept {
			x = std::move(other.x);
			y = std::move(other.y);
			z = std::move(other.z);

			return *this;
		}

		constexpr usize size() const noexcept { return (usize)w * h * d; }

		constexpr usize dot() const noexcept { return (usize)x * x + (usize)y * y + (usize)z * z; }

		constexpr point_t<T> cross(cref<coord_t> other) const noexcept {
			return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
		}

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

		constexpr bool operator==(cref<coord_t> other) const noexcept {
			return x == other.x && y == other.y && z == other.z;
		}

		constexpr bool operator!=(cref<coord_t> other) const noexcept {
			return x != other.x || y != other.y || z != other.z;
		}

		constexpr bool operator<(cref<coord_t> other) const { return x < other.x && y < other.y && z < other.z; }

		constexpr bool operator>(cref<coord_t> other) const { return x > other.x && y > other.y && z > other.z; }

		constexpr bool operator<=(cref<coord_t> other) const { return x <= other.x && y <= other.y && z <= other.zy; }

		constexpr bool operator>=(cref<coord_t> other) const { return x >= other.x && y >= other.y && z >= other.z; }

		constexpr coord_t operator-() const noexcept { return { -x, -y, -z }; }

		constexpr coord_t operator+(cref<coord_t> other) const noexcept {
			return { x + other.x, y + other.y, z + other.z };
		}

		constexpr coord_t operator-(cref<coord_t> other) const noexcept {
			return { x - other.x, y - other.y, z - other.z };
		}

		constexpr coord_t operator*(cref<coord_t> other) const noexcept {
			return { x * other.x, y * other.y, z * other.z };
		}

		constexpr coord_t operator/(cref<coord_t> other) const noexcept {
			return { x / other.x, y / other.y, z / other.z };
		}

		constexpr coord_t operator%(cref<coord_t> other) const noexcept {
			return { x % other.x, y % other.y, z % other.z };
		}

		template<f32> coord_t operator%(cref<coord_t> other) const noexcept {
			return { std::fmodf(x, other.x), std::fmodf(y, other.y), std::fmodf(z, other.z) };
		}

		template<f64> coord_t operator%(cref<coord_t> other) const noexcept {
			return { std::fmodl(x, other.x), std::fmodl(y, other.y), std::fmodl(z, other.z) };
		}

		constexpr coord_t operator+(T scalar) const noexcept { return { x + scalar, y + scalar, z + scalar }; }

		constexpr coord_t operator-(T scalar) const noexcept { return { x - scalar, y - scalar, z - scalar }; }

		constexpr coord_t operator*(T scalar) const noexcept { return { x * scalar, y * scalar, z * scalar }; }

		constexpr coord_t operator/(T scalar) const noexcept { return { x / scalar, y / scalar, z / scalar }; }

		constexpr coord_t operator%(T scalar) const noexcept { return { x % scalar, y % scalar, z % scalar }; }

		template<f32> coord_t operator%(T scalar) const noexcept {
			return { std::fmodf(x, scalar), std::fmodf(y, scalar), std::fmodf(z, scalar) };
		}

		template<f64> coord_t operator%(T scalar) const noexcept {
			return { std::fmodl(x, scalar), std::fmodl(y, scalar), std::fmodl(z, scalar) };
		}

		constexpr ref<coord_t> operator-() noexcept {
			x = -x;
			y = -y;
			z = -z;

			return *this;
		}

		constexpr ref<coord_t> operator+=(cref<coord_t> other) noexcept {
			x += other.x;
			y += other.y;
            z += other.z;

			return *this;
		}

		constexpr ref<coord_t> operator-=(cref<coord_t> other) noexcept {
			x -= other.x;
			y -= other.y;
            z -= other.z;

			return *this;
		}

		constexpr ref<coord_t> operator*=(cref<coord_t> other) noexcept {
			x *= other.x;
			y *= other.y;
            z *= other.z;

			return *this;
		}

		constexpr ref<coord_t> operator/=(cref<coord_t> other) noexcept {
			x /= other.x;
			y /= other.y;
            z /= other.z;

			return *this;
		}

		constexpr ref<coord_t> operator%=(cref<coord_t> other) noexcept {
			x %= other.x;
			y %= other.y;
            z %= other.z;

			return *this;
		}

		template<f32> ref<coord_t> operator%=(cref<coord_t> other) noexcept {
			x = std::fmodf(x, other.x);
			y = std::fmodf(y, other.y);
			y = std::fmodf(z, other.z);

			return *this;
		}

		template<f64> ref<coord_t> operator%=(cref<coord_t> other) noexcept {
			x = std::fmodl(x, other.x);
			y = std::fmodl(y, other.y);
			y = std::fmodl(z, other.z);

			return *this;
		}

		constexpr ref<coord_t> operator+=(T scalar) noexcept {
			x += scalar;
			y += scalar;
            z += scalar;

			return *this;
		}

		constexpr ref<coord_t> operator-=(T scalar) noexcept {
			x -= scalar;
			y -= scalar;
            z -= scalar;

			return *this;
		}

		constexpr ref<coord_t> operator*=(T scalar) noexcept {
			x *= scalar;
			y *= scalar;
            z *= scalar;

			return *this;
		}

		constexpr ref<coord_t> operator/=(T scalar) noexcept {
			x /= scalar;
			y /= scalar;
            z /= scalar;

			return *this;
		}

		constexpr ref<coord_t> operator%=(T scalar) noexcept {
			x %= scalar;
			y %= scalar;
            z %= scalar;

			return *this;
		}

		template<f32> ref<coord_t> operator%=(T scalar) noexcept {
			x = std::fmodf(x, scalar);
			y = std::fmodf(y, scalar);
			z = std::fmodf(z, scalar);

			return *this;
		}

		template<f64> ref<coord_t> operator%=(T scalar) noexcept {
			x = std::fmodl(x, scalar);
			y = std::fmodl(y, scalar);
			z = std::fmodl(z, scalar);

			return *this;
		}

		constexpr operator std::string() const noexcept { return std::format("[{}, {}, {}]", x, y, z); }

		template<i32> constexpr operator cardinal_t() const noexcept {
			if (x == coord_t<>::Zero.x && y == coord_t<>::Zero.x) {
				return cardinal_t::Central;
			} else {
				cardinal_t result = cardinal_t::Central;

				if (x != coord_t<>::Zero.x) {
					result |= x < coord_t<>::Zero.x ? cardinal_t::West : cardinal_t::East;
				}
				if (y != coord_t<>::Zero.y) {
					result |= y < coord_t<>::Zero.y ? cardinal_t::North : cardinal_t::South;
				}

				return result;
			}
		}
	};

	template<typename T = i32> using bounds_t = coord_t<T>;

	template<> constexpr const coord_t coord_t<i32>::Zero { 0 };

	template<> constexpr const coord_t coord_t<i32>::Horizontal::Central { 0, 0, 0 };

	template<> constexpr const coord_t coord_t<i32>::Horizontal::North = { 0, -1 };
	template<> constexpr const coord_t coord_t<i32>::Horizontal::South = { 0, 1 };

	template<> constexpr const coord_t coord_t<i32>::Horizontal::East = { 1, 0 };
	template<> constexpr const coord_t coord_t<i32>::Horizontal::West = { -1, 0 };

	template<> constexpr const coord_t coord_t<i32>::Horizontal::Northeast = { 1, -1 };
	template<> constexpr const coord_t coord_t<i32>::Horizontal::Northwest = { -1, -1 };

	template<> constexpr const coord_t coord_t<i32>::Horizontal::Southeast = { 1, 1 };
	template<> constexpr const coord_t coord_t<i32>::Horizontal::Southwest = { -1, 1 };

	static const coord_t Zero;

	struct Horizontal {
		static const coord_t North;
		static const coord_t South;
		static const coord_t East;
		static const coord_t West;

		static const coord_t Northeast;
		static const coord_t Northwest;

		static const coord_t Southeast;
		static const coord_t Southwest;

		const coord_t Central;
	};

	struct Vertical {
		struct Up {
			static const coord_t North;
			static const coord_t South;
			static const coord_t East;
			static const coord_t West;

			static const coord_t Northeast;
			static const coord_t Northwest;

			static const coord_t Southeast;
			static const coord_t Southwest;

			static const coord_t Central;
		};

		struct Down {
			static const coord_t North;
			static const coord_t South;
			static const coord_t East;
			static const coord_t West;

			static const coord_t Northeast;
			static const coord_t Northwest;

			static const coord_t Southeast;
			static const coord_t Southwest;

			static const coord_t Central;
		};
	};
} // namespace Bleakdepth
