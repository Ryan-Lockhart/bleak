#pragma once

#include <bleak.hpp>

#include <flecs.h>

namespace necrowarp {
	using namespace bleak;

	namespace systems {
		template<typename T> struct is_system {
			static constexpr bool value{ false };
		};

		template<typename T> constexpr bool is_system_v{ is_system<T>::value };

		template<typename T> concept System = is_system<T>::value;

		template<System T> static inline void initialize(ref<flecs::world> ecs) noexcept {
			T::operator()(ecs);
		}

		template<System T, typename... Args> static inline void initialize(ref<flecs::world> ecs, Args... args) noexcept {
			T::operator()(ecs, args...);
		}

		template<System T, typename... Args> static inline void initialize(ref<flecs::world> ecs, cref<Args>... args) noexcept {
			T::operator()(ecs, args...);
		}

		template<System T, typename... Args> static inline void initialize(ref<flecs::world> ecs, rval<Args>... args) noexcept {
			T::operator()(ecs, std::move(args)...);
		}
	} // namespace systems
} // namespace necrowarp
