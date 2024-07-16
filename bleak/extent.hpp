#pragma once

#include <bleak/primitive.hpp> // IWYU pragma: export

#include <cmath>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

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

	typedef struct c_extent_3d_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::uhalf scalar_t;
		typedef bleak::usize product_t;
#else
		typedef bleak::uqrtr scalar_t;
		typedef bleak::uhalf product_t;
#endif
		scalar_t w{ 0 };
		scalar_t h{ 0 };
		scalar_t d{ 0 };

		static_assert(
			(product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max() * (product_t)std::numeric_limits<scalar_t>::max() <= std::numeric_limits<product_t>::max(), "product_t is too small for scalar_t"
		);
	} c_extent_3d_t;
}

namespace bleak {
	struct extent_1d_t : public c_extent_1d_t {
		using underlying_t = c_extent_1d_t;

		template<typename T> static constexpr scalar_t scalar_cast(T value) noexcept { return static_cast<scalar_t>(value); }

		template<typename T> static constexpr product_t product_cast(T value) noexcept { return static_cast<product_t>(value); }

		static const extent_1d_t zero;

		constexpr explicit extent_1d_t() noexcept {}

		constexpr explicit extent_1d_t(scalar_t width) noexcept : underlying_t{ width } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && (std::is_same<T, scalar_t>::value == false)
		constexpr explicit extent_1d_t(T scalar) noexcept : underlying_t{ scalar_cast(scalar) } {}

		constexpr bool operator==(cref<extent_1d_t> other) const noexcept { return w == other.w; }

		constexpr bool operator!=(cref<extent_1d_t> other) const noexcept { return w != other.w; }

		constexpr bool operator==(scalar_t scalar) const noexcept { return w == scalar; }

		constexpr bool operator!=(scalar_t scalar) const noexcept { return w != scalar; }

		constexpr bool operator<(cref<extent_1d_t> other) const { return w < other.w; }

		constexpr bool operator>(cref<extent_1d_t> other) const { return w > other.w; }

		constexpr bool operator<=(cref<extent_1d_t> other) const { return w <= other.w; }

		constexpr bool operator>=(cref<extent_1d_t> other) const { return w >= other.w; }

		constexpr extent_1d_t operator+(cref<extent_1d_t> other) const noexcept { return extent_1d_t{ w + other.w }; }

		constexpr extent_1d_t operator-(cref<extent_1d_t> other) const noexcept { return extent_1d_t{ w - other.w }; }

		constexpr extent_1d_t operator*(cref<extent_1d_t> other) const noexcept { return extent_1d_t{ w * other.w }; }

		constexpr extent_1d_t operator/(cref<extent_1d_t> other) const noexcept { return extent_1d_t{ w / other.w }; }

		constexpr extent_1d_t operator%(cref<extent_1d_t> other) const noexcept { return extent_1d_t{ w % other.w }; }

		constexpr extent_1d_t operator+(scalar_t scalar) const noexcept { return extent_1d_t{ w + scalar }; }

		constexpr extent_1d_t operator-(scalar_t scalar) const noexcept { return extent_1d_t{ w - scalar }; }

		constexpr extent_1d_t operator*(scalar_t scalar) const noexcept { return extent_1d_t{ w * scalar }; }

		constexpr extent_1d_t operator/(scalar_t scalar) const noexcept { return extent_1d_t{ w / scalar }; }

		constexpr extent_1d_t operator%(scalar_t scalar) const noexcept { return extent_1d_t{ w % scalar }; }

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

		struct hasher {
			constexpr usize operator()(cref<extent_1d_t> extent) const noexcept { return hash_combine(extent.w); }
		};
	};

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

