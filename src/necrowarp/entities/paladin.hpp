#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct paladin_t {
		offset_t position;
		i8 health;

		static constexpr i8 MaximumHealth{ 2 };
		static constexpr i8 MaximumDamage{ 1 };
		static constexpr i8 DeathBoon{ 2 };

		inline paladin_t(cref<offset_t> position) noexcept : position{ position }, health{ MaximumHealth } {}

		inline entity_command_t think() const noexcept;

		inline void draw() const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Paladin], position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Paladin], position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Paladin], position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Paladin], position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Paladin; }

		struct hasher {
			using is_transparent = void;

			static constexpr usize operator()(cref<paladin_t> paladin) noexcept { return offset_t::hasher::operator()(paladin.position); }

			static constexpr usize operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
		};

		struct comparator {
			using is_transparent = void;
			
			static constexpr bool operator()(cref<paladin_t> lhs, cref<paladin_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

			static constexpr bool operator()(cref<paladin_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

			static constexpr bool operator()(cref<offset_t> lhs, cref<paladin_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
		};
	};

	template<> struct is_entity<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<paladin_t, entity_type_t::Paladin> {
		static constexpr bool value = true;
	};
} // namespace necrowarp
