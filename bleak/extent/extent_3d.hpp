#pragma once

#include "bleak/primitive.hpp"

#include <cmath>
#include <format>
#include <limits>
#include <string>

#include <SDL.h>

extern "C" {
	typedef struct c_extent_3d_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::uhalf scalar_t;
		typedef bleak::uquad product_t;
#else
		typedef bleak::uqrtr scalar_t;
		typedef bleak::usize product_t;
#endif
		scalar_t w { 0 };
		scalar_t h { 0 };
		scalar_t d { 0 };

		static_assert(
			(product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max()
				<= std::numeric_limits<product_t>::max(),
			"product_t is too small for scalar_t"
		);
	} c_extent_3d_t;
}

namespace bleak {
	struct extent_3d_t : public c_extent_3d_t {
		static const extent_3d_t zero;

		constexpr extent_3d_t() noexcept {}

		constexpr extent_3d_t(scalar_t scalar) noexcept : c_extent_3d_t { scalar, scalar, scalar } {}

		constexpr extent_3d_t(scalar_t width, scalar_t height, scalar_t depth) noexcept : c_extent_3d_t { width, height, depth } {}

		constexpr product_t area() const noexcept { return (product_t)w * h; }

		constexpr product_t volume() const noexcept { return (product_t)w * h * d; }

		constexpr bool operator==(cref<extent_3d_t> other) const noexcept { return w == other.w && h == other.h && d == other.d; }

		constexpr bool operator!=(cref<extent_3d_t> other) const noexcept { return w != other.w || h != other.h || d != other.d; }

		constexpr bool operator<(cref<extent_3d_t> other) const { return volume() < other.volume(); }

		constexpr bool operator>(cref<extent_3d_t> other) const { return volume() > other.volume(); }

		constexpr bool operator<=(cref<extent_3d_t> other) const { return volume() <= other.volume(); }

		constexpr bool operator>=(cref<extent_3d_t> other) const { return volume() >= other.volume(); }

		constexpr extent_3d_t operator+(cref<extent_3d_t> other) const noexcept {
			return { static_cast<scalar_t>(w + other.w), static_cast<scalar_t>(h + other.h), static_cast<scalar_t>(d + other.d) };
		}

		constexpr extent_3d_t operator-(cref<extent_3d_t> other) const noexcept {
			return { static_cast<scalar_t>(w - other.w), static_cast<scalar_t>(h - other.h), static_cast<scalar_t>(d - other.d) };
		}

		constexpr extent_3d_t operator*(cref<extent_3d_t> other) const noexcept {
			return { static_cast<scalar_t>(w * other.w), static_cast<scalar_t>(h * other.h), static_cast<scalar_t>(d * other.d) };
		}

		constexpr extent_3d_t operator/(cref<extent_3d_t> other) const noexcept {
			return { static_cast<scalar_t>(w / other.w), static_cast<scalar_t>(h / other.h), static_cast<scalar_t>(d / other.d) };
		}

		constexpr extent_3d_t operator%(cref<extent_3d_t> other) const noexcept {
			return { static_cast<scalar_t>(w % other.w), static_cast<scalar_t>(h % other.h), static_cast<scalar_t>(d % other.d) };
		}

		constexpr extent_3d_t operator+(scalar_t scalar) const noexcept {
			return { static_cast<scalar_t>(w + scalar), static_cast<scalar_t>(h + scalar), static_cast<scalar_t>(d + scalar) };
		}

		constexpr extent_3d_t operator-(scalar_t scalar) const noexcept {
			return { static_cast<scalar_t>(w - scalar), static_cast<scalar_t>(h - scalar), static_cast<scalar_t>(d - scalar) };
		}

		constexpr extent_3d_t operator*(scalar_t scalar) const noexcept {
			return { static_cast<scalar_t>(w * scalar), static_cast<scalar_t>(h * scalar), static_cast<scalar_t>(d * scalar) };
		}

		constexpr extent_3d_t operator/(scalar_t scalar) const noexcept {
			return { static_cast<scalar_t>(w / scalar), static_cast<scalar_t>(h / scalar), static_cast<scalar_t>(d / scalar) };
		}

		constexpr extent_3d_t operator%(scalar_t scalar) const noexcept {
			return { static_cast<scalar_t>(w % scalar), static_cast<scalar_t>(h % scalar), static_cast<scalar_t>(d % scalar) };
		}

		constexpr ref<extent_3d_t> operator+=(cref<extent_3d_t> other) noexcept {
			w += other.w;
			h += other.h;
			d += other.d;

			return *this;
		}

		constexpr ref<extent_3d_t> operator-=(cref<extent_3d_t> other) noexcept {
			w -= other.w;
			h -= other.h;
			d -= other.d;

			return *this;
		}

		constexpr ref<extent_3d_t> operator*=(cref<extent_3d_t> other) noexcept {
			w *= other.w;
			h *= other.h;
			d *= other.d;

			return *this;
		}

		constexpr ref<extent_3d_t> operator/=(cref<extent_3d_t> other) noexcept {
			w /= other.w;
			h /= other.h;
			d /= other.d;

			return *this;
		}

		constexpr ref<extent_3d_t> operator%=(cref<extent_3d_t> other) noexcept {
			w %= other.w;
			h %= other.h;
			d %= other.d;

			return *this;
		}

		constexpr ref<extent_3d_t> operator+=(scalar_t scalar) noexcept {
			w += scalar;
			h += scalar;
			d += scalar;

			return *this;
		}

		constexpr ref<extent_3d_t> operator-=(scalar_t scalar) noexcept {
			w -= scalar;
			h -= scalar;
			d -= scalar;

			return *this;
		}

		constexpr ref<extent_3d_t> operator*=(scalar_t scalar) noexcept {
			w *= scalar;
			h *= scalar;
			d *= scalar;

			return *this;
		}

		constexpr ref<extent_3d_t> operator/=(scalar_t scalar) noexcept {
			w /= scalar;
			h /= scalar;
			d /= scalar;

			return *this;
		}

		constexpr ref<extent_3d_t> operator%=(scalar_t scalar) noexcept {
			w %= scalar;
			h %= scalar;
			d %= scalar;

			return *this;
		}

		constexpr operator std::string() const noexcept { return std::format("[{} x {} x {}]", w, h, d); }

		constexpr explicit operator sdl::point() const noexcept { return { static_cast<i32>(w), static_cast<i32>(h) }; }
		constexpr explicit operator sdl::fpoint() const noexcept { return { static_cast<f32>(w), static_cast<f32>(h) }; }

		constexpr explicit operator extent_1d_t() const noexcept;
		constexpr explicit operator extent_2d_t() const noexcept;

		constexpr explicit operator offset_1d_t() const noexcept;
		constexpr explicit operator offset_2d_t() const noexcept;
		constexpr explicit operator offset_3d_t() const noexcept;
	};
} // namespace bleak
