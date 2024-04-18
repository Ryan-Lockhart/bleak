#include "path.hpp"
#include "line.hpp"

#include <stdexcept>

inline Path::Path() noexcept : points{} {}
inline Path::Path(cref<vec<Point>> points) : points{}
{
	if (points.size() < 2)
	{
		throw std::invalid_argument("path cannot be comprised of a single point");
	}

	vec<Point> result;
	usize counter{ 0 };

	while (counter < points.size())
	{
		if (counter > 0 && counter + 1 < points.size() && (points[counter - 1].x == points[counter].x || points[counter - 1].y == points[counter].y) && (points[counter + 1].x == points[counter].x || points[counter + 1].y == points[counter].y) && points[counter - 1].x != points[counter + 1].x && points[counter - 1].y != points[counter + 1].y)
		{
			++counter;
		}

		result.push_back(points[counter]);
		++counter;
	}
}

inline Path::Path(cref<vec<Line>> lines)
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

inline Path::Path(cref<Path> path) noexcept : points{ path.points } {}
inline Path::Path(rval<Path> path) noexcept : points{ std::move(path.points) } {}

inline bool Path::empty() const { return points.empty(); }
inline usize Path::size() const { return points.size(); }

inline cref<Point> Path::front() const { return points.front(); }
inline cref<Point> Path::back() const { return points.back(); }

inline cref<Point> Path::operator[](usize index) const { return *std::next(points.begin(), index); }

inline void Path::push_front(cref<Point> point) { points.push_front(point); }
inline void Path::push_back(cref<Point> point) { points.push_back(point); }

inline void Path::pop_front() { points.pop_front(); }
inline void Path::pop_back() { points.pop_back(); }

inline void Path::emplace_front(rval<Point> point) { points.emplace_front(point); }
inline void Path::emplace_back(rval<Point> point) { points.emplace_back(point); }

inline rval<Point> Path::extract_front() { rval<Point> p = std::move(points.front()); points.pop_front(); return std::move(p); }
inline rval<Point> Path::extract_back() { rval<Point> p = std::move(points.back()); points.pop_back(); return std::move(p); }

inline void Path::clear() { points.clear(); }

inline void Path::insert(usize index, cref<Point> point) { points.insert(std::next(points.begin(), index), point); }
inline void Path::insert(usize index, cref<Path> path) { points.insert(std::next(points.begin(), index), path.points.begin(), path.points.end()); }
inline void Path::erase(usize index) { points.erase(std::next(points.begin(), index)); }

inline void Path::append(cref<Path> path) { points.insert(points.end(), path.points.begin(), path.points.end()); }

inline void Path::reverse() { points.reverse(); }
