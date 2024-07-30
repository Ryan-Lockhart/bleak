#pragma once

#include <bleak/typedef.hpp>

#include <bleak/offset.hpp>

namespace bleak {
	struct arc_t {
		offset_t position;
		f64 radius;

		f64 angle;
		f64 span;

        constexpr arc_t(cref<offset_t> position, cref<f64> radius, cref<f64> angle, cref<f64> span) noexcept : position{ position }, radius{ radius }, angle{ angle }, span{ span } {}
	};
} // namespace bleak
