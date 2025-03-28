#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<player_t, entity_type_t::Player> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Player> {
		using type = player_t;
	};

	template<> struct is_evil_entity<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<player_t> {
		static constexpr bool value = true;
	};

	template<> struct is_non_player_entity<player_t> {
		static constexpr bool value = false;
	};

	template<> struct is_player<player_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<player_t>{ glyphs::UnarmoredPlayer };

	template<> inline constexpr glyph_t command_icons<command_type_t::RandomWarp>{ 0x00, colors::White };
	template<> inline constexpr glyph_t command_icons<command_type_t::TargetWarp>{ 0x01, colors::White };

	template<> inline constexpr glyph_t command_icons<command_type_t::CalciticInvocation>{ 0x02, colors::White };
	template<> inline constexpr glyph_t command_icons<command_type_t::SpectralInvocation>{ 0x10, colors::White };
	template<> inline constexpr glyph_t command_icons<command_type_t::SanguineInvocation>{ 0x11, colors::White };

	template<> inline constexpr glyph_t command_icons<command_type_t::NecromanticAscendance>{ 0x12, colors::White };

	enum class patron_e : u8 {
		None,
		Rathghul,
		Akurakhaithan,
		Merirfin,
		Saeiligarkeuss
	};

	struct discount_t {
		const i8 minimum{ 0 };
		const i8 maximum{ 0 };
		
		constexpr i8 current(i8 relation) const noexcept {
			return map<i8>(relation,
				std::numeric_limits<i8>::lowest(), std::numeric_limits<i8>::max(),
				minimum, maximum
			);
		} 
	};

	struct patron_t {
		i8 relationship{ 0 };

		const discount_t random_warp{};
		const discount_t target_warp{};

		const discount_t calcitic_invocation{};
		const discount_t spectral_invocation{};
		const discount_t sanguine_invocation{};

		const discount_t necromantic_ascendance{};
	};

	static inline patron_t patrons[static_cast<usize>(patron_e::Saeiligarkeuss) + 1]{
		{ // None
			std::numeric_limits<i8>::max(),
			{ -2, 2 },
			{ -2, 3 },
		},
		{ // Rathghul
			std::numeric_limits<i8>::max(),
			{ -2, 1 },
			{ -2, 3 },
			{ -4, 4 },
			{ -6, 2 },
			{ -8, 4 },
			{ -6, 6 },
		},
		{
			std::numeric_limits<i8>::max(),
			{ -2, 1 },
			{ -2, 3 },
			{ -6, 2 },
			{ -4, 4 },
			{ -8, 0 },
			{ -4, 8 },
		},
		{
			std::numeric_limits<i8>::max(),
			{ -2, 1 },
			{ -2, 3 },
			{ -6, 2 },
			{ -8, 0 },
			{ -4, 4 },
			{ -6, 6 },
		},
		{
			std::numeric_limits<i8>::max(),
			{ -2, 1 },
			{ -2, 4 },
			{ -8, 2 },
			{ -8, 2 },
			{ -8, 2 },
			{ -4, 16 },
		}
	};
	
	static inline void reset_patrons() noexcept {
		for (usize i{ static_cast<usize>(patron_e::None) }; i < static_cast<usize>(patron_e::Saeiligarkeuss) + 1; ++i) {
			patrons[i].relationship = 0;
		}
	}

	struct player_t {
		entity_command_t command;
		offset_t position;

		static constexpr i8 MinimumEnergy{ 4 };
		static constexpr i8 MaximumEnergy{ 24 };

		static constexpr i8 MinimumArmor{ 2 };
		static constexpr i8 MaximumArmor{ 12 };

		static constexpr i8 MinimumDivinity{ 3 };
		static constexpr i8 MaximumDivinity{ 9 };

		static constexpr i8 DivinityErosionRate{ 1 };

		static constexpr i8 StartingEnergy{ 3 };
		static constexpr i8 StartingArmor{ 0 };
		static constexpr i8 StartingDivinity{ 0 };

		static constexpr i8 MaximumDamage{ 1 };
		static constexpr i8 MinimumDamage{ 1 };

		static constexpr i8 RandomWarpCost{ 2 };
		static constexpr i8 TargetWarpCost{ 4 };

		static constexpr i8 CalciticInvocationCost{ 8 };
		static constexpr i8 SpectralInvocationCost{ 8 };
		static constexpr i8 SanguineInvocationCost{ 8 };

		static constexpr i8 NecromanticAscendanceCost{ MaximumEnergy };

		static constexpr i8 SkullBoon{ 1 };
		static constexpr i8 FailedWarpBoon{ 1 };
		static constexpr i8 UnsafeWarpBoon{ 1 };

	  private:
		i8 energy;
		i8 armor;
		i8 divinity;

		patron_e patron;

		inline void set_energy(i8 value) noexcept {
			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.energy.is_enabled()) {
				energy = clamp<i8>(value, 0, max_energy());
			} else {
				energy = max<i8>(game_stats.cheats.energy.current_minimum(), value);
			}
		}

		inline void set_armor(i8 value) noexcept {
			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.armor.is_enabled()) {
				armor = clamp<i8>(value, 0, max_armor());
			} else {
				armor = max<i8>(game_stats.cheats.armor.current_minimum(), value);
			}
		}

		inline void set_divinity(i8 value) noexcept {
			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.divinity.is_enabled()) {
				divinity = clamp<i8>(value, 0, max_divinity());
			} else {
				divinity = max<i8>(game_stats.cheats.divinity.current_minimum(), value);
			}
		}

	  public:
		inline player_t() noexcept : command{}, position{}, energy{ StartingEnergy }, armor{ StartingArmor }, divinity{ StartingDivinity } {}

		inline player_t(cref<offset_t> position) noexcept : command{}, position{ position }, energy{ StartingEnergy }, armor{ StartingArmor }, divinity{ StartingDivinity } {}

		inline i8 get_energy() const noexcept { return energy; }

		inline i8 get_armor() const noexcept { return armor; }

		inline i8 get_divinity() const noexcept { return divinity; }

		inline cref<patron_t> get_patron() const noexcept { return patrons[static_cast<usize>(patron)]; }

		inline ref<patron_t> get_patron() noexcept { return patrons[static_cast<usize>(patron)]; }

		inline void set_patron(patron_e value) noexcept {
			if (static_cast<i32>(value) < static_cast<i32>(patron_e::None) || static_cast<i32>(value) > static_cast<i32>(patron_e::Saeiligarkeuss)) {
				return;
			}

			patron = value;
		}

		inline bool has_energy() const noexcept { return energy > 0; }

		inline bool has_armor() const noexcept { return armor > 0; }

		inline bool has_ascended() const noexcept { return divinity > 0; }

		inline bool bypass_invocations_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.bypass_invocations; }

		inline i8 max_energy() const noexcept {
			const i8 calculated_max{ clamp<i8>(MinimumEnergy + game_stats.minion_kills / globals::KillsPerEnergySlot, MinimumEnergy, MaximumEnergy) };

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.energy.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.energy.current_maximum(), calculated_max);
		}

		inline i8 max_armor() const noexcept {
			const i8 calculated_max{ clamp<i8>(MinimumArmor + game_stats.player_kills / globals::KillsPerArmorSlot, MinimumArmor, MaximumArmor) };

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.armor.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.energy.current_maximum(), calculated_max);
		}

		inline i8 max_divinity() const noexcept {
			const i8 calculated_max{
				clamp<i8>(MinimumDivinity + game_stats.total_kills() / globals::KillsPerDivinityTurn, MinimumDivinity, MaximumDivinity)
			};

			if (!game_stats.cheats.is_enabled() || !game_stats.cheats.divinity.is_enabled()) {
				return calculated_max;
			}

			return max<i8>(game_stats.cheats.energy.current_maximum(), calculated_max);
		}

		inline bool no_hit_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.no_hit; }

		inline bool can_survive(i8 damage_amount) const noexcept {
			return no_hit_enabled() || has_ascended() || armor >= damage_amount;
		}

		inline void receive_damage(i8 damage_amount) noexcept {
			patrons[static_cast<usize>(patron_e::Merirfin)].relationship += damage_amount;

			if (no_hit_enabled() || has_ascended()) {
				return;
			}
			
			set_armor(armor - damage_amount);
		}

		inline bool free_costs_enabled() const noexcept { return game_stats.cheats.is_enabled() && game_stats.cheats.free_costs; }

		inline i8 random_warp_cost() const noexcept { return RandomWarpCost - get_patron().random_warp.current(get_patron().relationship); }

		inline bool can_random_warp() const noexcept { return free_costs_enabled() || energy >= random_warp_cost(); }
		inline bool can_random_warp(i8 discount) const noexcept { return free_costs_enabled() || energy >= random_warp_cost() - discount; }

		inline i8 target_warp_cost() const noexcept { return TargetWarpCost - get_patron().target_warp.current(get_patron().relationship); }

		inline bool can_target_warp() const noexcept { return free_costs_enabled() || energy >= target_warp_cost(); }
		inline bool can_target_warp(i8 discount) const noexcept { return free_costs_enabled() || energy >= target_warp_cost() - discount; }

		inline i8 calcitic_invocation_cost() const noexcept { return CalciticInvocationCost - get_patron().calcitic_invocation.current(get_patron().relationship); }

		inline bool can_perform_calcitic_invocation() const noexcept { return free_costs_enabled() || energy >= calcitic_invocation_cost(); }
		inline bool can_perform_calcitic_invocation(i8 discount) const noexcept { return free_costs_enabled() || energy >= calcitic_invocation_cost() - discount; }

		inline i8 spectral_invocation_cost() const noexcept { return SpectralInvocationCost - get_patron().spectral_invocation.current(get_patron().relationship); }

		inline bool can_perform_spectral_invocation() const noexcept { return free_costs_enabled() || energy >= spectral_invocation_cost(); }
		inline bool can_perform_spectral_invocation(i8 discount) const noexcept { return free_costs_enabled() || energy >= spectral_invocation_cost() - discount; }

		inline i8 sanguine_invocation_cost() const noexcept { return SanguineInvocationCost - get_patron().sanguine_invocation.current(get_patron().relationship); }

		inline bool can_perform_sanguine_invocation() const noexcept { return free_costs_enabled() || energy >= sanguine_invocation_cost(); }
		inline bool can_perform_sanguine_invocation(i8 discount) const noexcept { return energy >= sanguine_invocation_cost() - discount; }

		inline i8 necromantic_ascendance_cost() const noexcept { return NecromanticAscendanceCost - get_patron().necromantic_ascendance.current(get_patron().relationship); }

		inline bool can_perform_necromantic_ascendance() const noexcept { return free_costs_enabled() || energy >= necromantic_ascendance_cost(); }
		inline bool can_perform_necromantic_ascendance(i8 discount) const noexcept { return free_costs_enabled() || energy >= necromantic_ascendance_cost() - discount; }

		inline void pay_random_warp_cost() noexcept {
			++patrons[static_cast<usize>(patron_e::None)].relationship;

			if (free_costs_enabled()) {
				return;
			}	
			
			set_energy(energy - random_warp_cost());
		}

		inline void pay_random_warp_cost(i8 discount) noexcept {
			++patrons[static_cast<usize>(patron_e::None)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}	
			
			set_energy(energy - random_warp_cost() + discount);
		}


		inline void pay_target_warp_cost() noexcept {
			++patrons[static_cast<usize>(patron_e::Saeiligarkeuss)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - target_warp_cost());
		}

		inline void pay_target_warp_cost(i8 discount) noexcept {
			++patrons[static_cast<usize>(patron_e::Saeiligarkeuss)].relationship;	
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - target_warp_cost() + discount);
		}


		inline void pay_calcitic_invocation_cost() noexcept {
			--patrons[static_cast<usize>(patron_e::Rathghul)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - calcitic_invocation_cost());
		}

		inline void pay_calcitic_invocation_cost(i8 discount) noexcept {
			--patrons[static_cast<usize>(patron_e::Rathghul)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - calcitic_invocation_cost() + discount);
		}


		inline void pay_spectral_invocation_cost() noexcept {
			--patrons[static_cast<usize>(patron_e::Akurakhaithan)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - spectral_invocation_cost());
		}

		inline void pay_spectral_invocation_cost(i8 discount) noexcept {
			--patrons[static_cast<usize>(patron_e::Akurakhaithan)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - spectral_invocation_cost() + discount);
		}


		inline void pay_sanguine_invocation_cost() noexcept {
			--patrons[static_cast<usize>(patron_e::Merirfin)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - sanguine_invocation_cost());
		}

		inline void pay_sanguine_invocation_cost(i8 discount) noexcept {
			--patrons[static_cast<usize>(patron_e::Merirfin)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - sanguine_invocation_cost() + discount);
		}


		inline void pay_necromantic_ascendance_cost() noexcept {
			--patrons[static_cast<usize>(patron_e::Saeiligarkeuss)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - necromantic_ascendance_cost());
		}

		inline void pay_necromantic_ascendance_cost(i8 discount) noexcept {
			--patrons[static_cast<usize>(patron_e::Saeiligarkeuss)].relationship;
			
			if (free_costs_enabled()) {
				return;
			}
			
			set_energy(energy - necromantic_ascendance_cost() + discount);
		}

		inline void receive_skull_boon() noexcept { set_energy(energy + SkullBoon); }

		inline void receive_failed_warp_boon() noexcept { set_energy(energy + FailedWarpBoon); }

		inline void receive_unsafe_warp_boon() noexcept { set_energy(energy + UnsafeWarpBoon); }

		inline void max_out_energy() noexcept { energy = max_energy(); }

		inline void max_out_armor() noexcept { armor = max_armor(); }

		inline void max_out_divinity() noexcept { divinity = max_divinity(); }

		inline void zero_out_energy() noexcept { set_energy(0); }

		inline void zero_out_armor() noexcept { set_armor(0); }

		inline void zero_out_divinity() noexcept { set_divinity(0); }

		template<entity_type_t EntityType> inline bool will_perish() const noexcept;

		template<entity_type_t EntityType> inline void receive_damage() noexcept;

		template<entity_type_t EntityType> inline void receive_death_boon() noexcept;

		inline command_type_t clash_or_consume(cref<offset_t> position) const noexcept;

		inline void bolster_armor(i8 value) noexcept { set_armor(armor + value); }

		inline void erode_divinity() noexcept { set_divinity(divinity - DivinityErosionRate); }

		inline glyph_t current_glyph() const noexcept { return !has_armor() ? entity_glyphs<player_t> : glyphs::ArmoredPlayer; }

		inline void draw() const noexcept { game_atlas.draw(current_glyph(), position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Player; }
	};

	static inline patron_e desired_patron{ patron_e::Rathghul };
} // namespace necrowarp
