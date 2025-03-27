#pragma once

#include <necrowarp/entities/player.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	template<entity_type_t EntityType> inline bool player_t::will_perish() const noexcept {
		if (has_ascended() || no_hit_enabled()) {
			return false;
		}

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
		if (has_ascended() || no_hit_enabled()) {
			return;
		}
		
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
			set_energy(energy + adventurer_t::DeathBoon);
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			set_energy(energy + paladin_t::DeathBoon);
		} else if constexpr (EntityType == entity_type_t::Priest) {
			set_energy(energy + priest_t::DeathBoon);
		}
	}

	inline command_type_t player_t::clash_or_consume(cref<offset_t> position) const noexcept {
		const entity_type_t type{ entity_registry.at(position) };
		
		switch (type) {
			case entity_type_t::Adventurer:
			case entity_type_t::Paladin:
			case entity_type_t::Priest: {
				return command_type_t::Clash;			
			} case entity_type_t::Skull:
			  case entity_type_t::Skeleton:
			  case entity_type_t::Wraith: {
				return command_type_t::Consume;
			} case entity_type_t::Ladder: {
				return command_type_t::Descend;
			} default: {
				return command_type_t::None;
			}
		}
	}
} // namespace necrowarp
