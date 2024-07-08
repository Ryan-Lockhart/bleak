#pragma once

#include "bleak/octant.hpp"

namespace bleak {
	constexpr const octant_t<i32> Octants[] {
		{  0,  1,  1,  0 },
		{  1,  0,  0,  1 },
		{  0, -1,  1,  0 },
		{ -1,  0,  0,  1 },
		{  0, -1, -1,  0 },
		{ -1,  0,  0, -1 },
		{  0,  1, -1,  0 },
		{  1,  0,  0, -1 }
	};
}
