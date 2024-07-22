#pragma once

#include <bleak/typedef.hpp>

#include <cmath>

#include <bleak/concepts.hpp>

namespace bleak {
	template<typename T> constexpr inline T min(T a, T b) { return a < b ? a : b; }

	template<typename T> constexpr inline T max(T a, T b) { return a > b ? a : b; }

	template<typename T> constexpr inline T clamp(T value, T min, T max) { return value < min ? min : value > max ? max : value; }

	template<typename T> constexpr inline T wrap(T value, T min, T max) {
		if (value < min) {
			return max - (min - value) + 1;
		}
		if (value > max) {
			return min + (value - max) - 1;
		}

		return value;
	}

	template<Integer T> constexpr inline T wrap(T value, T target) { return (value % target + target) % target; }

	template<FloatingPoint T> constexpr inline T wrap(T value, T target) {
		while (value < 0) {
			value += target;
		}
		while (value >= target) {
			value -= target;
		}

		return value;
	}

	template<typename T> constexpr inline T lerp(T a, T b, T t) { return a + (b - a) * t; }

	template<typename T> constexpr inline T usage(T value, T min, T max) { return (value - min) / (max - min); }

	namespace memory {
		// size in bytes of one byte
		constexpr const usize byte{ usize{ 1 } << 0 };
		// size in bytes of one kilobyte
		constexpr const usize kilobyte{ usize{ 1 } << 10 };
		// size in bytes of one megabyte
		constexpr const usize megabyte{ usize{ 1 } << 20 };
		// size in bytes of one gigabyte
		constexpr const usize gigabyte{ usize{ 1 } << 30 };
		// size in bytes of one terabyte
		constexpr const usize terabyte{ usize{ 1 } << 40 };
		// size in bytes of one petabyte
		constexpr const usize petabyte{ usize{ 1 } << 50 };
		// size in bytes of one exabyte
		constexpr const usize exabyte{ usize{ 1 } << 60 };

		// size in bytes of one zettabyte minus one byte
		constexpr const usize limit{ usize{ 0 } - 1 };
		// size in bytes of the maximum size of an array
		constexpr const usize maximum{ gigabyte * 4 };
	}; // namespace memory
} // namespace bleak
