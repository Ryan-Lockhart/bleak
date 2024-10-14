#pragma once

#include <necrowarp/entities/priest.hpp>

#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	inline entity_command_t priest_t::think() const noexcept {
		if (!has_piety()) {
			cauto flock_to_paladin_pos{ entity_goal_map<paladin_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (!flock_to_paladin_pos.has_value() && entity_registry.empty<adventurer_t>()) {
				return entity_command_t{ command_type_t::None };
			} else if (flock_to_paladin_pos.has_value()) {
				return entity_command_t{ command_type_t::Move, position, flock_to_paladin_pos.value() };
			}

			cauto flock_to_adventurer_pos { entity_goal_map<adventurer_t>.descend<zone_region_t::Interior>(position, entity_registry) };

			if (!flock_to_adventurer_pos.has_value()) {
				cauto flee_from_evil_pos{ evil_goal_map.ascend<zone_region_t::Interior>(position, entity_registry) };

				if (!flee_from_evil_pos.has_value()) {
					return entity_command_t{ command_type_t::None };
				}

				return entity_command_t{ command_type_t::Move, position, flee_from_evil_pos.value() };
			}

			return entity_command_t{ command_type_t::Move, position, flock_to_adventurer_pos.value() };
		}

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t offset_pos{ position + offset };

			switch (entity_registry.at(offset_pos)) {
				case entity_type_t::Skull: {
					if (!can_resurrect() && !can_exorcise()) {
						continue;
					}

					cauto skull{ entity_registry.at<skull_t>(offset_pos) };

					if (skull == nullptr) {
						continue;
					}

					if (skull->fresh && can_resurrect()) {
						return entity_command_t{ command_type_t::Resurrect, position, offset_pos };
					} else if (!skull->fresh && can_exorcise()) {
						return entity_command_t{ command_type_t::Exorcise, position, offset_pos }; 
					}
					
					continue;
				} case entity_type_t::Adventurer: {
					if (!can_anoint()) {
						continue;
					}

					return entity_command_t{ command_type_t::Anoint, position, offset_pos };
				} default: {
					continue;
				}
			}
		}

		cauto descent_pos = [&]() -> std::optional<offset_t> {
			cauto skull_pos = entity_goal_map<skull_t>.descend<zone_region_t::Interior>(position, entity_registry);

			cauto adventurer_pos = entity_goal_map<adventurer_t>.descend<zone_region_t::Interior>(position, entity_registry);

			if (skull_pos.has_value() && adventurer_pos.has_value()) {
				cauto skull_distance{ entity_goal_map<skull_t>.at(skull_pos.value()) };
				cauto adventurer_distance{ entity_goal_map<adventurer_t>.at(adventurer_pos.value()) };

				return skull_distance < adventurer_distance ? skull_pos : adventurer_pos;
			} else if (skull_pos.has_value()) {
				return skull_pos;
			} else if (adventurer_pos.has_value()) {
				return adventurer_pos;
			}

			cauto flee_from_evil_pos{ evil_goal_map.ascend<zone_region_t::Interior>(position, entity_registry) };

			if (!flee_from_evil_pos.has_value()) {
				return std::nullopt;
			}

			return flee_from_evil_pos;
		}();

		if (!descent_pos.has_value()) {
			return entity_command_t{ command_type_t::None };
		}

		return entity_command_t{ command_type_t::Move, position, descent_pos.value() };
	}
} // namespace necrowarp
