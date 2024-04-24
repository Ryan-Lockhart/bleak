#pragma once

#include "typedef.hpp"

#include "point.hpp"

template<typename T = i32> struct Line
{
	Point<T> start;
	Point<T> end;

	constexpr Line() = delete;

	constexpr Line(T x1, T y1, T x2, T y2) noexcept : start(x1, y1), end(x2, y2) {}

	constexpr Line(cref<Point<T>> start, cref<Point<T>> end) noexcept : start(start), end(end) {}
	constexpr Line(rval<Point<T>> start, rval<Point<T>> end) noexcept : start(std::move(start)), end(std::move(end)) {}

	constexpr Line(cref<Line> line) noexcept : start(line.start), end(line.end) {}
	constexpr Line(rval<Line> line) noexcept : start(std::move(line.start)), end(std::move(line.end)) {}

	constexpr ~Line() = default;

	constexpr ref<Line> operator=(cref<Line> line) noexcept
	{
		start = line.start;
		end = line.end;

		return *this;
	}
	
	constexpr ref<Line> operator=(rval<Line> line) noexcept
	{
		start = std::move(line.start);
		end = std::move(line.end);

		return *this;
	}
};
