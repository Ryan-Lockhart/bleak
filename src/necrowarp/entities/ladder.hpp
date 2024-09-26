#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<ladder_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<ladder_t, entity_type_t::Ladder> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Ladder> {
		using type = ladder_t;
	};

	template<> struct is_inanimate<ladder_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<ladder_t>{ 0x47, colors::White };

	struct ladder_t {
		offset_t position;

		inline ladder_t(cref<offset_t> position) noexcept : position{ position } {}

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<ladder_t>, position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<ladder_t>, position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(entity_glyphs<ladder_t>, position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(entity_glyphs<ladder_t>, position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Ladder; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<ladder_t> skull) noexcept { return offset_t::hasher::operator()(skull.position); }

				static constexpr usize operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<ladder_t> lhs, cref<ladder_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<ladder_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(cref<offset_t> lhs, cref<ladder_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