		struct hasher {
			constexpr usize operator()(cref<extent_2d_t> extent) const noexcept { return hash_combine(extent.w, extent.h); }
		};
	};

	struct extent_3d_t : public c_extent_3d_t {
		using underlying_t = c_extent_3d_t;

		template<typename T> static constexpr scalar_t scalar_cast(T value) noexcept { return static_cast<scalar_t>(value); }

		template<typename T> static constexpr product_t product_cast(T value) noexcept { return static_cast<product_t>(value); }

		static const extent_3d_t zero;

		constexpr explicit extent_3d_t() noexcept {}

		constexpr explicit extent_3d_t(scalar_t scalar) noexcept : c_extent_3d_t{ scalar, scalar, scalar } {}

		constexpr explicit extent_3d_t(scalar_t width, scalar_t height, scalar_t depth) noexcept : c_extent_3d_t{ width, height, depth } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value
		constexpr explicit extent_3d_t(T width, T height, T depth) noexcept : c_extent_3d_t{ scalar_cast(width), scalar_cast(height), scalar_cast(depth) } {}

		template<typename W, typename H, typename D>
			requires std::is_convertible<W, scalar_t>::value && std::is_convertible<H, scalar_t>::value && std::is_convertible<D, scalar_t>::value
		constexpr explicit extent_3d_t(W width, H height, D depth) noexcept : c_extent_3d_t{ scalar_cast(width), scalar_cast(height), scalar_cast(depth) } {}

		constexpr product_t area() const noexcept { return product_cast(w) * h; }

		constexpr product_t volume() const noexcept { return product_cast(w) * h * d; }

		constexpr bool operator==(cref<extent_3d_t> other) const noexcept { return w == other.w && h == other.h && d == other.d; }

		constexpr bool operator!=(cref<extent_3d_t> other) const noexcept { return w != other.w || h != other.h || d != other.d; }

		constexpr bool operator<(cref<extent_3d_t> other) const { return volume() < other.volume(); }

		constexpr bool operator>(cref<extent_3d_t> other) const { return volume() > other.volume(); }

		constexpr bool operator<=(cref<extent_3d_t> other) const { return volume() <= other.volume(); }

		constexpr bool operator>=(cref<extent_3d_t> other) const { return volume() >= other.volume(); }

		constexpr extent_3d_t operator+(cref<extent_3d_t> other) const noexcept { return extent_3d_t{ scalar_cast(w + other.w), scalar_cast(h + other.h), scalar_cast(d + other.d) }; }

		constexpr extent_3d_t operator-(cref<extent_3d_t> other) const noexcept { return extent_3d_t{ scalar_cast(w - other.w), scalar_cast(h - other.h), scalar_cast(d - other.d) }; }

		constexpr extent_3d_t operator*(cref<extent_3d_t> other) const noexcept { return extent_3d_t{ scalar_cast(w * other.w), scalar_cast(h * other.h), scalar_cast(d * other.d) }; }

		constexpr extent_3d_t operator/(cref<extent_3d_t> other) const noexcept { return extent_3d_t{ scalar_cast(w / other.w), scalar_cast(h / other.h), scalar_cast(d / other.d) }; }

		constexpr extent_3d_t operator%(cref<extent_3d_t> other) const noexcept { return extent_3d_t{ scalar_cast(w % other.w), scalar_cast(h % other.h), scalar_cast(d % other.d) }; }

		constexpr extent_3d_t operator+(scalar_t scalar) const noexcept { return extent_3d_t{ scalar_cast(w + scalar), scalar_cast(h + scalar), scalar_cast(d + scalar) }; }

		constexpr extent_3d_t operator-(scalar_t scalar) const noexcept { return extent_3d_t{ scalar_cast(w - scalar), scalar_cast(h - scalar), scalar_cast(d - scalar) }; }

		constexpr extent_3d_t operator*(scalar_t scalar) const noexcept { return extent_3d_t{ scalar_cast(w * scalar), scalar_cast(h * scalar), scalar_cast(d * scalar) }; }

		constexpr extent_3d_t operator/(scalar_t scalar) const noexcept { return extent_3d_t{ scalar_cast(w / scalar), scalar_cast(h / scalar), scalar_cast(d / scalar) }; }

		constexpr extent_3d_t operator%(scalar_t scalar) const noexcept { return extent_3d_t{ scalar_cast(w % scalar), scalar_cast(h % scalar), scalar_cast(d % scalar) }; }

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

		constexpr product_t size() const noexcept { return volume(); }

		constexpr operator std::string() const noexcept { return std::format("[{} x {} x {}]", w, h, d); }

		constexpr explicit operator sdl::point() const noexcept { return { static_cast<i32>(w), static_cast<i32>(h) }; }

		constexpr explicit operator sdl::fpoint() const noexcept { return { static_cast<f32>(w), static_cast<f32>(h) }; }

		constexpr explicit operator extent_1d_t() const noexcept;
		constexpr explicit operator extent_2d_t() const noexcept;

		constexpr explicit operator offset_1d_t() const noexcept;
		constexpr explicit operator offset_2d_t() const noexcept;
		constexpr explicit operator offset_3d_t() const noexcept;

		struct hasher {
			constexpr usize operator()(cref<extent_3d_t> extent) const noexcept { return hash_combine(extent.w, extent.h, extent.d); }
		};
	};
} // namespace bleak

