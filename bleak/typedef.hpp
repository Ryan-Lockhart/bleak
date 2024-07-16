#pragma once

#include <concepts>
#include <functional>
#include <limits>
#include <type_traits>

namespace bleak {
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(_M_ARM64)
#define BLEAKDEPTH_64
#elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || defined(_M_ARM)
#define BLEAKDEPTH_32
#endif

#define __TIME_FILE_LINE__ __TIME__, __FILE__, __LINE__

#if defined(BLEAKDEPTH_RELEASE)
#define BLEAKDEPTH_BIG_GRID 1
#elif defined(BLEAKDEPTH_DEBUG)
#define BLEAKDEPTH_BIG_GRID 0
#endif

#define forever for (;;)

	using u8 = unsigned char;
	using i8 = signed char;

#if defined(HAS_MINIFLOAT)
	using f8 = __minifloat;
#endif

	using u16 = unsigned short;
	using i16 = signed short;
	using f16 = _Float16;

	using u32 = unsigned int;
	using i32 = signed int;
	using f32 = float;

	using u64 = unsigned long long;
	using i64 = signed long long;
	using f64 = double;

	using f80 = long double;

#if defined(BLEAKDEPTH_64)
	using u128 = unsigned __int128;
	using i128 = signed __int128;
	using f128 = __float128;
#endif

	// following assumes IEEE 754 floating point standard for all floating point types
	// any platform that does not conform to this standard will not work with this library (until I add support for it :P)

	namespace bitdef {
		template<typename T> static constexpr auto exponent();
		template<typename T> static constexpr auto mantissa();
		template<typename T> static constexpr auto bits();
		template<typename T> static constexpr auto bytes();

		constexpr auto sign_bit = 1;

#if defined(HAS_MINIFLOAT)
		namespace f8 {
			constexpr auto exponent = 4;
			constexpr auto mantissa = 3;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(::f8) == bytes, "f8 is not 8 bits!");
		} // namespace f8
#endif

		namespace f16 {
			constexpr auto exponent = 5;
			constexpr auto mantissa = 10;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(bleak::f16) == bytes, "f16 is not 16 bits!");
		} // namespace f16

		namespace f32 {
			constexpr auto exponent = 8;
			constexpr auto mantissa = 23;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(bleak::f32) == bytes, "f32 is not 32 bits!");
		} // namespace f32

		namespace f64 {
			constexpr auto exponent = 11;
			constexpr auto mantissa = 52;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(bleak::f64) == bytes, "f64 is not 64 bits!");
		} // namespace f64

#if defined(BLEAKDEPTH_64)
		namespace f128 {
			constexpr auto exponent = 15;
			constexpr auto mantissa = 112;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(bleak::f128) == bytes, "f128 is not 128 bits!");
		} // namespace f128
#endif

#if defined(HAS_MINIFLOAT)
		template<> constexpr auto exponent<bleak::f8>() { return f8::exponent; }

		template<> constexpr auto mantissa<bleak::f8>() { return f8::mantissa; }

		template<> constexpr auto bits<bleak::f8>() { return f8::bits; }

		template<> constexpr auto bytes<bleak::f8>() { return f8::bytes; }
#endif

		template<> constexpr auto exponent<bleak::f16>() { return f16::exponent; }

		template<> constexpr auto mantissa<bleak::f16>() { return f16::mantissa; }

		template<> constexpr auto bits<bleak::f16>() { return f16::bits; }

		template<> constexpr auto bytes<bleak::f16>() { return f16::bytes; }

		template<> constexpr auto exponent<bleak::f32>() { return f32::exponent; }

		template<> constexpr auto mantissa<bleak::f32>() { return f32::mantissa; }

		template<> constexpr auto bits<bleak::f32>() { return f32::bits; }

		template<> constexpr auto bytes<bleak::f32>() { return f32::bytes; }

		template<> constexpr auto exponent<bleak::f64>() { return f64::exponent; }

		template<> constexpr auto mantissa<bleak::f64>() { return f64::mantissa; }

		template<> constexpr auto bits<bleak::f64>() { return f64::bits; }

		template<> constexpr auto bytes<bleak::f64>() { return f64::bytes; }

#if defined(BLEAKDEPTH_64)
		template<> constexpr auto exponent<bleak::f128>() { return f128::exponent; }

		template<> constexpr auto mantissa<bleak::f128>() { return f128::mantissa; }

		template<> constexpr auto bits<bleak::f128>() { return f128::bits; }

		template<> constexpr auto bytes<bleak::f128>() { return f128::bytes; }
