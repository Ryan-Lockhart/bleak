#pragma once

#include "bleak/primitive.hpp"

#include <cmath>
#include <format>
#include <limits>
#include <string>

#include <SDL.h>

extern "C" {
	typedef struct c_extent_2d_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::usize scalar_t;
		typedef bleak::uquad product_t;
#else
		typedef bleak::uhalf scalar_t;
		typedef bleak::usize product_t;
#endif

		scalar_t w { 0 };
		scalar_t h { 0 };

		static_assert(
			(product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max() <= std::numeric_limits<product_t>::max(),
			"product_t is too small for scalar_t"
		);
	} c_extent_2d_t;
}

namespace bleak {
	struct extent_2d_t : public c_extent_2d_t {
		static const extent_2d_t zero;

		constexpr extent_2d_t() noexcept {}

		constexpr extent_2d_t(scalar_t scalar) noexcept : c_extent_2d_t { scalar, scalar } {}

		constexpr extent_2d_t(scalar_t width, scalar_t height) noexcept : c_extent_2d_t { width, height } {}

		constexpr product_t area() const noexcept { return (product_t)w * h; }

		constexpr bool operator==(cref<extent_2d_t> other) const noexcept { return w == other.w && h == other.h; }

		constexpr bool operator!=(cref<extent_2d_t> other) const noexcept { return w != other.w || h != other.h; }

		constexpr bool operator<(cref<extent_2d_t> other) const { return area() < other.area(); }

		constexpr bool operator>(cref<extent_2d_t> other) const { return area() > other.area(); }

		constexpr bool operator<=(cref<extent_2d_t> other) const { return area() <= other.area(); }

		constexpr bool operator>=(cref<extent_2d_t> other) const { return area() >= other.area(); }

		constexpr extent_2d_t operator+(cref<extent_2d_t> other) const noexcept { return { w + other.w, h + other.h }; }

		constexpr extent_2d_t operator-(cref<extent_2d_t> other) const noexcept { return { w - other.w, h - other.h }; }

		constexpr extent_2d_t operator*(cref<extent_2d_t> other) const noexcept { return { w * other.w, h * other.h }; }

		constexpr extent_2d_t operator/(cref<extent_2d_t> other) const noexcept { return { w / other.w, h / other.h }; }

		constexpr extent_2d_t operator%(cref<extent_2d_t> other) const noexcept { return { w % other.w, h % other.h }; }

		constexpr extent_2d_t operator+(scalar_t scalar) const noexcept { return { w + scalar, h + scalar }; }

		constexpr extent_2d_t operator-(scalar_t scalar) const noexcept { return { w - scalar, h - scalar }; }

		constexpr extent_2d_t operator*(scalar_t scalar) const noexcept { return { w * scalar, h * scalar }; }

		constexpr extent_2d_t operator/(scalar_t scalar) const noexcept { return { w / scalar, h / scalar }; }

		constexpr extent_2d_t operator%(scalar_t scalar) const noexcept { return { w % scalar, h % scalar }; }

		constexpr ref<extent_2d_t> operator+=(cref<extent_2d_t> other) noexcept {
			w += other.w;
			h += other.h;

			return *this;
		}

		constexpr ref<extent_2d_t> operator-=(cref<extent_2d_t> other) noexcept {
			w -= other.w;
			h -= other.h;

			return *this;
		}

		constexpr ref<extent_2d_t> operator*=(cref<extent_2d_t> other) noexcept {
			w *= other.w;
			h *= other.h;

			return *this;
		}

		constexpr ref<extent_2d_t> operator/=(cref<extent_2d_t> other) noexcept {
			w /= other.w;
			h /= other.h;

			return *this;
		}

		constexpr ref<extent_2d_t> operator%=(cref<extent_2d_t> other) noexcept {
			w %= other.w;
			h %= other.h;

			return *this;
		}

		constexpr ref<extent_2d_t> operator+=(scalar_t scalar) noexcept {
			w += scalar;
			h += scalar;

			return *this;
		}

		constexpr ref<extent_2d_t> operator-=(scalar_t scalar) noexcept {
			w -= scalar;
			h -= scalar;

			return *this;
		}

		constexpr ref<extent_2d_t> operator*=(scalar_t scalar) noexcept {
			w *= scalar;
			h *= scalar;

			return *this;
		}

		constexpr ref<extent_2d_t> operator/=(scalar_t scalar) noexcept {
			w /= scalar;
			h /= scalar;

			return *this;
		}

		constexpr ref<extent_2d_t> operator%=(scalar_t scalar) noexcept {
			w %= scalar;
			h %= scalar;

			return *this;
		}

		constexpr operator std::string() const noexcept { return std::format("[{} x {}]", w, h); }

		constexpr explicit operator sdl::point() const noexcept { return { static_cast<i32>(w), static_cast<i32>(h) }; }
		constexpr explicit operator sdl::fpoint() const noexcept { return { static_cast<f32>(w), static_cast<f32>(h) }; }

		constexpr explicit operator extent_1d_t() const noexcept;
		constexpr explicit operator extent_3d_t() const noexcept;

		constexpr explicit operator offset_1d_t() const noexcept;
		constexpr explicit operator offset_2d_t() const noexcept;
		constexpr explicit operator offset_3d_t() const noexcept;
	};
} // namespace bleak
