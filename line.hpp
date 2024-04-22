#pragma once

#include "typedef.hpp"

#include "point.hpp"

struct Line
{
	using point_t = Point::point_t;

	Point start;
	Point end;

	constexpr Line() = delete;

	constexpr Line(point_t x1, point_t y1, point_t x2, point_t y2) noexcept : start(x1, y1), end(x2, y2) {}

	constexpr Line(cref<Point> start, cref<Point> end) noexcept : start(start), end(end) {}
	constexpr Line(rval<Point> start, rval<Point> end) noexcept : start(std::move(start)), end(std::move(end)) {}

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
