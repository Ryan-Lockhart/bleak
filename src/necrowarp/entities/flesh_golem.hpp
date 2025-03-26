#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<flesh_golem_t, entity_type_t::FleshGolem> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::FleshGolem> {
		using type = flesh_golem_t;
	};

	template<> struct is_evil_entity<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<flesh_golem_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<flesh_golem_t>{ glyphs::FleshGolem };

	struct flesh_golem_t {
		offset_t position;

	private:
		i8 health;

		inline void set_health(i8 value) noexcept { health = clamp<i8>(value, 0, max_health()); }
	
	public:
		static constexpr i8 MaximumHealth{ 8 };
		static constexpr i8 MaximumDamage{ 1 };

		static constexpr i8 MinimumArmorBoon{ 1 };

		static constexpr i8 MinimumDamageReceived{ 1 };

		inline i8 armor_boon() const noexcept { return max<i8>(MinimumArmorBoon, health) * 2; }
		
		inline flesh_golem_t(cref<offset_t> position, i8 health) noexcept : position{ position }, health{ health } {}
		
		inline i8 get_health() const noexcept { return health; }

		inline bool has_health() const noexcept { return health > 0; }

		constexpr i8 max_health() const noexcept { return MaximumHealth; }

		inline i8 filter_damage(i8 damage_amount) const noexcept { return min<i8>(MinimumDamageReceived, damage_amount); }

		inline bool can_survive(i8 damage_amount) const noexcept { return health > filter_damage(damage_amount); }

		inline void receive_damage(i8 damage_amount) noexcept { set_health(health - filter_damage(damage_amount)); }

		inline entity_command_t think() const noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<flesh_golem_t>, position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<flesh_golem_t>, position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<flesh_golem_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<flesh_golem_t>, position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::FleshGolem; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<flesh_golem_t> flesh_golem) noexcept { return offset_t::hasher::operator()(flesh_golem.position); }

				static constexpr usize operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<flesh_golem_t> lhs, cref<flesh_golem_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<flesh_golem_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(cref<offset_t> lhs, cref<flesh_golem_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
