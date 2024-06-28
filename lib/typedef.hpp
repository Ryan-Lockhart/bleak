#pragma once

#if defined(__x86_64__)
#define BLEAKDEPTH_64
#elif defined(__i386__)
#define BLEAKDEPTH_32
#endif

#define forever for(;;)

namespace Bleakdepth {
	using u8 = unsigned char;
	using i8 = signed char;

	using u16 = unsigned short;
	using i16 = signed short;

	using u32 = unsigned int;
	using i32 = signed int;

	using u64 = unsigned long long;
	using i64 = signed long long;

	using u128 = unsigned __int128;
	using i128 = signed __int128;

	using f16 = _Float16;
	using f32 = float;
	using f64 = double;
	using f80 = long double;
	using f128 = __float128;

#if defined(BLEAKDEPTH_32)

	using usize = u32;
	using isize = i32;

	using uhalf = u16;
	using ihalf = i16;

	using uqrtr = u8;
	using iqrtr = i8;

	using fsize = f32;
	using fhalf = f16;

#else

	using usize = u64;
	using isize = i64;

	using uhalf = u32;
	using ihalf = i32;

	using uqrtr = u16;
	using iqrtr = i16;

	using fsize = f64;
	using fhalf = f32;

#endif

	template<typename T> using ref = T&;
	template<typename T> using cref = const T&;
	template<typename T> using rval = T&&;
	template<typename T> using crval = const T&&;

	template<typename T> using ptr = T*;
	template<typename T> using cptr = const T*;
	template<typename T> using ptrc = T* const;
	template<typename T> using cptrc = const T* const;

	using ubyte = u8;
	using ibyte = i8;

	using mem = ptr<void>;
	using cmem = cptr<void>;

	using str = ptr<char>;
	using cstr = cptr<char>;

	using buffer = ptr<ubyte>;
	using cbuffer = cptr<ubyte>;

	template<typename Return, typename... Args> using fn_ptr = Return(*)(Args...);
	template<typename T, typename Return, typename... Args> using mfn_ptr = Return(T::*)(Args...);

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
