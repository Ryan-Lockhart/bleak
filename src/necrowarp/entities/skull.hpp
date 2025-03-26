#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<skull_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<skull_t, entity_type_t::Skull> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Skull> {
		using type = skull_t;
	};

	template<> struct is_inanimate<skull_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<skull_t>{ glyphs::FreshSkull };

	struct skull_t {
		offset_t position;
		const bool fresh;

		inline skull_t(cref<offset_t> position) noexcept : position{ position }, fresh{ false } {}

		inline skull_t(cref<offset_t> position, bool fresh) noexcept : position{ position }, fresh{ fresh } {}

		inline bool is_fresh() const noexcept { return fresh; }

		inline glyph_t current_glyph() const noexcept { return fresh ? entity_glyphs<skull_t> : glyphs::AnimateSkull; }

		inline void draw() const noexcept { game_atlas.draw(entity_glyphs<skull_t>, position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(current_glyph(), position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(current_glyph(), position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Skull; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<skull_t> skull) noexcept { return offset_t::hasher::operator()(skull.position); }

				static constexpr usize operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<skull_t> lhs, cref<skull_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<skull_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(cref<offset_t> lhs, cref<skull_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
