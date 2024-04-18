#include "line.hpp"

constexpr Line::Line(point_t x1, point_t y1, point_t x2, point_t y2) : start(x1, y1), end(x2, y2) {}

constexpr Line::Line(cref<Point> start, cref<Point> end) : start(start), end(end) {}
constexpr Line::Line(rval<Point> start, rval<Point> end) : start(std::move(start)), end(std::move(end)) {}

constexpr Line::Line(cref<Line> line) : start(line.start), end(line.end) {}
constexpr Line::Line(rval<Line> line) : start(std::move(line.start)), end(std::move(line.end)) {}

constexpr ref<Line> Line::operator=(cref<Line> line)
{
	start = line.start;
	end = line.end;

	return *this;
}

constexpr ref<Line> Line::operator=(rval<Line> line)
{
	start = std::move(line.start);
	end = std::move(line.end);

	return *this;
}

inline Line::operator Path() const
{
	Path result{};

	point_t x = start.x;
	point_t y = start.y;

	point_t dx = abs(end.x - start.x);
	point_t dy = abs(end.y - start.y);

	point_t sx = start.x < end.x ? 1 : -1;
	point_t sy = start.y < end.y ? 1 : -1;

	point_t err = dx - dy;

	while (true)
	{
		result.push_back({ start.x, start.y });

		if (start.x == end.x && start.y == end.y) break;

		i32 e2 = 2 * err;

		if (e2 > -dy)
		{
			err -= dy;
			x += sx;
		}

		if (e2 < dx)
		{
			err += dx;
			y += sy;
		}
	}

	return result;
}
