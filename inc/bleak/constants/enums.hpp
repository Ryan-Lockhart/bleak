#pragma once

#include <bleak/typedef.hpp>

namespace bleak {
	enum struct region_e : u8 {
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

	enum struct wave_e {
		Sine,
		Square,
		Triangle,
		Sawtooth,
		Cycloid
	};
} // namespace bleak
