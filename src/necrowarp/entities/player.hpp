#pragma once

#include <necrowarp/entities/entity.hpp>

#include <utility>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct player_t {
		offset_t position;

		inline player_t() noexcept : position{} {}

		inline player_t(cref<offset_t> position) noexcept : position{ position } {}

		inline player_t(cref<player_t> other) noexcept : position{ other.position } {}

		inline player_t(rval<player_t> other) noexcept : position{ std::move(other.position) } {}

		inline ref<player_t> operator=(cref<player_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			position = other.position;
			
			return *this;
		}

		inline ref<player_t> operator=(rval<player_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			position = std::move(other.position);
			
			return *this;
		}

		inline void draw() const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Player], position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Player], position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Player], position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Player], position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Player; }
	};

	template<> struct is_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<player_t, entity_type_t::Player> {
		static constexpr bool value = true;
	};
} // namespace necrowarp
