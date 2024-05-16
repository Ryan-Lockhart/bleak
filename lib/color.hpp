#pragma once

#include "typedef.hpp"

#include <string>
#include <utility>

#include <format>

namespace Bleakdepth {
	struct color_t {
		u8 r, g, b, a;

		constexpr color_t() noexcept : r {}, g {}, b {}, a {} {}

		constexpr color_t(u8 r, u8 g, u8 b, u8 a = u8 { 0xFF }) noexcept : r { r }, g { g }, b { b }, a { a } {}

		constexpr color_t(f32 r, f32 g, f32 b, f32 a = 1.0f) noexcept :
			r { static_cast<u8>(r * 255.0f) },
			g { static_cast<u8>(g * 255.0f) },
			b { static_cast<u8>(b * 255.0f) },
			a { static_cast<u8>(a * 255.0f) } {}

		constexpr color_t(f64 r, f64 g, f64 b, f64 a = 1.0) noexcept :
			r { static_cast<u8>(r * 255.0) },
			g { static_cast<u8>(g * 255.0) },
			b { static_cast<u8>(b * 255.0) },
			a { static_cast<u8>(a * 255.0) } {}

		constexpr color_t(cref<color_t> other) noexcept : r { other.r }, g { other.g }, b { other.b }, a { other.a } {}

		constexpr color_t(rval<color_t> other) noexcept :
			r { std::move(other.r) },
			g { std::move(other.g) },
			b { std::move(other.b) },
			a { std::move(other.a) } {}

		constexpr color_t(cref<color_t> other, u8 a) noexcept : r { other.r }, g { other.g }, b { other.b }, a { a } {}

		constexpr color_t(rval<color_t> other, u8 a) noexcept :
			r { std::move(other.r) },
			g { std::move(other.g) },
			b { std::move(other.b) },
			a { a } {}

		constexpr color_t(cref<color_t> other, f32 a) noexcept :
			r { other.r },
			g { other.g },
			b { other.b },
			a { static_cast<u8>(a * 255.0f) } {}

		constexpr color_t(rval<color_t> other, f32 a) noexcept :
			r { std::move(other.r) },
			g { std::move(other.g) },
			b { std::move(other.b) },
			a { static_cast<u8>(a * 255.0f) } {}

		constexpr color_t(cref<color_t> other, f64 a) noexcept :
			r { other.r },
			g { other.g },
			b { other.b },
			a { static_cast<u8>(a * 255.0) } {}

		constexpr color_t(rval<color_t> other, f64 a) noexcept :
			r { std::move(other.r) },
			g { std::move(other.g) },
			b { std::move(other.b) },
			a { static_cast<u8>(a * 255.0) } {}

		constexpr ref<color_t> operator=(cref<color_t> other) noexcept {
			r = other.r;
			g = other.g;
			b = other.b;
			a = other.a;

			return *this;
		}

		constexpr ref<color_t> operator=(rval<color_t> other) noexcept {
			r = std::move(other.r);
			g = std::move(other.g);
			b = std::move(other.b);
			a = std::move(other.a);

			return *this;
		}

		constexpr u32 packed() const noexcept { return u32 { *(ptr<u32>)&r }; }

		constexpr bool operator==(cref<color_t> other) const noexcept { return packed() == other.packed(); }

		constexpr bool operator!=(cref<color_t> other) const noexcept { return packed() != other.packed(); }

		constexpr bool operator<(cref<color_t> other) const noexcept = delete;
		constexpr bool operator<=(cref<color_t> other) const noexcept = delete;

		constexpr bool operator>(cref<color_t> other) const noexcept = delete;
		constexpr bool operator>=(cref<color_t> other) const noexcept = delete;

		constexpr void setRGB(f32 red, f32 green, f32 blue) noexcept {
			r = static_cast<u8>(red * 255.0f);
			g = static_cast<u8>(green * 255.0f);
			b = static_cast<u8>(blue * 255.0f);
		}

		constexpr void setRGB(f64 red, f64 green, f64 blue) noexcept {
			r = static_cast<u8>(red * 255.0);
			g = static_cast<u8>(green * 255.0);
			b = static_cast<u8>(blue * 255.0);
		}

		constexpr void setAlpha(f32 alpha) noexcept { a = static_cast<u8>(alpha * 255.0f); }

		constexpr void setAlpha(f64 alpha) noexcept { a = static_cast<u8>(alpha * 255.0); }

		constexpr explicit operator u32() const noexcept { return packed(); }

		inline operator std::string() const noexcept { return std::format("[{}, {}, {}, {}]", r, g, b, a); }
	};
} // namespace Bleakdepth
