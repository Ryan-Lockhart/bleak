#pragma once

#include "octant.hpp"

constexpr const Octant Octants[]
{
	Octant{  0,  1,  1,  0 },
	Octant{  1,  0,  0,  1 },
	Octant{  0, -1,  1,  0 },
	Octant{ -1,  0,  0,  1 },
	Octant{  0, -1, -1,  0 },
	Octant{ -1,  0,  0, -1 },
	Octant{  0,  1, -1,  0 },
	Octant{  1,  0,  0, -1 }
};
