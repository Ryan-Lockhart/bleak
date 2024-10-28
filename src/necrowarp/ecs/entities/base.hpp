#pragma once

#include <bleak.hpp>

#include <flecs.h>

namespace necrowarp {
	using namespace bleak;

	namespace entities {
		template<typename T> struct is_entity {
			static constexpr bool value{ false };
		};

		template<typename T> constexpr bool is_entity_v{ is_entity<T>::value };

		template<typename T> concept Entity = is_entity<T>::value;

		template<Entity T> inline void spawn(ref<flecs::world> ecs, cstr name);

		template<Entity T, typename... Args> inline void spawn(ref<flecs::world> ecs, cstr name, Args... args);

		template<Entity T, typename... Args> inline void spawn(ref<flecs::world> ecs, cstr name, cref<Args>... args);

		template<Entity T, typename... Args> inline void spawn(ref<flecs::world> ecs, cstr name, rval<Args>... args);
	} // namespace entities
} // namespace necrowarp
