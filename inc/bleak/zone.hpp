#pragma once

#include <bleak/typedef.hpp>

namespace bleak {
	enum struct zone_region_e : u8 {
		None = 0,
		Interior = 1 << 0,
		Border = 1 << 1,
		All = Interior | Border
	};

	enum struct solver_e : u8 {
		Moore,
		VonNeumann,
		Extended,
		MarchingSquares,
		Melded
	};
}

// IWYU pragma: begin_exports
#include <bleak/zone/zone_ct.hpp>
#include <bleak/zone/zone_rt.hpp>
// IWYU pragma: end_exports
