#pragma once

namespace bleak {
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(_M_ARM64)
#define BLEAK_64
#elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || defined(_M_ARM)
#define BLEAK_32
#endif

#define __TIME_FILE_LINE__ __TIME__, __FILE__, __LINE__

#define cauto const auto

#define rauto auto&
#define crauto const auto&

#define rvauto auto&&
#define crvauto const auto&&

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

#if defined(BLEAK_64)
	using u128 = unsigned __int128;
	using i128 = signed __int128;
	using f128 = __float128;
#endif

#if defined(BLEAK_32)

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
} // namespace bleak
