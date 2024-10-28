#include <bleak.hpp>

#include <necrowarp/ecs/entities/base.hpp>

#include <necrowarp/ecs/components.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace entities {
		struct adventurer {};
		struct paladin {};
		struct priest {};

		template<> struct is_entity<adventurer> { static constexpr bool value{ true }; };
		template<> struct is_entity<paladin> { static constexpr bool value{ true }; };
		template<> struct is_entity<priest> { static constexpr bool value{ true }; };

		template<> inline void spawn<adventurer>(ref<flecs::world> ecs, cstr name, offset_t position) {			
			ecs.entity(name)
				.set<offset_t>({ position })
				.add<components::animate>()
				.add<components::npc>()
				.add<components::good>()
				.add<components::adventurer>()
				.set<components::healthy>({ 1 })
				.set<components::fresh_spawn>({ 1 });
		}

		template<> inline void spawn<paladin>(ref<flecs::world> ecs, cstr name, offset_t position) {
			ecs.entity(name)
				.set<offset_t>({ position })
				.add<components::animate>()
				.add<components::npc>()
				.add<components::good>()
				.add<components::paladin>()
				.set<components::healthy>({ 2 })
				.set<components::fresh_spawn>({ 1 });
		}

		template<> inline void spawn<priest>(ref<flecs::world> ecs, cstr name, offset_t position) {
			ecs.entity(name)
				.set<offset_t>({ position })
				.add<components::animate>()
				.add<components::npc>()
				.add<components::good>()
				.add<components::priest>()
				.set<components::healthy>({ 1 })
				.set<components::pious>({8})
				.set<components::fresh_spawn>({ 1 });
		}
	} // namespace entities
} // namespace necrowarp
