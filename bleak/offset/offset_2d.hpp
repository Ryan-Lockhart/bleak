#pragma once

#include "bleak/primitive.hpp"

#include <cmath>
#include <format>
#include <string>

#include <SDL.h>

#include "bleak/cardinal.hpp"

extern "C" {
	typedef struct c_offset_2d_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::isize scalar_t;
		typedef bleak::iquad product_t;
		typedef bleak::fquad float_t;
#else
		typedef bleak::ihalf scalar_t;
		typedef bleak::isize product_t;
		typedef bleak::fsize float_t;
#endif

		scalar_t x { 0 };
		scalar_t y { 0 };

		static_assert(
			(product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max() <= std::numeric_limits<product_t>::max(),
			"product_t is too small for scalar_t"
		);
	} c_offset_2d_t;
}

namespace bleak {
	struct offset_2d_t : c_offset_2d_t {
		static const offset_2d_t zero;

		static const offset_2d_t central;

		static const offset_2d_t north;
		static const offset_2d_t south;

		static const offset_2d_t east;
		static const offset_2d_t west;

		static const offset_2d_t northwest;
		static const offset_2d_t northeast;

		static const offset_2d_t southwest;
		static const offset_2d_t southeast;

		constexpr offset_2d_t() noexcept {}

		constexpr offset_2d_t(scalar_t scalar) noexcept : c_offset_2d_t { scalar, scalar } {}

		constexpr offset_2d_t(scalar_t x, scalar_t y) noexcept : c_offset_2d_t { x, y } {}

		constexpr product_t dot() const noexcept { return (usize)x * x + (usize)y * y; }

		template<typename L, typename = f32> constexpr L length() const noexcept;

		template<> constexpr f32 length() const noexcept { return std::sqrtf(static_cast<f32>(dot())); }

		template<> constexpr f64 length() const noexcept { return std::sqrtl(static_cast<f64>(dot())); }

		template<> constexpr product_t length() const noexcept {
			static_assert(sizeof(product_t) == sizeof(float_t), "length of product_t and float_t must be equal");

			union {
				float_t f;
				product_t i;
			} v { .f = (float_t)dot() };

			constexpr auto sub_shift { bitdef::mantissa<float_t>() };
			constexpr auto add_shift { sub_shift + 6 };

			v.i -= product_t { 1 } << sub_shift;
			v.i >>= 1;
			v.i += product_t { 1 } << add_shift;

			return (product_t)((float_t)v.i);
		}

