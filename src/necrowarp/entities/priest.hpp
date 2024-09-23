#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct priest_t {
		offset_t position;
		
	private:
		i8 energy;

		inline void set_energy(i8 value) noexcept { energy = clamp<i8>(value, 0, max_energy()); }

	public:
		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumEnergy{ 2 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr i8 DeathBoon{ 2 };
		static constexpr i8 ExorcismBoon{ 1 };

		static constexpr i8 StartingEnergy{ 1 };

		static constexpr i8 ResurrectCost{ 1 };
		static constexpr i8 OrdainCost{ 2 };

		inline priest_t(cref<offset_t> position) noexcept : position{ position }, energy{ StartingEnergy } {}

		inline i8 get_energy() const noexcept { return energy; }

		inline bool has_energy() const noexcept { return energy > 0; }

		inline i8 max_energy() const noexcept { return MaximumEnergy; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline bool can_resurrect() const noexcept { return energy >= ResurrectCost; }

		inline bool can_ordain() const noexcept { return energy >= OrdainCost; }

		inline void pay_resurrect_cost() noexcept { set_energy(energy - ResurrectCost); }

		inline void pay_ordain_cost() noexcept { set_energy(energy - OrdainCost); }

		inline void receive_exorcism_boon() noexcept { set_energy(energy + ExorcismBoon); }

		inline void receive_exorcism_boon(bool is_fresh) noexcept { set_energy(energy + ExorcismBoon * is_fresh ? 2 : 1); }

		inline entity_command_t think() const noexcept;

		inline void draw() const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Priest], position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Priest], position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Priest], position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Priest], position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Priest; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<priest_t> priest) noexcept { return offset_t::hasher::operator()(priest.position); }

				static constexpr usize operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<priest_t> lhs, cref<priest_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<priest_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(cref<offset_t> lhs, cref<priest_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};

	template<> struct is_entity<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<priest_t, entity_type_t::Priest> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Priest> {
		using type = priest_t;
	};
} // namespace necrowarp
