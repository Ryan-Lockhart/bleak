#include "bleak/zone.hpp"

#include <cassert>
#include <cstdlib>

#include "bleak/extent.hpp"
#include "bleak/offset.hpp"

int main(int argc, char* argv[]) {
	// Test default constructor
	bleak::zone_t<int, bleak::extent_t{ 5, 5 }> zone1;
	assert(zone1.data().size == 25);

	// Test constructor with path
	bleak::zone_t<int, bleak::extent_t{ 5, 5 }> zone2("path/to/file");
	assert(zone2.data().size == 25);

	// Test copy constructor
	bleak::zone_t<int, bleak::extent_t{ 5, 5 }> zone3(zone1);
	assert(zone3.data().size == 25);

	// Test assignment operator
	bleak::zone_t<int, bleak::extent_t{ 5, 5 }> zone4;
	zone4 = zone2;
	assert(zone4.data().size == 25);

	// Test indexing operator
	zone1[0] = 10;
	assert(zone1[0] == 10);

	// Test on_x_edge function
	assert(zone1.on_x_edge(bleak::offset_t{ 0, 0 }) == true);
	assert(zone1.on_x_edge(bleak::offset_t{ 2, 0 }) == false);

	// Test on_y_edge function
	assert(zone1.on_y_edge(bleak::offset_t{ 0, 0 }) == true);
	assert(zone1.on_y_edge(bleak::offset_t{ 0, 2 }) == false);

	// Test on_edge function
	assert(zone1.on_edge(bleak::offset_t{ 0, 0 }) == true);
	assert(zone1.on_edge(bleak::offset_t{ 2, 2 }) == false);

	// Test edge_state function
	assert(zone1.edge_state(bleak::offset_t{ 0, 0 }) == bleak::cardinal_t::Northwest);
	assert(zone1.edge_state(bleak::offset_t{ 2, 0 }) == bleak::cardinal_t::North);
	assert(zone1.edge_state(bleak::offset_t{ 4, 0 }) == bleak::cardinal_t::Northeast);
	assert(zone1.edge_state(bleak::offset_t{ 0, 2 }) == bleak::cardinal_t::West);
	assert(zone1.edge_state(bleak::offset_t{ 4, 2 }) == bleak::cardinal_t::East);
	assert(zone1.edge_state(bleak::offset_t{ 0, 4 }) == bleak::cardinal_t::Southwest);
	assert(zone1.edge_state(bleak::offset_t{ 2, 4 }) == bleak::cardinal_t::South);
	assert(zone1.edge_state(bleak::offset_t{ 4, 4 }) == bleak::cardinal_t::Southeast);

	// Test within function
	assert(zone1.within<bleak::zone_region_t::All>(bleak::offset_t{ 2, 2 }) == true);
	assert(zone1.within<bleak::zone_region_t::Interior>(bleak::offset_t{ 2, 2 }) == true);
	assert(zone1.within<bleak::zone_region_t::Border>(bleak::offset_t{ 2, 2 }) == false);

	// Test set function
	zone1.set<bleak::zone_region_t::All>(5);
	assert(zone1[0] == 5);

	// Test swap_buffers function
	zone1.swap_buffers();

	// Test synchronize function
	zone1.synchronize<true>();

	return 0;
}
