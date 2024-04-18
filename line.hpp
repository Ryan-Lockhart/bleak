#pragma once

#include "typedef.hpp"
#include "point.hpp"

#include "path.hpp"

struct Line
{
	using point_t = Point::point_t;

	Point start;
	Point end;

	constexpr Line() = delete;

	constexpr Line(point_t x1, point_t y1, point_t x2, point_t y2);

	constexpr Line(cref<Point> start, cref<Point> end);
	constexpr Line(rval<Point> start, rval<Point> end);

	constexpr Line(cref<Line> line);
	constexpr Line(rval<Line> line);

	constexpr ref<Line> operator=(cref<Line> line);
	constexpr ref<Line> operator=(rval<Line> line);

	inline operator Path() const;
};
