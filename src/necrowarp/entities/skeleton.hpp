#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct skeleton_t {
	  private:
		offset_t position;
		i32 kills;

	  public:
		inline skeleton_t(cref<offset_t> position) noexcept : position{ position }, kills{ 0 } {}

		inline ~skeleton_t() noexcept { total_kills += kills; }

		inline void draw() const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Skeleton], position); }

		inline void draw(cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Skeleton], position + offset); }

		inline void draw(cref<camera_t> camera) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Skeleton], position + camera.get_offset()); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept { game_atlas.draw(EntityGlyphs[entity_type_t::Skeleton], position + camera.get_offset() + offset); }

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Skeleton; }
	};

	template<> struct is_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<skeleton_t, entity_type_t::Skeleton> {
		static constexpr bool value = true;
	};
} // namespace necrowarp
