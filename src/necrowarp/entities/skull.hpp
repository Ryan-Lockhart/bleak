#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct skull_t {
		offset_t position;

		inline skull_t(cref<offset_t> position) noexcept : position{ position } {}

		inline void draw() const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Skull], position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Skull], position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Skull], position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Skull], position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Skull; }
	};

	template<> struct is_entity<skull_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<skull_t, entity_type_t::Skull> {
		static constexpr bool value = true;
	};
} // namespace necrowarp
