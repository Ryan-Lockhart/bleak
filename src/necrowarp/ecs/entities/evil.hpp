#include <bleak.hpp>

#include <necrowarp/ecs/entities/base.hpp>

#include <necrowarp/ecs/components.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace entities {
		struct player {};
		struct skeleton {};
		struct wraith {};

		template<> struct is_entity<player> { static constexpr bool value{ true }; };
		template<> struct is_entity<skeleton> { static constexpr bool value{ true }; };
		template<> struct is_entity<wraith> { static constexpr bool value{ true }; };

		template<> inline void spawn<player>(ref<flecs::world> ecs, cstr name, offset_t position) {			
			ecs.entity(name)
				.set<offset_t>({ position })
				.add<components::animate>()
				.add<components::evil>()
				.add<components::player>()
				.add<components::unarmoured>()
				.set<components::healthy>({ 1 })
				.set<components::energized>({ 2 })
				.set<components::fresh_spawn>({ 1 });
		}

		template<> inline void spawn<skeleton>(ref<flecs::world> ecs, cstr name, offset_t position, bool fresh) {
			auto e = ecs.entity(name)
				.set<offset_t>({ position })
				.add<components::animate>()
				.add<components::npc>()
				.add<components::evil>()
				.add<components::skeleton>()
				.set<components::healthy>({ 1 })
				.set<components::fresh_spawn>({ 1 });

			if (fresh) {
				e.add<components::fresh>();
			} else {
				e.add<components::rotted>();
			}
		}

		template<> inline void spawn<wraith>(ref<flecs::world> ecs, cstr name, offset_t position, u16 health) {
			ecs.entity(name)
				.set<offset_t>({ position })
				.add<components::animate>()
				.add<components::npc>()
				.add<components::evil>()
				.add<components::wraith>()
				.set<components::healthy>({ health })
				.set<components::fresh_spawn>({ 1 });
		}
	} // namespace entities
} // namespace necrowarp
