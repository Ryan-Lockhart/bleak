#pragma once

#include <bleak.hpp>
#include <flecs.h>

#include <iostream>
#include <format>

#include <necrowarp/ecs/components.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace systems {
		using namespace components;

		struct health_system {
			static void initialize(ref<flecs::world> ecs) {
				ecs.system<health_t>().each([](flecs::entity e, ref<health_t> health) {
					if (health.value <= 0) {
						e.destruct();
					}
				});
			}
		};

		struct affliction_system {
			static void initialize(ref<flecs::world> ecs) {
				ecs.system<const afflicted_t, health_t>().each([](flecs::entity e, cref<afflicted_t> afflicted, ref<health_t> health) { --health.value; });
			}
		};

		struct emit_health_system {
			static void initialize(ref<flecs::world> ecs) {
				ecs.system<const health_t>().each([](cref<flecs::entity> e, cref<health_t> health) {
					const bool is_afflicted{ e.has<afflicted_t>() };

					std::cout << std::format("Name: {}, Health: {}, Afflicted?: {}\n", e.name().c_str(), health.value, is_afflicted ? "Yes" : "No");
				});
			}
		};
	} // namespace systems
} // namespace necrowarp
