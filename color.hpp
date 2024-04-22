#pragma once

#include "typedef.hpp"

#include <utility>
#include <string>

struct Color
{
	union {
		struct { u8 r, g, b, a; };
		u32 packed;
		u8 data[4];
	};

	constexpr Color() noexcept : packed(0) {}
	constexpr Color(u32 packed) noexcept : packed(packed) {}
	constexpr Color(u8 r, u8 g, u8 b, u8 a = 255u) noexcept : r(r), g(g), b(b), a(a) {}
	constexpr Color(f32 r, f32 g, f32 b, f32 a = 1.0f) noexcept : r(static_cast<u8>(r * 255.0f)), g(static_cast<u8>(g * 255.0f)), b(static_cast<u8>(b * 255.0f)), a(static_cast<u8>(a * 255.0f)) {}
	
	constexpr Color(cref<Color> color) noexcept : packed(color.packed) {}
	constexpr Color(rval<Color> color) noexcept : packed(std::move(color.packed)) {}

	constexpr Color(cref<Color> color, u8 a) noexcept : packed(color.packed) { this->a = a; }
	constexpr Color(rval<Color> color, u8 a) noexcept : packed(std::move(color.packed)) { this->a = a; }

	constexpr ref<Color> operator=(cref<Color> color) noexcept
	{
		packed = color.packed;
		return *this;
	}

	constexpr ref<Color> operator=(rval<Color> color) noexcept
	{
		packed = std::move(color.packed);
		return *this;
	}

	constexpr bool operator==(cref<Color> color) const noexcept { return packed == color.packed; }
	constexpr bool operator!=(cref<Color> color) const noexcept { return packed != color.packed; }

	constexpr bool operator<(cref<Color> color) const noexcept = delete;
	constexpr bool operator<=(cref<Color> color) const noexcept = delete;

	constexpr bool operator>(cref<Color> color) const noexcept = delete;
	constexpr bool operator>=(cref<Color> color) const noexcept = delete;

	constexpr explicit operator u32() const noexcept { return packed; }

	inline operator std::string() const noexcept
	{
		return "[" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + "]";
	}
};
