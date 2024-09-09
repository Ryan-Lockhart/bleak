#pragma once

#include "necrowarp/globals.hpp"
#include <necrowarp/entities/entity.hpp>

#include <queue>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct entity_registry_t {
		static inline entity_type_t at(cref<offset_t> position) noexcept;

		template<entity_type_t EntityType> static inline usize count() noexcept;

		template<entity_type_t EntityType> static inline usize empty() noexcept;

		static inline bool contains(cref<offset_t> position) noexcept;

		template<entity_type_t EntityType> static inline bool contains(cref<offset_t> position) noexcept;

		template<entity_type_t EntityType> static inline void announce_population() noexcept;

		template<typename T>
			requires is_entity<T>::value && (!is_entity_type<T, entity_type_t::Player>::value)
		static inline bool add(rval<T> entity) noexcept;

		template<entity_type_t EntityType> static inline bool remove(cref<offset_t> position) noexcept;

		template<typename T>
			requires is_entity<T>::value && (!is_entity_type<T, entity_type_t::Player>::value)
		static inline bool spawn(usize count) noexcept;

		static inline bool update(cref<offset_t> current, cref<offset_t> target) noexcept;

		template<entity_type_t EntityType> static inline bool update(cref<offset_t> current, cref<offset_t> target) noexcept;

		static inline void update() noexcept;

		static inline void draw() noexcept;

		static inline void draw(cref<camera_t> camera) noexcept;
	} static inline entity_registry{};
} // namespace necrowarp

#include <necrowarp/entities/adventurer.hpp>
#include <necrowarp/entities/player.hpp>
#include <necrowarp/entities/skeleton.hpp>
#include <necrowarp/entities/skull.hpp>

namespace necrowarp {
	using namespace bleak;

	static inline player_t player{};

	static inline std::unordered_set<skull_t, skull_t::hasher, skull_t::comparator> skulls{};
	static inline std::unordered_set<skeleton_t, skeleton_t::hasher, skeleton_t::comparator> skeletons{};
	static inline std::unordered_set<adventurer_t, adventurer_t::hasher, adventurer_t::comparator> adventurers{};

	inline bool entity_registry_t::contains(cref<offset_t> position) noexcept { return player.position == position || skulls.contains(position) || skeletons.contains(position) || adventurers.contains(position); }

	template<entity_type_t EntityType> inline bool entity_registry_t::contains(cref<offset_t> position) noexcept {
		if constexpr (EntityType == entity_type_t::Player) {
			return player.position == position;
		} else if constexpr (EntityType == entity_type_t::Skull) {
			return skulls.contains(position);
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			return skeletons.contains(position);
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			return adventurers.contains(position);
		} else {
			return player.position != position && !skulls.contains(position) && !skeletons.contains(position) && !adventurers.contains(position);
		}
	}

