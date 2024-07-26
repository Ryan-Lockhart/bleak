#pragma once

#include <bleak/primitive.hpp> // IWYU pragma: export

#include <cmath>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

#include <SDL.h>

#include <bleak/bitdef.hpp>
#include <bleak/cardinal.hpp>
#include <bleak/concepts.hpp>
#include <bleak/hash.hpp>

extern "C" {
	typedef struct c_offset_t {
#if BLEAKDEPTH_BIG_GRID
		typedef bleak::ihalf scalar_t;
		typedef bleak::isize product_t;
		typedef bleak::fsize float_t;
#else
		typedef bleak::iqrtr scalar_t;
		typedef bleak::ihalf product_t;
		typedef bleak::fhalf float_t;
#endif

		scalar_t x{ 0 };
		scalar_t y{ 0 };

		static_assert(
			(product_t)std::numeric_limits<scalar_t>::max() *
			(product_t)std::numeric_limits<scalar_t>::max() <=
			std::numeric_limits<product_t>::max(), "product_t is too small for scalar_t"
		);
	} c_offset_t;
}

namespace bleak {
	struct offset_t : c_offset_t {
		using underlying_t = c_offset_t;

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

		static const offset_t Zero;

		static const offset_t Central;

		static const offset_t North;
		static const offset_t South;

		static const offset_t East;
		static const offset_t West;

		static const offset_t Northwest;
		static const offset_t Northeast;

		static const offset_t Southwest;
		static const offset_t Southeast;

		constexpr explicit offset_t() noexcept {}

		constexpr explicit offset_t(scalar_t scalar) noexcept : underlying_t{ scalar, scalar } {}

		constexpr offset_t(scalar_t x, scalar_t y) noexcept : underlying_t{ x, y } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && is_numeric<T>::value
		constexpr explicit offset_t(T scalar) noexcept : underlying_t{ scalar_cast(scalar), scalar_cast(scalar) } {}

		template<typename T>
			requires std::is_convertible<T, scalar_t>::value && is_numeric<T>::value
		constexpr explicit offset_t(T x, T y) noexcept : underlying_t{ scalar_cast(x), scalar_cast(y) } {}

		template<typename X, typename Y>
			requires std::is_convertible<X, scalar_t>::value && std::is_convertible<Y, scalar_t>::value && is_numeric<X>::value && is_numeric<Y>::value
		constexpr explicit offset_t(X x, Y y) noexcept : underlying_t{ scalar_cast(x), scalar_cast(y) } {}

