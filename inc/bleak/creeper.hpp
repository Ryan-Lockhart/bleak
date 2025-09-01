#pragma once

#include <bleak/typedef.hpp>

#include <bleak/concepts.hpp>
#include <bleak/hash.hpp>
#include <bleak/memory.hpp>
#include <bleak/offset.hpp>
#include <bleak/random.hpp>

namespace bleak {
	template<Numeric N> struct creeper_t {
		offset_t position;
		N distance;

		constexpr creeper_t() : position{}, distance{} {}

		constexpr creeper_t(offset_t position, cref<N> distance) : position{ position }, distance{ distance } {}

		struct hasher {
			static constexpr usize operator()(cref<creeper_t<N>> creeper) noexcept { return hash_combine(creeper.position, creeper.distance); }
		};

		struct less {
			static constexpr bool operator()(cref<creeper_t<N>> lhs, cref<creeper_t<N>> rhs) noexcept { return lhs.distance > rhs.distance; }
		};
	};

	template<Numeric N> struct memory_creeper_t {
		rememberance_t<offset_t> position;
		N distance;

		constexpr memory_creeper_t() : position{}, distance{} {}

		constexpr memory_creeper_t(cref<rememberance_t<offset_t>> memory, cref<N> distance) : position{ memory }, distance{ distance } {}

		constexpr memory_creeper_t(offset_t current, offset_t previous, cref<N> distance) : position{ current, previous }, distance{ distance } {}

		constexpr bool is_origin() const { return *this == memory_creeper_t<N>{}; }

		struct hasher {
			static constexpr usize operator()(cref<memory_creeper_t<N>> creeper) noexcept { return hash_combine(creeper.position, creeper.distance); }
		};

		struct less {
			static constexpr bool operator()(cref<memory_creeper_t<N>> lhs, cref<memory_creeper_t<N>> rhs) noexcept { return lhs.distance > rhs.distance; }
		};
	};

	template<distance_function_e Distance, Numeric D> static constexpr auto neighbourhood_creepers{
		[]() {
			if constexpr (Distance == distance_function_e::VonNeumann || Distance == distance_function_e::Manhattan) {
				return std::array<creeper_t<D>, 4>{
					creeper_t<D>{ offset_t::North, 1 },
					creeper_t<D>{ offset_t::South, 1 },
					creeper_t<D>{ offset_t::West, 1 },
					creeper_t<D>{ offset_t::East, 1 }
				};
			} else if constexpr (Distance == distance_function_e::Chebyshev) {
				return std::array<creeper_t<D>, 8>{
					creeper_t<D>{ offset_t::North, 1 },
					creeper_t<D>{ offset_t::South, 1 },
					creeper_t<D>{ offset_t::West, 1 },
					creeper_t<D>{ offset_t::East, 1 },
					creeper_t<D>{ offset_t::Northwest, 1 },
					creeper_t<D>{ offset_t::Northeast, 1 },
					creeper_t<D>{ offset_t::Southwest, 1 },
					creeper_t<D>{ offset_t::Southeast, 1 }
				};
			} else {
				return std::array<creeper_t<D>, 8>{
					creeper_t<D>{ offset_t::North, 1.0 },
					creeper_t<D>{ offset_t::South, 1.0 },
					creeper_t<D>{ offset_t::West, 1.0 },
					creeper_t<D>{ offset_t::East, 1.0 },
					creeper_t<D>{ offset_t::Northwest, 1.414 },
					creeper_t<D>{ offset_t::Northeast, 1.414 },
					creeper_t<D>{ offset_t::Southwest, 1.414 },
					creeper_t<D>{ offset_t::Southeast, 1.414 }
				};
			}
		}()
	};

	template<distance_function_e Distance, Numeric D, RandomEngine Generator> static inline auto shuffled_creepers(ref<Generator> engine) noexcept {
		return [&]() {
			auto creepers{ neighbourhood_creepers<Distance, D> };

			std::shuffle(creepers.begin(), creepers.end(), engine);

			return creepers;
		}();
	}
} // namespace bleak
