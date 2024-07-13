#pragma once

#include "bleak/primitive.hpp"

#include <cmath>
#include <format>
#include <string>
#include <type_traits>

#include <SDL.h>

#include "bleak/cardinal.hpp"

extern "C" {
	typedef struct c_offset_1d_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::iquad scalar_t;
		typedef bleak::iquad product_t;
		typedef bleak::fquad float_t;
#else
		typedef bleak::isize scalar_t;
		typedef bleak::isize product_t;
		typedef bleak::fsize float_t;
#endif
		scalar_t x{ 0 };
	} c_offset_1d_t;
}

namespace bleak {
	struct offset_1d_t : public c_offset_1d_t {
		using underlying_t = c_offset_1d_t;

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

		static const offset_1d_t zero;

		static const offset_1d_t central;

		static const offset_1d_t east;
		static const offset_1d_t west;

		constexpr explicit offset_1d_t() noexcept {}

		constexpr explicit offset_1d_t(scalar_t scalar) noexcept : c_offset_1d_t{ scalar } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && (std::is_same<T, scalar_t>::value == false)
		constexpr explicit offset_1d_t(T scalar) noexcept : c_offset_1d_t{ scalar_cast(scalar) } {}

		constexpr product_t dot() const noexcept { return product_cast(x) * x; }

		template<typename T = product_t>
			requires std::is_floating_point<T>::value || std::is_same<T, product_t>::value
		constexpr T length() const noexcept;

		template<> constexpr f32 length() const noexcept { return static_cast<f32>(x); }

		template<> constexpr f64 length() const noexcept { return static_cast<f64>(x); }

		template<> constexpr product_t length() const noexcept { return product_cast(x); }

		constexpr ref<offset_1d_t> clamp(cref<offset_1d_t> min, cref<offset_1d_t> max) {
			x = x < min.x ? min.x : x > max.x ? max.x : x;

			return *this;
		}

		static constexpr offset_1d_t clamp(offset_1d_t value, cref<offset_1d_t> min, cref<offset_1d_t> max) { return value.clamp(min, max); }

		constexpr ref<offset_1d_t> clamp(cref<extent_1d_t> min, cref<extent_1d_t> max);

		static constexpr offset_1d_t clamp(offset_1d_t value, cref<extent_1d_t> min, cref<extent_1d_t> max);

		constexpr bool operator==(cref<offset_1d_t> other) const noexcept { return x == other.x; }

		constexpr bool operator!=(cref<offset_1d_t> other) const noexcept { return x != other.x; }

		constexpr bool operator==(scalar_t scalar) const noexcept { return x == scalar; }

		constexpr bool operator!=(scalar_t scalar) const noexcept { return x != scalar; }

		constexpr bool operator<(cref<offset_1d_t> other) const { return x < other.x; }

		constexpr bool operator>(cref<offset_1d_t> other) const { return x > other.x; }

		constexpr bool operator<=(cref<offset_1d_t> other) const { return x <= other.x; }

		constexpr bool operator>=(cref<offset_1d_t> other) const { return x >= other.x; }

		constexpr offset_1d_t operator-() const noexcept { return offset_1d_t{ scalar_cast(-x) }; }

		constexpr offset_1d_t operator+(cref<offset_1d_t> other) const noexcept { return offset_1d_t{ x + other.x }; }

		constexpr offset_1d_t operator-(cref<offset_1d_t> other) const noexcept { return offset_1d_t{ x - other.x }; }

		constexpr offset_1d_t operator*(cref<offset_1d_t> other) const noexcept { return offset_1d_t{ x * other.x }; }

		constexpr offset_1d_t operator/(cref<offset_1d_t> other) const noexcept { return offset_1d_t{ x / other.x }; }

		constexpr offset_1d_t operator%(cref<offset_1d_t> other) const noexcept { return offset_1d_t{ x % other.x }; }

		constexpr offset_1d_t operator+(scalar_t scalar) const noexcept { return offset_1d_t{ x + scalar }; }

		constexpr offset_1d_t operator-(scalar_t scalar) const noexcept { return offset_1d_t{ x - scalar }; }

		constexpr offset_1d_t operator*(scalar_t scalar) const noexcept { return offset_1d_t{ x * scalar }; }

		constexpr offset_1d_t operator/(scalar_t scalar) const noexcept { return offset_1d_t{ x / scalar }; }

		constexpr offset_1d_t operator%(scalar_t scalar) const noexcept { return offset_1d_t{ x % scalar }; }

		constexpr offset_1d_t operator+(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_1d_t operator-(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_1d_t operator*(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_1d_t operator/(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_1d_t operator%(cref<extent_1d_t> extent) const noexcept;

		constexpr ref<offset_1d_t> operator+=(cref<offset_1d_t> other) noexcept {
			x += other.x;

			return *this;
		}

		constexpr ref<offset_1d_t> operator-=(cref<offset_1d_t> other) noexcept {
			x -= other.x;

			return *this;
		}

		constexpr ref<offset_1d_t> operator*=(cref<offset_1d_t> other) noexcept {
			x *= other.x;

			return *this;
		}

		constexpr ref<offset_1d_t> operator/=(cref<offset_1d_t> other) noexcept {
			x /= other.x;

			return *this;
		}

		constexpr ref<offset_1d_t> operator%=(cref<offset_1d_t> other) noexcept {
			x %= other.x;

			return *this;
		}

		constexpr ref<offset_1d_t> operator+=(scalar_t scalar) noexcept {
			x += scalar;

			return *this;
		}

		constexpr ref<offset_1d_t> operator-=(scalar_t scalar) noexcept {
			x -= scalar;

			return *this;
		}

		constexpr ref<offset_1d_t> operator*=(scalar_t scalar) noexcept {
			x *= scalar;

			return *this;
		}

		constexpr ref<offset_1d_t> operator/=(scalar_t scalar) noexcept {
			x /= scalar;

			return *this;
		}

		constexpr ref<offset_1d_t> operator%=(scalar_t scalar) noexcept {
			x %= scalar;

			return *this;
		}

		constexpr ref<offset_1d_t> operator+=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_1d_t> operator-=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_1d_t> operator*=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_1d_t> operator/=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_1d_t> operator%=(cref<extent_1d_t> extent) noexcept;

		constexpr operator std::string() const noexcept { return std::format("[{}]", x); }

		constexpr operator cardinal_t() const noexcept { return x == 0 ? cardinal_t::Central : x < 0 ? cardinal_t::West : cardinal_t::East; }

		constexpr explicit operator sdl::point() const noexcept { return { static_cast<i32>(x), 0 }; }

		constexpr explicit operator sdl::fpoint() const noexcept { return { static_cast<f32>(x), 0.0f }; }

		constexpr operator scalar_t() const noexcept { return x; }

		constexpr explicit operator extent_1d_t() const noexcept;
		constexpr explicit operator extent_2d_t() const noexcept;
		constexpr explicit operator extent_3d_t() const noexcept;

		constexpr explicit operator offset_2d_t() const noexcept;
		constexpr explicit operator offset_3d_t() const noexcept;
	};
} // namespace bleak
