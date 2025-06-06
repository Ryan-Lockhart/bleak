#pragma once

#include <bleak/offset.hpp>

namespace bleak {
	struct region_offset_t {
		offset_t zone;
		offset_t cell;
	};
} // namespace bleak
