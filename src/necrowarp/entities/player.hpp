#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct player_t {
		entity_command_t command;
		offset_t position;
		u16 kills;
		u8 energy;
		u8 armor;

		inline player_t() noexcept : command{}, position{}, kills{ 0 }, energy{ globals::StartingEnergy }, armor{ globals::StartingArmor } {}

		inline player_t(cref<offset_t> position) noexcept : command{}, position{ position }, kills{ 0 }, energy{ globals::StartingEnergy }, armor{ globals::StartingArmor } {}

		inline bool has_energy() const noexcept { return energy > 0; }

		inline bool has_armor() const noexcept { return armor > 0; }

		inline bool can_random_warp() const noexcept { return energy >= globals::RandomWarpCost; }

		inline bool can_target_warp() const noexcept { return energy >= globals::TargetWarpCost; }

		inline void increment_energy() noexcept {
			if (energy < globals::MaximumEnergy) {
				++energy;
			}
		}

		inline void decrement_energy() noexcept {
			if (energy > globals::MinimumEnergy) {
				--energy;
			}
		}

		inline void increment_armor() noexcept {
			if (armor < globals::MaximumArmor) {
				++armor;
			}
		}

		inline void decrement_armor() noexcept {
			if (armor > globals::MinimumArmor) {
				--armor;
			}
		}

		inline void draw() const noexcept { game_atlas.draw(has_armor() ? PlayerArmored : EntityGlyphs[entity_type_t::Player], position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(has_armor() ? PlayerArmored : EntityGlyphs[entity_type_t::Player], position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(has_armor() ? PlayerArmored : EntityGlyphs[entity_type_t::Player], position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(has_armor() ? PlayerArmored : EntityGlyphs[entity_type_t::Player], position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Player; }
	};

	template<> struct is_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<player_t, entity_type_t::Player> {
		static constexpr bool value = true;
	};
} // namespace necrowarp
