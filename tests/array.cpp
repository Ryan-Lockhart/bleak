#include "bleak/array.hpp"

#include <cassert>

#include "bleak/extent.hpp"
#include "bleak/offset.hpp"

int main(int argc, char* argv[]) {
	// Test default constructor
	bleak::row_t<int, bleak::extent_1d_t{ 5 }> arr1;
	assert(arr1.size == 5);
	assert(arr1.byte_size == 5 * sizeof(int));

	// Test initializer list constructor
	bleak::row_t<int, bleak::extent_1d_t{ 3 }> arr2 = { 1, 2, 3 };
	assert(arr2[0] == 1);
	assert(arr2[1] == 2);
	assert(arr2[2] == 3);

	// Test copy constructor
	bleak::row_t<int, bleak::extent_1d_t{ 3 }> arr3 = arr2;
	assert(arr3[0] == 1);
	assert(arr3[1] == 2);
	assert(arr3[2] == 3);

	// Test assignment operator
	bleak::row_t<int, bleak::extent_1d_t{ 3 }> arr4;
	arr4 = arr2;
	assert(arr4[0] == 1);
	assert(arr4[1] == 2);
	assert(arr4[2] == 3);

	// Test subscript operator
	arr4[1] = 5;
	assert(arr4[1] == 5);

	// Test valid function
	assert(arr4.valid(bleak::offset_1d_t{ 0 }));
	assert(arr4.valid(bleak::offset_1d_t{ 1 }));
	assert(arr4.valid(bleak::offset_1d_t{ 2 }));
	assert(!arr4.valid(bleak::offset_1d_t{ 3 }));

	// Test at function
	assert(arr4.at(bleak::offset_1d_t{ 0 }) == 1);
	assert(arr4.at(bleak::offset_1d_t{ 1 }) == 5);
	assert(arr4.at(bleak::offset_1d_t{ 2 }) == 3);

	// Test front and back functions
	assert(arr4.front() == 1);
	assert(arr4.back() == 3);

	// Test iterators
	int sum = 0;
	for (auto it = arr4.begin(); it != arr4.end(); ++it) {
		sum += *it;
	}
	assert(sum == 9);

	return 0;
}
