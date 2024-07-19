#pragma once
#include <bleak/typedef.hpp>

#include <bleak/concepts.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	template<typename T>
		requires is_numeric<T>::value
	struct creeper_t {
		offset_t position;
		T distance;

		constexpr creeper_t() : position{}, distance{} {}

		constexpr creeper_t(cref<offset_t> position, cref<T> distance) : position{ position }, distance{ distance } {}
	};
} // namespace bleak
