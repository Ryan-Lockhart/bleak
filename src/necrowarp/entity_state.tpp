#pragma once

#include "bleak/utility.hpp"
#include "necrowarp/entities/priest.hpp"
#include "necrowarp/entities/skeleton.hpp"
#include "necrowarp/entities/skull.hpp"
#include "necrowarp/entities/wraith.hpp"
#include <necrowarp/entities.hpp>

#include <type_traits>
#include <unordered_set>

namespace necrowarp {
	using namespace bleak;

	static inline player_t player{};

	static inline std::unordered_set<skull_t, skull_t::hasher, skull_t::comparator> skulls{};

	static inline std::unordered_set<skeleton_t, skeleton_t::hasher, skeleton_t::comparator> skeletons{};
	static inline std::unordered_set<wraith_t, wraith_t::hasher, wraith_t::comparator> wraithes{};

	static inline std::unordered_set<adventurer_t, adventurer_t::hasher, adventurer_t::comparator> adventurers{};
	static inline std::unordered_set<paladin_t, paladin_t::hasher, paladin_t::comparator> paladins{};
	static inline std::unordered_set<priest_t, priest_t::hasher, priest_t::comparator> priests{};

	inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept {
		return player.position == position || skulls.contains(position) || skeletons.contains(position) || wraithes.contains(position) || adventurers.contains(position) || paladins.contains(position) || priests.contains(position);
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept {
		if constexpr (EntityType == entity_type_t::Player) {
			return player.position == position;
		} else if constexpr (EntityType == entity_type_t::Skull) {
			return skulls.contains(position);
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			return skeletons.contains(position);
		} else if constexpr (EntityType == entity_type_t::Wraith) {
			return wraithes.contains(position);
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			return adventurers.contains(position);
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			return paladins.contains(position);
		} else if constexpr (EntityType == entity_type_t::Priest) {
			return priests.contains(position);
		} else {
			return player.position != position && !skulls.contains(position) && !skeletons.contains(position) && !wraithes.contains(position) && !adventurers.contains(position) && !paladins.contains(position) && !priests.contains(position);
		}
	}

	inline entity_type_t entity_registry_t::at(cref<offset_t> position) const noexcept {
		if (player.position == position) {
			return entity_type_t::Player;
		} else if (skulls.contains(position)) {
			return entity_type_t::Skull;
		} else if (skeletons.contains(position)) {
			return entity_type_t::Skeleton;
		} else if (adventurers.contains(position)) {
			return entity_type_t::Adventurer;
		} else {
			return entity_type_t::None;
		}
	}

	template<typename T>
		requires is_entity<T>::value
	inline cptr<T> entity_registry_t::at(cref<offset_t> position) const noexcept {
		if constexpr (std::is_same<T, player_t>::value) {
            if (player.position != position) {
                return nullptr;
            }

            return &player;
		} else if constexpr (std::is_same<T, skull_t>::value) {
            cauto iter{ skulls.find(position) };
            
            if (iter == skulls.end()) {
                return nullptr;
            }
            
            return deref_addr_of<skull_t>(iter);
		} else if constexpr (std::is_same<T, skeleton_t>::value) {
            cauto iter{ skeletons.find(position) };
            
            if (iter == skeletons.end()) {
                return nullptr;
            }
            
            return deref_addr_of<skeleton_t>(iter);
		} else if constexpr (std::is_same<T, wraith_t>::value) {
            cauto iter{ wraithes.find(position) };
            
            if (iter == wraithes.end()) {
                return nullptr;
            }
            
            return deref_addr_of<wraith_t>(iter);
		} else if constexpr (std::is_same<T, adventurer_t>::value) {
            cauto iter{ adventurers.find(position) };
            
            if (iter == adventurers.end()) {
                return nullptr;
            }
            
            return deref_addr_of<adventurer_t>(iter);
		} else if constexpr (std::is_same<T, paladin_t>::value) {
            cauto iter{ paladins.find(position) };
            
            if (iter == paladins.end()) {
                return nullptr;
            }
            
            return deref_addr_of<paladin_t>(iter);
		} else if constexpr (std::is_same<T, priest_t>::value) {
            cauto iter{ priests.find(position) };
            
            if (iter == priests.end()) {
                return nullptr;
            }
            
            return deref_addr_of<priest_t>(iter);
		}
        
        return nullptr;
	}

	template<entity_type_t EntityType> inline usize entity_registry_t::count() const noexcept {
		if constexpr (EntityType == entity_type_t::Skull) {
			return skulls.size();
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			return skeletons.size();
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			return adventurers.size();
		} else if constexpr (EntityType == entity_type_t::Player) {
			return 1;
		} else if constexpr (EntityType == entity_type_t::None) {
			return 0;
		}
	}

	template<entity_type_t EntityType> inline usize entity_registry_t::empty() const noexcept {
		if constexpr (EntityType == entity_type_t::Skull) {
			return skulls.empty();
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			return skeletons.empty();
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			return adventurers.empty();
		} else if constexpr (EntityType == entity_type_t::Player) {
			return false;
		} else if constexpr (EntityType == entity_type_t::None) {
			return true;
		}
	}

	template<entity_type_t EntityType> inline void entity_registry_t::announce_population() const noexcept {
		if constexpr (EntityType == entity_type_t::Skull) {
			std::cout << "skull positions:\n";

			for (crauto entity : skulls) {
				std::cout << std::format("{}\n", (std::string)entity.position);
			}
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			std::cout << "skeleton positions:\n";

			for (crauto entity : skeletons) {
				std::cout << std::format("{}\n", (std::string)entity.position);
			}
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			std::cout << "adventurer positions:\n";

			for (crauto entity : adventurers) {
				std::cout << std::format("{}\n", (std::string)entity.position);
			}
		} else if constexpr (EntityType == entity_type_t::Player) {
			std::cout << std::format("player position: {}\n", (std::string)player.position);
		} else if constexpr (EntityType == entity_type_t::None) {
			return;
		}
	}

	template<typename T>
		requires is_entity<T>::value && (!is_entity_type<T, entity_type_t::Player>::value)
	inline bool entity_registry_t::add(rval<T> entity) noexcept {
		if (entity_registry.contains(entity.position)) {
			return false;
		}

		if constexpr (is_entity_type<T, entity_type_t::Skull>::value) {
			return skulls.emplace(entity).second;
		} else if constexpr (is_entity_type<T, entity_type_t::Skeleton>::value) {
			return skeletons.emplace(entity).second;
		} else if constexpr (is_entity_type<T, entity_type_t::Adventurer>::value) {
			cauto[iter, inserted]{ adventurers.emplace(entity) };

			if (inserted) {
				evil_goal_map += iter->position;
			}

			return inserted;
		}

		return false;
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::remove(cref<offset_t> position) noexcept {
		if (!entity_registry.contains(position)) {
			return false;
		}

		if constexpr (EntityType == entity_type_t::Skull) {
			return skulls.erase(position);
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			return skeletons.erase(position);
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			const bool removed{ static_cast<bool>(adventurers.erase(position)) };

			if (removed) {
				evil_goal_map -= position;
			}

			return removed;
		}

		return false;
	}

	template<typename T>
		requires is_entity<T>::value && (!is_entity_type<T, entity_type_t::Player>::value)
	inline bool entity_registry_t::spawn(usize count) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(T{ maybe_position.value() });
		}

		return true;
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::update(cref<offset_t> current, cref<offset_t> target) noexcept {
		if (!entity_registry.contains(current) || entity_registry.contains(target)) {
			return false;
		}

		if constexpr (EntityType == entity_type_t::Skull) {
			cauto skull{ skulls.find(current) };

			if (skull == skulls.end()) {
				return false;
			}

			rvauto node{ skulls.extract(skull) };

			node.value().position = target;

			skulls.insert(std::move(node));

			return true;
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			cauto skeleton{ skeletons.find(current) };

			if (skeleton == skeletons.end()) {
				return false;
			}

			rvauto node{ skeletons.extract(skeleton) };

			node.value().position = target;

			skeletons.insert(std::move(node));

			return true;
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			cauto adventurer{ adventurers.find(current) };

			if (adventurer == adventurers.end()) {
				return false;
			}

			rvauto node{ adventurers.extract(adventurer) };

			node.value().position = target;

			adventurers.insert(std::move(node));

			return true;
		}

		return false;
	}

	inline bool entity_registry_t::update(cref<offset_t> current, cref<offset_t> target) noexcept {
		if (!entity_registry.contains(current) || entity_registry.contains(target)) {
			return false;
		}

		const entity_type_t entity_type{ entity_registry.at(current) };

		if (entity_type == entity_type_t::None) {
			return false;
		} else if (entity_type == entity_type_t::Player) {
			player.position = target;

			good_goal_map.update(current, target);
		} else if (entity_type == entity_type_t::Skull) {
			cauto skull{ skulls.find(current) };

			if (skull == skulls.end()) {
				return false;
			}

			rvauto node{ skulls.extract(skull) };

			node.value().position = target;

			skulls.insert(std::move(node));
		} else if (entity_type == entity_type_t::Skeleton) {
			cauto skeleton{ skeletons.find(current) };

			if (skeleton == skeletons.end()) {
				return false;
			}

			rvauto node{ skeletons.extract(skeleton) };

			node.value().position = target;

			skeletons.insert(std::move(node));
		} else if (entity_type == entity_type_t::Adventurer) {
			cauto adventurer{ adventurers.find(current) };

			if (adventurer == adventurers.end()) {
				return false;
			}

			rvauto node{ adventurers.extract(adventurer) };

			node.value().position = target;

			adventurers.insert(std::move(node));

			evil_goal_map.update(current, target);
		}

		return true;
	}

	inline void entity_registry_t::update() noexcept {
		// command priority: player -> adventurers -> skeletons
		// skeletons depend upon the actions of adventurers
		// adventurers depend upon the actions of the player

		// process the player's command

		entity_registry.process_command(player.command);

		std::queue<entity_command_t> commands{};

		// collect and process all adventurer commands

		for (crauto adventurer : adventurers) {
			commands.push(adventurer.think());
		}

		entity_registry.process_commands(commands);

		// collect and process all skeleton commands

		for (crauto skeleton : skeletons) {
			commands.push(skeleton.think());
		}

		entity_registry.process_commands(commands);

		good_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);

		evil_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
	}

