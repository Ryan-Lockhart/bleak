#pragma once

#include <bleak/typedef.hpp>

#include <bleak/offset.hpp>

namespace bleak {
	struct circle_t {
		offset_t position;
		f64 radius;

        constexpr circle_t(offset_t position, cref<f64> radius) noexcept : position{ position }, radius{ radius } {}
	};
} // namespace bleak
