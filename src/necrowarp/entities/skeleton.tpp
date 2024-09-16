#pragma once

#include <necrowarp/entities/skeleton.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline entity_command_t skeleton_t::think() const noexcept {
		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t current_position{ position + offset };
			const entity_type_t current_entity{ entity_registry.at(current_position) };

			switch (current_entity) {
			case entity_type_t::Adventurer:
			case entity_type_t::Paladin:
			case entity_type_t::Priest:
				break;
			default:
				continue;
			}

			return entity_command_t{ command_type_t::Clash, position, current_position };
		}

		cauto descent_pos{ evil_goal_map.descend<zone_region_t::Interior>(position, entity_registry) };

		if (!descent_pos.has_value()) {
			return entity_command_t{ command_type_t::None };
		}

		return entity_command_t{ command_type_t::Move, position, descent_pos.value() };
	}
} // namespace necrowarp
