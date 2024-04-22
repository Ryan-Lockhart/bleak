#pragma once

#include "typedef.hpp"

#include <list>
#include <vector>
#include <stdexcept>

#include "point.hpp"
#include "line.hpp"

class Path
{
private:
	std::list<Point> points;

public:
	inline Path() noexcept : points{} {}
	inline Path(cref<std::vector<Point>> points) : points{} {}

	inline Path(cref<std::vector<Line>> lines)
	{
		if (lines.size() < 2)
		{
			throw std::invalid_argument("path cannot be comprised of a single point");
		}

		points.push_back(lines.front().start);

		for (usize i = 0; i < lines.size() - 1; ++i)
		{
			if (lines[i].end != lines[i + 1].start)
			{
				throw std::invalid_argument("path is discontiguous");
			}

			points.push_back(lines[i].end);
		}

		points.push_back(lines.back().end);
	}

	inline Path(cref<Path> path) noexcept : points{ path.points } {}
	inline Path(rval<Path> path) noexcept : points{ std::move(path.points) } {}

	inline ~Path() = default;

	inline Path& operator=(cref<Path> path) noexcept
	{
		points = path.points;
		return *this;
	}

	inline Path& operator=(rval<Path> path) noexcept
	{
		points = std::move(path.points);
		return *this;
	}

	inline bool empty() const { return points.empty(); }
	inline usize size() const { return points.size(); }

	inline cref<Point> front() const { return points.front(); }
	inline cref<Point> back() const { return points.back(); }

	inline cref<Point> operator[](usize index) const { return *std::next(points.begin(), index); }

	inline void push_front(cref<Point> point) { points.push_front(point); }
	inline void push_back(cref<Point> point) { points.push_back(point); }

	inline void pop_front() { points.pop_front(); }
	inline void pop_back() { points.pop_back(); }

	inline void emplace_front(rval<Point> point) { points.emplace_front(point); }
	inline void emplace_back(rval<Point> point) { points.emplace_back(point); }

	inline rval<Point> extract_front() { rval<Point> p = std::move(points.front()); points.pop_front(); return std::move(p); }
	inline rval<Point> extract_back() { rval<Point> p = std::move(points.back()); points.pop_back(); return std::move(p); }

	inline void clear() { points.clear(); }

	inline void insert(usize index, cref<Point> point) { points.insert(std::next(points.begin(), index), point); }
	inline void insert(usize index, cref<Path> path) { points.insert(std::next(points.begin(), index), path.points.begin(), path.points.end()); }
	inline void erase(usize index) { points.erase(std::next(points.begin(), index)); }

	inline void append(cref<Path> path) { points.insert(points.end(), path.points.begin(), path.points.end()); }

	inline void reverse() { points.reverse(); }
};