		constexpr explicit offset_t(cref<cardinal_t> direction) noexcept : underlying_t{} {
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

		template<typename T = product_t> static constexpr T distance(cref<offset_t> start, cref<offset_t> end) noexcept;

		template<> constexpr f32 distance(cref<offset_t> start, cref<offset_t> end) noexcept { return (end - start).length<f32>(); }

		template<> constexpr f64 distance(cref<offset_t> start, cref<offset_t> end) noexcept { return (end - start).length<f64>(); }

		template<> constexpr product_t distance(cref<offset_t> start, cref<offset_t> end) noexcept { return (end - start).length<product_t>(); }

		static constexpr cardinal_t direction(cref<offset_t> start, cref<offset_t> end) noexcept { return static_cast<cardinal_t>(end - start); }

		template<typename T = product_t> constexpr ref<offset_t> normalize() noexcept {
			const auto len = length<T>();

			if (len != 0) {
				x = scalar_cast(x / len);
				y = scalar_cast(y / len);
			}

			return *this;
		}

		template<typename T = product_t> constexpr offset_t normalized() const noexcept {
			const auto len = length<T>();

			if (len != 0) {
				return offset_t{ scalar_cast(x / len), scalar_cast(y / len) };
			} else {
				return offset_t{ *this };
			}
		}

		constexpr ref<offset_t> clamp(cref<offset_t> min, cref<offset_t> max) {
			x = x < min.x ? min.x : x > max.x ? max.x : x;
			y = y < min.y ? min.y : y > max.y ? max.y : y;

			return *this;
		}

		static constexpr offset_t clamp(offset_t value, cref<offset_t> min, cref<offset_t> max) { return value.clamp(min, max); }

		constexpr ref<offset_t> clamp(cref<extent_t> min, cref<extent_t> max);

		static constexpr offset_t clamp(offset_t value, cref<extent_t> min, cref<extent_t> max);

		constexpr bool operator==(cref<offset_t> other) const noexcept { return x == other.x && y == other.y; }

		constexpr bool operator!=(cref<offset_t> other) const noexcept { return x != other.x || y != other.y; }

		constexpr bool operator<(cref<offset_t> other) const { return x < other.x && y < other.y; }

		constexpr bool operator>(cref<offset_t> other) const { return x > other.x && y > other.y; }

		constexpr bool operator<=(cref<offset_t> other) const { return x <= other.x && y <= other.y; }

		constexpr bool operator>=(cref<offset_t> other) const { return x >= other.x && y >= other.y; }

		constexpr offset_t operator-() const noexcept { return offset_t{ scalar_cast(-x), scalar_cast(-y) }; }

		constexpr offset_t operator+(cref<offset_t> other) const noexcept { return offset_t{ scalar_cast(x + other.x), scalar_cast(y + other.y) }; }

		constexpr offset_t operator-(cref<offset_t> other) const noexcept { return offset_t{ scalar_cast(x - other.x), scalar_cast(y - other.y) }; }

		constexpr offset_t operator*(cref<offset_t> other) const noexcept { return offset_t{ scalar_cast(x * other.x), scalar_cast(y * other.y) }; }

		constexpr offset_t operator/(cref<offset_t> other) const noexcept { return offset_t{ scalar_cast(x / other.x), scalar_cast(y / other.y) }; }

		constexpr offset_t operator%(cref<offset_t> other) const noexcept { return offset_t{ scalar_cast(x % other.x), scalar_cast(y % other.y) }; }

		constexpr offset_t operator+(scalar_t scalar) const noexcept { return offset_t{ scalar_cast(x + scalar), scalar_cast(y + scalar) }; }

		constexpr offset_t operator-(scalar_t scalar) const noexcept { return offset_t{ scalar_cast(x - scalar), scalar_cast(y - scalar) }; }

		constexpr offset_t operator*(scalar_t scalar) const noexcept { return offset_t{ scalar_cast(x * scalar), scalar_cast(y * scalar) }; }

		constexpr offset_t operator/(scalar_t scalar) const noexcept { return offset_t{ scalar_cast(x / scalar), scalar_cast(y / scalar) }; }

		constexpr offset_t operator%(scalar_t scalar) const noexcept { return offset_t{ scalar_cast(x % scalar), scalar_cast(y % scalar) }; }

		constexpr offset_t operator+(cref<extent_t> extent) const noexcept;

		constexpr offset_t operator-(cref<extent_t> extent) const noexcept;

		constexpr offset_t operator*(cref<extent_t> extent) const noexcept;

		constexpr offset_t operator/(cref<extent_t> extent) const noexcept;

		constexpr offset_t operator%(cref<extent_t> extent) const noexcept;

		constexpr ref<offset_t> operator+=(cref<offset_t> other) noexcept {
			x += other.x;
			y += other.y;

			return *this;
		}

		constexpr ref<offset_t> operator-=(cref<offset_t> other) noexcept {
			x -= other.x;
			y -= other.y;

			return *this;
		}

		constexpr ref<offset_t> operator*=(cref<offset_t> other) noexcept {
			x *= other.x;
			y *= other.y;

			return *this;
		}

		constexpr ref<offset_t> operator/=(cref<offset_t> other) noexcept {
			x /= other.x;
			y /= other.y;

			return *this;
		}

		constexpr ref<offset_t> operator%=(cref<offset_t> other) noexcept {
			x %= other.x;
			y %= other.y;

			return *this;
		}

		constexpr ref<offset_t> operator+=(scalar_t scalar) noexcept {
			x += scalar;
			y += scalar;

			return *this;
		}

		constexpr ref<offset_t> operator-=(scalar_t scalar) noexcept {
			x -= scalar;
			y -= scalar;

			return *this;
		}

		constexpr ref<offset_t> operator*=(scalar_t scalar) noexcept {
			x *= scalar;
			y *= scalar;

			return *this;
		}

		constexpr ref<offset_t> operator/=(scalar_t scalar) noexcept {
			x /= scalar;
			y /= scalar;

			return *this;
		}

		constexpr ref<offset_t> operator%=(scalar_t scalar) noexcept {
			x %= scalar;
			y %= scalar;

			return *this;
		}

		constexpr ref<offset_t> operator+=(cref<extent_t> extent) noexcept;

		constexpr ref<offset_t> operator-=(cref<extent_t> extent) noexcept;

		constexpr ref<offset_t> operator*=(cref<extent_t> extent) noexcept;

		constexpr ref<offset_t> operator/=(cref<extent_t> extent) noexcept;

		constexpr ref<offset_t> operator%=(cref<extent_t> extent) noexcept;

		constexpr operator std::string() const noexcept { return std::format("[{}, {}]", x, y); }

		constexpr operator cardinal_t() const noexcept {
			if (*this == offset_t::Zero) {
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

		constexpr explicit operator extent_t() const noexcept;

		struct hasher {
			static constexpr usize operator()(cref<offset_t> offset) noexcept { return hash_combine(offset.x, offset.y); }
		};
	};
} // namespace bleak

#include <bleak/extent.hpp> // IWYU pragma: export

namespace bleak {
	constexpr const offset_t offset_t::Zero{ 0, 0 };

	constexpr const offset_t offset_t::Central{ offset_t::Zero };

	constexpr const offset_t offset_t::North{ 0, -1 };
	constexpr const offset_t offset_t::South{ 0, 1 };

	constexpr const offset_t offset_t::East{ 1, 0 };
	constexpr const offset_t offset_t::West{ -1, 0 };

	constexpr const offset_t offset_t::Northwest{ offset_t::North + offset_t::West };
	constexpr const offset_t offset_t::Northeast{ offset_t::North + offset_t::East };

	constexpr const offset_t offset_t::Southwest{ offset_t::South + offset_t::West };
	constexpr const offset_t offset_t::Southeast{ offset_t::South + offset_t::East };

	constexpr offset_t offset_t::clamp(offset_t value, cref<extent_t> min, cref<extent_t> max) { return value.clamp(min, max); }

	constexpr ref<offset_t> offset_t::clamp(cref<extent_t> min, cref<extent_t> max) {
		x = x < min.w ? min.w : x > max.w ? max.w : x;
		y = y < min.h ? min.h : y > max.h ? max.h : y;

		return *this;
	}

	constexpr offset_t offset_t::operator+(cref<extent_t> extent) const noexcept { return offset_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h) }; }