#include <bleak/offset.hpp> // IWYU pragma: export
namespace bleak {
    constexpr const extent_1d_t extent_1d_t::zero{ extent_1d_t::scalar_t{ 0 } };

    constexpr const extent_2d_t extent_2d_t::zero{ extent_2d_t::scalar_t{ 0 }, extent_2d_t::scalar_t{ 0 } };

    constexpr const extent_3d_t extent_3d_t::zero{ extent_3d_t::scalar_t{ 0 }, extent_3d_t::scalar_t{ 0 }, extent_3d_t::scalar_t{ 0 } };

    constexpr offset_2d_t extent_2d_t::operator-() const noexcept { return offset_2d_t{ -offset_2d_t::scalar_cast(w), -offset_2d_t::scalar_cast(h) }; }

    constexpr extent_1d_t::operator extent_2d_t() const noexcept { return extent_2d_t{ static_cast<extent_2d_t::scalar_t>(w), extent_2d_t::scalar_t{ 0 } }; }

    constexpr extent_1d_t::operator extent_3d_t() const noexcept { return extent_3d_t{ static_cast<extent_3d_t::scalar_t>(w), extent_3d_t::scalar_t{ 0 }, extent_3d_t::scalar_t{ 0 } }; }

    constexpr extent_1d_t::operator offset_1d_t() const noexcept { return offset_1d_t{ static_cast<offset_1d_t::scalar_t>(w) }; }

    constexpr extent_1d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(w), offset_2d_t::scalar_t{ 0 } }; }

    constexpr extent_1d_t::operator offset_3d_t() const noexcept { return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(w), offset_3d_t::scalar_t{ 0 }, offset_3d_t::scalar_t{ 0 } }; }

    constexpr extent_2d_t::operator offset_1d_t() const noexcept { return offset_1d_t{ static_cast<offset_1d_t::scalar_t>(w) }; }

    constexpr extent_2d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(w), static_cast<offset_2d_t::scalar_t>(h) }; }

    constexpr extent_2d_t::operator offset_3d_t() const noexcept { return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(w), static_cast<offset_3d_t::scalar_t>(h), offset_3d_t::scalar_t{ 0 } }; }

    constexpr extent_2d_t::operator extent_1d_t() const noexcept { return extent_1d_t{ static_cast<extent_1d_t::scalar_t>(w) }; }

    constexpr extent_2d_t::operator extent_3d_t() const noexcept { return extent_3d_t{ static_cast<extent_3d_t::scalar_t>(w), static_cast<extent_3d_t::scalar_t>(h), extent_3d_t::scalar_t{ 0 } }; }

    constexpr extent_3d_t::operator extent_2d_t() const noexcept { return extent_2d_t{ static_cast<extent_2d_t::scalar_t>(w), static_cast<extent_2d_t::scalar_t>(h) }; }

    constexpr extent_3d_t::operator extent_1d_t() const noexcept { return extent_1d_t{ static_cast<extent_1d_t::scalar_t>(w) }; }

    constexpr extent_3d_t::operator offset_1d_t() const noexcept { return offset_1d_t{ static_cast<offset_1d_t::scalar_t>(w) }; }

    constexpr extent_3d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(w), static_cast<offset_2d_t::scalar_t>(h) }; }

    constexpr extent_3d_t::operator offset_3d_t() const noexcept { return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(w), static_cast<offset_3d_t::scalar_t>(h), static_cast<offset_3d_t::scalar_t>(d) }; }
} // namespace bleak
