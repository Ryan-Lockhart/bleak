#pragma once

#include "necrowarp/entity_state.hpp"
#include "necrowarp/game_state.hpp"
#include <necrowarp/entities.hpp>

#include <cstddef>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace necrowarp {
	using namespace bleak;

	static inline player_t player{};

	static inline std::unordered_set<skull_t, skull_t::hasher, skull_t::comparator> skulls{};

	static inline std::unordered_set<skeleton_t, skeleton_t::hasher, skeleton_t::comparator> skeletons{};
	static inline std::unordered_set<wraith_t, wraith_t::hasher, wraith_t::comparator> wraithes{};

	static inline std::unordered_set<ladder_t, ladder_t::hasher, ladder_t::comparator> ladders{};

	static inline std::unordered_set<adventurer_t, adventurer_t::hasher, adventurer_t::comparator> adventurers{};
	static inline std::unordered_set<paladin_t, paladin_t::hasher, paladin_t::comparator> paladins{};
	static inline std::unordered_set<priest_t, priest_t::hasher, priest_t::comparator> priests{};

	inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept {
		return player.position == position || skulls.contains(position) || skeletons.contains(position) || wraithes.contains(position) || ladders.contains(position) || adventurers.contains(position) || paladins.contains(position) || priests.contains(position);
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
		} else if constexpr (EntityType == entity_type_t::Ladder) {
			return ladders.contains(position);
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			return adventurers.contains(position);
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			return paladins.contains(position);
		} else if constexpr (EntityType == entity_type_t::Priest) {
			return priests.contains(position);
		}
	}

	inline entity_type_t entity_registry_t::at(cref<offset_t> position) const noexcept {
		if (player.position == position) {
			return entity_type_t::Player;
		} else if (skulls.contains(position)) {
			return entity_type_t::Skull;
		} else if (skeletons.contains(position)) {
			return entity_type_t::Skeleton;
		} else if (wraithes.contains(position)) {
			return entity_type_t::Wraith;
		} else if (ladders.contains(position)) {
			return entity_type_t::Ladder;
		} else if (adventurers.contains(position)) {
			return entity_type_t::Adventurer;
		} else if (paladins.contains(position)) {
			return entity_type_t::Paladin;
		} else if (priests.contains(position)) {
			return entity_type_t::Priest;
		} else {
			return entity_type_t::None;
		}
	}

	template<typename T>
		requires is_entity<T>::value
	inline cptr<T> entity_registry_t::at(cref<offset_t> position) const noexcept {
		if constexpr (std::is_same<T, std::nullptr_t>::value) {
			return nullptr;
		} else if constexpr (std::is_same<T, player_t>::value) {
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
		} else if constexpr (std::is_same<T, ladder_t>::value) {
			cauto iter{ ladders.find(position) };

			if (iter == ladders.end()) {
				return nullptr;
			}

			return deref_addr_of<ladder_t>(iter);
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

	inline usize entity_registry_t::count() const noexcept { return 1 + skulls.size() + skeletons.size() + wraithes.size() + ladders.size() + adventurers.size() + paladins.size() + priests.size(); }

	template<entity_type_t EntityType> inline usize entity_registry_t::count() const noexcept {
		if constexpr (EntityType == entity_type_t::None) {
			return game_map.zone_size.area() - entity_registry.count();
		} else if constexpr (EntityType == entity_type_t::Player) {
			return 1;
		} else if constexpr (EntityType == entity_type_t::Skull) {
			return skulls.size();
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			return skeletons.size();
		} else if constexpr (EntityType == entity_type_t::Wraith) {
			return wraithes.size();
		} else if constexpr (EntityType == entity_type_t::Ladder) {
			return ladders.size();
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			return adventurers.size();
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			return paladins.size();
		} else if constexpr (EntityType == entity_type_t::Priest) {
			return priests.size();
		}
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::empty() const noexcept {
		if constexpr (EntityType == entity_type_t::None) {
			return false;
		} else if constexpr (EntityType == entity_type_t::Player) {
			return false;
		} else if constexpr (EntityType == entity_type_t::Skull) {
			return skulls.empty();
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			return skeletons.empty();
		} else if constexpr (EntityType == entity_type_t::Wraith) {
			return wraithes.empty();
		} else if constexpr (EntityType == entity_type_t::Ladder) {
			return ladders.empty();
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			return adventurers.empty();
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			return paladins.empty();
		} else if constexpr (EntityType == entity_type_t::Priest) {
			return priests.empty();
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
		// command priority: player -> skeletons -> adventurers

		// process the player's command

		entity_registry.process_command(player.command);

		std::queue<entity_command_t> commands{};

		// collect and process all skeleton commands

		for (crauto skeleton : skeletons) {
			commands.push(skeleton.think());
		}

		entity_registry.process_commands(commands);

		good_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);

		evil_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);

		// collect and process all adventurer commands

		for (crauto adventurer : adventurers) {
			commands.push(adventurer.think());
		}

		entity_registry.process_commands(commands);

		good_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);

		evil_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
	}

	inline bool entity_registry_t::is_command_valid(cref<entity_command_t> command) const noexcept {
		if (command.type == command_type_t::None || !command.source.has_value()) {
			return false;
		}

		const offset_t source_position{ command.source.value() };

		if (!game_map.within<zone_region_t::Interior>(source_position)) {
			return false;
		}

		const entity_type_t source_type{ entity_registry.at(source_position) };

		switch (source_type) {
		case entity_type_t::None:
		case entity_type_t::Skull:
			return false;
		default:
			break;
		}

		if (source_type != entity_type_t::Player) {
			switch (command.type) {
			case command_type_t::Consume:
			case command_type_t::RandomWarp:
			case command_type_t::TargetWarp:
			case command_type_t::ConsumeWarp:
				return false;
			default:
				break;
			}
		}

		switch (command.type) {
		case command_type_t::RandomWarp:
		case command_type_t::SummonWraith:
		case command_type_t::GrandSummoning:
			return true;
		default:
			break;
		}

		const offset_t target_position{ command.target.value() };

		if (!game_map.within<zone_region_t::Interior>(target_position)) {
			return false;
		}

		const entity_type_t target_type{ entity_registry.at(target_position) };

		switch (command.type) {
		case command_type_t::Move:
		case command_type_t::TargetWarp:
			if (entity_registry.contains(target_position) || target_type != entity_type_t::None) {
				return false;
			}

			break;
		case command_type_t::Clash:
		case command_type_t::Consume:
		case command_type_t::ConsumeWarp:
			if (!entity_registry.contains(target_position) || target_type == entity_type_t::None) {
				return false;
			}

			break;
		default:
			break;
		}

		return true;
	}

	inline void entity_registry_t::random_warp(cref<offset_t> source) noexcept {
		cauto random_safe_position{ evil_goal_map.find_random<zone_region_t::Interior>(game_map, random_engine, cell_trait_t::Open, entity_registry, 8) };

		if (!random_safe_position.has_value()) {
			cauto random_unsafe_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

			if (!random_unsafe_position.has_value()) {
				player.receive_failed_warp_boon();

				draw_warp_cursor = false;

				return;
			}
			
			player.receive_unsafe_warp_boon();

			entity_registry.update(source, random_unsafe_position.value());
			
			draw_warp_cursor = true;

			return;
		}

		entity_registry.update(source, random_safe_position.value());

		draw_warp_cursor = true;
	}

	template<> void entity_registry_t::process_command<command_type_t::None>(cref<entity_command_t> command) noexcept {}

	template<> void entity_registry_t::process_command<command_type_t::Move>(cref<entity_command_t> command) noexcept { entity_registry.update(command.source.value(), command.target.value()); }

	template<> void entity_registry_t::process_command<command_type_t::Consume>(cref<entity_command_t> command) noexcept {
		const offset_t source_position{ command.source.value() };

		const offset_t target_position{ command.target.value() };

		const entity_type_t target_type{ entity_registry.at(target_position) };

		if (target_type == entity_type_t::Skull) {
			entity_registry.remove<entity_type_t::Skull>(target_position);
			entity_registry.add<skeleton_t>(skeleton_t{ target_position });

			player.receive_skull_boon();

			random_warp(source_position);
		} else if (target_type == entity_type_t::Skeleton) {
			entity_registry.remove<entity_type_t::Skeleton>(target_position);

			entity_registry.update(source_position, target_position);

			player.max_out_armor();

			draw_warp_cursor = false;
		}
	}

	template<> inline void entity_registry_t::process_clash<entity_type_t::Adventurer>(cref<offset_t> source, cref<offset_t> target) noexcept {}

	template<> inline void entity_registry_t::process_command<command_type_t::Clash>(cref<entity_command_t> command) noexcept {
		const offset_t source_position{ command.source.value() };

		const entity_type_t source_type{ entity_registry.at(source_position) };

		const offset_t target_position{ command.target.value() };

		const entity_type_t target_type{ entity_registry.at(target_position) };

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
	}

	template<> void entity_registry_t::process_command<command_type_t::SummonWraith>(cref<entity_command_t> command) noexcept {
		if (!player.can_summon_wraith()) {
			return;
		}

		player.pay_summon_wraith_cost();
	}

	template<> void entity_registry_t::process_command<command_type_t::GrandSummoning>(cref<entity_command_t> command) noexcept {
		if (!player.can_grand_summon() || !skulls.empty()) {
			return;
		}

		std::vector<offset_t> skull_positions{};

		for (crauto skull : skulls) {
			skull_positions.push_back(skull.position);
		}

		skulls.clear();

		for (rvauto position : skull_positions) {
			entity_registry_t::add(skeleton_t{ std::move(position) });
		}
	}

	template<> void entity_registry_t::process_command<command_type_t::ConsumeWarp>(cref<entity_command_t> command) noexcept {
		const offset_t target_position{ command.target.value() };

		const entity_type_t target_type{ entity_registry.at(target_position) };

		if (target_type != entity_type_t::Skull ? !player.can_target_warp() : !player.can_target_warp(player_t::SkullBoon)) {
			return;
		}

		const offset_t source_position{ command.source.value() };

		switch (target_type) {
		case entity_type_t::Skull: {
			entity_registry.remove<entity_type_t::Skull>(target_position);
			entity_registry.add<skeleton_t>(skeleton_t{ target_position });

			player.pay_target_warp_cost(player_t::SkullBoon);

			random_warp(source_position);

			return;
		}
		case entity_type_t::Skeleton: {
			entity_registry.remove<entity_type_t::Skeleton>(target_position);

			entity_registry.update(source_position, target_position);

			player.pay_target_warp_cost();
			player.bolster_armor(skeleton_t::ArmorBoon);

			draw_warp_cursor = false;
			return;
		}
		case entity_type_t::Wraith: {
			const i8 armor_boon = entity_registry.at<wraith_t>(target_position)->armor_boon();

			entity_registry.remove<entity_type_t::Wraith>(target_position);

			entity_registry.update(source_position, target_position);

			player.pay_target_warp_cost();
			player.bolster_armor(armor_boon);

			draw_warp_cursor = false;
			return;
		}
		default:
			return;
		}
	}

	template<> void entity_registry_t::process_command<command_type_t::TargetWarp>(cref<entity_command_t> command) noexcept {
		if (!player.can_target_warp()) {
			return;
		}

		player.pay_target_warp_cost();

		entity_registry.update(command.source.value(), command.target.value());

		draw_warp_cursor = false;
	}

	template<> void entity_registry_t::process_command<command_type_t::RandomWarp>(cref<entity_command_t> command) noexcept {
		if (!player.can_random_warp()) {
			return;
		}

		player.pay_random_warp_cost();
		
		random_warp(command.source.value());
	}

	inline void entity_registry_t::process_command(cref<entity_command_t> command) noexcept {
		if (!is_command_valid(command)) {
			return;
		}

		switch (command.type) {
		case command_type_t::Move: {
			return process_command<command_type_t::Move>(command);
		}
		case command_type_t::Consume: {
			return process_command<command_type_t::Consume>(command);
		}
		case command_type_t::Clash: {
			return process_command<command_type_t::Clash>(command);
		}
		case command_type_t::ConsumeWarp: {
			return process_command<command_type_t::ConsumeWarp>(command);
		}
		case command_type_t::TargetWarp: {
			return process_command<command_type_t::TargetWarp>(command);
		}
		case command_type_t::RandomWarp: {
			return process_command<command_type_t::RandomWarp>(command);
		}
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
