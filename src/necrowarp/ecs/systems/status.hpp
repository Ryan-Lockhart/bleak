#include <bleak.hpp>

#include <necrowarp/ecs/systems/base.hpp>

#include <necrowarp/ecs/components.hpp>
#include <necrowarp/ecs/entities.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace systems {
		struct health_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system<components::healthy, components::health_affected>().each([](flecs::entity e, cref<components::healthy> health, components::health_affected) {
					if (health.value == 0) {
						e.remove<components::healthy>();
						e.add<components::dead>();
					}

					e.remove<components::health_affected>();
				});
			}
		};

		template<> struct is_system<health_system> {
			static constexpr bool value{ true };
		};

		struct armour_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system<components::armoured, components::armour_affected>().each([](flecs::entity e, cref<components::armoured> armour, components::armour_affected) {
					if (armour.value == 0) {
						e.remove<components::armoured>();
						e.add<components::unarmoured>();
					}

					e.remove<components::armour_affected>();
				});
			}
		};

		template<> struct is_system<armour_system> {
			static constexpr bool value{ true };
		};

		struct energy_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system<components::energized, components::energy_affected>().each([](flecs::entity e, cref<components::energized> energy, components::energy_affected) {
					if (energy.value == 0) {
						e.remove<components::energized>();
						e.add<components::enervated>();
					}

					e.remove<components::energy_affected>();
				});
			}
		};

		template<> struct is_system<energy_system> {
			static constexpr bool value{ true };
		};

		struct piety_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system<components::pious, components::piety_affected>().each([](flecs::entity e, cref<components::pious> piety, components::piety_affected) {
					if (piety.value == 0) {
						e.remove<components::pious>();
						e.add<components::impious>();
					}

					e.remove<components::piety_affected>();
				});
			}
		};

		template<> struct is_system<piety_system> {
			static constexpr bool value{ true };
		};
		
		struct player_death_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system<components::dead, components::player>().each([](flecs::entity e, components::dead, components::player) {
					// game over
				});
			}
		};

		template<> struct is_system<player_death_system> {
			static constexpr bool value{ true };
		};

		struct fresh_death_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system<components::dead, components::fresh, offset_t>().each([](flecs::entity e, components::dead, components::fresh, cref<offset_t> position) {
					e.clear();

					e.set<offset_t>({ position })
						.add<components::inanimate>()
						.add<components::skull>()
						.add<components::fresh>();
				});
			}
		};

		template<> struct is_system<fresh_death_system> {
			static constexpr bool value{ true };
		};

		struct rotted_death_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system<components::dead, components::rotted, offset_t>().each([](flecs::entity e, components::dead, components::rotted, cref<offset_t> position) {
					e.clear();

					e.set<offset_t>({ position })
						.add<components::inanimate>()
						.add<components::skull>()
						.add<components::rotted>();
				});
			}
		};

		template<> struct is_system<rotted_death_system> {
			static constexpr bool value{ true };
		};

		struct destroy_dead_orphans_system {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				ecs.system()
					.with<components::dead>()
					.without<offset_t>()
					.each([](flecs::entity e) {
						e.world().defer([e](){ e.destruct(); });
					});
			}
		};

		template<> struct is_system<destroy_dead_orphans_system> {
			static constexpr bool value{ true };
		};

		struct status_systems {
			static inline void operator()(ref<flecs::world> ecs) noexcept {
				health_system::operator()(ecs);
				energy_system::operator()(ecs);
				piety_system::operator()(ecs);

				player_death_system::operator()(ecs);
				fresh_death_system::operator()(ecs);
				rotted_death_system::operator()(ecs);

				destroy_dead_orphans_system::operator()(ecs);
			}
		};

		template<> struct is_system<status_systems> {
			static constexpr bool value{ true };
		};
	} // namespace systems
} // namespace necrowarp
