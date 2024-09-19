#pragma once

#include "bleak/offset.hpp"
#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct player_t {
		entity_command_t command;
		offset_t position;

		static constexpr i8 MaximumEnergy{ 32 };
		static constexpr i8 MinimumEnergy{ 4 };

		static constexpr i8 StartingEnergy{ 3 };
		static constexpr i8 StartingArmor{ 0 };

		static constexpr i8 MaximumArmor{ 16 };
		static constexpr i8 MinimumArmor{ 2 };

		static constexpr i8 MaximumDamage{ 1 };
		static constexpr i8 MinimumDamage{ 1 };

		static constexpr i8 RandomWarpCost{ 1 };
		static constexpr i8 TargetWarpCost{ 2 };
		static constexpr i8 SummonWraithCost{ 4 };
		static constexpr i8 GrandSummoningCost{ 8 };

		static constexpr i8 SkullBoon{ 1 };
		static constexpr i8 FailedWarpBoon{ 1 };
		static constexpr i8 UnsafeWarpBoon{ 1 };

	  private:
		i8 energy;
		i8 armor;

		inline void set_energy(i8 value) noexcept { energy = clamp<i8>(value, 0, max_energy()); }

		inline void set_armor(i8 value) noexcept { armor = clamp<i8>(value, 0, max_armor()); }

	  public:
		inline player_t() noexcept : command{}, position{}, energy{ StartingEnergy }, armor{ StartingArmor } {}

		inline player_t(cref<offset_t> position) noexcept : command{}, position{ position }, energy{ StartingEnergy }, armor{ StartingArmor } {}

		inline i8 get_energy() const noexcept { return energy; }

		inline i8 get_armor() const noexcept { return armor; }

		inline bool has_energy() const noexcept { return energy > 0; }

		inline bool has_armor() const noexcept { return armor > 0; }

		inline i8 max_energy() const noexcept { return MinimumEnergy + minion_kills / 16; }

		inline i8 max_armor() const noexcept { return MinimumArmor + player_kills / 8; }

		inline bool can_survive(i8 damage_amount) const noexcept { return armor >= damage_amount; }

		inline void receive_damage(i8 damage_amount) noexcept { set_armor(armor - damage_amount); }

		inline bool can_random_warp() const noexcept { return energy >= RandomWarpCost; }

		inline bool can_random_warp(i8 discount) const noexcept { return energy >= RandomWarpCost - discount; }

		inline bool can_target_warp() const noexcept { return energy >= TargetWarpCost; }

		inline bool can_target_warp(i8 discount) const noexcept { return energy >= TargetWarpCost - discount; }

		inline bool can_summon_wraith() const noexcept { return energy >= SummonWraithCost; }

		inline bool can_summon_wraith(i8 discount) const noexcept { return energy >= SummonWraithCost - discount; }

		inline bool can_grand_summon() const noexcept { return energy >= GrandSummoningCost; }

		inline bool can_grand_summon(i8 discount) const noexcept { return energy >= GrandSummoningCost - discount; }

		inline void pay_random_warp_cost() noexcept { set_energy(energy - RandomWarpCost); }

		inline void pay_random_warp_cost(i8 discount) noexcept { set_energy(energy - RandomWarpCost + discount); }

		inline void pay_target_warp_cost() noexcept { set_energy(energy - TargetWarpCost); }

		inline void pay_target_warp_cost(i8 discount) noexcept { set_energy(energy - TargetWarpCost + discount); }

		inline void pay_summon_wraith_cost() noexcept { set_energy(energy - SummonWraithCost); }

		inline void pay_summon_wraith_cost(i8 discount) noexcept { set_energy(energy - SummonWraithCost + discount); }

		inline void pay_grand_summon_cost() noexcept { set_energy(energy - GrandSummoningCost); }

		inline void pay_grand_summon_cost(i8 discount) noexcept { set_energy(energy - GrandSummoningCost + discount); }

		inline void receive_skull_boon() noexcept { set_energy(energy + SkullBoon); }

		inline void receive_failed_warp_boon() noexcept { set_energy(energy + FailedWarpBoon); }

		inline void receive_unsafe_warp_boon() noexcept { set_energy(energy + UnsafeWarpBoon); }

		inline void max_out_energy() noexcept { energy = max_energy(); }

		inline void max_out_armor() noexcept { armor = max_armor(); }

		inline void zero_out_energy() noexcept { energy = 0; }

		inline void zero_out_armor() noexcept { armor = 0; }

		template<entity_type_t EntityType> inline bool will_perish() const noexcept;

		template<entity_type_t EntityType> inline void receive_damage() noexcept;

		template<entity_type_t EntityType> inline void receive_death_boon() noexcept;

		inline command_type_t clash_or_consume(cref<offset_t> position) const noexcept;

		inline void bolster_armor(i8 value) noexcept { set_armor(armor + value); }

		inline void increment_energy() noexcept {
			if (energy < max_energy()) {
				++energy;
			}
		}

		inline void decrement_energy() noexcept {
			if (energy > MinimumEnergy) {
				--energy;
			}
		}

		inline void increment_armor() noexcept {
			if (armor < max_armor()) {
				++armor;
			}
		}

		inline void decrement_armor() noexcept {
			if (armor > MinimumArmor) {
				--armor;
			}
		}

		inline void draw() const noexcept { game_atlas.draw(has_armor() ? PlayerArmoredGlyph : EntityGlyphs[entity_type_t::Player], position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(has_armor() ? PlayerArmoredGlyph : EntityGlyphs[entity_type_t::Player], position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(has_armor() ? PlayerArmoredGlyph : EntityGlyphs[entity_type_t::Player], position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(has_armor() ? PlayerArmoredGlyph : EntityGlyphs[entity_type_t::Player], position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Player; }
	};

	template<> struct is_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<player_t, entity_type_t::Player> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Player> {
		using type = player_t;
	};
} // namespace necrowarp
