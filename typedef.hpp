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

typedef ptr<char> str;
typedef cptr<char> cstr;

#include <iostream>

void log(cstr message, bool newline = true)
{
	std::cout << message;

	if (newline)
		std::cout << "\n";
}

#include <string>

using string = std::string;

void log(cstr message, cstr file, usize line, bool newline = true)
{
	std::cout << "[" << __TIME__ << ", \"" << file << "\" (" << line << ")]: " << message;

	if (newline)
		std::cout << "\n";
}

void log(cref<std::string> message, cstr file, usize line, bool newline = true)
{
	std::cout << "[" << __TIME__ << ", \"" << file << "\" (" << line << ")]: " << message;

	if (newline)
		std::cout << "\n";
}

#include <memory>

template<typename T> using uptr = std::unique_ptr<T>;
template<typename T> using sptr = std::shared_ptr<T>;
template<typename T> using wptr = std::weak_ptr<T>;

#include <array>
#include <vector>

#include <queue>
#include <deque>

template<typename T, usize Width> using arr = std::array<T, Width>;
template<typename T, usize Width, usize Height> using arr2d = std::array<T, Width * Height>;
template<typename T, usize Width, usize Height, usize Depth> using arr3d = std::array<T, Width * Height * Depth>;
template<typename T, usize Width, usize Height, usize Depth, usize Time> using arr4d = std::array<T, Width * Height * Depth * Time>;

template<typename T> using vec = std::vector<T>;

template<typename T> using que = std::queue<T>;
template<typename T> using deq = std::deque<T>;
