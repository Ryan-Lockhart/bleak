#pragma once

typedef unsigned char u8;
typedef signed char i8;

typedef unsigned short u16;
typedef signed short i16;

typedef unsigned int u32;
typedef signed int i32;

typedef unsigned long long u64;
typedef signed long long i64;

typedef float f32;
typedef double f64;

#if defined(WIN32)

typedef u32 usize;
typedef i32 isize;

#else

typedef u64 usize;
typedef i64 isize;

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

mem offet(usize size) { return (mem)size; }

template<typename T>
mem offet(usize count) { return (mem)(sizeof(T) * count); }

typedef ptr<char> str;
typedef cptr<char> cstr;

#define __FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)



struct Rect { i32 x, y, w, h; };

struct Octant { i32 x, dx, y, dy; };