	inline entity_type_t entity_registry_t::at(cref<offset_t> position) noexcept {
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

	template<entity_type_t EntityType> inline usize entity_registry_t::count() noexcept {
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

	template<entity_type_t EntityType> inline usize entity_registry_t::empty() noexcept {
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

	template<entity_type_t EntityType> inline void entity_registry_t::announce_population() noexcept {
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
			cauto [iter, inserted] { adventurers.emplace(entity) };

			if (inserted) {
				skeleton_goal_map += iter->position;
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
				skeleton_goal_map -= position;
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

			adventurer_goal_map.update(current, target);
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

			skeleton_goal_map.update(current, target);
		}

		return true;
	}

	inline void entity_registry_t::update() noexcept {
		// command priority: player -> skeletons -> adventurers
		// failure to strictly enforce the command prioritiy will result in an experience unfair to the player

		std::queue<entity_command_t> commands{};

		// collect the player's command

		commands.push(player.command);

		// collect all skeleton commands

		for (crauto skeleton : skeletons) {
			commands.push(skeleton.think());
		}

		// collect all adventurer commands

		for (crauto adventurer : adventurers) {
			commands.push(adventurer.think());
		}

		// iterate over all commands semi-anonymously in order of appendation (use std::stack for reverse order)

		while (!commands.empty()) {
			const entity_command_t current_command{ commands.front() };
			commands.pop();

			// a command of none type or a command without a source position will be skipped

			if (current_command.type == command_type_t::None || !current_command.source.has_value()) {
				continue;
			}

			const offset_t source_position{ current_command.source.value() };

			// a source command outside of the map interior will be skipped

			if (!game_map.within<zone_region_t::Interior>(source_position)) {
				continue;
			}

			const entity_type_t source_type{ entity_registry.at(source_position) };

			// source entities of none/skull type will be skipped

			switch (source_type) {
			case entity_type_t::None:
			case entity_type_t::Skull:
				continue;
			default:
				break;
			}

			// only the player can issue warp or consume commands

			if (source_type != entity_type_t::Player) {
				switch (current_command.type) {
				case command_type_t::Consume:
				case command_type_t::RandomWarp:
				case command_type_t::TargetWarp:
				case command_type_t::ConsumeWarp:
					continue;
				default:
					break;
				}
			}

			// a command without a target position will be skipped if and only if the command is not a player random warping

			if (!current_command.target.has_value()) {
				if (current_command.type != command_type_t::RandomWarp || source_type != entity_type_t::Player || !player.can_random_warp()) {
					continue;
				}

				cauto random_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

				if (!random_position.has_value()) {
					continue;
				}

				player.energy -= globals::RandomWarpCost;

				entity_registry.update(source_position, random_position.value());

				draw_warp_cursor = true;

				continue;
			}

			const offset_t target_position{ current_command.target.value() };

			// a target command outside of the map interior will be skipped

			if (!game_map.within<zone_region_t::Interior>(target_position)) {
				continue;
			}

			const entity_type_t target_type{ entity_registry.at(target_position) };

			// commands will be skipped if the entity state is not compatible with the command

			switch (current_command.type) {
			case command_type_t::Move:
			case command_type_t::TargetWarp:
				// the former commands will be skipped if an entity is present at the target position

				if (entity_registry.contains(target_position) || target_type != entity_type_t::None) {
					continue;
				}

				break;
			case command_type_t::Clash:
			case command_type_t::Consume:
			case command_type_t::ConsumeWarp:
				// the former commands will be skipped if an entity isn't present at the target position

				if (!entity_registry.contains(target_position) || target_type == entity_type_t::None) {
					continue;
				}

				break;
			default:
				break;
			}

			switch (current_command.type) {
			case command_type_t::Move:
				entity_registry.update(source_position, target_position);

				continue;
			case command_type_t::Consume:
				if (target_type == entity_type_t::Skull) {
					entity_registry.remove<entity_type_t::Skull>(target_position);
					entity_registry.add<skeleton_t>(skeleton_t{ target_position });

					player.energy += globals::SkullBoon;

					cauto random_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

					if (!random_position.has_value()) {
						player.energy += globals::FailedWarpBoon;

						draw_warp_cursor = false;

						continue;
					}

					entity_registry.update(source_position, random_position.value());

					draw_warp_cursor = true;
				} else if (target_type == entity_type_t::Skeleton) {
					entity_registry.remove<entity_type_t::Skeleton>(target_position);

					entity_registry.update(source_position, target_position);

					player.armor = globals::MaximumArmor;

					draw_warp_cursor = false;
				}

				continue;
			case command_type_t::Clash:
				if (source_type == entity_type_t::Adventurer && target_type == entity_type_t::Player) {
					if (!player.has_armor()) {
						// game over
						continue;
					}

					entity_registry.remove<entity_type_t::Adventurer>(source_position);

					skeleton_goal_map -= source_position;

					entity_registry.add<skull_t>(skull_t{ source_position });

					player.energy += globals::PlayerKillBoon;
					player.armor -= globals::ClashArmorDamage;
					
					++player_kills;
				} else if (source_type == entity_type_t::Skeleton && target_type == entity_type_t::Adventurer) {
					entity_registry.remove<entity_type_t::Skeleton>(source_position);
					entity_registry.remove<entity_type_t::Adventurer>(target_position);

					entity_registry.add<skull_t>(skull_t{ target_position });

					player.energy += globals::SkeletonKillBoon;
					
					++skeleton_kills;
				} else if (source_type == entity_type_t::Player && target_type == entity_type_t::Adventurer) {
					if (!player.has_armor()) {
						// game over
						continue;
					}

					entity_registry.remove<entity_type_t::Adventurer>(target_position);

					entity_registry.add<skull_t>(skull_t{ target_position });

					player.energy += globals::PlayerKillBoon;
					player.armor -= globals::ClashArmorDamage;
					
					++player_kills;

					draw_warp_cursor = false;
				}

				continue;
			case command_type_t::ConsumeWarp:
				if (!player.can_target_warp()) {
					continue;
				}

				if (target_type == entity_type_t::Skull) {
					entity_registry.remove<entity_type_t::Skull>(target_position);
					entity_registry.add<skeleton_t>(skeleton_t{ target_position });

					player.energy -= globals::TargetWarpCost + globals::SkullBoon;

					cauto random_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

					if (!random_position.has_value()) {
						player.energy += globals::FailedWarpBoon;
						continue;
					}

					entity_registry.update(source_position, random_position.value());

					draw_warp_cursor = true;
				} else if (target_type == entity_type_t::Skeleton) {
					entity_registry.remove<entity_type_t::Skeleton>(target_position);

					player.energy -= globals::TargetWarpCost;
					player.armor = globals::MaximumArmor;

					entity_registry.update(source_position, target_position);

					draw_warp_cursor = false;
				}

				continue;
			case command_type_t::TargetWarp:
				if (!player.can_target_warp()) {
					continue;
				}

				player.energy -= globals::TargetWarpCost;

				entity_registry.update(source_position, target_position);

				draw_warp_cursor = false;

				continue;
			default:
				continue;
			}
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
