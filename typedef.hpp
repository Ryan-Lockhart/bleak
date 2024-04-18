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

static void Prune(ref<que<string>> queue, usize count) { while (queue.size() > count) queue.pop(); }

#include <fmt/core.h>

void log(cstr message, cstr file, usize line) { ERRORS.emplace(fmt::format("[{}]: \"{}\" ({}): {}", __TIME__, file, line, message)); }
void log(cref<string> message, cstr file, usize line) { ERRORS.emplace(fmt::format("[{}]: \"{}\" ({}): {}", __TIME__, file, line, message)); }

struct Point { i32 x, y; };

static vec<Point> BresenhamLine(Point start, cref<Point> end)
{
	vec<Point> result;

	i32 dx = abs(end.x - start.x);
	i32 dy = abs(end.y - start.y);

	i32 sx = start.x < end.x ? 1 : -1;
	i32 sy = start.y < end.y ? 1 : -1;

	i32 err = dx - dy;

	while (true)
	{
		result.push_back({ start.x, start.y });

		if (start.x == end.x && start.y == end.y) break;

		i32 e2 = 2 * err;

		if (e2 > -dy)
		{
			err -= dy;
			start.x += sx;
		}

		if (e2 < dx)
		{
			err += dx;
			start.y += sy;
		}
	}

	return result;
}

static vec<Point> PixelPerfectLine(cref<vec<Point>> path)
{
	if (path.size() < 2) return vec<Point>(path);

	vec<Point> result;
	usize counter{ 0 };

	while (counter < path.size())
	{
		if (counter > 0 && counter + 1 < path.size() && (path[counter - 1].x == path[counter].x || path[counter - 1].y == path[counter].y) && (path[counter + 1].x == path[counter].x || path[counter + 1].y == path[counter].y) && path[counter - 1].x != path[counter + 1].x && path[counter - 1].y != path[counter + 1].y)
		{
			++counter;
		}

		result.push_back(path[counter]);
		++counter;
	}

	return result;
}

static vec<Point> PathToDirections(cref<vec<Point>> path)
{
	vec<Point> result;

	for (usize i = 1; i < path.size(); ++i)
	{
		Point direction{ path[i].x - path[i - 1].x, path[i].y - path[i - 1].y };

		if (direction.x < 0) direction.x = -1;
		else if (direction.x > 0) direction.x = 1;

		if (direction.y < 0) direction.y = -1;
		else if (direction.y > 0) direction.y = 1;

		result.push_back(direction);
	}

	return result;
}
