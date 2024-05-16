#pragma once

#include "typedef.hpp"

#include "glyph.hpp"
#include "point.hpp"

#include "atlas.hpp"

namespace Bleakdepth {
	struct sprite_t {
		point_t<i32> position;
		glyph_t glyph;

		constexpr inline sprite_t() noexcept = delete;

		constexpr inline sprite_t(cref<glyph_t> glyph) noexcept : position { 0, 0 }, glyph { glyph } {}

		constexpr inline sprite_t(rval<glyph_t> glyph) noexcept : position { 0, 0 }, glyph { std::move(glyph) } {}

		constexpr inline sprite_t(cref<glyph_t> glyph, cref<point_t<i32>> position) noexcept : position { position }, glyph { glyph } {}

		constexpr inline sprite_t(rval<glyph_t> glyph, rval<point_t<i32>> position) noexcept : position { std::move(position) }, glyph { std::move(glyph) } {}

		constexpr inline sprite_t(cref<sprite_t> other) noexcept : position { other.position }, glyph { other.glyph } {}

		constexpr inline sprite_t(rval<sprite_t> other) noexcept : position { std::move(other.position) }, glyph { std::move(other.glyph) } {}

		constexpr inline ~sprite_t() noexcept = default;

		constexpr inline ref<sprite_t> operator=(cref<sprite_t> other) noexcept {
			if (this != &other) {
				position = other.position;
				glyph = other.glyph;
			}

			return *this;
		}

		constexpr inline ref<sprite_t> operator=(rval<sprite_t> other) noexcept {
			if (this != &other) {
				position = std::move(other.position);
				glyph = std::move(other.glyph);
			}

			return *this;
		}

		template<usize Width, usize Height> constexpr inline void draw(cref<atlas_t<Width, Height>> atlas) const { atlas.draw(glyph, position); }

		template<usize Width, usize Height> constexpr inline void draw(cref<atlas_t<Width, Height>> atlas, cref<point_t<i32>> offset) const {
			atlas.draw((glyph_t)glyph, position + offset);
		}
	};

	template<usize Length> struct animated_sprite_t {
		animated_glyph_t<Length> glyph;
		point_t<i32> position;

		constexpr inline animated_sprite_t() noexcept = delete;

		constexpr inline animated_sprite_t(rval<animated_glyph_t<Length>> glyph) noexcept : glyph { std::move(glyph) }, position { 0, 0 } {}

		constexpr inline animated_sprite_t(rval<animated_glyph_t<Length>> glyph, cref<point_t<i32>> position) noexcept :
			glyph { std::move(glyph) },
			position { position } {}

		constexpr inline animated_sprite_t(rval<animated_glyph_t<Length>> glyph, rval<point_t<i32>> position) noexcept :
			glyph { std::move(glyph) },
			position { std::move(position) } {}

		constexpr inline animated_sprite_t(cref<animated_sprite_t> other) noexcept : glyph { other.glyph }, position { other.position } {}

		constexpr inline animated_sprite_t(rval<animated_sprite_t> other) noexcept : glyph { std::move(other.glyph) }, position { std::move(other.position) } {}

		constexpr inline ~animated_sprite_t() noexcept = default;

		constexpr inline ref<animated_sprite_t> operator=(cref<animated_sprite_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			glyph = other.glyph;
			position = other.position;

			return *this;
		}

		constexpr inline ref<animated_sprite_t> operator=(rval<animated_sprite_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			glyph = std::move(other.glyph);
			position = std::move(other.position);

			return *this;
		}

		template<usize Width, usize Height> constexpr inline void draw(cref<atlas_t<Width, Height>> atlas) const { atlas.draw((glyph_t)glyph, position); }

		template<usize Width, usize Height> constexpr inline void draw(cref<atlas_t<Width, Height>> atlas, cref<point_t<i32>> offset) const {
			atlas.draw((glyph_t)glyph, position + offset);
		}
	};
} // namespace Bleakdepth
