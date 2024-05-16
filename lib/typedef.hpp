#pragma once

namespace Bleakdepth {

#if defined(__x86_64__)
#define BLEAKDEPTH_64
#elif defined(__i386__)
#define BLEAKDEPTH_32
#endif

	typedef unsigned char u8;
	typedef signed char i8;

	typedef unsigned short u16;
	typedef signed short i16;

	typedef unsigned int u32;
	typedef signed int i32;

	typedef unsigned long long u64;
	typedef signed long long i64;

	typedef unsigned __int128 u128;
	typedef signed __int128 i128;

	typedef _Float16 f16;
	typedef float f32;
	typedef double f64;

#if defined(BLEAKDEPTH_32)

	typedef u32 usize;
	typedef i32 isize;

	typedef u16 uhalf;
	typedef i16 ihalf;

	typedef f16 fhalf;
	typedef f32 fsize;

#else

	typedef u64 usize;
	typedef i64 isize;

	typedef u32 uhalf;
	typedef i32 ihalf;

	typedef f32 fhalf;
	typedef f64 fsize;

#endif

	template<typename T> using ref = T&;
	template<typename T> using cref = const T&;
	template<typename T> using rval = T&&;

	template<typename T> using ptr = T*;
	template<typename T> using cptr = const T*;
	template<typename T> using ptrc = T* const;
	template<typename T> using cptrc = const T* const;

	typedef ptr<void> mem;
	typedef cptr<void> cmem;

	typedef ptr<char> str;
	typedef cptr<char> cstr;

	typedef ptr<u8> buffer;
	typedef cptr<u8> cbuffer;

	template<typename T> constexpr inline T wrap(T value, T min, T max) {
		if (value < min) {
			return max - (min - value) + 1;
		}
		if (value > max) {
			return min + (value - max) - 1;
		}

		return value;
	}
} // namespace Bleakdepth
