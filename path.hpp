#pragma once

#include "typedef.hpp"

#include <list>
#include <stdexcept>

#include "point.hpp"

struct Line;

class Path
{
private:
	std::list<Point> points;

public:
	inline Path() noexcept;
	inline Path(cref<vec<Point>> points);

	inline Path(cref<vec<Line>> lines);

	inline Path(cref<Path> path) noexcept;
	inline Path(rval<Path> path) noexcept;

	inline ~Path() = default;

	inline bool empty() const;
	inline usize size() const;

	inline cref<Point> front() const;
	inline cref<Point> back() const;

	inline cref<Point> operator[](usize index) const;

	inline void push_front(cref<Point> point);
	inline void push_back(cref<Point> point);

	inline void pop_front();
	inline void pop_back();

	inline void emplace_front(rval<Point> point);
	inline void emplace_back(rval<Point> point);

	inline rval<Point> extract_front();
	inline rval<Point> extract_back();

	inline void clear();

	inline void insert(usize index, cref<Point> point);
	inline void insert(usize index, cref<Path> path);
	inline void erase(usize index);

	inline void append(cref<Path> path);

	inline void reverse();
};
