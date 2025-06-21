#pragma once

#include <bleak/typedef.hpp>

#include <bleak/atlas.hpp>
#include <bleak/extent.hpp>
#include <bleak/glyph.hpp>
#include <bleak/hash.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	struct sprite_t {
		offset_t position;
		glyph_t glyph;

		constexpr inline sprite_t() noexcept = delete;

		constexpr inline sprite_t(glyph_t glyph) noexcept : position{ 0, 0 }, glyph{ glyph } {}

		constexpr inline sprite_t(rval<glyph_t> glyph) noexcept : position{ 0, 0 }, glyph{ std::move(glyph) } {}

		constexpr inline sprite_t(glyph_t glyph, offset_t position) noexcept : position{ position }, glyph{ glyph } {}

		constexpr inline sprite_t(rval<glyph_t> glyph, rval<offset_t> position) noexcept : position{ std::move(position) }, glyph{ std::move(glyph) } {}

		constexpr inline sprite_t(cref<sprite_t> other) noexcept : position{ other.position }, glyph{ other.glyph } {}

		constexpr inline sprite_t(rval<sprite_t> other) noexcept : position{ std::move(other.position) }, glyph{ std::move(other.glyph) } {}

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

		template<extent_t AtlasSize> constexpr inline void draw(cref<atlas_t<AtlasSize>> atlas) const { atlas.draw(glyph, position); }

		template<extent_t AtlasSize> constexpr inline void draw(cref<atlas_t<AtlasSize>> atlas, offset_t offset) const { atlas.draw((glyph_t)glyph, position + offset); }

		struct hasher {
			static constexpr inline size_t operator()(cref<sprite_t> sprite) noexcept { return hash_combine(sprite.position, sprite.glyph); }

			struct offset {
				static constexpr inline size_t operator()(cref<sprite_t> sprite) noexcept { return hash_combine(sprite.position); }
			};
		};
	};
} // namespace bleak
