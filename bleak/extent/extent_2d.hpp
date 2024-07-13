#pragma once

#include "bleak/primitive.hpp"

#include <cmath>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

#include <SDL.h>

extern "C" {
	typedef struct c_extent_2d_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::uhalf scalar_t;
		typedef bleak::usize product_t;
#else
		typedef bleak::uqrtr scalar_t;
		typedef bleak::uhalf product_t;
#endif

		scalar_t w{ 0 };
		scalar_t h{ 0 };

		static_assert((product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max() <= std::numeric_limits<product_t>::max(), "product_t is too small for scalar_t");
	} c_extent_2d_t;
}

namespace bleak {
	struct extent_2d_t : public c_extent_2d_t {
		using underlying_t = c_extent_2d_t;

		template<typename T> static constexpr scalar_t scalar_cast(T value) noexcept { return static_cast<scalar_t>(value); }

		template<typename T> static constexpr product_t product_cast(T value) noexcept { return static_cast<product_t>(value); }

		static const extent_2d_t zero;

		constexpr explicit extent_2d_t() noexcept {}

		constexpr explicit extent_2d_t(scalar_t scalar) noexcept : c_extent_2d_t{ scalar, scalar } {}

		constexpr explicit extent_2d_t(scalar_t width, scalar_t height) noexcept : c_extent_2d_t{ width, height } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value
		constexpr explicit extent_2d_t(T width, T height) noexcept : underlying_t{ scalar_cast(width), scalar_cast(height) } {}

		template<typename W, typename H>
			requires std::is_convertible<W, scalar_t>::value && std::is_convertible<H, scalar_t>::value
		constexpr explicit extent_2d_t(W width, H height) noexcept : underlying_t{ scalar_cast(width), scalar_cast(height) } {}

		constexpr product_t area() const noexcept { return (product_t)w * h; }

		constexpr bool operator==(cref<extent_2d_t> other) const noexcept { return w == other.w && h == other.h; }

		constexpr bool operator!=(cref<extent_2d_t> other) const noexcept { return w != other.w || h != other.h; }

		constexpr bool operator<(cref<extent_2d_t> other) const { return area() < other.area(); }

		constexpr bool operator>(cref<extent_2d_t> other) const { return area() > other.area(); }

		constexpr bool operator<=(cref<extent_2d_t> other) const { return area() <= other.area(); }

		constexpr bool operator>=(cref<extent_2d_t> other) const { return area() >= other.area(); }

		constexpr offset_2d_t operator-() const noexcept;

		constexpr extent_2d_t operator+(cref<extent_2d_t> other) const noexcept { return extent_2d_t{ scalar_cast(w + other.w), scalar_cast(h + other.h) }; }

		constexpr extent_2d_t operator-(cref<extent_2d_t> other) const noexcept { return extent_2d_t{ scalar_cast(w - other.w), scalar_cast(h - other.h) }; }

		constexpr extent_2d_t operator*(cref<extent_2d_t> other) const noexcept { return extent_2d_t{ scalar_cast(w * other.w), scalar_cast(h * other.h) }; }

		constexpr extent_2d_t operator/(cref<extent_2d_t> other) const noexcept { return extent_2d_t{ scalar_cast(w / other.w), scalar_cast(h / other.h) }; }

		constexpr extent_2d_t operator%(cref<extent_2d_t> other) const noexcept { return extent_2d_t{ scalar_cast(w % other.w), scalar_cast(h % other.h) }; }

		constexpr extent_2d_t operator+(scalar_t scalar) const noexcept { return extent_2d_t{ scalar_cast(w + scalar), scalar_cast(h + scalar) }; }

		constexpr extent_2d_t operator-(scalar_t scalar) const noexcept { return extent_2d_t{ scalar_cast(w - scalar), scalar_cast(h - scalar) }; }

		constexpr extent_2d_t operator*(scalar_t scalar) const noexcept { return extent_2d_t{ scalar_cast(w * scalar), scalar_cast(h * scalar) }; }

		constexpr extent_2d_t operator/(scalar_t scalar) const noexcept { return extent_2d_t{ scalar_cast(w / scalar), scalar_cast(h / scalar) }; }

		constexpr extent_2d_t operator%(scalar_t scalar) const noexcept { return extent_2d_t{ scalar_cast(w % scalar), scalar_cast(h % scalar) }; }

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

		constexpr product_t size() const noexcept { return area(); }

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
