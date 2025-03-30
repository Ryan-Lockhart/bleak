#pragma once

#include "necrowarp/game_state.hpp"
#include <necrowarp/entities.hpp>

#include <cstddef>
#include <optional>
#include <type_traits>

#include <bleak/sparse.hpp>

namespace necrowarp {
	constexpr distance_function_t DistanceFunction{ distance_function_t::Octile };
	
	extern grid_cursor_t<globals::CellSize> warp_cursor;

	extern bool draw_cursor;
	extern bool draw_warp_cursor;
	
	using namespace bleak;

	static inline player_t player{};

	static inline bool update_camera() noexcept {
		bool force_width{ globals::MapSize.w <= globals::game_grid_size().w };
		bool force_height{ globals::MapSize.h <= globals::game_grid_size().h };

		if (force_width || force_height) {
			return camera.center_on(
				force_width, force_width ? globals::MapCenter.x : player.position.x,
				force_height, force_height ? globals::MapCenter.y : player.position.y
			);
		}

		if (camera_locked) {
			return camera.center_on(player.position);
		}

		return false;
	}

	template<NonPlayerEntity EntityType> static inline sparse_t<EntityType> entity_storage{};

	static inline sparse_t<sparseling_t<bool>> newborns{};

	static inline field_t<float, DistanceFunction, globals::MapSize, globals::BorderSize> good_goal_map{};
	static inline field_t<float, DistanceFunction, globals::MapSize, globals::BorderSize> evil_goal_map{};

	template<typename T> static inline field_t<float, DistanceFunction, globals::MapSize, globals::BorderSize> entity_goal_map{};

	static inline bool descent_flag{ false };

