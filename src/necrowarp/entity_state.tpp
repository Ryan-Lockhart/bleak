#pragma once

#include <necrowarp/entities.hpp>

#include <cstddef>
#include <type_traits>
#include <unordered_set>

namespace necrowarp {
	using namespace bleak;

	static inline player_t player{};

	template<typename T> static inline std::unordered_set<T, typename T::hasher, typename T::comparator> entity_storage{};

	template<typename... EntityTypes>
		requires((is_entity<EntityTypes>::value && ...) && !(is_entity_type<EntityTypes, entity_type_t::Player>::value && ...))
	inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept {
		return (entity_storage<EntityTypes>.contains(position) || ...);
	}

	inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept { return player.position == position || contains<ALL_NON_PLAYER>(position); }

	template<entity_type_t EntityType> inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept {
		if constexpr (EntityType == entity_type_t::None) {
			return true;
		} else if constexpr (EntityType == entity_type_t::Player) {
			return player.position == position;
		} else {
			return entity_storage<typename to_entity_type<EntityType>::type>.contains(position);
		}
	}

	inline entity_type_t entity_registry_t::at(cref<offset_t> position) const noexcept {
		if (player.position == position) {
			return entity_type_t::Player;
		} else if (entity_storage<skeleton_t>.contains(position)) {
			return entity_type_t::Skeleton;
		} else if (entity_storage<wraith_t>.contains(position)) {
			return entity_type_t::Wraith;
		} else if (entity_storage<adventurer_t>.contains(position)) {
			return entity_type_t::Adventurer;
		} else if (entity_storage<paladin_t>.contains(position)) {
			return entity_type_t::Paladin;
		} else if (entity_storage<priest_t>.contains(position)) {
			return entity_type_t::Priest;
		} else if (entity_storage<skull_t>.contains(position)) {
			return entity_type_t::Skull;
		} else if (entity_storage<ladder_t>.contains(position)) {
			return entity_type_t::Ladder;
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
		}

		cauto iter{ entity_storage<T>.find(position) };

		if (iter == entity_storage<T>.end()) {
			return nullptr;
		}

		return deref_addr_of<T>(iter);
	}

	template<typename... EntityTypes>
		requires((is_entity<EntityTypes>::value && ...) && !(is_entity_type<EntityTypes, entity_type_t::Player>::value && ...))
	inline usize entity_registry_t::count() const noexcept {
		return (entity_storage<EntityTypes>.size() + ...);
	}

	inline usize entity_registry_t::count() const noexcept { return count<ALL_NON_PLAYER>() + 1; }

