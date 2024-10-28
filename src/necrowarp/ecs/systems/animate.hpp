#pragma once

#include <bleak.hpp>

#include <necrowarp/ecs/systems/base.hpp>

#include <necrowarp/ecs/components.hpp>
#include <necrowarp/ecs/entities.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace systems {
		struct fresh_spawn_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.defer([](flecs::world& ecs) {
					ecs.system<components::animate, components::fresh_spawn>().each([](flecs::entity e, components::animate, ref<components::fresh_spawn> fs) {
						if (fs.turns_remaining > 0) {
							--fs.turns_remaining;
						} else {
							e.remove<components::fresh_spawn>();
							e.add<components::awaiting_turn>();
						}
					});
				});
			}
		};

		struct distribute_turns_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system<components::animate, components::awaiting_turn>()
					.without<components::awaiting_input>().singleton()
					.each([](flecs::entity e, components::animate, components::awaiting_turn) {
						e.remove<components::awaiting_turn>();
					}
				);
			}
		};
	}
}
