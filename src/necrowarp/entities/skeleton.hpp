#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;
	
	template<> struct is_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<skeleton_t, entity_type_t::Skeleton> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Skeleton> {
		using type = skeleton_t;
	};

	template<> struct is_evil_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<skeleton_t>{ glyphs::Skeleton };

	struct skeleton_t {
		offset_t position;
		const bool rotted;

		static constexpr i8 MaximumHealth{ 1 };
		static constexpr i8 MaximumDamage{ 1 };
		
		constexpr i8 armor_boon() const noexcept { return rotted ? 1 : 2; }

		inline skeleton_t(cref<offset_t> position, bool rotted) noexcept : position{ position }, rotted{ rotted } {}

		inline bool can_survive(i8 damage_amount) const noexcept { return damage_amount <= 0; }

		inline entity_command_t think() const noexcept;

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<skeleton_t>, position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<skeleton_t>, position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<skeleton_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<skeleton_t>, position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Skeleton; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<skeleton_t> skeleton) noexcept { return offset_t::hasher::operator()(skeleton.position); }

				static constexpr usize operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<skeleton_t> lhs, cref<skeleton_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<skeleton_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(cref<offset_t> lhs, cref<skeleton_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