		template<typename T, typename = f32> static constexpr T distance(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept;

		template<> constexpr f32 distance(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept { return (end - start).length<f32>(); }

		template<> constexpr f64 distance(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept { return (end - start).length<f64>(); }

		template<> constexpr product_t distance(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept { return (end - start).length<product_t>(); }

		constexpr ref<offset_2d_t> normalize() noexcept {
			const auto len = length<product_t>();

			if (len != 0) {
				x = static_cast<scalar_t>(x / len);
				y = static_cast<scalar_t>(y / len);
			}

			return *this;
		}

		constexpr offset_2d_t normalized() const noexcept {
			const auto len = length<product_t>();

			if (len != 0) {
				return { static_cast<scalar_t>(x / len), static_cast<scalar_t>(y / len) };
			} else {
				return { *this };
			}
		}

		constexpr ref<offset_2d_t> clamp(offset_2d_t min, offset_2d_t max) {
			x = x < min.x ? min.x : x > max.x ? max.x : x;
			y = y < min.y ? min.y : y > max.y ? max.y : y;

			return *this;
		}

		static constexpr offset_2d_t clamp(offset_2d_t value, offset_2d_t min, offset_2d_t max) { return value.clamp(min, max); }

		constexpr bool operator==(cref<offset_2d_t> other) const noexcept { return x == other.x && y == other.y; }

		constexpr bool operator!=(cref<offset_2d_t> other) const noexcept { return x != other.x || y != other.y; }

		constexpr bool operator<(cref<offset_2d_t> other) const { return x < other.x && y < other.y; }

		constexpr bool operator>(cref<offset_2d_t> other) const { return x > other.x && y > other.y; }

		constexpr bool operator<=(cref<offset_2d_t> other) const { return x <= other.x && y <= other.y; }

		constexpr bool operator>=(cref<offset_2d_t> other) const { return x >= other.x && y >= other.y; }

		constexpr offset_2d_t operator-() const noexcept { return { -x, -y }; }

		constexpr offset_2d_t operator+(cref<offset_2d_t> other) const noexcept { return { x + other.x, y + other.y }; }

		constexpr offset_2d_t operator-(cref<offset_2d_t> other) const noexcept { return { x - other.x, y - other.y }; }

		constexpr offset_2d_t operator*(cref<offset_2d_t> other) const noexcept { return { x * other.x, y * other.y }; }

		constexpr offset_2d_t operator/(cref<offset_2d_t> other) const noexcept { return { x / other.x, y / other.y }; }

		constexpr offset_2d_t operator%(cref<offset_2d_t> other) const noexcept { return { x % other.x, y % other.y }; }

		template<f32> offset_2d_t operator%(cref<offset_2d_t> other) const noexcept {
			return { static_cast<scalar_t>(fmodf(x, other.x)), static_cast<scalar_t>(fmodf(y, other.y)) };
		}

		template<f64> offset_2d_t operator%(cref<offset_2d_t> other) const noexcept {
			return { static_cast<scalar_t>(fmodl(x, other.x)), static_cast<scalar_t>(fmodl(y, other.y)) };
		}

		constexpr offset_2d_t operator+(scalar_t scalar) const noexcept { return { x + scalar, y + scalar }; }

		constexpr offset_2d_t operator-(scalar_t scalar) const noexcept { return { x - scalar, y - scalar }; }

		constexpr offset_2d_t operator*(scalar_t scalar) const noexcept { return { x * scalar, y * scalar }; }

		constexpr offset_2d_t operator/(scalar_t scalar) const noexcept { return { x / scalar, y / scalar }; }

		constexpr offset_2d_t operator%(scalar_t scalar) const noexcept { return { x % scalar, y % scalar }; }

		template<f32> offset_2d_t operator%(scalar_t scalar) const noexcept {
			return { static_cast<ihalf>(fmodf(x, scalar)), static_cast<scalar_t>(fmodf(y, scalar)) };
		}

		template<f64> offset_2d_t operator%(scalar_t scalar) const noexcept {
			return { static_cast<scalar_t>(fmodl(x, scalar)), static_cast<scalar_t>(fmodl(y, scalar)) };
		}

		constexpr ref<offset_2d_t> operator-() noexcept {
			x = -x;
			y = -y;

			return *this;
		}

		constexpr ref<offset_2d_t> operator+=(cref<offset_2d_t> other) noexcept {
			x += other.x;
			y += other.y;

			return *this;
		}

		constexpr ref<offset_2d_t> operator-=(cref<offset_2d_t> other) noexcept {
			x -= other.x;
			y -= other.y;

			return *this;
		}

		constexpr ref<offset_2d_t> operator*=(cref<offset_2d_t> other) noexcept {
			x *= other.x;
			y *= other.y;

			return *this;
		}

		constexpr ref<offset_2d_t> operator/=(cref<offset_2d_t> other) noexcept {
			x /= other.x;
			y /= other.y;

			return *this;
		}

		constexpr ref<offset_2d_t> operator%=(cref<offset_2d_t> other) noexcept {
			x %= other.x;
			y %= other.y;

			return *this;
		}

		template<f32> ref<offset_2d_t> operator%=(cref<offset_2d_t> other) noexcept {
			x = static_cast<scalar_t>(fmodf(x, other.x));
			y = static_cast<scalar_t>(fmodf(y, other.y));

			return *this;
		}

		template<f64> ref<offset_2d_t> operator%=(cref<offset_2d_t> other) noexcept {
			x = static_cast<scalar_t>(fmodl(x, other.x));
			y = static_cast<scalar_t>(fmodl(y, other.y));

			return *this;
		}

		constexpr ref<offset_2d_t> operator+=(scalar_t scalar) noexcept {
			x += scalar;
			y += scalar;

			return *this;
		}

		constexpr ref<offset_2d_t> operator-=(scalar_t scalar) noexcept {
			x -= scalar;
			y -= scalar;

			return *this;
		}

		constexpr ref<offset_2d_t> operator*=(scalar_t scalar) noexcept {
			x *= scalar;
			y *= scalar;

			return *this;
		}

		constexpr ref<offset_2d_t> operator/=(scalar_t scalar) noexcept {
			x /= scalar;
			y /= scalar;

			return *this;
		}

		constexpr ref<offset_2d_t> operator%=(scalar_t scalar) noexcept {
			x %= scalar;
			y %= scalar;

			return *this;
		}

		template<f32> ref<offset_2d_t> operator%=(scalar_t scalar) noexcept {
			x = static_cast<scalar_t>(fmodf(x, scalar));
			y = static_cast<scalar_t>(fmodf(y, scalar));

			return *this;
		}

		template<f64> ref<offset_2d_t> operator%=(scalar_t scalar) noexcept {
			x = static_cast<scalar_t>(fmodl(x, scalar));
			y = static_cast<scalar_t>(fmodl(y, scalar));

			return *this;
		}

		constexpr operator std::string() const noexcept { return std::format("[{}, {}]", x, y); }

		constexpr operator cardinal_t() const noexcept {
			if (*this == offset_2d_t::zero) {
				return cardinal_t::Central;
			} else {
				cardinal_t result = cardinal_t::Central;

				if (x != 0) {
					result += x < 0 ? cardinal_t::West : cardinal_t::East;
				}
				if (y != 0) {
					result += y < 0 ? cardinal_t::North : cardinal_t::South;
				}

				return result;
			}
		}

		constexpr explicit operator sdl::point() const noexcept { return { static_cast<i32>(x), static_cast<i32>(y) }; }
		constexpr explicit operator sdl::fpoint() const noexcept { return { static_cast<f32>(x), static_cast<f32>(y) }; }

		constexpr explicit operator offset_1d_t() const noexcept;
		constexpr explicit operator offset_3d_t() const noexcept;

		constexpr explicit operator extent_1d_t() const noexcept;
		constexpr explicit operator extent_2d_t() const noexcept;
		constexpr explicit operator extent_3d_t() const noexcept;
	};
} // namespace bleak