	template<entity_type_t EntityType> inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept {
		if constexpr (EntityType == entity_type_t::None) {
			return true;
		} else if constexpr (EntityType == entity_type_t::Player) {
			return player.position == position;
		} else {
			return entity_storage<to_entity_type_t<EntityType>>.contains(position);
		}
	}

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept {
		return entity_storage<EntityType>.contains(position);
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept {
		return (contains<EntityTypes>(position) || ...);
	}

	inline bool entity_registry_t::contains(cref<offset_t> position) const noexcept { return player.position == position || contains<ALL_NON_PLAYER>(position); }

	inline entity_type_t entity_registry_t::at(cref<offset_t> position) const noexcept {
		if (player.position == position) {
			return entity_type_t::Player;
		} else if (entity_storage<skull_t>.contains(position)) {
			return entity_type_t::Skull;
		} else if (entity_storage<skeleton_t>.contains(position)) {
			return entity_type_t::Skeleton;
		} else if (entity_storage<wraith_t>.contains(position)) {
			return entity_type_t::Wraith;
		} else if (entity_storage<flesh_golem_t>.contains(position)) {
			return entity_type_t::FleshGolem;
		} else if (entity_storage<adventurer_t>.contains(position)) {
			return entity_type_t::Adventurer;
		} else if (entity_storage<paladin_t>.contains(position)) {
			return entity_type_t::Paladin;
		} else if (entity_storage<priest_t>.contains(position)) {
			return entity_type_t::Priest;
		} else if (entity_storage<ladder_t>.contains(position)) {
			return entity_type_t::Ladder;
		} else {
			return entity_type_t::None;
		}
	}

	template<Entity EntityType> inline ptr<EntityType> entity_registry_t::at(cref<offset_t> position) noexcept {
		if constexpr (std::is_same<EntityType, std::nullptr_t>::value) {
			return nullptr;
		} else if constexpr (std::is_same<EntityType, player_t>::value) {
			if (player.position != position) {
				return nullptr;
			}

			return &player;
		} else {
			return entity_storage<EntityType>[position];
		}
	}

	template<Entity EntityType> inline cptr<EntityType> entity_registry_t::at(cref<offset_t> position) const noexcept {
		if constexpr (std::is_same<EntityType, std::nullptr_t>::value) {
			return nullptr;
		} else if constexpr (std::is_same<EntityType, player_t>::value) {
			if (player.position != position) {
				return nullptr;
			}

			return &player;
		} else {
			return entity_storage<EntityType>[position];
		}
	}

	template<entity_type_t EntityType> inline usize entity_registry_t::count() const noexcept {
		if constexpr (EntityType == entity_type_t::None) {
			return game_map.zone_size.area() - entity_registry.count();
		} else if constexpr (EntityType == entity_type_t::Player) {
			return 1;
		} else {
			return entity_storage<to_entity_type_t<EntityType>>.size();
		}
	}

	template<NonPlayerEntity EntityTypes> inline usize entity_registry_t::count() const noexcept {
		return entity_storage<EntityTypes>.size();
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline usize entity_registry_t::count() const noexcept {
		return (count<EntityTypes>() + ...);
	}

	inline usize entity_registry_t::count() const noexcept { return count<ALL_NON_PLAYER>() + 1; }

	template<entity_type_t EntityType> inline bool entity_registry_t::empty() const noexcept {
		if constexpr (EntityType == entity_type_t::None) {
			return false;
		} else if constexpr (EntityType == entity_type_t::Player) {
			return false;
		}

		return entity_storage<to_entity_type_t<EntityType>>.empty();
	}
	
	template<NonPlayerEntity EntityType> inline bool entity_registry_t::empty() const noexcept {
		return entity_storage<EntityType>.empty();
	}
	
	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline bool entity_registry_t::empty() const noexcept {
		return (empty<EntityTypes>() && ...);
	}

	template<bool Force, NonPlayerEntity EntityType> inline bool entity_registry_t::add(rval<EntityType> entity) noexcept {
		if constexpr (!Force) {
			if (entity_registry.contains(entity.position)) {
				return false;
			}
		}
		
		const offset_t position{ entity.position };
		const bool inserted{ entity_storage<EntityType>.add(std::move(entity)) };

		if (inserted) {
			if constexpr (is_evil_entity<EntityType>::value) {
				good_goal_map.add(position);
			} else if constexpr (is_good_entity<EntityType>::value) {
				evil_goal_map.add(position);
			}

			entity_goal_map<EntityType>.add(position);

			if constexpr (is_animate<EntityType>::value) {
				newborns.add(sparseling_t<bool>{ position });
			}
		}

		return inserted;
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::remove(cref<offset_t> position) noexcept {
		if constexpr (EntityType == entity_type_t::None || EntityType == entity_type_t::Player) {
			return false;
		} else {
			if (!entity_registry.contains(position)) {
				return false;
			}

			using entity_type = to_entity_type_t<EntityType>;
			
			if (!entity_storage<entity_type>.remove(position)) {
				return false;
			}
			
			if constexpr (is_evil_entity<entity_type>::value) {
				good_goal_map.remove(position);
			} else if constexpr (is_good_entity<entity_type>::value) {
				evil_goal_map.remove(position);
			}

			entity_goal_map<entity_type>.remove(position);

			return true;
		}
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::clear() noexcept {
		entity_storage<EntityType>.clear();
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::clear() noexcept {
		(clear<EntityTypes>(), ...);
	}

	inline void entity_registry_t::clear() noexcept {
		clear<ALL_NON_PLAYER>();

		player = player_t{};
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::reset() noexcept {
		entity_storage<EntityType>.clear();
		reset_goal_map<EntityType>();
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::reset() noexcept {
		(clear<EntityTypes>(), ...);
		(reset_goal_map<EntityTypes>(), ...);
	}

	inline void entity_registry_t::reset() noexcept {
		clear();

		reset_goal_maps();
	}

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::spawn(usize count) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(EntityType{ maybe_position.value() });
		}

		return true;
	}

	template<NonPlayerEntity EntityType, typename... Args> inline bool entity_registry_t::spawn(usize count, Args... args) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open, entity_registry) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(EntityType{ maybe_position.value(), args... });
		}

		return true;
	}

	template<NonPlayerEntity EntityType> inline bool entity_registry_t::spawn(usize count, u32 minimum_distance) noexcept {
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ entity_goal_map<EntityType>.template find_random<zone_region_t::Interior>(game_map, random_engine, cell_trait_t::Open, entity_registry, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(EntityType{ maybe_position.value() });

			entity_goal_map<EntityType>.template recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
		}

		return true;
	}

	template<NonPlayerEntity EntityType, typename... Args> inline bool entity_registry_t::spawn(usize count, u32 minimum_distance, Args... args) noexcept {
		entity_goal_map<EntityType>.template recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
		
		for (usize i{ 0 }; i < count; ++i) {
			cauto maybe_position{ entity_goal_map<EntityType>.template find_random<zone_region_t::Interior>(game_map, random_engine, cell_trait_t::Open, entity_registry, minimum_distance) };

			if (!maybe_position.has_value()) {
				return false;
			}

			entity_registry.add(EntityType{ maybe_position.value(), args... });

			entity_goal_map<EntityType>.template recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
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
		  	case entity_type_t::Ladder: {
				return false;
			} case entity_type_t::Player: {
				player.position = target;
				good_goal_map.update(current, target);
				return true;
			} case entity_type_t::Skeleton: {
				return update<entity_type_t::Skeleton>(current, target);
			} case entity_type_t::Wraith: {
				return update<entity_type_t::Wraith>(current, target);
			} case entity_type_t::FleshGolem: {
				return update<entity_type_t::FleshGolem>(current, target);
			} case entity_type_t::Adventurer: {
				return update<entity_type_t::Adventurer>(current, target);
			} case entity_type_t::Paladin: {
				return update<entity_type_t::Paladin>(current, target);
			} case entity_type_t::Priest: {
				return update<entity_type_t::Priest>(current, target);
			} default: {
				return false;
			} 
		}
	}

	template<entity_type_t EntityType> inline bool entity_registry_t::update(cref<offset_t> current, cref<offset_t> target) noexcept {
		if (!entity_registry.contains(current) || !game_map.within<zone_region_t::Interior>(current) || entity_registry.contains(target) || !game_map.within<zone_region_t::Interior>(target)) {
			return false;
		}

		if constexpr (EntityType == entity_type_t::None || EntityType == entity_type_t::Player) {
			return false;
		}

		using entity_type = to_entity_type_t<EntityType>;
		
		if (!entity_storage<entity_type>.move(current, target)) {
			return false;
		}

		if constexpr (is_evil_entity<entity_type>::value) {
			good_goal_map.update(current, target);
		} else if constexpr (is_good_entity<entity_type>::value) {
			evil_goal_map.update(current, target);
		}

		entity_goal_map<entity_type>.update(current, target);

		return true;
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::update(ref<std::queue<entity_command_t>> commands) noexcept {
		for (crauto entity : entity_storage<EntityType>) {
			if (newborns.contains(entity.position)) {
				newborns.remove(entity.position);
				continue;
			}

			commands.push(entity.think());
		}

		entity_registry.process_commands(commands);
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::update(ref<std::queue<entity_command_t>> commands) noexcept {
		(update<EntityTypes>(commands), ...);
	}

	inline void entity_registry_t::update() noexcept {
		entity_registry.process_command(player.command);

		if (camera_locked) {
			update_camera();
		}

		if (descent_flag) {
			return;
		}

		std::queue<entity_command_t> commands{};

		update<ALL_NPCS>(commands);

		if (player.has_ascended()) {
			player.erode_divinity();
		}

		recalculate_goal_maps();
	}

	template<Entity EntityType> inline void entity_registry_t::recalculate_goal_map() noexcept {
		entity_goal_map<EntityType>.template recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
	}

	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::recalculate_goal_maps() noexcept {
		(recalculate_goal_map<EntityTypes>(), ...);
	}

	inline void entity_registry_t::recalculate_good_goal_map() noexcept {
		good_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
	}

	inline void entity_registry_t::recalculate_evil_goal_map() noexcept {
		evil_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
	}

	inline void entity_registry_t::recalculate_alignment_goal_maps() noexcept {
		recalculate_good_goal_map();
		recalculate_evil_goal_map();
	}

	inline void entity_registry_t::recalculate_goal_maps() noexcept {
		recalculate_goal_maps<ALL_ENTITIES>();

		recalculate_alignment_goal_maps();
	}

	template<Entity EntityType> inline void entity_registry_t::reset_goal_map() noexcept {
		entity_goal_map<EntityType>.reset();
	}

	template<Entity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::reset_goal_maps() noexcept {
		(reset_goal_map<EntityTypes>(), ...);
	}

	inline void entity_registry_t::reset_good_goal_map() noexcept {
		good_goal_map.reset();
	}

	inline void entity_registry_t::reset_evil_goal_map() noexcept {
		evil_goal_map.reset();
	}

	inline void entity_registry_t::reset_alignment_goal_maps() noexcept {
		reset_good_goal_map();
		reset_evil_goal_map();
	}

	inline void entity_registry_t::reset_goal_maps() noexcept {
		reset_goal_maps<ALL_ENTITIES>();

		reset_alignment_goal_maps();
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
			case entity_type_t::Ladder: {
				return false;
			} default: {
				break;
			}
		}

		if (source_type != entity_type_t::Player) {
			switch (command.type) {
				case command_type_t::Descend:
				case command_type_t::Consume:
				case command_type_t::RandomWarp:
				case command_type_t::TargetWarp:
				case command_type_t::ConsumeWarp:
				case command_type_t::CalciticInvocation:
				case command_type_t::SpectralInvocation:
				case command_type_t::SanguineInvocation:
				case command_type_t::NecromanticAscendance: {
					return false;
				} default: {
					break;
				}
			}
		}

		if (source_type != entity_type_t::Priest) {
			switch (command.type) {
				case command_type_t::Exorcise:
				case command_type_t::Resurrect: {
					return false;
				} default: {
					break;
				}
			}
		}

		switch (command.type) {
			case command_type_t::RandomWarp:
			case command_type_t::CalciticInvocation:
			case command_type_t::SpectralInvocation:
			case command_type_t::SanguineInvocation:
			case command_type_t::NecromanticAscendance: {
				return true;
			}
			default: {
				break;
			}
		}

		const offset_t target_position{ command.target.value() };

		if (!game_map.within<zone_region_t::Interior>(target_position)) {
			return false;
		}

		const entity_type_t target_type{ entity_registry.at(target_position) };

		switch (command.type) {
			case command_type_t::Move:
			case command_type_t::TargetWarp: {
				if (entity_registry.contains(target_position) || target_type != entity_type_t::None) {
					return false;
				}

				break;
			} case command_type_t::Clash:
			  case command_type_t::Consume:
			  case command_type_t::ConsumeWarp:
			  case command_type_t::Exorcise:
			  case command_type_t::Resurrect:
			  case command_type_t::Anoint: {
				if (!entity_registry.contains(target_position) || target_type == entity_type_t::None) {
					return false;
				}

				break;
			} case command_type_t::Descend: {
				if (!entity_registry.contains(target_position) || target_type != entity_type_t::Ladder) {
					return false;
				}

				cref<ladder_t> descension_point{ *entity_registry.at<ladder_t>(target_position) };

				if (descension_point.is_up_ladder() || descension_point.has_shackle()) {
					return false;
				}

				break;
			} default: {
				break;
			}
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

			warp_cursor.set(player.position);
			draw_warp_cursor = true;

			return true;
		}

		entity_registry.update(source, random_safe_position.value());

		warp_cursor.set(player.position);
		draw_warp_cursor = true;

		return true;
	}

	template<> void entity_registry_t::process_command<command_type_t::None>(cref<entity_command_t> command) noexcept {}

	template<> void entity_registry_t::process_command<command_type_t::Move>(cref<entity_command_t> command) noexcept { entity_registry.update(command.source.value(), command.target.value()); }

	template<> void entity_registry_t::process_command<command_type_t::Descend>(cref<entity_command_t> command) noexcept {
		if (!command.target.has_value()) {
			return;
		}
		
		descent_flag = true;
	}

	template<> void entity_registry_t::process_command<command_type_t::Consume>(cref<entity_command_t> command) noexcept {
		const offset_t source_position{ command.source.value() };

		const offset_t target_position{ command.target.value() };

		const entity_type_t target_type{ entity_registry.at(target_position) };
		
		switch (target_type) {
			case entity_type_t::Skull: {
				const bool is_fresh{ entity_registry.at<skull_t>(target_position)->fresh };

				entity_registry.remove<entity_type_t::Skull>(target_position);
				entity_registry.add(skeleton_t{ target_position, !is_fresh });

				if (is_fresh) {
					player.receive_skull_boon();
				}

				random_warp(source_position);
				
				return;
			} case entity_type_t::Skeleton: {
				const i8 armor_boon = entity_registry.at<skeleton_t>(target_position)->armor_boon();

				entity_registry.remove<entity_type_t::Skeleton>(target_position);

				entity_registry.update(source_position, target_position);
				
				player.bolster_armor(armor_boon + player.max_armor() / 8);

				draw_warp_cursor = false;
				
				return;
			} case entity_type_t::Wraith: {
				const i8 armor_boon = entity_registry.at<wraith_t>(target_position)->armor_boon();

				entity_registry.remove<entity_type_t::Wraith>(target_position);

				entity_registry.update(source_position, target_position);

				player.bolster_armor(armor_boon + player.max_armor() / 4);

				draw_warp_cursor = false;
				
				return;
			} default: {
				return;
			}
		}
	}

	template<entity_type_t Victim>
		requires (Victim != entity_type_t::None)
	inline bool entity_registry_t::process_clash(cref<offset_t> target_position, i8 damage_amount) noexcept {
		if (entity_registry.at(target_position) != Victim) {
			return false;
		}

		using entity_type = to_entity_type_t<Victim>;

		ptr<entity_type> victim{ entity_registry.at<entity_type>(target_position) };

		if (victim == nullptr) {
			return false;
		}

		if constexpr (Victim != entity_type_t::Skeleton && Victim != entity_type_t::Adventurer && Victim != entity_type_t::Priest) {
			if (victim->can_survive(damage_amount)) {
				victim->receive_damage(damage_amount);

				return false;
			}
		}

		if constexpr (Victim == entity_type_t::Player) {
			phase.transition(game_phase_t::GameOver);

			return true;
		} else if constexpr (Victim == entity_type_t::Skeleton) {
			const bool is_rotted{ victim->rotted };

			entity_registry.remove<Victim>(target_position);

			if (is_rotted) {
				return true;
			}

			entity_registry.add(skull_t{ target_position, false });

			return true;
		} else if constexpr (Victim == entity_type_t::Wraith) {
			entity_registry.remove<Victim>(target_position);
			entity_registry.add(skull_t{ target_position, false });

			return true;
		} else if constexpr (Victim == entity_type_t::FleshGolem) {
			entity_registry.remove<Victim>(target_position);
			game_map[target_position].set(cell_trait_t::Bloodied);

			return true;
		} else {
			entity_registry.remove<Victim>(target_position);
			entity_registry.add(skull_t{ target_position, true });

			player.receive_death_boon<Victim>();

			return true;
		}
	}

	template<> inline void entity_registry_t::process_command<command_type_t::Clash>(cref<entity_command_t> command) noexcept {
		const offset_t source_position{ command.source.value() };
		const entity_type_t source_type{ entity_registry.at(source_position) };

		const offset_t target_position{ command.target.value() };
		const entity_type_t target_type{ entity_registry.at(target_position) };

		i8 source_damage{ 0 };

		switch (source_type) {
			case entity_type_t::Player:
			case entity_type_t::Skeleton:
			case entity_type_t::FleshGolem:
			case entity_type_t::Adventurer:
			case entity_type_t::Paladin: {
				source_damage = 1;
				break;
			} case entity_type_t::Wraith:
			  case entity_type_t::Priest: {
				source_damage = 2;
				break;
			} default: {
				return;
			}
		}

		i8 target_damage{ 0 };

		switch (target_type) {
			case entity_type_t::Player:
			case entity_type_t::Skeleton:
			case entity_type_t::FleshGolem:
			case entity_type_t::Adventurer:
			case entity_type_t::Paladin: {
				target_damage = 1;
				break;
			} case entity_type_t::Wraith:
			  case entity_type_t::Priest: {
				target_damage = 2;
				break;
			} default: {
				return;
			}
		}

		bool target_killed{ false };

		switch(target_type) {
			case entity_type_t::Player: {
				target_killed = process_clash<entity_type_t::Player>(target_position, source_damage);
				break;
			} case entity_type_t::Skeleton: {
				target_killed = process_clash<entity_type_t::Skeleton>(target_position, source_damage);
				break;
			} case entity_type_t::Wraith: {
				target_killed = process_clash<entity_type_t::Wraith>(target_position, source_damage);
				break;
			} case entity_type_t::FleshGolem: {
				target_killed = process_clash<entity_type_t::FleshGolem>(target_position, source_damage);
				break;
			} case entity_type_t::Adventurer: {
				target_killed = process_clash<entity_type_t::Adventurer>(target_position, source_damage);
				break;
			} case entity_type_t::Paladin: {
				target_killed = process_clash<entity_type_t::Paladin>(target_position, source_damage);
				break;
			} case entity_type_t::Priest: {
				target_killed = process_clash<entity_type_t::Priest>(target_position, source_damage);
				break;
			} default: {
				return;
			}
		}

		if (target_killed) {
			switch (source_type) {
				case entity_type_t::Player: {
					++game_stats.player_kills;
					draw_warp_cursor = false;
					break;
				} case entity_type_t::Skeleton:
				  case entity_type_t::Wraith:
				  case entity_type_t::FleshGolem: {
					++game_stats.minion_kills;
					break;
				} default: {
					break;
				}
			}

			switch (target_type) {
				case entity_type_t::Adventurer:
				case entity_type_t::Paladin:
				case entity_type_t::Priest: {
					game_map[target_position].set(cell_trait_t::Bloodied);
				} default: {
					break;
				}
			}
		}

		bool source_killed{ false };

		switch(source_type) {
			case entity_type_t::Player: {
				source_killed = process_clash<entity_type_t::Player>(source_position, target_damage);
				break;
			} case entity_type_t::Skeleton: {
				source_killed = process_clash<entity_type_t::Skeleton>(source_position, target_damage);
				break;
			} case entity_type_t::Wraith: {
				target_killed = process_clash<entity_type_t::Wraith>(source_position, target_damage);
				break;
			} case entity_type_t::FleshGolem: {
				target_killed = process_clash<entity_type_t::FleshGolem>(source_position, target_damage);
				break;
			} case entity_type_t::Adventurer: {
				source_killed = process_clash<entity_type_t::Adventurer>(source_position, target_damage);
				break;
			} case entity_type_t::Paladin: {
				source_killed = process_clash<entity_type_t::Paladin>(source_position, target_damage);
				break;
			} case entity_type_t::Priest: {
				source_killed = process_clash<entity_type_t::Priest>(source_position, target_damage);
				break;
			} default: {
				return;
			}
		}

		if (source_killed) {
			switch (target_type) {
				case entity_type_t::Player: {
					++game_stats.player_kills;
					break;
				} case entity_type_t::Skeleton:
				  case entity_type_t::Wraith:
				  case entity_type_t::FleshGolem: {
					++game_stats.minion_kills;
					break;
				} default: {
					break;
				}
			}

			switch (source_type) {
				case entity_type_t::Adventurer:
				case entity_type_t::Paladin:
				case entity_type_t::Priest: {
					game_map[source_position].set(cell_trait_t::Bloodied);
				} default: {
					break;
				}
			}
		}
	}

	template<> void entity_registry_t::process_command<command_type_t::RandomWarp>(cref<entity_command_t> command) noexcept {
		if (!player.can_random_warp()) {
			return;
		}

		player.pay_random_warp_cost();

		random_warp(command.source.value());
	}

	template<> void entity_registry_t::process_command<command_type_t::TargetWarp>(cref<entity_command_t> command) noexcept {
		if (!player.can_target_warp()) {
			return;
		}

		player.pay_target_warp_cost();

		entity_registry.update(command.source.value(), command.target.value());

		draw_warp_cursor = false;
	}

	template<> void entity_registry_t::process_command<command_type_t::ConsumeWarp>(cref<entity_command_t> command) noexcept {
		const offset_t target_position{ command.target.value() };

		const entity_type_t target_type{ entity_registry.at(target_position) };

		if (target_type != entity_type_t::Skull && !player.can_target_warp()) {
			return;
		}

		const offset_t source_position{ command.source.value() };

		switch (target_type) {
		case entity_type_t::Skull: {
			const bool is_fresh{ entity_registry.at<skull_t>(target_position)->fresh };

			if (!player.can_target_warp(is_fresh ? player_t::SkullBoon : 0)) {
				return;
			}

			entity_registry.remove<entity_type_t::Skull>(target_position);
			entity_registry.add(skeleton_t{ target_position, !is_fresh });

			player.pay_target_warp_cost(is_fresh ? player_t::SkullBoon : 0);

			random_warp(source_position);

			return;
		} case entity_type_t::Skeleton: {
			const i8 armor_boon = entity_registry.at<skeleton_t>(target_position)->armor_boon();

			entity_registry.remove<entity_type_t::Skeleton>(target_position);

			entity_registry.update(source_position, target_position);

			player.pay_target_warp_cost();
			player.bolster_armor(armor_boon + player.max_armor() / 8);

			draw_warp_cursor = false;
			return;
		} case entity_type_t::Wraith: {
			const i8 armor_boon = entity_registry.at<wraith_t>(target_position)->armor_boon();

			entity_registry.remove<entity_type_t::Wraith>(target_position);

			entity_registry.update(source_position, target_position);

			player.pay_target_warp_cost();
			player.bolster_armor(armor_boon + player.max_armor() / 4);

			draw_warp_cursor = false;
			return;
		} default:
			return;
		}
	}

	template<> void entity_registry_t::process_command<command_type_t::CalciticInvocation>(cref<entity_command_t> command) noexcept {
		if (!player.can_perform_calcitic_invocation() || entity_registry.empty<skull_t>()) {
			return;
		}

		i8 accumulated_skulls{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t position{ player.position + offset };

			const bool is_empty{ entity_registry.at(position) == entity_type_t::None };

			if (is_empty && player.bypass_invocations_enabled()) {
				entity_registry.add(skeleton_t{ position, true });
				++accumulated_skulls;

				continue;
			}

			const bool has_skull{ entity_registry.at(position) == entity_type_t::Skull };
			const bool has_ladder{ entity_registry.at(position) == entity_type_t::Ladder };

			if (!game_map.within<zone_region_t::Interior>(position) || (!has_skull && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_skull) {
				const bool is_fresh{ entity_registry.at<skull_t>(position)->fresh };

				entity_registry.remove<entity_type_t::Skull>(position);
				entity_registry.add(skeleton_t{ position, !is_fresh });

				++accumulated_skulls;
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = entity_registry.at<ladder_t>(position);

				switch (eligible_ladder->shackle) {
					case shackle_type_t::None: {
						if (eligible_ladder->is_down_ladder()) {
							eligible_ladder = nullptr;
						}
						break;
					} default: {
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_type_t::Calcitic) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (accumulated_skulls <= 0) {
			return;
		} else if (accumulated_skulls >= 4 && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();
			} else {
				eligible_ladder->enshackle(shackle_type_t::Calcitic);
			}

			eligible_ladder = nullptr;
		}

		player.pay_calcitic_invocation_cost();
	}

	template<> void entity_registry_t::process_command<command_type_t::SpectralInvocation>(cref<entity_command_t> command) noexcept {
		if (!player.can_perform_spectral_invocation() || entity_registry.empty<skull_t>()) {
			return;
		}

		i8 accumulated_health{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t position{ player.position + offset };

			const bool has_skull{ entity_registry.at(position) == entity_type_t::Skull };
			const bool has_ladder{ entity_registry.at(position) == entity_type_t::Ladder };

			if (!game_map.within<zone_region_t::Interior>(position) || (!has_skull && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_skull && entity_registry.at<skull_t>(position)->fresh) {
				entity_registry.remove<entity_type_t::Skull>(position);
				++accumulated_health;
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = entity_registry.at<ladder_t>(position);

				switch (eligible_ladder->shackle) {
					case shackle_type_t::None: {
						if (eligible_ladder->is_down_ladder()) {
							eligible_ladder = nullptr;
						}
						break;
					} default: {
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_type_t::Spectral) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (player.bypass_invocations_enabled()) {
			accumulated_health = wraith_t::MaximumHealth;
		}

		if (accumulated_health <= 0) {
			return;
		} else if (accumulated_health >= 4 && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();
			} else {
				eligible_ladder->enshackle(shackle_type_t::Spectral);
			}

			eligible_ladder = nullptr;
		}

		player.pay_spectral_invocation_cost();

		if (!random_warp(command.source.value())) {
			player.bolster_armor(accumulated_health);

			return;
		}

		entity_registry.add(wraith_t{ command.source.value(), accumulated_health });
	}

	template<> void entity_registry_t::process_command<command_type_t::SanguineInvocation>(cref<entity_command_t> command) noexcept {
		if (!player.can_perform_spectral_invocation() || !game_map.template contains<zone_region_t::Interior>(cell_trait_t::Bloodied)) {
			return;
		}

		i8 accumulated_health{ 0 };

		ptr<ladder_t> eligible_ladder{ nullptr };

		for (crauto offset : neighbourhood_offsets<distance_function_t::Chebyshev>) {
			const offset_t position{ player.position + offset };

			const bool has_blood{ game_map[position].bloodied };
			const bool has_ladder{ entity_registry.at(position) == entity_type_t::Ladder };

			if (!game_map.within<zone_region_t::Interior>(position) || (!has_blood && (eligible_ladder != nullptr || !has_ladder))) {
				continue;
			}

			if (has_blood) {
				game_map[position].unset(cell_trait_t::Bloodied);
				++accumulated_health;
			}

			if (eligible_ladder == nullptr && has_ladder) {
				eligible_ladder = entity_registry.at<ladder_t>(position);

				switch (eligible_ladder->shackle) {
					case shackle_type_t::None: {
						if (eligible_ladder->is_down_ladder()) {
							eligible_ladder = nullptr;
						}
						break;
					} default: {
						if (eligible_ladder->is_up_ladder() || eligible_ladder->shackle != shackle_type_t::Sanguine) {
							eligible_ladder = nullptr;
						}
						break;
					}
				}
			}
		}

		if (player.bypass_invocations_enabled()) {
			accumulated_health = flesh_golem_t::MaximumHealth;
		}

		if (accumulated_health <= 0) {
			return;
		} else if (accumulated_health >= 4 && eligible_ladder != nullptr) {
			if (eligible_ladder->is_down_ladder()) {
				eligible_ladder->unshackle();
			} else {
				eligible_ladder->enshackle(shackle_type_t::Sanguine);
			}

			eligible_ladder = nullptr;
		}

		player.pay_sanguine_invocation_cost();

		if (!random_warp(command.source.value())) {
			player.bolster_armor(accumulated_health);

			return;
		}

		entity_registry.add(flesh_golem_t{ command.source.value(), accumulated_health });
	}

	template<> void entity_registry_t::process_command<command_type_t::NecromanticAscendance>(cref<entity_command_t> command) noexcept {
		if (!player.can_perform_necromantic_ascendance() || player.has_ascended()) {
			return;
		}

		player.max_out_divinity();

		player.pay_necromantic_ascendance_cost();
	}

	template<> void entity_registry_t::process_command<command_type_t::Exorcise>(cref<entity_command_t> command) noexcept {
		const offset_t source_position{ command.source.value() };

		if (!entity_registry.contains<entity_type_t::Priest>(source_position)) {
			return;
		}

		const offset_t target_position{ command.target.value() };

		if (!entity_registry.contains<entity_type_t::Skull>(target_position)) {
			return;
		}

		auto priest{ entity_registry.at<priest_t>(source_position) };

		if (priest == nullptr) {
			return;
		}

		if (!priest->can_exorcise()) {
			return;
		}

		entity_registry.remove<entity_type_t::Skull>(target_position);

		priest->pay_exorcise_cost();
	}

	template<> void entity_registry_t::process_command<command_type_t::Resurrect>(cref<entity_command_t> command) noexcept {
		const offset_t source_position{ command.source.value() };

		if (!entity_registry.contains<entity_type_t::Priest>(source_position)) {
			return;
		}

		const offset_t target_position{ command.target.value() };

		if (!entity_registry.contains<entity_type_t::Skull>(target_position)) {
			return;
		}

		cauto skull{ entity_registry.at<skull_t>(target_position) };

		if (skull == nullptr || !skull->fresh) {
			return;
		}

		auto priest{ entity_registry.at<priest_t>(source_position) };

		if (priest == nullptr || !priest->can_resurrect()) {
			return;
		}

		entity_registry.remove<entity_type_t::Skull>(target_position);

		entity_registry.add(adventurer_t{ target_position });

		priest->pay_resurrect_cost();
	}

	template<> void entity_registry_t::process_command<command_type_t::Anoint>(cref<entity_command_t> command) noexcept {
		const offset_t source_position{ command.source.value() };

		if (!entity_registry.contains<entity_type_t::Priest>(source_position)) {
			return;
		}

		const offset_t target_position{ command.target.value() };

		if (!entity_registry.contains<entity_type_t::Adventurer>(target_position)) {
			return;
		}

		auto priest{ entity_registry.at<priest_t>(source_position) };

		if (priest == nullptr || !priest->can_anoint()) {
			return;
		}

		entity_registry.remove<entity_type_t::Adventurer>(target_position);

		entity_registry.add(paladin_t{ target_position });

		priest->pay_ordain_cost();
	}

	inline void entity_registry_t::process_command(cref<entity_command_t> command) noexcept {
		if (!is_command_valid(command)) {
			return;
		}

		switch (command.type) {
			case command_type_t::Move: {
				return process_command<command_type_t::Move>(command);
			} case command_type_t::Descend: {
				return process_command<command_type_t::Descend>(command);
			} case command_type_t::Consume: {
				return process_command<command_type_t::Consume>(command);
			} case command_type_t::Clash: {
				return process_command<command_type_t::Clash>(command);
			} case command_type_t::ConsumeWarp: {
				return process_command<command_type_t::ConsumeWarp>(command);
			} case command_type_t::TargetWarp: {
				return process_command<command_type_t::TargetWarp>(command);
			} case command_type_t::RandomWarp: {
				return process_command<command_type_t::RandomWarp>(command);
			} case command_type_t::CalciticInvocation: {
				return process_command<command_type_t::CalciticInvocation>(command);
			} case command_type_t::SpectralInvocation: {
				return process_command<command_type_t::SpectralInvocation>(command);
			} case command_type_t::SanguineInvocation: {
				return process_command<command_type_t::SanguineInvocation>(command);
			} case command_type_t::NecromanticAscendance: {
				return process_command<command_type_t::NecromanticAscendance>(command);
			} case command_type_t::Exorcise: {
				return process_command<command_type_t::Exorcise>(command);
			} case command_type_t::Resurrect: {
				return process_command<command_type_t::Resurrect>(command);
			} case command_type_t::Anoint: {
				return process_command<command_type_t::Anoint>(command);
			} default: {
				return;
			}
		}
	}

	inline void entity_registry_t::process_commands(ref<std::queue<entity_command_t>> commands) noexcept {
		while (!commands.empty()) {
			const entity_command_t command{ std::move(commands.front()) };
			commands.pop();

			entity_registry.process_command(command);
		}
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::draw() const noexcept {
		for (crauto entity : entity_storage<EntityType>) {
			entity.draw();
		}
	}

	template<NonPlayerEntity EntityType> inline void entity_registry_t::draw(cref<camera_t> camera) const noexcept {
		for (crauto entity : entity_storage<EntityType>) {
			entity.draw(camera);
		}
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::draw() const noexcept {
		(draw<EntityTypes>(), ...);
	}

	template<NonPlayerEntity... EntityTypes>
		requires is_plurary<EntityTypes...>::value
	inline void entity_registry_t::draw(cref<camera_t> camera) const noexcept {
		(draw<EntityTypes>(camera), ...);
	}

	inline void entity_registry_t::draw() const noexcept {
		draw<ALL_INANIMATE>();
		draw<ALL_NPCS>();

		player.draw();
	}

	inline void entity_registry_t::draw(cref<camera_t> camera) const noexcept {
		draw<ALL_INANIMATE>(camera);
		draw<ALL_NPCS>(camera);

		player.draw(camera);
	}
} // namespace necrowarp
