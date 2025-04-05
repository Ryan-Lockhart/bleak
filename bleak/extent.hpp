#pragma once

#include <bleak/primitive.hpp> // IWYU pragma: export

#include <cmath>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

#include <SDL.h>

#include <bleak/hash.hpp>

extern "C" {
	typedef struct c_extent_t {
		typedef bleak::ihalf scalar_t;
		typedef bleak::isize product_t;

		scalar_t w{ 0 };
		scalar_t h{ 0 };

		static_assert((product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max() <= std::numeric_limits<product_t>::max(), "product_t is too small for scalar_t");
	} c_extent_t;
}

namespace bleak {
	struct extent_t : public c_extent_t {
		using underlying_t = c_extent_t;

		template<typename T> static constexpr scalar_t scalar_cast(T value) noexcept { return static_cast<scalar_t>(value); }

		template<typename T> static constexpr product_t product_cast(T value) noexcept { return static_cast<product_t>(value); }

		static const extent_t Zero;

		constexpr extent_t() noexcept {}

		constexpr explicit extent_t(scalar_t scalar) noexcept : c_extent_t{ scalar, scalar } {}

		constexpr explicit extent_t(scalar_t width, scalar_t height) noexcept : c_extent_t{ width, height } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value
		constexpr explicit extent_t(T width, T height) noexcept : underlying_t{ scalar_cast(width), scalar_cast(height) } {}

		template<typename W, typename H>
			requires std::is_convertible<W, scalar_t>::value && std::is_convertible<H, scalar_t>::value
		constexpr explicit extent_t(W width, H height) noexcept : underlying_t{ scalar_cast(width), scalar_cast(height) } {}

		constexpr product_t area() const noexcept { return product_cast(w) * product_cast(h); }

		constexpr product_t size() const noexcept { return area(); }

		constexpr bool operator==(cref<extent_t> other) const noexcept { return w == other.w && h == other.h; }

		constexpr bool operator!=(cref<extent_t> other) const noexcept { return w != other.w || h != other.h; }

		constexpr bool operator<(cref<extent_t> other) const { return area() < other.area(); }

		constexpr bool operator>(cref<extent_t> other) const { return area() > other.area(); }

		constexpr bool operator<=(cref<extent_t> other) const { return area() <= other.area(); }

		constexpr bool operator>=(cref<extent_t> other) const { return area() >= other.area(); }

		constexpr offset_t operator-() const noexcept;

		constexpr extent_t operator+(cref<extent_t> other) const noexcept { return extent_t{ scalar_cast(w + other.w), scalar_cast(h + other.h) }; }

		constexpr extent_t operator-(cref<extent_t> other) const noexcept { return extent_t{ scalar_cast(w - other.w), scalar_cast(h - other.h) }; }

		constexpr extent_t operator*(cref<extent_t> other) const noexcept { return extent_t{ scalar_cast(w * other.w), scalar_cast(h * other.h) }; }

		constexpr extent_t operator/(cref<extent_t> other) const noexcept { return extent_t{ scalar_cast(w / other.w), scalar_cast(h / other.h) }; }

		constexpr extent_t operator%(cref<extent_t> other) const noexcept { return extent_t{ scalar_cast(w % other.w), scalar_cast(h % other.h) }; }

		constexpr extent_t operator+(scalar_t scalar) const noexcept { return extent_t{ scalar_cast(w + scalar), scalar_cast(h + scalar) }; }

		constexpr extent_t operator-(scalar_t scalar) const noexcept { return extent_t{ scalar_cast(w - scalar), scalar_cast(h - scalar) }; }

		constexpr extent_t operator*(scalar_t scalar) const noexcept { return extent_t{ scalar_cast(w * scalar), scalar_cast(h * scalar) }; }

		constexpr extent_t operator/(scalar_t scalar) const noexcept { return extent_t{ scalar_cast(w / scalar), scalar_cast(h / scalar) }; }

		constexpr extent_t operator%(scalar_t scalar) const noexcept { return extent_t{ scalar_cast(w % scalar), scalar_cast(h % scalar) }; }

		constexpr extent_t operator+(cref<offset_t> other) const noexcept;

		constexpr extent_t operator-(cref<offset_t> other) const noexcept;

		constexpr extent_t operator*(cref<offset_t> other) const noexcept;

		constexpr extent_t operator/(cref<offset_t> other) const noexcept;

		constexpr extent_t operator%(cref<offset_t> other) const noexcept;

		constexpr ref<extent_t> operator+=(cref<extent_t> other) noexcept {
			w += other.w;
			h += other.h;

			return *this;
		}

		constexpr ref<extent_t> operator-=(cref<extent_t> other) noexcept {
			w -= other.w;
			h -= other.h;

			return *this;
		}

		constexpr ref<extent_t> operator*=(cref<extent_t> other) noexcept {
			w *= other.w;
			h *= other.h;

			return *this;
		}

		constexpr ref<extent_t> operator/=(cref<extent_t> other) noexcept {
			w /= other.w;
			h /= other.h;

			return *this;
		}

		constexpr ref<extent_t> operator%=(cref<extent_t> other) noexcept {
			w %= other.w;
			h %= other.h;

			return *this;
		}

		constexpr ref<extent_t> operator+=(scalar_t scalar) noexcept {
			w += scalar;
			h += scalar;

			return *this;
		}

		constexpr ref<extent_t> operator-=(scalar_t scalar) noexcept {
			w -= scalar;
			h -= scalar;

			return *this;
		}

		constexpr ref<extent_t> operator*=(scalar_t scalar) noexcept {
			w *= scalar;
			h *= scalar;

			return *this;
		}

		constexpr ref<extent_t> operator/=(scalar_t scalar) noexcept {
			w /= scalar;
			h /= scalar;

			return *this;
		}

		constexpr ref<extent_t> operator%=(scalar_t scalar) noexcept {
			w %= scalar;
			h %= scalar;

			return *this;
		}

		constexpr ref<extent_t> operator+=(cref<offset_t> other) noexcept;

		constexpr ref<extent_t> operator-=(cref<offset_t> other) noexcept;

		constexpr ref<extent_t> operator*=(cref<offset_t> other) noexcept;

		constexpr ref<extent_t> operator/=(cref<offset_t> other) noexcept;

		constexpr ref<extent_t> operator%=(cref<offset_t> other) noexcept;

		constexpr operator std::string() const noexcept { return std::format("[{} x {}]", w, h); }

		constexpr explicit operator sdl::point() const noexcept { return { static_cast<i32>(w), static_cast<i32>(h) }; }

		constexpr explicit operator sdl::fpoint() const noexcept { return { static_cast<f32>(w), static_cast<f32>(h) }; }
		
		constexpr explicit operator offset_t() const noexcept;

		struct hasher {
			static constexpr usize operator()(cref<extent_t> extent) noexcept { return hash_combine(extent.w, extent.h); }
		};
	};
} // namespace bleak

#include <bleak/offset.hpp> // IWYU pragma: export

namespace bleak {
	constexpr const extent_t extent_t::Zero { 0, 0 };

	constexpr extent_t extent_t::operator+(cref<offset_t> other) const noexcept { return extent_t{ scalar_cast(w + other.x), scalar_cast(h + other.y) }; }

	constexpr extent_t extent_t::operator-(cref<offset_t> other) const noexcept { return extent_t{ scalar_cast(w - other.x), scalar_cast(h - other.y) }; }

	constexpr extent_t extent_t::operator*(cref<offset_t> other) const noexcept { return extent_t{ scalar_cast(w * other.x), scalar_cast(h * other.y) }; }

	constexpr extent_t extent_t::operator/(cref<offset_t> other) const noexcept { return extent_t{ scalar_cast(w / other.x), scalar_cast(h / other.y) }; }

	constexpr extent_t extent_t::operator%(cref<offset_t> other) const noexcept { return extent_t{ scalar_cast(w % other.x), scalar_cast(h % other.y) }; }

	constexpr ref<extent_t> extent_t::operator+=(cref<offset_t> other) noexcept {
		w += other.x;
		h += other.y;

		return *this;
	}

	constexpr ref<extent_t> extent_t::operator-=(cref<offset_t> other) noexcept {
		w -= other.x;
		h -= other.y;

		return *this;
	}

	constexpr ref<extent_t> extent_t::operator*=(cref<offset_t> other) noexcept {
		w *= other.x;
		h *= other.y;

		return *this;
	}

	constexpr ref<extent_t> extent_t::operator/=(cref<offset_t> other) noexcept {
		w /= other.x;
		h /= other.y;

		return *this;
	}

	constexpr ref<extent_t> extent_t::operator%=(cref<offset_t> other) noexcept {
		w %= other.x;
		h %= other.y;

		return *this;
	}

    constexpr extent_t::operator offset_t() const noexcept { return offset_t{ offset_t::scalar_cast(w), offset_t::scalar_cast(h) }; }
} // namespace bleak
