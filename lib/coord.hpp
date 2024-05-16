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
	  public:
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

		static const coord_t Central;

		static const coord_t North;
		static const coord_t South;

		static const coord_t East;
		static const coord_t West;

		static const coord_t Up;
		static const coord_t Down;

		static const coord_t Northwest;
		static const coord_t Northeast;

		static const coord_t Southwest;
		static const coord_t Southeast;

		struct Vertical {
			struct Up {
				static const coord_t Central;

				static const coord_t North;
				static const coord_t South;

				static const coord_t East;
				static const coord_t West;

				static const coord_t Northwest;
				static const coord_t Northeast;

				static const coord_t Southwest;
				static const coord_t Southeast;
			};

			struct Down {
				static const coord_t Central;

				static const coord_t North;
				static const coord_t South;

				static const coord_t East;
				static const coord_t West;

				static const coord_t Northwest;
				static const coord_t Northeast;

				static const coord_t Southwest;
				static const coord_t Southeast;
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
			return { ::fmodf(x, other.x), ::fmodf(y, other.y), ::fmodf(z, other.z) };
		}

		template<f64> coord_t operator%(cref<coord_t> other) const noexcept {
			return { ::fmodl(x, other.x), ::fmodl(y, other.y), ::fmodl(z, other.z) };
		}

		constexpr coord_t operator+(T scalar) const noexcept { return { x + scalar, y + scalar, z + scalar }; }

		constexpr coord_t operator-(T scalar) const noexcept { return { x - scalar, y - scalar, z - scalar }; }

		constexpr coord_t operator*(T scalar) const noexcept { return { x * scalar, y * scalar, z * scalar }; }

		constexpr coord_t operator/(T scalar) const noexcept { return { x / scalar, y / scalar, z / scalar }; }

		constexpr coord_t operator%(T scalar) const noexcept { return { x % scalar, y % scalar, z % scalar }; }

		template<f32> coord_t operator%(T scalar) const noexcept {
			return { ::fmodf(x, scalar), ::fmodf(y, scalar), ::fmodf(z, scalar) };
		}

		template<f64> coord_t operator%(T scalar) const noexcept {
			return { ::fmodl(x, scalar), ::fmodl(y, scalar), ::fmodl(z, scalar) };
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
			x = ::fmodf(x, other.x);
			y = ::fmodf(y, other.y);
			y = ::fmodf(z, other.z);

			return *this;
		}

		template<f64> ref<coord_t> operator%=(cref<coord_t> other) noexcept {
			x = ::fmodl(x, other.x);
			y = ::fmodl(y, other.y);
			y = ::fmodl(z, other.z);

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
			x = ::fmodf(x, scalar);
			y = ::fmodf(y, scalar);
			z = ::fmodf(z, scalar);

			return *this;
		}

		template<f64> ref<coord_t> operator%=(T scalar) noexcept {
			x = ::fmodl(x, scalar);
			y = ::fmodl(y, scalar);
			z = ::fmodl(z, scalar);

			return *this;
		}

		constexpr operator std::string() const noexcept { return std::format("[{}, {}, {}]", x, y, z); }

		template<i32> constexpr operator cardinal_t() const noexcept {
			if (x == coord_t<>::Zero.x && y == coord_t<>::Zero.x) {
				return coord_t::Central;
			} else {
				coord_t result = coord_t::Central;

				if (x != coord_t<>::Zero.x) {
					result |= x < coord_t<>::Zero.x ? coord_t::West : coord_t::East;
				}
				if (y != coord_t<>::Zero.y) {
					result |= y < coord_t<>::Zero.y ? coord_t::North : coord_t::South;
				}
				if (z != coord_t<>::Zero.z) {
					result |= z < coord_t<>::Zero.z ? coord_t::Down : coord_t::Up;
				}

				return result;
			}
		}
	};

	template<typename T = i32> using bounds_t = coord_t<T>;

	template<> constexpr const coord_t coord_t<i32>::Zero { 0 };

	template<> constexpr const coord_t coord_t<i32>::Central { coord_t<i32>::Zero };

	template<> constexpr const coord_t coord_t<i32>::North = { 0, -1, 0 };
	template<> constexpr const coord_t coord_t<i32>::South = { 0, 1, 0 };

	template<> constexpr const coord_t coord_t<i32>::East = { 1, 0, 0 };
	template<> constexpr const coord_t coord_t<i32>::West = { -1, 0, 0 };

	template<> constexpr const coord_t coord_t<i32>::Up { 0, 0, 1 };
	template<> constexpr const coord_t coord_t<i32>::Down { 0, 0, -1 };

	template<> constexpr const coord_t coord_t<i32>::Northwest { coord_t::North + coord_t::West };
	template<> constexpr const coord_t coord_t<i32>::Northeast { coord_t::North + coord_t::East };

	template<> constexpr const coord_t coord_t<i32>::Southwest { coord_t::South + coord_t::West };
	template<> constexpr const coord_t coord_t<i32>::Southeast { coord_t::South + coord_t::East };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::Central { coord_t::Central + coord_t::Up };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::North { coord_t::North + coord_t::Up };
	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::South { coord_t::South + coord_t::Up };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::West { coord_t::West + coord_t::Up };
	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::East { coord_t::East + coord_t::Up };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::Northwest { coord_t::Northwest + coord_t::Up };
	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::Northeast { coord_t::Northeast + coord_t::Up };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::Southwest { coord_t::Southwest + coord_t::Up };
	template<> constexpr const coord_t coord_t<i32>::Vertical::Up::Southeast { coord_t::Southeast + coord_t::Up };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::Central { coord_t::Central + coord_t::Down };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::North { coord_t::North + coord_t::Down };
	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::South { coord_t::South + coord_t::Down };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::West { coord_t::West + coord_t::Down };
	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::East { coord_t::East + coord_t::Down };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::Northwest { coord_t::Northwest + coord_t::Down };
	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::Northeast { coord_t::Northeast + coord_t::Down };

	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::Southwest { coord_t::Southwest + coord_t::Down };
	template<> constexpr const coord_t coord_t<i32>::Vertical::Down::Southeast { coord_t::Southeast + coord_t::Down };
} // namespace Bleakdepth
