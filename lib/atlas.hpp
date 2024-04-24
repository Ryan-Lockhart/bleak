#pragma once

#include <stdexcept>
#include <utility>

#include <SDL.h>
#include <SDL_image.h>

#include "array.hpp"
#include "texture.hpp"

#include "point.hpp"
#include "rect.hpp"

#include "glyph.hpp"

namespace Bleakdepth {
	template<usize Width, usize Height> class atlas_t {
	  private:
		static_assert(Width > 0, "atlas width must be greater than zero!");
		static_assert(Height > 0, "atlas height must be greater than zero!");

		Array<rect_t<i32>, Width, Height> rects;

		texture_t texture;

		const size_t<i32> imageSize;
		const size_t<i32> glyphSize;

	  public:
		static constexpr size_t<i32> size { Width, Height };

		inline atlas_t() = delete;

		inline atlas_t(rval<texture_t> texture) :
			rects {},
			texture { std::move(texture) },

			imageSize { this->texture.info.size },
			glyphSize { imageSize / size } {
			if (imageSize.x <= 0 || imageSize.y <= 0) {
				throw std::runtime_error("image size must be greater than zero!");
			}
			if (glyphSize.x <= 0 || glyphSize.y <= 0) {
				throw std::runtime_error("glyph size must be greater than zero!");
			}

			if (imageSize.x % size.w != 0 || imageSize.y % size.h != 0) {
				throw std::runtime_error("image size must be divisible by the atlas size!");
			}

			for (int y = 0; y < size.y; ++y) {
				for (int x = 0; x < size.x; ++x) {
					rects[x, y] = rect_t { x * glyphSize.x, y * glyphSize.y, glyphSize.w, glyphSize.h };
				}
			}
		}

		inline atlas_t(cref<atlas_t> other) = delete;
		inline atlas_t(rval<atlas_t> other) = delete;

		inline ref<atlas_t> operator=(cref<atlas_t> other) = delete;
		inline ref<atlas_t> operator=(rval<atlas_t> other) = delete;

		inline ~atlas_t() = default;

		inline void draw(cref<glyph_t> glyph, cref<point_t<i32>> position, bool onGrid = true) const {
			if (glyph.index < 0 || glyph.index >= rects.size()) {
				throw std::out_of_range("glyph index out of range!");
			}

			const point_t pos = onGrid ? position * glyphSize : position;
			const SDL_Rect dst { pos.x, pos.y, glyphSize.x, glyphSize.y };

			texture.draw(&rects[glyph.index], &dst, glyph.color);
		}
	};
} // namespace Bleakdepth