	constexpr offset_t offset_t::operator-(cref<extent_t> extent) const noexcept { return offset_t{ scalar_cast(x - extent.w), scalar_cast(y - extent.h) }; }

	constexpr offset_t offset_t::operator*(cref<extent_t> extent) const noexcept { return offset_t{ scalar_cast(x * extent.w), scalar_cast(y * extent.h) }; }

	constexpr offset_t offset_t::operator/(cref<extent_t> extent) const noexcept { return offset_t{ scalar_cast(x / extent.w), scalar_cast(y / extent.h) }; }

	constexpr offset_t offset_t::operator%(cref<extent_t> extent) const noexcept { return offset_t{ scalar_cast(x % extent.w), scalar_cast(y % extent.h) }; }

	constexpr ref<offset_t> offset_t::operator+=(cref<extent_t> extent) noexcept {
		x = scalar_cast(x + extent.w);
		y = scalar_cast(y + extent.h);

		return *this;
	}

	constexpr ref<offset_t> offset_t::operator-=(cref<extent_t> extent) noexcept {
		x = scalar_cast(x - extent.w);
		y = scalar_cast(y - extent.h);

		return *this;
	}

	constexpr ref<offset_t> offset_t::operator*=(cref<extent_t> extent) noexcept {
		x = scalar_cast(x * extent.w);
		y = scalar_cast(y * extent.h);

		return *this;
	}

	constexpr ref<offset_t> offset_t::operator/=(cref<extent_t> extent) noexcept {
		x = scalar_cast(x / extent.w);
		y = scalar_cast(y / extent.h);

		return *this;
	}

	constexpr ref<offset_t> offset_t::operator%=(cref<extent_t> extent) noexcept {
		x = scalar_cast(x % extent.w);
		y = scalar_cast(y % extent.h);

		return *this;
	}

	constexpr offset_t::operator extent_t() const noexcept { return extent_t{ extent_t::scalar_cast(x), extent_t::scalar_cast(y) }; }
} // namespace bleak
