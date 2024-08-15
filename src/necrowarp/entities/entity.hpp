#pragma once

#include <bleak.hpp>

namespace necrowarp {
    using namespace bleak;

    enum struct entity_type_t : u8 {
        None = 0,
        Player,
        Skull,
        Skeleton,
        Adventurer
    };

    constexpr usize EntityTypeCount { static_cast<usize>(entity_type_t::Adventurer) + 1 };

    template<typename T> struct is_entity {
        static constexpr bool value = false;
    };

    template<typename T> constexpr bool is_entity_v = is_entity<T>::value;

    template<typename T, entity_type_t EntityType> struct is_entity_type {
        static constexpr bool value = false;
    };

    template<typename T, entity_type_t EntityType> constexpr bool is_entity_type_v = is_entity_type<T, EntityType>::value;

    constexpr lut_t<entity_type_t, glyph_t, EntityTypeCount> EntityGlyphs {
        pair_t<entity_type_t, glyph_t>{ entity_type_t::None, glyph_t{ 0x00, colors::White } },
        pair_t<entity_type_t, glyph_t>{ entity_type_t::Player, glyph_t{ 0x40, colors::White } },
        pair_t<entity_type_t, glyph_t>{ entity_type_t::Skull, glyph_t{ 0x46, colors::White } },
        pair_t<entity_type_t, glyph_t>{ entity_type_t::Skeleton, glyph_t{ 0x43, colors::White } },
        pair_t<entity_type_t, glyph_t>{ entity_type_t::Adventurer, glyph_t{ 0x42, colors::White } }
    };
}