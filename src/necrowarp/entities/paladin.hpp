#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<paladin_t, entity_type_t::Paladin> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Paladin> {
		using type = paladin_t;
	};

	template<> struct is_good_entity<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<paladin_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<paladin_t>{ glyphs::Paladin };

	struct paladin_t {
		offset_t position;
		
	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }

	public:
		static constexpr i8 MaximumHealth{ 2 };
		static constexpr i8 MaximumDamage{ 1 };
		static constexpr i8 DeathBoon{ 2 };

		inline paladin_t(cref<offset_t> position) noexcept : position{ position }, health{ MaximumHealth } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > damage_amount; }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - damage_amount); }

		inline entity_command_t think() const noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<paladin_t>, position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<paladin_t>, position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<paladin_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<paladin_t>, position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Paladin; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<paladin_t> paladin) noexcept { return offset_t::hasher::operator()(paladin.position); }

				static constexpr usize operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<paladin_t> lhs, cref<paladin_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<paladin_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(cref<offset_t> lhs, cref<paladin_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
