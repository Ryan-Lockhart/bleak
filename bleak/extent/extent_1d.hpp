#pragma once

#include "bleak/primitive.hpp"

#include <cmath>
#include <format>
#include <string>

#include <SDL.h>

extern "C" {
	typedef struct c_extent_1d_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::usize scalar_t;
		typedef bleak::usize product_t;
#else
		typedef bleak::uhalf scalar_t;
		typedef bleak::uhalf product_t;
#endif
		scalar_t w{ 0 };
	} c_extent_1d_t;
}

namespace bleak {
	struct extent_1d_t : public c_extent_1d_t {
		template<typename T> static constexpr scalar_t scalar_cast(T value) noexcept { return static_cast<scalar_t>(value); }

		template<typename T> static constexpr product_t product_cast(T value) noexcept { return static_cast<product_t>(value); }

		static const extent_1d_t zero;

		constexpr extent_1d_t() noexcept {}

		constexpr extent_1d_t(scalar_t width) noexcept : c_extent_1d_t{ width } {}

		constexpr bool operator==(cref<extent_1d_t> other) const noexcept { return w == other.w; }

		constexpr bool operator!=(cref<extent_1d_t> other) const noexcept { return w != other.w; }

		constexpr bool operator==(scalar_t scalar) const noexcept { return w == scalar; }

		constexpr bool operator!=(scalar_t scalar) const noexcept { return w != scalar; }

		constexpr bool operator<(cref<extent_1d_t> other) const { return w < other.w; }

		constexpr bool operator>(cref<extent_1d_t> other) const { return w > other.w; }

		constexpr bool operator<=(cref<extent_1d_t> other) const { return w <= other.w; }

		constexpr bool operator>=(cref<extent_1d_t> other) const { return w >= other.w; }

		constexpr extent_1d_t operator+(cref<extent_1d_t> other) const noexcept { return { w + other.w }; }

		constexpr extent_1d_t operator-(cref<extent_1d_t> other) const noexcept { return { w - other.w }; }

		constexpr extent_1d_t operator*(cref<extent_1d_t> other) const noexcept { return { w * other.w }; }

		constexpr extent_1d_t operator/(cref<extent_1d_t> other) const noexcept { return { w / other.w }; }

		constexpr extent_1d_t operator%(cref<extent_1d_t> other) const noexcept { return { w % other.w }; }

		constexpr extent_1d_t operator+(scalar_t scalar) const noexcept { return { w + scalar }; }

		constexpr extent_1d_t operator-(scalar_t scalar) const noexcept { return { w - scalar }; }

		constexpr extent_1d_t operator*(scalar_t scalar) const noexcept { return { w * scalar }; }

		constexpr extent_1d_t operator/(scalar_t scalar) const noexcept { return { w / scalar }; }

		constexpr extent_1d_t operator%(scalar_t scalar) const noexcept { return { w % scalar }; }

		constexpr ref<extent_1d_t> operator+=(cref<extent_1d_t> other) noexcept {
			w += other.w;

			return *this;
		}

		constexpr ref<extent_1d_t> operator-=(cref<extent_1d_t> other) noexcept {
			w -= other.w;

			return *this;
		}

		constexpr ref<extent_1d_t> operator*=(cref<extent_1d_t> other) noexcept {
			w *= other.w;

			return *this;
		}

		constexpr ref<extent_1d_t> operator/=(cref<extent_1d_t> other) noexcept {
			w /= other.w;

			return *this;
		}

		constexpr ref<extent_1d_t> operator%=(cref<extent_1d_t> other) noexcept {
			w %= other.w;

			return *this;
		}

		constexpr ref<extent_1d_t> operator+=(scalar_t scalar) noexcept {
			w += scalar;

			return *this;
		}

		constexpr ref<extent_1d_t> operator-=(scalar_t scalar) noexcept {
			w -= scalar;

			return *this;
		}

		constexpr ref<extent_1d_t> operator*=(scalar_t scalar) noexcept {
			w *= scalar;

			return *this;
		}

		constexpr ref<extent_1d_t> operator/=(scalar_t scalar) noexcept {
			w /= scalar;

			return *this;
		}

		constexpr ref<extent_1d_t> operator%=(scalar_t scalar) noexcept {
			w %= scalar;

			return *this;
		}

		constexpr product_t size() const noexcept { return w; }

		constexpr operator std::string() const noexcept { return std::format("[{}]", w); }

		constexpr explicit operator sdl::point() const noexcept { return { static_cast<i32>(w), 0 }; }

		constexpr explicit operator sdl::fpoint() const noexcept { return { static_cast<f32>(w), 0.0f }; }

		constexpr operator scalar_t() const noexcept { return w; }

		constexpr explicit operator extent_2d_t() const noexcept;
		constexpr explicit operator extent_3d_t() const noexcept;

		constexpr explicit operator offset_1d_t() const noexcept;
		constexpr explicit operator offset_2d_t() const noexcept;
		constexpr explicit operator offset_3d_t() const noexcept;
	};
} // namespace bleak
