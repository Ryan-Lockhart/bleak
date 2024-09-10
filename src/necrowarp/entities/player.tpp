#pragma once

#include <necrowarp/entities/player.hpp>
#include <necrowarp/entities/adventurer.hpp>
#include <necrowarp/entities/paladin.hpp>
#include <necrowarp/entities/priest.hpp>

#include <necrowarp/entity_state.hpp>

namespace necrowarp {
	template<entity_type_t EntityType> inline bool player_t::will_perish() const noexcept {
		if constexpr (EntityType == entity_type_t::Adventurer) {
			return armor < adventurer_t::MaximumDamage;
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			return armor < paladin_t::MaximumDamage;
		} else if constexpr (EntityType == entity_type_t::Priest) {
			return armor < priest_t::MaximumDamage;
		}

		return false;
	}

	template<entity_type_t EntityType> inline void player_t::receive_damage() noexcept {
		if constexpr (EntityType == entity_type_t::Adventurer) {
			set_armor(armor - adventurer_t::MaximumDamage);
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			set_armor(armor - paladin_t::MaximumDamage);
		} else if constexpr (EntityType == entity_type_t::Priest) {
			set_armor(armor - priest_t::MaximumDamage);
		}
	}

	template<entity_type_t EntityType> inline void player_t::receive_death_boon() noexcept {
		if constexpr (EntityType == entity_type_t::Adventurer) {
			set_armor(armor - adventurer_t::DeathBoon);
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			set_armor(armor - paladin_t::DeathBoon);
		} else if constexpr (EntityType == entity_type_t::Priest) {
			set_armor(armor - priest_t::DeathBoon);
		}
	}

	inline void player_t::bolster_armor(i8 value) noexcept {
		set_armor(armor + value);
	}
} // namespace necrowarp
