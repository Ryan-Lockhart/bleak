#pragma once

#include "bleak/primitive.hpp"

#include <cmath>
#include <format>
#include <string>

#include <SDL.h>

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
		scalar_t x { 0 };
	} c_offset_1d_t;
}

namespace bleak {
	struct offset_1d_t : public c_offset_1d_t {
		template<typename T> static constexpr scalar_t scalar_cast(T value) noexcept { return static_cast<scalar_t>(value); }

		template<typename T> static constexpr product_t product_cast(T value) noexcept { return static_cast<product_t>(value); }

		template<typename T> static constexpr float_t float_cast(T value) noexcept { return static_cast<float_t>(value); }

		static const offset_1d_t zero;

		static const offset_1d_t central;

		static const offset_1d_t east;
		static const offset_1d_t west;

		constexpr offset_1d_t() noexcept {}

		constexpr offset_1d_t(scalar_t scalar) noexcept : c_offset_1d_t { scalar } {}

		constexpr bool operator==(cref<offset_1d_t> other) const noexcept { return x == other.x; }

		constexpr bool operator!=(cref<offset_1d_t> other) const noexcept { return x != other.x; }

		constexpr bool operator==(scalar_t scalar) const noexcept { return x == scalar; }

		constexpr bool operator!=(scalar_t scalar) const noexcept { return x != scalar; }

		constexpr bool operator<(cref<offset_1d_t> other) const { return x < other.x; }

		constexpr bool operator>(cref<offset_1d_t> other) const { return x > other.x; }

		constexpr bool operator<=(cref<offset_1d_t> other) const { return x <= other.x; }

		constexpr bool operator>=(cref<offset_1d_t> other) const { return x >= other.x; }

		constexpr offset_1d_t operator+(cref<offset_1d_t> other) const noexcept { return { x + other.x }; }

		constexpr offset_1d_t operator-(cref<offset_1d_t> other) const noexcept { return { x - other.x }; }

		constexpr offset_1d_t operator*(cref<offset_1d_t> other) const noexcept { return { x * other.x }; }

		constexpr offset_1d_t operator/(cref<offset_1d_t> other) const noexcept { return { x / other.x }; }

		constexpr offset_1d_t operator%(cref<offset_1d_t> other) const noexcept { return { x % other.x }; }

		constexpr offset_1d_t operator+(scalar_t scalar) const noexcept { return { x + scalar }; }

		constexpr offset_1d_t operator-(scalar_t scalar) const noexcept { return { x - scalar }; }

		constexpr offset_1d_t operator*(scalar_t scalar) const noexcept { return { x * scalar }; }

		constexpr offset_1d_t operator/(scalar_t scalar) const noexcept { return { x / scalar }; }

		constexpr offset_1d_t operator%(scalar_t scalar) const noexcept { return { x % scalar }; }

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
