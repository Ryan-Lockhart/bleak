#pragma once

#include <necrowarp/entities/entity.hpp>

#include <queue>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct entity_registry_t {
		inline entity_type_t at(cref<offset_t> position) const noexcept;

		template<Entity EntityType> inline cptr<EntityType> at(cref<offset_t> position) const noexcept;

		template<Entity EntityType> inline  ptr<EntityType> at(cref<offset_t> position) noexcept;

		template<entity_type_t EntityType> inline usize count() const noexcept;

		template<NonPlayerEntity EntityTypes> inline usize count() const noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline usize count() const noexcept;

		inline usize count() const noexcept;

		template<entity_type_t EntityType> inline bool empty() const noexcept;

		template<NonPlayerEntity EntityType> inline bool empty() const noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool empty() const noexcept;

		inline bool empty() const noexcept { return false; }

		inline bool contains(cref<offset_t> position) const noexcept;

		template<NonPlayerEntity EntityType> inline bool contains(cref<offset_t> position) const noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline bool contains(cref<offset_t> position) const noexcept;

		template<entity_type_t EntityType> inline bool contains(cref<offset_t> position) const noexcept;

		template<bool Force = false, NonPlayerEntity T> inline bool add(rval<T> entity) noexcept;

		template<entity_type_t EntityType> inline bool remove(cref<offset_t> position) noexcept;

		template<NonPlayerEntity EntityType> inline void clear() noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void clear() noexcept;

		inline void clear() noexcept;

		template<NonPlayerEntity EntityType> inline void reset() noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void reset() noexcept;

		inline void reset() noexcept;

		template<NonPlayerEntity EntityType> inline bool spawn(usize count) noexcept;

		template<NonPlayerEntity EntityType, typename... Args> inline bool spawn(usize count, Args... args) noexcept;

		template<NonPlayerEntity EntityType> inline bool spawn(usize count, u32 minimum_distance) noexcept;

		template<NonPlayerEntity EntityType, typename... Args> inline bool spawn(usize count, u32 minimum_distance, Args... args) noexcept;

		inline bool update(cref<offset_t> current, cref<offset_t> target) noexcept;

		template<entity_type_t EntityType> inline bool update(cref<offset_t> current, cref<offset_t> target) noexcept;

		inline bool is_command_valid(cref<entity_command_t> command) const noexcept;

		inline bool random_warp(cref<offset_t> source) noexcept;

		template<entity_type_t Victim>
			requires (Victim != entity_type_t::None)
		inline bool process_clash(cref<offset_t> target_position, i8 damage_amount) noexcept;

		template<command_type_t CommandType> inline void process_command(cref<entity_command_t> command) noexcept;

		inline void process_commands(ref<std::queue<entity_command_t>> commands) noexcept;

		inline void process_command(cref<entity_command_t> command) noexcept;

		template<NonPlayerEntity EntityType> inline void update(ref<std::queue<entity_command_t>> commands) noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void update(ref<std::queue<entity_command_t>> commands) noexcept;

		inline void update() noexcept;

		template<Entity EntityType> inline void recalculate_goal_map() noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void recalculate_goal_maps() noexcept;

		inline void recalculate_goal_maps() noexcept;

		inline void recalculate_good_goal_map() noexcept;

		inline void recalculate_evil_goal_map() noexcept;

		inline void recalculate_alignment_goal_maps() noexcept;

		template<Entity EntityType> inline void reset_goal_map() noexcept;

		template<Entity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void reset_goal_maps() noexcept;

		inline void reset_goal_maps() noexcept;

		inline void reset_good_goal_map() noexcept;

		inline void reset_evil_goal_map() noexcept;

		inline void reset_alignment_goal_maps() noexcept;

		template<NonPlayerEntity EntityType> inline void draw() const noexcept;

		template<NonPlayerEntity EntityType> inline void draw(cref<camera_t> camera) const noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw() const noexcept;

		template<NonPlayerEntity... EntityTypes>
			requires is_plurary<EntityTypes...>::value
		inline void draw(cref<camera_t> camera) const noexcept;

		inline void draw() const noexcept;

		inline void draw(cref<camera_t> camera) const noexcept;
	} static inline entity_registry{};
} // namespace necrowarp
