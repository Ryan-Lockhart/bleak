#include "bleak/region.hpp"

#include <cassert>

#include "bleak/extent.hpp"
#include "bleak/offset.hpp"

int main(int argc, char* argv[]) {
	bleak::region_t<int, bleak::extent_t{ 3, 3 }, bleak::extent_t{ 2, 2 }> region;

	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			region[bleak::offset_t{ x, y }, bleak::offset_t{ 0, 0 }] = x + y;
		}
	}

	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			assert((region[bleak::offset_t{ x, y }, bleak::offset_t{ 0, 0 }] == x + y));
		}
	}

	return 0;
}
