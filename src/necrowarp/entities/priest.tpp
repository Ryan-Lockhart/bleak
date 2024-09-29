#pragma once

#include <necrowarp/entities/priest.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline entity_command_t priest_t::think() const noexcept {
		if (entity_registry.empty<skull_t>() && !entity_registry.empty<adventurer_t, paladin_t>()) {
			cauto flock_to_paladin_pos{ entity_goal_map<paladin_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (!flock_to_paladin_pos.has_value() && entity_registry.empty<adventurer_t>()) {
				return entity_command_t{ command_type_t::None };
			} else if (flock_to_paladin_pos.has_value()) {
				return entity_command_t{ command_type_t::Move, position, flock_to_paladin_pos.value() };
			}

			cauto flock_to_adventurer_pos { entity_goal_map<adventurer_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (!flock_to_adventurer_pos.has_value()) {
				return entity_command_t{ command_type_t::None };
			}

			return entity_command_t{ command_type_t::Move, position, flock_to_adventurer_pos.value() };
		}

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t offset_pos{ position + offset };

			switch (entity_registry.at(offset_pos)) {
				case entity_type_t::Skull: {
					cauto skull{ entity_registry.at<skull_t>(offset_pos) };

					if (skull == nullptr) {
						continue;
					}

					if (!skull->fresh || !can_resurrect()) {
						return entity_command_t{ command_type_t::Exorcise, position, offset_pos }; 
					}

					return entity_command_t{ command_type_t::Resurrect, position, offset_pos };
				} case entity_type_t::Adventurer: {
					if (!can_ordain()) {
						continue;
					}

					return entity_command_t{ command_type_t::Ordain, position, offset_pos };
				} default: {
					continue;
				}
			}
		}

		cauto descent_pos{ entity_goal_map<skull_t>.descend<zone_region_t::Interior>(position, entity_registry) };

		if (!descent_pos.has_value()) {
			return entity_command_t{ command_type_t::None };
		}

		return entity_command_t{ command_type_t::Move, position, descent_pos.value() };
	}
} // namespace necrowarp