#endif

	} // namespace bitdef

#if defined(BLEAKDEPTH_32)

	using uquad = u64;
	using iquad = i64;
	using fquad = f64;

	using usize = u32;
	using isize = i32;
	using fsize = f32;

	using uhalf = u16;
	using ihalf = i16;
	using fhalf = f16;

	using uqrtr = u8;
	using iqrtr = i8;

#if defined(HAS_MINIFLOAT)
	using fqrtr = f8;
#endif

#else

	using uquad = u128;
	using iquad = i128;
	using fquad = f128;

	using usize = u64;
	using isize = i64;
	using fsize = f64;

	using uhalf = u32;
	using ihalf = i32;
	using fhalf = f32;

	using uqrtr = u16;
	using iqrtr = i16;

#if defined(HAS_MINIFLOAT)
	using fqrtr = f16;
#endif

#endif

	// reference to type
	template<typename T> using ref = T&;
	// immutable reference to type
	template<typename T> using cref = const T&;
	// rvalue reference to type
	template<typename T> using rval = T&&;
	// immutable rvalue reference to type
	template<typename T> using crval = const T&&;

	// pointer to type
	template<typename T> using ptr = T*;
	// pointer to immutable type
	template<typename T> using cptr = const T*;
	// immutable pointer to type
	template<typename T> using ptrc = T* const;
	// immutable pointer to immutable type
	template<typename T> using cptrc = const T* const;

	using ubyte = u8;
	using ibyte = i8;

	using mem = ptr<void>;
	using cmem = cptr<void>;

	using str = ptr<char>;
	using cstr = cptr<char>;

	using buffer = ptr<ubyte>;
	using cbuffer = cptr<ubyte>;

	template<typename Return, typename... Args> using fn_ptr = Return (*)(Args...);
	template<typename T, typename Return, typename... Args> using mfn_ptr = Return (T::*)(Args...);

	template<typename T> struct is_numeric {
		static bool constexpr value = std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value;
	};

	template<typename T> constexpr bool is_numeric_v = is_numeric<T>::value;

	template<typename T, typename U>
	concept Equatable = requires(T a, U b) {
		{ a == b } -> std::convertible_to<bool>;
		{ a != b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> struct is_equatable {
		static bool constexpr value = Equatable<T, U>;
	};

	template<typename T, typename U> constexpr bool is_equatable_v = is_equatable<T, U>::value;

	template<typename T, typename U>
	concept Comparable = requires(T a, U b) {
		{ a < b } -> std::convertible_to<bool>;
		{ a > b } -> std::convertible_to<bool>;
		{ a <= b } -> std::convertible_to<bool>;
		{ a >= b } -> std::convertible_to<bool>;
		{ a == b } -> std::convertible_to<bool>;
		{ a != b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> struct is_comparable {
		static bool constexpr value = Comparable<T, U>;
	};

	template<typename T, typename U> constexpr bool is_comparable_v = is_comparable<T, U>::value;

	template<typename T> struct is_drawable {
		static bool constexpr value = false;
	};

	template<typename T> constexpr bool is_drawable_v = is_drawable<T>::value;

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

	template<typename T> constexpr inline T usage(T value, T min, T max) { return (value - min) / (max - min); }

	template<typename T, typename... Values> void hash_combine(ref<usize> seed, cref<T> v, cref<Values>... values) {
		static constexpr auto digits = std::numeric_limits<std::size_t>::digits;
		static_assert(digits == 64 || digits == 32);

		if constexpr (digits == 64) {
			seed += 0x9e3779b9 + std::hash<T>()(v);
			const std::size_t m = 0xe9846af9b1a615d;
			seed ^= seed >> 32;
			seed *= m;
			seed ^= seed >> 32;
			seed *= m;
			seed ^= seed >> 28;
		} else {
			seed += 0x9e3779b9 + std::hash<T>()(v);
			const std::size_t m1 = 0x21f0aaad;
			const std::size_t m2 = 0x735a2d97;
			seed ^= seed >> 16;
			seed *= m1;
			seed ^= seed >> 15;
			seed *= m2;
			seed ^= seed >> 15;
		}

		(hash_combine(seed, values), ...);
	}

	template<typename T, typename... Values> usize hash_combine(cref<T> v, cref<Values>... values) {
		std::size_t seed{ 0 };

		(hash_combine(seed, v, values), ...);

		return seed;
	}

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
