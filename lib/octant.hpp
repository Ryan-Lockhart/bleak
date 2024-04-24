#pragma once

#include "typedef.hpp"

#include <utility>

struct Octant
{
	using octant_t = i32;

	octant_t x, dx, y, dy;

	constexpr Octant() = delete;
	constexpr Octant(octant_t x, octant_t dx, octant_t y, octant_t dy) : x(x), dx(dx), y(y), dy(dy) {}

	constexpr Octant(cref<Octant> other) : x(other.x), dx(other.dx), y(other.y), dy(other.dy) {}
	constexpr Octant(rval<Octant> other) : x(std::move(other.x)), dx(std::move(other.dx)), y(std::move(other.y)), dy(std::move(other.dy)) {}

	constexpr ~Octant() = default;

	constexpr Octant& operator=(cref<Octant> other)
	{
		x = other.x;
		dx = other.dx;

		y = other.y;
		dy = other.dy;

		return *this;
	}

	constexpr Octant& operator=(rval<Octant> other)
	{
		x = std::move(other.x);
		dx = std::move(other.dx);

		y = std::move(other.y);
		dy = std::move(other.dy);

		return *this;
	}
};
