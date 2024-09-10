#pragma once

#include <necrowarp/entities/entity.hpp>

#include <queue>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct entity_registry_t {
		inline entity_type_t at(cref<offset_t> position) const noexcept;

		template<typename T>
			requires is_entity<T>::value
		inline cptr<T> at(cref<offset_t> position) const noexcept;

		template<entity_type_t EntityType> inline usize count() const noexcept;

		template<entity_type_t EntityType> inline usize empty() const noexcept;

		inline bool contains(cref<offset_t> position) const noexcept;

		template<entity_type_t EntityType> inline bool contains(cref<offset_t> position) const noexcept;

		template<entity_type_t EntityType> inline void announce_population() const noexcept;

		template<typename T>
			requires is_entity<T>::value && (!is_entity_type<T, entity_type_t::Player>::value)
		inline bool add(rval<T> entity) noexcept;

		template<entity_type_t EntityType> inline bool remove(cref<offset_t> position) noexcept;

		template<typename T>
			requires is_entity<T>::value && (!is_entity_type<T, entity_type_t::Player>::value)
		inline bool spawn(usize count) noexcept;

		inline bool update(cref<offset_t> current, cref<offset_t> target) noexcept;

		template<entity_type_t EntityType> inline bool update(cref<offset_t> current, cref<offset_t> target) noexcept;

		inline void process_commands(ref<std::queue<entity_command_t>> commands) noexcept;

		inline void process_command(cref<entity_command_t> command) noexcept;

		inline void update() noexcept;

		inline void draw() noexcept;

		inline void draw(cref<camera_t> camera) noexcept;
	} static inline entity_registry{};
} // namespace necrowarp
