#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<priest_t, entity_type_t::Priest> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Priest> {
		using type = priest_t;
	};

	template<> struct is_good_entity<priest_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<priest_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<priest_t>{ glyphs::Priest };

	struct priest_t {
		offset_t position;
		
	private:
		i8 piety;

		inline void set_piety(i8 value) noexcept { piety = clamp<i8>(value, 0, max_piety()); }

	public:
		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumPiety{ 8 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr i8 DeathBoon{ 2 };

		static constexpr i8 StartingPiety{ 8 };

		static constexpr i8 ExorcismCost{ 1 };
		static constexpr i8 ResurrectCost{ 2 };
		static constexpr i8 AnointCost{ 4 };

		inline priest_t(cref<offset_t> position) noexcept : position{ position }, piety{ StartingPiety } {}

		inline i8 get_piety() const noexcept { return piety; }

		inline bool has_piety() const noexcept { return piety > 0; }

		inline i8 max_piety() const noexcept { return MaximumPiety; }

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline bool can_exorcise() const noexcept { return piety >= ExorcismCost; }

		inline bool can_resurrect() const noexcept { return piety >= ResurrectCost; }

		inline bool can_anoint() const noexcept { return piety >= AnointCost; }

		inline void pay_exorcise_cost() noexcept { set_piety(piety - ExorcismCost); }

		inline void pay_resurrect_cost() noexcept { set_piety(piety - ResurrectCost); }

		inline void pay_ordain_cost() noexcept { set_piety(piety - AnointCost); }

		inline entity_command_t think() const noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<priest_t>, position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<priest_t>, position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<priest_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<priest_t>, position + camera.get_offset() + offset); }

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
} // namespace necrowarp
