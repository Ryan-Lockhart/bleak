#pragma once

#include <bleak/primitive.hpp> // IWYU pragma: export

#include <cmath>
#include <format>
#include <limits>
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
			(product_t)std::numeric_limits<scalar_t>::max() *
			(product_t)std::numeric_limits<scalar_t>::max() <=
			std::numeric_limits<product_t>::max(), "product_t is too small for scalar_t"
		);
	} c_offset_2d_t;
	
	typedef struct c_offset_3d_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::ihalf scalar_t;
		typedef bleak::iquad product_t;
		typedef bleak::fquad float_t;
#else
		typedef bleak::iqrtr scalar_t;
		typedef bleak::isize product_t;
		typedef bleak::fsize float_t;
#endif

		scalar_t x{ 0 };
		scalar_t y{ 0 };
		scalar_t z{ 0 };

		static_assert(
			(product_t)std::numeric_limits<scalar_t>::max() *
			(product_t)std::numeric_limits<scalar_t>::max() *
			(product_t)std::numeric_limits<scalar_t>::max() <=
			std::numeric_limits<product_t>::max(), "product_t is too small for scalar_t"
		);
	} c_offset_3d_t;
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

		struct hasher {
			constexpr usize operator()(cref<offset_1d_t> offset) const noexcept {
				return hash_combine(offset.x);
			}
		};
	};
	
	struct offset_2d_t : c_offset_2d_t {
		using underlying_t = c_offset_2d_t;

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

		constexpr explicit offset_2d_t() noexcept {}

		constexpr explicit offset_2d_t(scalar_t scalar) noexcept : underlying_t{ scalar, scalar } {}

		constexpr explicit offset_2d_t(scalar_t x, scalar_t y) noexcept : underlying_t{ x, y } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && is_numeric<T>::value
		constexpr explicit offset_2d_t(T scalar) noexcept : underlying_t{ scalar_cast(scalar), scalar_cast(scalar) } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && is_numeric<T>::value
		constexpr explicit offset_2d_t(T x, T y) noexcept : underlying_t{ scalar_cast(x), scalar_cast(y) } {}

		template<typename X, typename Y>
			requires std::is_convertible<X, scalar_t>::value && std::is_convertible<Y, scalar_t>::value && is_numeric<X>::value && is_numeric<Y>::value
		constexpr explicit offset_2d_t(X x, Y y) noexcept : underlying_t{ scalar_cast(x), scalar_cast(y) } {}

		constexpr explicit offset_2d_t(cref<cardinal_t> direction) noexcept : underlying_t{} {
			if (direction == cardinal_t::Central) {
				return;
			} else {
				if (!direction.is_lat_neutral()) {
					x = direction.east ? 1 : -1;
				}
				if (!direction.is_long_neutral()) {
					y = direction.north ? -1 : 1;
				}
			}
		}

		constexpr product_t dot() const noexcept { return product_cast(x) * x + product_cast(y) * y; }

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

		template<typename T = product_t> static constexpr offset_2d_t direction(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept { return (end - start).normalized<T>(); }

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
				return offset_2d_t{ scalar_cast(x / len), scalar_cast(y / len) };
			} else {
				return offset_2d_t{ *this };
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

		constexpr offset_2d_t operator-() const noexcept { return offset_2d_t{ scalar_cast(-x), scalar_cast(-y) }; }

		constexpr offset_2d_t operator+(cref<offset_2d_t> other) const noexcept { return offset_2d_t{ scalar_cast(x + other.x), scalar_cast(y + other.y) }; }

		constexpr offset_2d_t operator-(cref<offset_2d_t> other) const noexcept { return offset_2d_t{ scalar_cast(x - other.x), scalar_cast(y - other.y) }; }

		constexpr offset_2d_t operator*(cref<offset_2d_t> other) const noexcept { return offset_2d_t{ scalar_cast(x * other.x), scalar_cast(y * other.y) }; }

		constexpr offset_2d_t operator/(cref<offset_2d_t> other) const noexcept { return offset_2d_t{ scalar_cast(x / other.x), scalar_cast(y / other.y) }; }

		constexpr offset_2d_t operator%(cref<offset_2d_t> other) const noexcept { return offset_2d_t{ scalar_cast(x % other.x), scalar_cast(y % other.y) }; }

		constexpr offset_2d_t operator+(scalar_t scalar) const noexcept { return offset_2d_t{ scalar_cast(x + scalar), scalar_cast(y + scalar) }; }

		constexpr offset_2d_t operator-(scalar_t scalar) const noexcept { return offset_2d_t{ scalar_cast(x - scalar), scalar_cast(y - scalar) }; }

		constexpr offset_2d_t operator*(scalar_t scalar) const noexcept { return offset_2d_t{ scalar_cast(x * scalar), scalar_cast(y * scalar) }; }

		constexpr offset_2d_t operator/(scalar_t scalar) const noexcept { return offset_2d_t{ scalar_cast(x / scalar), scalar_cast(y / scalar) }; }

		constexpr offset_2d_t operator%(scalar_t scalar) const noexcept { return offset_2d_t{ scalar_cast(x % scalar), scalar_cast(y % scalar) }; }

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

		struct hasher {
			constexpr usize operator()(cref<offset_2d_t> offset) const noexcept {
				return hash_combine(offset.x, offset.y);
			}
		};
	};
	
	struct offset_3d_t : c_offset_3d_t {
		using underlying_t = c_offset_3d_t;

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && is_numeric<T>::value
		static constexpr scalar_t scalar_cast(T value) noexcept {
			return static_cast<scalar_t>(value);
		}

		template<typename T>
			requires std::is_convertible<T, product_t>::value && is_numeric<T>::value
		static constexpr product_t product_cast(T value) noexcept {
			return static_cast<product_t>(value);
		}

		template<typename T>
			requires std::is_convertible<T, float_t>::value && is_numeric<T>::value
		static constexpr float_t float_cast(T value) noexcept {
			return static_cast<float_t>(value);
		}

		static const offset_3d_t zero;

		static const offset_3d_t central;

		static const offset_3d_t north;
		static const offset_3d_t south;

		static const offset_3d_t east;
		static const offset_3d_t west;

		static const offset_3d_t up;
		static const offset_3d_t down;

		static const offset_3d_t northwest;
		static const offset_3d_t northeast;

		static const offset_3d_t southwest;
		static const offset_3d_t southeast;

		struct vertical {
			struct up {
				static const offset_3d_t central;

				static const offset_3d_t north;
				static const offset_3d_t south;

				static const offset_3d_t east;
				static const offset_3d_t west;

				static const offset_3d_t northwest;
				static const offset_3d_t Northeast;

				static const offset_3d_t southwest;
				static const offset_3d_t southeast;
			};

			struct down {
				static const offset_3d_t central;

				static const offset_3d_t north;
				static const offset_3d_t south;

				static const offset_3d_t east;
				static const offset_3d_t west;

				static const offset_3d_t northwest;
				static const offset_3d_t northeast;

				static const offset_3d_t southwest;
				static const offset_3d_t southeast;
			};
		};

		constexpr explicit offset_3d_t() noexcept {}

		constexpr explicit offset_3d_t(scalar_t scalar) noexcept : underlying_t{ scalar, scalar, scalar } {}

		constexpr explicit offset_3d_t(scalar_t x, scalar_t y, scalar_t z) noexcept : underlying_t{ x, y, z } {}

		template<typename T>
			requires is_numeric<T>::value && std::is_convertible<T, scalar_t>::value
		constexpr explicit offset_3d_t(T x, T y, T z) noexcept : underlying_t{ scalar_cast(x), scalar_cast(y), scalar_cast(z) } {}

		template<typename X, typename Y, typename Z>
			requires is_numeric<X>::value && std::is_convertible<X, scalar_t>::value && is_numeric<Y>::value && std::is_convertible<Y, scalar_t>::value && is_numeric<Z>::value && std::is_convertible<Z, scalar_t>::value
		constexpr explicit offset_3d_t(X x, Y y, Z z) noexcept : underlying_t{ scalar_cast(x), scalar_cast(y), scalar_cast(z) } {}

		constexpr explicit offset_3d_t(cref<cardinal_t> direction) noexcept : underlying_t{} {
			if (direction == cardinal_t::Central) {
				return;
			} else {
				if (!direction.is_lat_neutral()) {
					x = direction.east ? 1 : -1;
				}
				if (!direction.is_long_neutral()) {
					y = direction.north ? -1 : 1;
				}
				if (!direction.is_vert_neutral()) {
					z = direction.down ? -1 : 1;
				}
			}
		}

		constexpr product_t dot() const noexcept { return product_cast(x) * x + product_cast(y) * y + product_cast(z) * z; }

		constexpr offset_3d_t cross(cref<offset_3d_t> other) const noexcept { return offset_3d_t{ scalar_cast(y * other.z - z * other.y), scalar_cast(z * other.x - x * other.z), scalar_cast(x * other.y - y * other.x) }; }

		template<typename T>
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

		template<typename T>
			requires std::is_floating_point<T>::value || std::is_same<T, product_t>::value
		static constexpr T distance(cref<offset_3d_t> start, cref<offset_3d_t> end) noexcept;

		template<> constexpr f32 distance(cref<offset_3d_t> start, cref<offset_3d_t> end) noexcept { return (end - start).length<f32>(); }

		template<> constexpr f64 distance(cref<offset_3d_t> start, cref<offset_3d_t> end) noexcept { return (end - start).length<f64>(); }

		template<> constexpr product_t distance(cref<offset_3d_t> start, cref<offset_3d_t> end) noexcept { return (end - start).length<product_t>(); }

		static constexpr offset_3d_t direction(cref<offset_3d_t> start, cref<offset_3d_t> end) noexcept { return (end - start).normalized(); }

		constexpr ref<offset_3d_t> normalize() noexcept {
			const auto len = length<product_t>();

			if (len != 0) {
				x = scalar_cast(x / len);
				y = scalar_cast(y / len);
				z = scalar_cast(z / len);
			}

			return *this;
		}

		constexpr offset_3d_t normalized() const noexcept {
			const auto len = length<product_t>();

			if (len != 0) {
				return offset_3d_t{ scalar_cast(x / len), scalar_cast(y / len), scalar_cast(z / len) };
			} else {
				return { *this };
			}
		}

		constexpr ref<offset_3d_t> clamp(cref<offset_3d_t> min, cref<offset_3d_t> max) {
			x = x < min.x ? min.x : x > max.x ? max.x : x;
			y = y < min.y ? min.y : y > max.y ? max.y : y;
			z = z < min.z ? min.z : z > max.z ? max.z : z;

			return *this;
		}

		static constexpr offset_3d_t clamp(offset_3d_t value, cref<offset_3d_t> min, cref<offset_3d_t> max) { return value.clamp(min, max); }

		constexpr ref<offset_3d_t> clamp(cref<extent_3d_t> min, cref<extent_3d_t> max);

		static constexpr offset_3d_t clamp(offset_3d_t value, cref<extent_3d_t> min, cref<extent_3d_t> max);

		static constexpr product_t flatten(cref<offset_3d_t> offset) noexcept;

		static constexpr offset_3d_t unflatten(cref<product_t> index) noexcept;

		constexpr bool operator==(cref<offset_3d_t> other) const noexcept { return x == other.x && y == other.y && z == other.z; }

		constexpr bool operator!=(cref<offset_3d_t> other) const noexcept { return x != other.x || y != other.y || z != other.z; }

		constexpr bool operator<(cref<offset_3d_t> other) const { return x < other.x && y < other.y && z < other.z; }

		constexpr bool operator>(cref<offset_3d_t> other) const { return x > other.x && y > other.y && z > other.z; }

		constexpr bool operator<=(cref<offset_3d_t> other) const { return x <= other.x && y <= other.y && z <= other.z; }

		constexpr bool operator>=(cref<offset_3d_t> other) const { return x >= other.x && y >= other.y && z >= other.z; }

		constexpr offset_3d_t operator-() const noexcept { return offset_3d_t{ scalar_cast(-x), scalar_cast(-y), scalar_cast(-z) }; }

		constexpr offset_3d_t operator+(cref<offset_3d_t> other) const noexcept { return offset_3d_t{ scalar_cast(x + other.x), scalar_cast(y + other.y), scalar_cast(z + other.z) }; }

		constexpr offset_3d_t operator-(cref<offset_3d_t> other) const noexcept { return offset_3d_t{ scalar_cast(x - other.x), scalar_cast(y - other.y), scalar_cast(z - other.z) }; }

		constexpr offset_3d_t operator*(cref<offset_3d_t> other) const noexcept { return offset_3d_t{ scalar_cast(x * other.x), scalar_cast(y * other.y), scalar_cast(z * other.z) }; }

		constexpr offset_3d_t operator/(cref<offset_3d_t> other) const noexcept { return offset_3d_t{ scalar_cast(x / other.x), scalar_cast(y / other.y), scalar_cast(z / other.z) }; }

		constexpr offset_3d_t operator%(cref<offset_3d_t> other) const noexcept { return offset_3d_t{ scalar_cast(x % other.x), scalar_cast(y % other.y), scalar_cast(z % other.z) }; }

		template<f32> offset_3d_t operator%(cref<offset_3d_t> other) const noexcept { return offset_3d_t{ scalar_cast(fmodf(x, other.x)), scalar_cast(fmodf(y, other.y)), scalar_cast(fmodf(z, other.z)) }; }

		template<f64> offset_3d_t operator%(cref<offset_3d_t> other) const noexcept { return offset_3d_t{ scalar_cast(fmodl(x, other.x)), scalar_cast(fmodl(y, other.y)), scalar_cast(fmodl(z, other.z)) }; }

		constexpr offset_3d_t operator+(scalar_t scalar) const noexcept { return offset_3d_t{ scalar_cast(x + scalar), scalar_cast(y + scalar), scalar_cast(z + scalar) }; }

		constexpr offset_3d_t operator-(scalar_t scalar) const noexcept { return offset_3d_t{ scalar_cast(x - scalar), scalar_cast(y - scalar), scalar_cast(z - scalar) }; }

		constexpr offset_3d_t operator*(scalar_t scalar) const noexcept { return offset_3d_t{ scalar_cast(x * scalar), scalar_cast(y * scalar), scalar_cast(z * scalar) }; }

		constexpr offset_3d_t operator/(scalar_t scalar) const noexcept { return offset_3d_t{ scalar_cast(x / scalar), scalar_cast(y / scalar), scalar_cast(z / scalar) }; }

		constexpr offset_3d_t operator%(scalar_t scalar) const noexcept { return offset_3d_t{ scalar_cast(x % scalar), scalar_cast(y % scalar), scalar_cast(z % scalar) }; }

		constexpr offset_3d_t operator+(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_3d_t operator-(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_3d_t operator*(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_3d_t operator/(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_3d_t operator%(cref<extent_1d_t> extent) const noexcept;

		constexpr offset_3d_t operator+(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_3d_t operator-(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_3d_t operator*(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_3d_t operator/(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_3d_t operator%(cref<extent_2d_t> extent) const noexcept;

		constexpr offset_3d_t operator+(cref<extent_3d_t> extent) const noexcept;

		constexpr offset_3d_t operator-(cref<extent_3d_t> extent) const noexcept;

		constexpr offset_3d_t operator*(cref<extent_3d_t> extent) const noexcept;

		constexpr offset_3d_t operator/(cref<extent_3d_t> extent) const noexcept;

		constexpr offset_3d_t operator%(cref<extent_3d_t> extent) const noexcept;

		constexpr ref<offset_3d_t> operator+=(cref<offset_3d_t> other) noexcept {
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}

		constexpr ref<offset_3d_t> operator-=(cref<offset_3d_t> other) noexcept {
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}

		constexpr ref<offset_3d_t> operator*=(cref<offset_3d_t> other) noexcept {
			x *= other.x;
			y *= other.y;
			z *= other.z;

			return *this;
		}

		constexpr ref<offset_3d_t> operator/=(cref<offset_3d_t> other) noexcept {
			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}

		constexpr ref<offset_3d_t> operator%=(cref<offset_3d_t> other) noexcept {
			x %= other.x;
			y %= other.y;
			z %= other.z;

			return *this;
		}

		constexpr ref<offset_3d_t> operator+=(scalar_t scalar) noexcept {
			x += scalar;
			y += scalar;
			z += scalar;

			return *this;
		}

		constexpr ref<offset_3d_t> operator-=(scalar_t scalar) noexcept {
			x -= scalar;
			y -= scalar;
			z -= scalar;

			return *this;
		}

		constexpr ref<offset_3d_t> operator*=(scalar_t scalar) noexcept {
			x *= scalar;
			y *= scalar;
			z *= scalar;

			return *this;
		}

		constexpr ref<offset_3d_t> operator/=(scalar_t scalar) noexcept {
			x /= scalar;
			y /= scalar;
			z /= scalar;

			return *this;
		}

		constexpr ref<offset_3d_t> operator%=(scalar_t scalar) noexcept {
			x %= scalar;
			y %= scalar;
			z %= scalar;

			return *this;
		}

		constexpr ref<offset_3d_t> operator+=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator-=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator*=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator/=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator%=(cref<extent_1d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator+=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator-=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator*=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator/=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator%=(cref<extent_2d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator+=(cref<extent_3d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator-=(cref<extent_3d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator*=(cref<extent_3d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator/=(cref<extent_3d_t> extent) noexcept;

		constexpr ref<offset_3d_t> operator%=(cref<extent_3d_t> extent) noexcept;

		constexpr operator std::string() const noexcept { return std::format("[{}, {}, {}]", x, y, z); }

		constexpr operator cardinal_t() const noexcept {
			if (*this == offset_3d_t::zero) {
				return cardinal_t::Central;
			} else {
				cardinal_t result{ cardinal_t::Central };

				if (x != 0) {
					result += x < 0 ? cardinal_t::West : cardinal_t::East;
				}
				if (y != 0) {
					result += y < 0 ? cardinal_t::North : cardinal_t::South;
				}
				if (z != 0) {
					result += z < 0 ? cardinal_t::Down : cardinal_t::Up;
				}

				return result;
			}
		}

		constexpr explicit operator sdl::point() const noexcept { return { static_cast<i32>(x), static_cast<i32>(y) }; }

		constexpr explicit operator sdl::fpoint() const noexcept { return { static_cast<f32>(x), static_cast<f32>(y) }; }

		constexpr explicit operator offset_1d_t() const noexcept;
		constexpr explicit operator offset_2d_t() const noexcept;

		constexpr explicit operator extent_1d_t() const noexcept;
		constexpr explicit operator extent_2d_t() const noexcept;
		constexpr explicit operator extent_3d_t() const noexcept;

		struct hasher {
			constexpr usize operator()(cref<offset_3d_t> offset) const noexcept {
				return hash_combine(offset.x, offset.y, offset.z);
			}
		};
	};
} // namespace bleak

#include <bleak/extent.hpp> // IWYU pragma: export

namespace bleak {
	constexpr const offset_1d_t offset_1d_t::zero{ 0 };

	constexpr const offset_1d_t offset_1d_t::central{ offset_1d_t::zero };

	constexpr const offset_1d_t offset_1d_t::east{ -1 };
	constexpr const offset_1d_t offset_1d_t::west{ 1 };

	constexpr ref<offset_1d_t> offset_1d_t::clamp(cref<extent_1d_t> min, cref<extent_1d_t> max) {
		x = x < min.w ? min.w : x > max.w ? max.w : x;

		return *this;
	}

	constexpr offset_1d_t offset_1d_t::clamp(offset_1d_t value, cref<extent_1d_t> min, cref<extent_1d_t> max) { return value.clamp(min, max); }

	constexpr offset_1d_t offset_1d_t::operator+(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x + extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator-(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x - extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator*(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x * extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator/(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x / extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator%(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x % extent.w) }; }

	constexpr ref<offset_1d_t> offset_1d_t::operator+=(cref<extent_1d_t> extent) noexcept {
		x += extent.w;

		return *this;
	}

	constexpr ref<offset_1d_t> offset_1d_t::operator-=(cref<extent_1d_t> extent) noexcept {
		x -= extent.w;

		return *this;
	}

	constexpr ref<offset_1d_t> offset_1d_t::operator*=(cref<extent_1d_t> extent) noexcept {
		x *= extent.w;

		return *this;
	}

	constexpr ref<offset_1d_t> offset_1d_t::operator/=(cref<extent_1d_t> extent) noexcept {
		x /= extent.w;

		return *this;
	}

	constexpr ref<offset_1d_t> offset_1d_t::operator%=(cref<extent_1d_t> extent) noexcept {
		x %= extent.w;

		return *this;
	}

	constexpr offset_1d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(x) }; }

	constexpr offset_1d_t::operator offset_3d_t() const noexcept {
		return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(x), offset_3d_t::scalar_t{ 0 }, offset_3d_t::scalar_t{ 0 } };
	}

	constexpr const offset_2d_t offset_2d_t::zero{ 0 };

	constexpr const offset_2d_t offset_2d_t::central{ offset_2d_t::zero };

	constexpr const offset_2d_t offset_2d_t::north{ 0, -1 };
	constexpr const offset_2d_t offset_2d_t::south{ 0, 1 };

	constexpr const offset_2d_t offset_2d_t::east{ 1, 0 };
	constexpr const offset_2d_t offset_2d_t::west{ -1, 0 };

	constexpr const offset_2d_t offset_2d_t::northwest{ offset_2d_t::north + offset_2d_t::west };
	constexpr const offset_2d_t offset_2d_t::northeast{ offset_2d_t::north + offset_2d_t::east };

	constexpr const offset_2d_t offset_2d_t::southwest{ offset_2d_t::south + offset_2d_t::west };
	constexpr const offset_2d_t offset_2d_t::southeast{ offset_2d_t::south + offset_2d_t::east };

	constexpr offset_2d_t offset_2d_t::clamp(offset_2d_t value, cref<extent_2d_t> min, cref<extent_2d_t> max) { return value.clamp(min, max); }

	constexpr ref<offset_2d_t> offset_2d_t::clamp(cref<extent_2d_t> min, cref<extent_2d_t> max) {
		x = x < min.w ? min.w : x > max.w ? max.w : x;
		y = y < min.h ? min.h : y > max.h ? max.h : y;

		return *this;
	}

	constexpr offset_2d_t offset_2d_t::operator+(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x + extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator-(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x - extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator*(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x * extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator/(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x / extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator%(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x % extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator+(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h) };
	}

	constexpr offset_2d_t offset_2d_t::operator-(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x - extent.w), scalar_cast(y - extent.h) };
	}

	constexpr offset_2d_t offset_2d_t::operator*(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x * extent.w), scalar_cast(y * extent.h) };
	}

	constexpr offset_2d_t offset_2d_t::operator/(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x / extent.w), scalar_cast(y / extent.h) };
	}

	constexpr offset_2d_t offset_2d_t::operator%(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x % extent.w), scalar_cast(y % extent.h) };
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator+=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x + extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator-=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x - extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator*=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x * extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator/=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x / extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator%=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x % extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator+=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x + extent.w);
		y = scalar_cast(y + extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator-=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x - extent.w);
		y = scalar_cast(y - extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator*=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x * extent.w);
		y = scalar_cast(y * extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator/=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x / extent.w);
		y = scalar_cast(y / extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator%=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x % extent.w);
		y = scalar_cast(y % extent.h);

		return *this;
	}

	constexpr offset_2d_t::operator offset_1d_t() const noexcept { return offset_1d_t{ static_cast<offset_1d_t::scalar_t>(x) }; }

	constexpr offset_2d_t::operator offset_3d_t() const noexcept {
		return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(x), static_cast<offset_3d_t::scalar_t>(y), offset_3d_t::scalar_t{ 0 } };
	}

	constexpr offset_2d_t::operator extent_2d_t() const noexcept { return extent_2d_t{ static_cast<extent_2d_t::scalar_t>(x), static_cast<extent_2d_t::scalar_t>(y) }; }

	constexpr offset_2d_t::operator extent_3d_t() const noexcept {
		return extent_3d_t{ static_cast<extent_3d_t::scalar_t>(x), static_cast<extent_3d_t::scalar_t>(y), extent_3d_t::scalar_t{ 0 } };
	}

	constexpr offset_3d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(x), static_cast<offset_2d_t::scalar_t>(y) }; }

	constexpr offset_3d_t::operator extent_2d_t() const noexcept { return extent_2d_t{ static_cast<extent_2d_t::scalar_t>(x), static_cast<extent_2d_t::scalar_t>(y) }; }

	constexpr offset_3d_t::operator extent_3d_t() const noexcept {
		return extent_3d_t{ static_cast<extent_3d_t::scalar_t>(x), static_cast<extent_3d_t::scalar_t>(y), static_cast<extent_3d_t::scalar_t>(z) };
	}

	constexpr const offset_3d_t offset_3d_t::zero{ 0 };

	constexpr const offset_3d_t offset_3d_t::central{ offset_3d_t::zero };

	constexpr const offset_3d_t offset_3d_t::north{ 0, -1, 0 };
	constexpr const offset_3d_t offset_3d_t::south{ 0, 1, 0 };

	constexpr const offset_3d_t offset_3d_t::east{ 1, 0, 0 };
	constexpr const offset_3d_t offset_3d_t::west{ -1, 0, 0 };

	constexpr const offset_3d_t offset_3d_t::up{ 0, 0, 1 };
	constexpr const offset_3d_t offset_3d_t::down{ 0, 0, -1 };

	constexpr const offset_3d_t offset_3d_t::northwest{ offset_3d_t::north + offset_3d_t::west };
	constexpr const offset_3d_t offset_3d_t::northeast{ offset_3d_t::north + offset_3d_t::east };

	constexpr const offset_3d_t offset_3d_t::southwest{ offset_3d_t::south + offset_3d_t::west };
	constexpr const offset_3d_t offset_3d_t::southeast{ offset_3d_t::south + offset_3d_t::east };

	constexpr const offset_3d_t offset_3d_t::vertical::up::central{ offset_3d_t::central + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::up::north{ offset_3d_t::north + offset_3d_t::up };
	constexpr const offset_3d_t offset_3d_t::vertical::up::south{ offset_3d_t::south + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::up::west{ offset_3d_t::west + offset_3d_t::up };
	constexpr const offset_3d_t offset_3d_t::vertical::up::east{ offset_3d_t::east + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::up::northwest{ offset_3d_t::northwest + offset_3d_t::up };
	constexpr const offset_3d_t offset_3d_t::vertical::up::Northeast{ offset_3d_t::northeast + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::up::southwest{ offset_3d_t::southwest + offset_3d_t::up };
	constexpr const offset_3d_t offset_3d_t::vertical::up::southeast{ offset_3d_t::southeast + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::down::central{ offset_3d_t::central + offset_3d_t::down };

	constexpr const offset_3d_t offset_3d_t::vertical::down::north{ offset_3d_t::north + offset_3d_t::down };
	constexpr const offset_3d_t offset_3d_t::vertical::down::south{ offset_3d_t::south + offset_3d_t::down };

	constexpr const offset_3d_t offset_3d_t::vertical::down::west{ offset_3d_t::west + offset_3d_t::down };
	constexpr const offset_3d_t offset_3d_t::vertical::down::east{ offset_3d_t::east + offset_3d_t::down };

	constexpr const offset_3d_t offset_3d_t::vertical::down::northwest{ offset_3d_t::northwest + offset_3d_t::down };
	constexpr const offset_3d_t offset_3d_t::vertical::down::northeast{ offset_3d_t::northeast + offset_3d_t::down };

	constexpr const offset_3d_t offset_3d_t::vertical::down::southwest{ offset_3d_t::southwest + offset_3d_t::down };
	constexpr const offset_3d_t offset_3d_t::vertical::down::southeast{ offset_3d_t::southeast + offset_3d_t::down };

	constexpr ref<offset_3d_t> offset_3d_t::clamp(cref<extent_3d_t> min, cref<extent_3d_t> max) {
		x = x < min.w ? min.w : x > max.w ? max.w : x;
		y = y < min.h ? min.h : y > max.h ? max.h : y;
		z = z < min.d ? min.d : z > max.d ? max.d : z;

		return *this;
	}

	constexpr offset_3d_t offset_3d_t::clamp(offset_3d_t value, cref<extent_3d_t> min, cref<extent_3d_t> max) { return value.clamp(min, max); }

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator+=(cref<extent_1d_t> extent) noexcept {
		x += extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator-=(cref<extent_1d_t> extent) noexcept {
		x -= extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator*=(cref<extent_1d_t> extent) noexcept {
		x *= extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator/=(cref<extent_1d_t> extent) noexcept {
		x /= extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator%=(cref<extent_1d_t> extent) noexcept {
		x %= extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator+=(cref<extent_2d_t> extent) noexcept {
		x += extent.w;
		y += extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator-=(cref<extent_2d_t> extent) noexcept {
		x -= extent.w;
		y -= extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator*=(cref<extent_2d_t> extent) noexcept {
		x *= extent.w;
		y *= extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator/=(cref<extent_2d_t> extent) noexcept {
		x /= extent.w;
		y /= extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator%=(cref<extent_2d_t> extent) noexcept {
		x %= extent.w;
		y %= extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator+=(cref<extent_3d_t> extent) noexcept {
		x += extent.w;
		y += extent.h;
		z += extent.d;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator-=(cref<extent_3d_t> extent) noexcept {
		x -= extent.w;
		y -= extent.h;
		z -= extent.d;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator*=(cref<extent_3d_t> extent) noexcept {
		x *= extent.w;
		y *= extent.h;
		z *= extent.d;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator/=(cref<extent_3d_t> extent) noexcept {
		x /= extent.w;
		y /= extent.h;
		z /= extent.d;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator%=(cref<extent_3d_t> extent) noexcept {
		x %= extent.w;
		y %= extent.h;
		z %= extent.d;

		return *this;
	}
} // namespace bleak
