#pragma once

#include "typedef.hpp"

#include <utility>
#include <string>

#include <format>

struct Color
{
	u8 r, g, b, a;

	constexpr Color() noexcept :
		r{ },
		g{ },
		b{ },
		a{ }
	{ }

	constexpr Color(u8 r, u8 g, u8 b, u8 a = u8{ 0xFF }) noexcept :
		r{ r },
		g{ g },
		b{ b },
		a{ a }
	{ }

	constexpr Color(f32 r, f32 g, f32 b, f32 a = 1.0f) noexcept :
		r{ static_cast<u8>(r * 255.0f) },
		g{ static_cast<u8>(g * 255.0f) },
		b{ static_cast<u8>(b * 255.0f) },
		a{ static_cast<u8>(a * 255.0f) }
	{ }
	
	constexpr Color(cref<Color> other) noexcept :
		r{ other.r },
		g{ other.g },
		b{ other.b },
		a{ other.a }
	{ }
	constexpr Color(rval<Color> other) noexcept :
		r{ std::move(other.r) },
		g{ std::move(other.g) },
		b{ std::move(other.b) },
		a{ std::move(other.a) }
	{ }

	constexpr Color(cref<Color> other, u8 a) noexcept :
		r{ other.r },
		g{ other.g },
		b{ other.b },
		a{ a }
	{ }
	
	constexpr Color(rval<Color> other, u8 a) noexcept :
		r{ std::move(other.r) },
		g{ std::move(other.g) },
		b{ std::move(other.b) },
		a{ a }
	{ }

	constexpr ref<Color> operator=(cref<Color> other) noexcept
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;

		return *this;
	}

	constexpr ref<Color> operator=(rval<Color> other) noexcept
	{
		r = std::move(other.r);
		g = std::move(other.g);
		b = std::move(other.b);
		a = std::move(other.a);

		return *this;
	}

	constexpr u32 packed() const noexcept { return *(ptr<u32>)&r; }

	constexpr bool operator==(cref<Color> other) const noexcept { return packed() == other.packed(); }
	constexpr bool operator!=(cref<Color> other) const noexcept { return packed() != other.packed(); }

	constexpr bool operator<(cref<Color> other) const noexcept = delete;
	constexpr bool operator<=(cref<Color> other) const noexcept = delete;

	constexpr bool operator>(cref<Color> other) const noexcept = delete;
	constexpr bool operator>=(cref<Color> other) const noexcept = delete;

	constexpr explicit operator u32() const noexcept { return packed(); }

	inline operator std::string() const noexcept { return std::format("[{}, {}, {}, {}]", r, g, b, a); }
};
