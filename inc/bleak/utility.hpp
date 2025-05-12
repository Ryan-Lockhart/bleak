#pragma once

#include <bleak/typedef.hpp>

#include <array>
#include <cassert>
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

	template<typename T, typename U> constexpr inline U usage(T value, T min, T max) { return (value - min) / (max - min); }

	template<Numeric N> constexpr inline bool between(N value, N min, N max) { return value >= min && value <= max; }

	template<Numeric N> constexpr inline N map(N value, N from_min, N from_max, N to_min, N to_max) {
		return clamp<N>(static_cast<N>(to_min + (1.0 * (to_max - to_min) / (from_max - from_min)) * (value - from_min)), to_min, to_max);
	}

	template<typename T> ptr<T> cast_away(cptr<T> pointer) { return const_cast<ptr<T>>(pointer); }

	template<typename T> ref<T> cast_away(cref<T> reference) { return const_cast<ref<T>>(reference); }

	template<usize Length, usize Start, usize End>
		requires(End - Start + 1 == Length)
	constexpr static std::array<usize, End - Start + 1> generate_contiguous_indices() noexcept {
		std::array<usize, End - Start + 1> indices{};

		for (usize i{ 0 }; i < Length; ++i) {
			indices[i] = Start + i;
		}

		return indices;
	}

	struct range_t {
		usize first;
		usize last;

		constexpr range_t(usize start, usize end) noexcept : first{ start }, last{ end } { assert(start <= end); }

		constexpr usize length() const noexcept { return last - first + 1; }

		constexpr usize begin() const noexcept { return first; }

		constexpr usize end() const noexcept { return last; }

		constexpr bool overlapping(cref<range_t> other) const noexcept { return first <= other.last && last >= other.first; }
	};
	
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

// forces the use of a macro to be terminated with a semicolon
#define REQUIRE_SEMICOLON() static_assert(true, "")

#define DEFINE_GENERIC_USING(type, value_type)                                                                                                                                                                                                 \
	using value_t = value_type;                                                                                                                                                                                                                \
                                                                                                                                                                                                                                               \
	using ptr_t = ptr<value_type>;                                                                                                                                                                                                             \
	using cptr_t = cptr<value_type>;                                                                                                                                                                                                           \
	using ptrc_t = ptrc<value_type>;                                                                                                                                                                                                           \
	using cptrc_t = cptrc<value_type>;                                                                                                                                                                                                         \
                                                                                                                                                                                                                                               \
	using ref_t = ref<value_type>;                                                                                                                                                                                                             \
	using cref_t = cref<value_type>;                                                                                                                                                                                                           \
	using rval_t = rval<value_type>;                                                                                                                                                                                                           \
	using crval_t = crval<value_type>;                                                                                                                                                                                                         \
                                                                                                                                                                                                                                               \
	using self_t = type<value_type>;                                                                                                                                                                                                           \
                                                                                                                                                                                                                                               \
	REQUIRE_SEMICOLON()

#define DEFINE_GENERIC_USING_VAARGS(type, value_type, ...)                                                                                                                                                                                                 \
	using value_t = value_type;                                                                                                                                                                                                                \
                                                                                                                                                                                                                                               \
	using ptr_t = ptr<value_type>;                                                                                                                                                                                                             \
	using cptr_t = cptr<value_type>;                                                                                                                                                                                                           \
	using ptrc_t = ptrc<value_type>;                                                                                                                                                                                                           \
	using cptrc_t = cptrc<value_type>;                                                                                                                                                                                                         \
                                                                                                                                                                                                                                               \
	using ref_t = ref<value_type>;                                                                                                                                                                                                             \
	using cref_t = cref<value_type>;                                                                                                                                                                                                           \
	using rval_t = rval<value_type>;                                                                                                                                                                                                           \
	using crval_t = crval<value_type>;                                                                                                                                                                                                         \
                                                                                                                                                                                                                                               \
	using self_t = type<value_type, __VA_ARGS__>;                                                                                                                                                                                                           \
                                                                                                                                                                                                                                               \
	REQUIRE_SEMICOLON()

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
                                                                                                                                                                                                                                               \
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
                                                                                                                                                                                                                                               \
	REQUIRE_SEMICOLON()

#define DEFINE_ITERATORS(qualifiers, type, value)                                                                                                                                                                                              \
	DEFINE_FWD_ITER(qualifiers, type, value);                                                                                                                                                                                                  \
	DEFINE_REV_ITER(qualifiers, type, value);                                                                                                                                                                                                  \
                                                                                                                                                                                                                                               \
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
                                                                                                                                                                                                                                               \
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
                                                                                                                                                                                                                                               \
	REQUIRE_SEMICOLON()

#define DECLARE_ITERATORS(qualifiers, type)                                                                                                                                                                                                    \
	DECLARE_FWD_ITER(qualifiers, type);                                                                                                                                                                                                        \
	DECLARE_REV_ITER(qualifiers, type);                                                                                                                                                                                                        \
                                                                                                                                                                                                                                               \
	REQUIRE_SEMICOLON()
} // namespace bleak
