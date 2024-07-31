#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct adventurer_t {
		offset_t position;

		inline adventurer_t(cref<offset_t> position) noexcept : position{ position } {}

		inline void draw() const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Adventurer], position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Adventurer], position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Adventurer], position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Adventurer], position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Adventurer; }
	};

	template<> struct is_entity<adventurer_t> {
		static constexpr bool value = true;
	};

	template<typename T> struct is_entity_type<T, entity_type_t::Adventurer> {
		static constexpr bool value = true;
	};
} // namespace necrowarp
