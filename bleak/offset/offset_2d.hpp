#pragma once

#include "bleak/primitive.hpp"

#include <cmath>
#include <format>
#include <string>
#include <type_traits>

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

		scalar_t x{ 0 };
		scalar_t y{ 0 };

		static_assert(
			(product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max() <= std::numeric_limits<product_t>::max(),
			"product_t is too small for scalar_t"
		);
	} c_offset_2d_t;
}

namespace bleak {
	struct offset_2d_t : c_offset_2d_t {
		template<typename T>
			requires std::is_convertible<T, scalar_t>::value
		static constexpr scalar_t scalar_cast(T value) noexcept {
			return static_cast<scalar_t>(value);
		}

		template<typename T>
			requires std::is_convertible<T, product_t>::value
		static constexpr product_t product_cast(T value) noexcept {
			return static_cast<product_t>(value);
		}

		template<typename T>
			requires std::is_convertible<T, float_t>::value
		static constexpr float_t float_cast(T value) noexcept {
			return static_cast<float_t>(value);
		}

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

		constexpr offset_2d_t(scalar_t scalar) noexcept : c_offset_2d_t{ scalar, scalar } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && (std::is_same<T, scalar_t>::value == false)
		constexpr offset_2d_t(T scalar) noexcept : c_offset_2d_t{ scalar_cast(scalar), scalar_cast(scalar) } {}

		constexpr offset_2d_t(scalar_t x, scalar_t y) noexcept : c_offset_2d_t{ x, y } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && (std::is_same<T, scalar_t>::value == false)
		constexpr offset_2d_t(T x, T y) noexcept : c_offset_2d_t{ scalar_cast(x), scalar_cast(y) } {}

		constexpr product_t dot() const noexcept { return (product_t)x * x + (product_t)y * y; }

		template<typename T = product_t>
			requires std::is_floating_point<T>::value || std::is_same<T, product_t>::value
		constexpr T length() const noexcept;

		template<> constexpr f32 length() const noexcept { return std::sqrt(static_cast<f32>(dot())); }

		template<> constexpr f64 length() const noexcept { return std::sqrt(static_cast<f64>(dot())); }

		template<> constexpr product_t length() const noexcept {
			static_assert(sizeof(product_t) == sizeof(float_t), "length of product_t and float_t must be equal");
			static_assert(bitdef::mantissa<float_t>() >= 6, "shift may not exceed bit length of mantissa");

			union {
				float_t f;
				product_t i;
			} v{ .f = (float_t)dot() };

			constexpr auto sub_shift{ bitdef::mantissa<float_t>() };
			constexpr auto add_shift{ sub_shift + 6 };

			v.i -= product_t{ 1 } << sub_shift;
			v.i >>= 1;
			v.i += product_t{ 1 } << add_shift;

			return (product_t)((float_t)v.i);
		}

		template<typename T = product_t> static constexpr T distance(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept;

		template<> constexpr f32 distance(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept { return (end - start).length<f32>(); }

		template<> constexpr f64 distance(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept { return (end - start).length<f64>(); }

		template<> constexpr product_t distance(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept { return (end - start).length<product_t>(); }

		template<typename T = product_t> static constexpr offset_2d_t direction(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept {
			return (end - start).normalized<T>();
		}

		template<typename T = product_t> constexpr ref<offset_2d_t> normalize() noexcept {
			const auto len = length<T>();

			if (len != 0) {
				x = scalar_cast(x / len);
				y = scalar_cast(y / len);
			}

			return *this;
		}

		template<typename T = product_t> constexpr offset_2d_t normalized() const noexcept {
			const auto len = length<T>();

			if (len != 0) {
				return { scalar_cast(x / len), scalar_cast(y / len) };
			} else {
				return { *this };
			}
		}

		constexpr ref<offset_2d_t> clamp(cref<offset_2d_t> min, cref<offset_2d_t> max) {
			x = x < min.x ? min.x : x > max.x ? max.x : x;
			y = y < min.y ? min.y : y > max.y ? max.y : y;

			return *this;
		}

		static constexpr offset_2d_t clamp(offset_2d_t value, cref<offset_2d_t> min, cref<offset_2d_t> max) { return value.clamp(min, max); }

		constexpr ref<offset_2d_t> clamp(cref<extent_2d_t> min, cref<extent_2d_t> max);

		static constexpr offset_2d_t clamp(offset_2d_t value, cref<extent_2d_t> min, cref<extent_2d_t> max);

		constexpr bool operator==(cref<offset_2d_t> other) const noexcept { return x == other.x && y == other.y; }

		constexpr bool operator!=(cref<offset_2d_t> other) const noexcept { return x != other.x || y != other.y; }

		constexpr bool operator<(cref<offset_2d_t> other) const { return x < other.x && y < other.y; }

		constexpr bool operator>(cref<offset_2d_t> other) const { return x > other.x && y > other.y; }

		constexpr bool operator<=(cref<offset_2d_t> other) const { return x <= other.x && y <= other.y; }

		constexpr bool operator>=(cref<offset_2d_t> other) const { return x >= other.x && y >= other.y; }

		constexpr offset_2d_t operator-() const noexcept { return { scalar_cast(-x), scalar_cast(-y) }; }

		constexpr offset_2d_t operator+(cref<offset_2d_t> other) const noexcept { return { scalar_cast(x + other.x), scalar_cast(y + other.y) }; }

		constexpr offset_2d_t operator-(cref<offset_2d_t> other) const noexcept { return { scalar_cast(x - other.x), scalar_cast(y - other.y) }; }

		constexpr offset_2d_t operator*(cref<offset_2d_t> other) const noexcept { return { scalar_cast(x * other.x), scalar_cast(y * other.y) }; }

		constexpr offset_2d_t operator/(cref<offset_2d_t> other) const noexcept { return { scalar_cast(x / other.x), scalar_cast(y / other.y) }; }

		constexpr offset_2d_t operator%(cref<offset_2d_t> other) const noexcept { return { scalar_cast(x % other.x), scalar_cast(y % other.y) }; }

		constexpr offset_2d_t operator+(scalar_t scalar) const noexcept { return { scalar_cast(x + scalar), scalar_cast(y + scalar) }; }

		constexpr offset_2d_t operator-(scalar_t scalar) const noexcept { return { scalar_cast(x - scalar), scalar_cast(y - scalar) }; }

		constexpr offset_2d_t operator*(scalar_t scalar) const noexcept { return { scalar_cast(x * scalar), scalar_cast(y * scalar) }; }

		constexpr offset_2d_t operator/(scalar_t scalar) const noexcept { return { scalar_cast(x / scalar), scalar_cast(y / scalar) }; }

		constexpr offset_2d_t operator%(scalar_t scalar) const noexcept { return { scalar_cast(x % scalar), scalar_cast(y % scalar) }; }

		constexpr offset_2d_t operator+(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_2d_t operator-(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_2d_t operator*(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_2d_t operator/(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_2d_t operator%(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_2d_t operator+(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_2d_t operator-(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_2d_t operator*(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_2d_t operator/(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_2d_t operator%(cref<extent_2d_t> extent) const noexcept;

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

		constexpr ref<offset_2d_t> operator+=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator-=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator*=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator/=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator%=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator+=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator-=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator*=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator/=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_2d_t> operator%=(cref<extent_2d_t> extent) noexcept;

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
