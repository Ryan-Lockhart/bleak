#pragma once

#include <bleak/typedef.hpp>

#include <cmath>

#include <bleak/concepts.hpp>
#include <bleak/iter.hpp>

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
		constexpr const usize Byte{ usize{ 1 } << 0 };
		// size in bytes of one kilobyte
		constexpr const usize Kilobyte{ usize{ 1 } << 10 };
		// size in bytes of one megabyte
		constexpr const usize Megabyte{ usize{ 1 } << 20 };
		// size in bytes of one gigabyte
		constexpr const usize Gigabyte{ usize{ 1 } << 30 };
		// size in bytes of one terabyte
		constexpr const usize Terabyte{ usize{ 1 } << 40 };
		// size in bytes of one petabyte
		constexpr const usize Petabyte{ usize{ 1 } << 50 };
		// size in bytes of one exabyte
		constexpr const usize Exabyte{ usize{ 1 } << 60 };

		// size in bytes of one zettabyte minus one byte
		constexpr const usize Limit{ usize{ 0 } - 1 };
		// size in bytes of the maximum size of an array
		constexpr const usize Maximum{ Gigabyte * 4 };
	}; // namespace memory

#define REQUIRE_SEMICOLON() static_assert(true, "")

#define DEFINE_FWD_ITER(qualifiers, type, value)                                                                                                                                                                                               \
	using iterator = type::iterator;                                                                                                                                                                                                           \
	using const_iterator = type::const_iterator;                                                                                                                                                                                               \
                                                                                                                                                                                                                                               \
	inline qualifiers iterator begin() noexcept { return value.begin(); }                                                                                                                                                                      \
	inline qualifiers iterator end() noexcept { return value.end(); }                                                                                                                                                                          \
                                                                                                                                                                                                                                               \
	inline qualifiers const_iterator begin() const noexcept { return value.begin(); }                                                                                                                                                          \
	inline qualifiers const_iterator end() const noexcept { return value.end(); }                                                                                                                                                              \
                                                                                                                                                                                                                                               \
	inline qualifiers const_iterator cbegin() const noexcept { return value.cbegin(); }                                                                                                                                                        \
	inline qualifiers const_iterator cend() const noexcept { return value.cend(); }                                                                                                                                                            \
	REQUIRE_SEMICOLON()

#define DEFINE_REV_ITER(qualifiers, type, value)                                                                                                                                                                                               \
	using reverse_iterator = type::reverse_iterator;                                                                                                                                                                                           \
	using const_reverse_iterator = type::const_reverse_iterator;                                                                                                                                                                               \
                                                                                                                                                                                                                                               \
	inline qualifiers reverse_iterator rbegin() noexcept { return value.rbegin(); }                                                                                                                                                            \
	inline qualifiers reverse_iterator rend() noexcept { return value.rend(); }                                                                                                                                                                \
                                                                                                                                                                                                                                               \
	inline qualifiers const_reverse_iterator rbegin() const noexcept { return value.rbegin(); }                                                                                                                                                \
	inline qualifiers const_reverse_iterator rend() const noexcept { return value.rend(); }                                                                                                                                                    \
                                                                                                                                                                                                                                               \
	inline qualifiers const_reverse_iterator crbegin() const noexcept { return value.crbegin(); }                                                                                                                                              \
	inline qualifiers const_reverse_iterator crend() const noexcept { return value.crend(); }                                                                                                                                                  \
	REQUIRE_SEMICOLON()

#define DEFINE_ITERATORS(qualifiers, type, value)                                                                                                                                                                                              \
	DEFINE_FWD_ITER(qualifiers, type, value);                                                                                                                                                                                                  \
	DEFINE_REV_ITER(qualifiers, type, value);                                                                                                                                                                                                  \
	REQUIRE_SEMICOLON()

#define DECLARE_FWD_ITER(qualifiers, type)                                                                                                                                                                                                     \
	using iterator = fwd_iter_t<T>;                                                                                                                                                                                                            \
	using const_iterator = fwd_iter_t<const T>;                                                                                                                                                                                                \
                                                                                                                                                                                                                                               \
	qualifiers iterator begin() noexcept;                                                                                                                                                                                                      \
	qualifiers iterator end() noexcept;                                                                                                                                                                                                        \
                                                                                                                                                                                                                                               \
	qualifiers const_iterator begin() const noexcept;                                                                                                                                                                                          \
	qualifiers const_iterator end() const noexcept;                                                                                                                                                                                            \
                                                                                                                                                                                                                                               \
	qualifiers const_iterator cbegin() const noexcept;                                                                                                                                                                                         \
	qualifiers const_iterator cend() const noexcept;                                                                                                                                                                                           \
	REQUIRE_SEMICOLON()

#define DECLARE_REV_ITER(qualifiers, type)                                                                                                                                                                                                     \
	using reverse_iterator = rev_iter_t<T>;                                                                                                                                                                                                    \
	using const_reverse_iterator = rev_iter_t<const T>;                                                                                                                                                                                        \
                                                                                                                                                                                                                                               \
	qualifiers reverse_iterator rbegin() noexcept;                                                                                                                                                                                             \
	qualifiers reverse_iterator rend() noexcept;                                                                                                                                                                                               \
                                                                                                                                                                                                                                               \
	qualifiers const_reverse_iterator rbegin() const noexcept;                                                                                                                                                                                 \
	qualifiers const_reverse_iterator rend() const noexcept;                                                                                                                                                                                   \
                                                                                                                                                                                                                                               \
	qualifiers const_reverse_iterator crbegin() const noexcept;                                                                                                                                                                                \
	qualifiers const_reverse_iterator crend() const noexcept;                                                                                                                                                                                  \
	REQUIRE_SEMICOLON()

#define DECLARE_ITERATORS(qualifiers, type)                                                                                                                                                                                                    \
	DECLARE_FWD_ITER(qualifiers, type);                                                                                                                                                                                                        \
	DECLARE_REV_ITER(qualifiers, type);                                                                                                                                                                                                        \
	REQUIRE_SEMICOLON()
} // namespace bleak