	inline void entity_registry_t::process_command(cref<entity_command_t> command) noexcept {
		// a command of none type or a command without a source position will be skipped

		if (command.type == command_type_t::None || !command.source.has_value()) {
			return;
		}

		const offset_t source_position{ command.source.value() };

		// a source command outside of the map interior will be skipped

		if (!game_map.within<zone_region_t::Interior>(source_position)) {
			return;
		}

		const entity_type_t source_type{ entity_registry.at(source_position) };

		// source entities of none/skull type will be skipped

		switch (source_type) {
		case entity_type_t::None:
		case entity_type_t::Skull:
			return;
		default:
			break;
		}

		// only the player can issue warp or consume commands

		if (source_type != entity_type_t::Player) {
			switch (command.type) {
			case command_type_t::Consume:
			case command_type_t::RandomWarp:
			case command_type_t::TargetWarp:
			case command_type_t::ConsumeWarp:
				return;
			default:
				break;
			}
		}

		// a command without a target position will be skipped if and only if the command is not a player random warping

		if (!command.target.has_value()) {
			if (command.type != command_type_t::RandomWarp || source_type != entity_type_t::Player || !player.can_random_warp()) {
				return;
			}

			cauto random_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

			if (!random_position.has_value()) {
				return;
			}

			player.pay_random_warp_cost();

			entity_registry.update(source_position, random_position.value());

			draw_warp_cursor = true;

			return;
		}

		const offset_t target_position{ command.target.value() };

		// a target command outside of the map interior will be skipped

		if (!game_map.within<zone_region_t::Interior>(target_position)) {
			return;
		}

		const entity_type_t target_type{ entity_registry.at(target_position) };

		// commands will be skipped if the entity state is not compatible with the command

		switch (command.type) {
		case command_type_t::Move:
		case command_type_t::TargetWarp:
			// the former commands will be skipped if an entity is present at the target position

			if (entity_registry.contains(target_position) || target_type != entity_type_t::None) {
				return;
			}

			break;
		case command_type_t::Clash:
		case command_type_t::Consume:
		case command_type_t::ConsumeWarp:
			// the former commands will be skipped if an entity isn't present at the target position

			if (!entity_registry.contains(target_position) || target_type == entity_type_t::None) {
				return;
			}

			break;
		default:
			break;
		}

		switch (command.type) {
		case command_type_t::Move:
			entity_registry.update(source_position, target_position);

			return;
		case command_type_t::Consume:
			if (target_type == entity_type_t::Skull) {
				entity_registry.remove<entity_type_t::Skull>(target_position);
				entity_registry.add<skeleton_t>(skeleton_t{ target_position });

				player.receive_skull_boon();

				cauto random_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

				if (!random_position.has_value()) {
					player.receive_failed_warp_boon();

					draw_warp_cursor = false;

					return;
				}

				entity_registry.update(source_position, random_position.value());

				draw_warp_cursor = true;
			} else if (target_type == entity_type_t::Skeleton) {
				entity_registry.remove<entity_type_t::Skeleton>(target_position);

				entity_registry.update(source_position, target_position);

				player.max_out_armor();

				draw_warp_cursor = false;
			}

			return;
		case command_type_t::Clash:
			if (source_type == entity_type_t::Adventurer && target_type == entity_type_t::Player) {
				if (!player.has_armor()) {
					// game over
					return;
				}

				entity_registry.remove<entity_type_t::Adventurer>(source_position);

				evil_goal_map -= source_position;

				entity_registry.add<skull_t>(skull_t{ source_position });

				player.receive_death_boon<entity_type_t::Adventurer>();
				player.receive_damage<entity_type_t::Adventurer>();

				++player_kills;
			} else if (source_type == entity_type_t::Skeleton && target_type == entity_type_t::Adventurer) {
				entity_registry.remove<entity_type_t::Skeleton>(source_position);
				entity_registry.remove<entity_type_t::Adventurer>(target_position);

				entity_registry.add<skull_t>(skull_t{ target_position });

				player.receive_death_boon<entity_type_t::Adventurer>();

				++skeleton_kills;
			} else if (source_type == entity_type_t::Player && target_type == entity_type_t::Adventurer) {
				if (!player.has_armor()) {
					// game over
					return;
				}

				entity_registry.remove<entity_type_t::Adventurer>(target_position);

				entity_registry.add<skull_t>(skull_t{ target_position });

				player.receive_death_boon<entity_type_t::Adventurer>();
				player.receive_damage<entity_type_t::Adventurer>();

				++player_kills;

				draw_warp_cursor = false;
			}

			return;
		case command_type_t::ConsumeWarp:
			if (!player.can_target_warp()) {
				return;
			}

			if (target_type == entity_type_t::Skull) {
				entity_registry.remove<entity_type_t::Skull>(target_position);
				entity_registry.add<skeleton_t>(skeleton_t{ target_position });

				player.pay_target_warp_cost();
				player.receive_skull_boon();

				cauto random_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

				if (!random_position.has_value()) {
					player.receive_failed_warp_boon();
					return;
				}

				entity_registry.update(source_position, random_position.value());

				draw_warp_cursor = true;
			} else if (target_type == entity_type_t::Skeleton) {
				i8 armor_boon = entity_registry.at<skeleton_t>(target_position)->armor_boon();

				entity_registry.remove<entity_type_t::Skeleton>(target_position);

				entity_registry.update(source_position, target_position);

				player.pay_target_warp_cost();
				player.bolster_armor(armor_boon);

				draw_warp_cursor = false;
			}

			return;
		case command_type_t::TargetWarp:
			if (!player.can_target_warp()) {
				return;
			}

			player.pay_target_warp_cost();

			entity_registry.update(source_position, target_position);

			draw_warp_cursor = false;

			return;
		default:
			return;
		}
	}

	inline void entity_registry_t::process_commands(ref<std::queue<entity_command_t>> commands) noexcept {
		while (!commands.empty()) {
			const entity_command_t command{ commands.front() };
			commands.pop();

			entity_registry.process_command(command);
		}
	}

	inline void entity_registry_t::draw() noexcept {
		player.draw();

		for (crauto skull : skulls) {
			skull.draw();
		}

		for (crauto skeleton : skeletons) {
			skeleton.draw();
		}

		for (crauto adventurer : adventurers) {
			adventurer.draw();
		}
	}

	inline void entity_registry_t::draw(cref<camera_t> camera) noexcept {
		player.draw(camera);

		for (crauto skull : skulls) {
			skull.draw(camera);
		}

		for (crauto skeleton : skeletons) {
			skeleton.draw(camera);
		}

		for (crauto adventurer : adventurers) {
			adventurer.draw(camera);
		}
	}
} // namespace necrowarp
