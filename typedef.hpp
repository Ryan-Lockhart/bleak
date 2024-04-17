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

#include <string>

#define __FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

using string = std::string;

static que<string> MESSAGES;
static usize MAX_MESSAGES = 30;

static que<string> ERRORS;
static usize MAX_ERRORS = 30;

void log(cstr message) { MESSAGES.emplace(message); }
void log(cref<string> message) { MESSAGES.emplace(message); }

void prune_message() { if (MESSAGES.size() > MAX_MESSAGES) MESSAGES.pop(); }
void prune_messages() { while (MESSAGES.size() > MAX_MESSAGES) MESSAGES.pop(); }

#include <fmt/core.h>

void log(cstr message, cstr file, usize line) { ERRORS.emplace(fmt::format("[{}]: \"{}\" ({}): {}", __TIME__, file, line, message)); }
void log(cref<string> message, cstr file, usize line) { ERRORS.emplace(fmt::format("[{}]: \"{}\" ({}): {}", __TIME__, file, line, message)); }

void prune_error() { if (ERRORS.size() > MAX_ERRORS) ERRORS.pop(); }
void prune_errors() { while (ERRORS.size() > MAX_ERRORS) ERRORS.pop(); }

#include <cmath>

template<typename T> T clamp(cref<T> value, cref<T> min, cref<T> max) { return std::min(std::max(value, min), max); }
