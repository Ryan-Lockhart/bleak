#pragma once

#include "typedef.hpp"

#include "point.hpp"
#include "glyph.hpp"

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
            position = other.position;
            glyph = other.glyph;

            return *this;
        }

        constexpr inline ref<sprite_t> operator=(rval<sprite_t> other) noexcept {
            position = std::move(other.position);
            glyph = std::move(other.glyph);

            return *this;
        }

        template<usize Width, usize Height> constexpr inline void draw(cref<atlas_t<Width, Height>> atlas) const {
            atlas.draw(glyph, position);
        }
    };
}
