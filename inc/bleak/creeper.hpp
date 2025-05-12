#pragma once

#include <bleak/typedef.hpp>

#include <bleak/concepts.hpp>
#include <bleak/hash.hpp>
#include <bleak/memory.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	template<typename T>
		requires is_numeric<T>::value
	struct creeper_t {
		offset_t position;
		T distance;

		constexpr creeper_t() : position{}, distance{} {}

		constexpr creeper_t(offset_t position, cref<T> distance) : position{ position }, distance{ distance } {}

		struct hasher {
			static constexpr usize operator()(cref<creeper_t<T>> creeper) noexcept { return hash_combine(creeper.position, creeper.distance); }
		};
	};

	template<typename T>
		requires is_numeric<T>::value
	struct memory_creeper_t {
		rememberance_t<offset_t> position;
		T distance;

		constexpr memory_creeper_t() : position{}, distance{} {}

		constexpr memory_creeper_t(cref<rememberance_t<offset_t>> memory, cref<T> distance) : position{ memory }, distance{ distance } {}

		constexpr memory_creeper_t(offset_t current, offset_t previous, cref<T> distance) : position{ current, previous }, distance{ distance } {}

		constexpr bool is_origin() const { return *this == memory_creeper_t<T>{}; }

		struct hasher {
			static constexpr usize operator()(cref<memory_creeper_t<T>> creeper) noexcept { return hash_combine(creeper.position, creeper.distance); }
		};

		struct less {
			static constexpr bool operator()(cref<memory_creeper_t<T>> lhs, cref<memory_creeper_t<T>> rhs) noexcept { return lhs.distance > rhs.distance; }
		};
	};
} // namespace bleak
