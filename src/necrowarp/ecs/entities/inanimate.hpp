#include <bleak.hpp>

#include <necrowarp/ecs/entities/base.hpp>

#include <necrowarp/ecs/components.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace entities {
		struct skull {};
		struct ladder {};

		template<> struct is_entity<skull> { static constexpr bool value{ true }; };
		template<> struct is_entity<ladder> { static constexpr bool value{ true }; };

		template<> inline void spawn<skull>(ref<flecs::world> ecs, cstr name, offset_t position, bool fresh) {			
			auto e = ecs.entity(name)
				.set<offset_t>({ position })
				.add<components::inanimate>()
				.add<components::skull>();
			
			if (fresh) {
				e.add<components::fresh>();
			} else {
				e.add<components::rotted>();
			}
		}

		template<> inline void spawn<ladder>(ref<flecs::world> ecs, cstr name, offset_t position) {
			ecs.entity(name)
				.set<offset_t>({ position })
				.add<components::inanimate>()
				.add<components::ladder>();
		}
	} // namespace entities
} // namespace necrowarp