	template<entity_type_t EntityType> inline usize entity_registry_t::count() const noexcept {
		if constexpr (EntityType == entity_type_t::None) {
			return game_map.zone_size.area() - entity_registry.count();
		} else if constexpr (EntityType == entity_type_t::Player) {
			return 1;
		} else if constexpr (EntityType == entity_type_t::Skull) {
			return entity_storage<skull_t>.size();
		} else if constexpr (EntityType == entity_type_t::Skeleton) {
			return entity_storage<skeleton_t>.size();
		} else if constexpr (EntityType == entity_type_t::Wraith) {
			return entity_storage<wraith_t>.size();
		} else if constexpr (EntityType == entity_type_t::Ladder) {
			return entity_storage<ladder_t>.size();
		} else if constexpr (EntityType == entity_type_t::Adventurer) {
			return entity_storage<adventurer_t>.size();
		} else if constexpr (EntityType == entity_type_t::Paladin) {
			return entity_storage<paladin_t>.size();
		} else if constexpr (EntityType == entity_type_t::Priest) {
			return entity_storage<priest_t>.size();
		}
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::empty() const noexcept {
		if constexpr (EntityType == entity_type_t::None) {
			return false;
		} else if constexpr (EntityType == entity_type_t::Player) {
			return false;
		}

		using entity_type = typename to_entity_type<EntityType>::type;

		return entity_storage<entity_type>.empty();
	}

	template<typename T>
		requires is_entity<T>::value && (!is_entity_type<T, entity_type_t::Player>::value)
	inline bool entity_registry_t::add(rval<T> entity) noexcept {
		if (entity_registry.contains(entity.position)) {
			return false;
		}

		cauto[iter, inserted]{ entity_storage<T>.emplace(std::move(entity)) };

		if (inserted) {
			if constexpr (is_evil_entity<T>::value) {
				good_goal_map += iter->position;
			} else if constexpr (is_good_entity<T>::value) {
				evil_goal_map += iter->position;
			}
		}

		return inserted;
	}

	template<typename T, bool Force>
		requires is_entity<T>::value && (!is_entity_type<T, entity_type_t::Player>::value)
	inline bool entity_registry_t::add(rval<T> entity) noexcept {
		if constexpr (!Force) {
			if (entity_registry.contains(entity.position)) {
			return false;
			}
		}		

		cauto[iter, inserted]{ entity_storage<T>.emplace(std::move(entity)) };

		if (inserted) {
			if constexpr (is_evil_entity<T>::value) {
				good_goal_map += iter->position;
			} else if constexpr (is_good_entity<T>::value) {
				evil_goal_map += iter->position;
			}
		}

		return inserted;
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::remove(cref<offset_t> position) noexcept {
		if (!entity_registry.contains(position)) {
			return false;
		}

		using entity_type = typename to_entity_type<EntityType>::type;

		const bool removed{ static_cast<bool>(entity_storage<entity_type>.erase(position)) };

		if (removed) {
			if constexpr (is_evil_entity<entity_type>::value) {
				good_goal_map -= position;
			} else if constexpr (is_good_entity<entity_type>::value) {
				evil_goal_map -= position;
			}
		}

		return removed;
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

	inline bool entity_registry_t::update(cref<offset_t> current, cref<offset_t> target) noexcept {
		if (!entity_registry.contains(current) || entity_registry.contains(target)) {
			return false;
		}

		cauto entity_type{ entity_registry.at(current) };

		switch (entity_type) {
		case entity_type_t::None:
		case entity_type_t::Skull:
		case entity_type_t::Ladder:
			return false;
		case entity_type_t::Player:
			player.position = target;
			good_goal_map.update(current, target);
			return true;
		case entity_type_t::Skeleton:
			return update<entity_type_t::Skeleton>(current, target);
		case entity_type_t::Wraith:
			return update<entity_type_t::Wraith>(current, target);
		case entity_type_t::Adventurer:
			return update<entity_type_t::Adventurer>(current, target);
		case entity_type_t::Paladin:
			return update<entity_type_t::Paladin>(current, target);
		case entity_type_t::Priest:
			return update<entity_type_t::Priest>(current, target);
		default:
			return false;
		}
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::update(cref<offset_t> current, cref<offset_t> target) noexcept {
		if (!entity_registry.contains(current) || !game_map.within<zone_region_t::Interior>(current) || entity_registry.contains(target) || !game_map.within<zone_region_t::Interior>(target)) {
			return false;
		}

		if constexpr (EntityType == entity_type_t::None || EntityType == entity_type_t::Player) {
			return false;
		}

		using entity_type = typename to_entity_type<EntityType>::type;

		cauto entity{ entity_storage<entity_type>.find(current) };

		if (entity == entity_storage<entity_type>.end()) {
			return false;
		}

		rvauto node{ entity_storage<entity_type>.extract(entity) };

		node.value().position = target;

		entity_storage<entity_type>.insert(std::move(node));

		if constexpr (is_evil_entity<entity_type>::value) {
			good_goal_map.update(current, target);
		} else if constexpr (is_good_entity<entity_type>::value) {
			evil_goal_map.update(current, target);
		}

		return true;
	}

	template<typename... EntityTypes>
		requires((is_entity<EntityTypes>::value && ...) && !(is_entity_type<EntityTypes, entity_type_t::Player>::value && ...))
	inline void entity_registry_t::update(ref<std::queue<entity_command_t>> commands) noexcept {
		auto process_entities{ [&commands](crauto entities) {
			for (crauto entity : entities) {
				commands.push(entity.think());
			}

			entity_registry.process_commands(commands);

			good_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);

			evil_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
		} };

		(process_entities(entity_storage<EntityTypes>), ...);
	}

	inline void entity_registry_t::update() noexcept {
		// command priority: player -> skeletons -> wraithes -> adventurers -> paladins -> priests

		// process the player's command

		entity_registry.process_command(player.command);

		std::queue<entity_command_t> commands{};

		update<ALL_NPCS>(commands);
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

	inline bool entity_registry_t::random_warp(cref<offset_t> source) noexcept {
		cauto random_safe_position{ evil_goal_map.find_random<zone_region_t::Interior>(game_map, random_engine, cell_trait_t::Open, entity_registry, 8) };

		if (!random_safe_position.has_value()) {
			cauto random_unsafe_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

			if (!random_unsafe_position.has_value()) {
				player.receive_failed_warp_boon();

				draw_warp_cursor = false;

				return false;
			}

			player.receive_unsafe_warp_boon();

			entity_registry.update(source, random_unsafe_position.value());

			draw_warp_cursor = true;

			return true;
		}

		entity_registry.update(source, random_safe_position.value());

		draw_warp_cursor = true;

		return true;
	}

	template<> void entity_registry_t::process_command<command_type_t::None>(cref<entity_command_t> command) noexcept {}

	template<> void entity_registry_t::process_command<command_type_t::Move>(cref<entity_command_t> command) noexcept { entity_registry.update(command.source.value(), command.target.value()); }

	template<> void entity_registry_t::process_command<command_type_t::Consume>(cref<entity_command_t> command) noexcept {
		const offset_t source_position{ command.source.value() };

		const offset_t target_position{ command.target.value() };

		const entity_type_t target_type{ entity_registry.at(target_position) };

		if (target_type == entity_type_t::Skull) {
			const bool is_fresh{ entity_registry.at<skull_t>(target_position)->fresh };

			entity_registry.remove<entity_type_t::Skull>(target_position);
			entity_registry.add<skeleton_t>(skeleton_t{ target_position });

			if (is_fresh) {
				player.receive_skull_boon();
			}

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
			entity_registry.add<skull_t>(skull_t{ source_position, true });

			player.receive_death_boon<entity_type_t::Adventurer>();
			player.receive_damage<entity_type_t::Adventurer>();

			++player_kills;
		} else if (source_type == entity_type_t::Skeleton && target_type == entity_type_t::Adventurer) {
			entity_registry.remove<entity_type_t::Skeleton>(source_position);
			entity_registry.add<skull_t>(skull_t{ source_position, false });

			entity_registry.remove<entity_type_t::Adventurer>(target_position);
			entity_registry.add<skull_t>(skull_t{ target_position, true });

			player.receive_death_boon<entity_type_t::Adventurer>();

			++skeleton_kills;
		} else if (source_type == entity_type_t::Player && target_type == entity_type_t::Adventurer) {
			if (!player.has_armor()) {
				// game over
				return;
			}

			entity_registry.remove<entity_type_t::Adventurer>(target_position);
			entity_registry.add<skull_t>(skull_t{ target_position, true });

			player.receive_death_boon<entity_type_t::Adventurer>();
			player.receive_damage<entity_type_t::Adventurer>();

			++player_kills;

			draw_warp_cursor = false;
		}
	}

	template<> void entity_registry_t::process_command<command_type_t::SummonWraith>(cref<entity_command_t> command) noexcept {
		if (!player.can_summon_wraith() || entity_storage<skull_t>.empty()) {
			return;
		}

		i8 wraith_health{ 0 };

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t position{ player.position + offset };

			if (!game_map.within<zone_region_t::Interior>(position) || entity_registry.at(position) != entity_type_t::Skull) {
				continue;
			}

			entity_registry.remove<entity_type_t::Skull>(position);

			++wraith_health;
		}

		if (wraith_health <= 0) {
			return;
		}

		player.pay_summon_wraith_cost();

		if (!random_warp(command.source.value())) {
			player.bolster_armor(wraith_health);

			return;
		}

		entity_registry.add<wraith_t>(wraith_t{ command.source.value(), wraith_health });
	}

	template<> void entity_registry_t::process_command<command_type_t::GrandSummoning>(cref<entity_command_t> command) noexcept {
		if (!player.can_grand_summon() || entity_storage<skull_t>.empty()) {
			return;
		}

		std::vector<offset_t> skull_positions{};

		for (crauto skull : entity_storage<skull_t>) {
			skull_positions.push_back(skull.position);
		}

		entity_storage<skull_t>.clear();

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
			const i8 armor_boon = entity_registry.at<skeleton_t>(target_position)->armor_boon();

			entity_registry.remove<entity_type_t::Skeleton>(target_position);

			entity_registry.update(source_position, target_position);

			player.pay_target_warp_cost();
			player.bolster_armor(armor_boon);

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
		case command_type_t::SummonWraith: {
			return process_command<command_type_t::SummonWraith>(command);
		}
		case command_type_t::GrandSummoning: {
			return process_command<command_type_t::GrandSummoning>(command);
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
			const entity_command_t command{ std::move(commands.front()) };
			commands.pop();

			entity_registry.process_command(command);
		}
	}

	template<typename... EntityTypes>
		requires((is_entity<EntityTypes>::value && ...) && !(is_entity_type<EntityTypes, entity_type_t::Player>::value && ...))
	inline void entity_registry_t::draw() const noexcept {
		auto draw_entities{ [](crauto entities) {
			for (crauto entity : entities) {
				entity.draw();
			}
		} };

		(draw_entities(entity_storage<EntityTypes>), ...);
	}

	template<typename... EntityTypes>
		requires((is_entity<EntityTypes>::value && ...) && !(is_entity_type<EntityTypes, entity_type_t::Player>::value && ...))
	inline void entity_registry_t::draw(cref<camera_t> camera) const noexcept {
		auto draw_entities{ [&camera](crauto entities) {
			for (crauto entity : entities) {
				entity.draw(camera);
			}
		} };

		(draw_entities(entity_storage<EntityTypes>), ...);
	}

	inline void entity_registry_t::draw() const noexcept {
		player.draw();

		draw<ALL_NON_PLAYER>();
	}

	inline void entity_registry_t::draw(cref<camera_t> camera) const noexcept {
		player.draw(camera);

		draw<ALL_NON_PLAYER>(camera);
	}
} // namespace necrowarp
