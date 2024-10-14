#pragma once

#include <bleak.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace components {
		struct health_t {
			i16 value;
		};

		struct afflicted_t {};
	} // namespace components
} // namespace necrowarp
