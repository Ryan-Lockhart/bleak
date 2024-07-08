#pragma once

#include <stdexcept>
#include <utility>

#include <SDL.h>
#include <SDL_image.h>

#include "bleak/array.hpp"
#include "bleak/color.hpp"
#include "bleak/extent.hpp"
#include "bleak/glyph.hpp"
#include "bleak/offset.hpp"
#include "bleak/offset/offset_2d.hpp"
#include "bleak/text.hpp"
#include "bleak/texture.hpp"

namespace bleak {
	template<extent_2d_t Size> class atlas_t {
	  public:
		// The size of the atlas in glyphs.
		static constexpr extent_2d_t size{ Size };

		// The size of the image in pixels.
		const extent_2d_t image_size;
		// The size of each glyph in pixels.
		const extent_2d_t glyph_size;

		inline atlas_t() = delete;

		inline atlas_t(rval<texture_t> texture) :
			rects{},
			texture{ std::move(texture) },

			image_size{ this->texture.info.size },
			glyph_size{ image_size / size } {
			if (image_size.w <= 0 || image_size.h <= 0) {
				throw std::runtime_error("image size must be greater than zero!");
			}
			if (glyph_size.w <= 0 || glyph_size.h <= 0) {
				throw std::runtime_error("glyph size must be greater than zero!");
			}

			if (image_size.w % size.w != 0 || image_size.h % size.h != 0) {
				throw std::runtime_error("image size must be divisible by the atlas size!");
			}

			for (int y = 0; y < size.h; ++y) {
				for (int x = 0; x < size.w; ++x) {
					rects[x, y] = { x * glyph_size.w, y * glyph_size.h, glyph_size.w, glyph_size.h };
				}
			}
		}

		inline atlas_t(cref<atlas_t> other) = delete;
		inline atlas_t(rval<atlas_t> other) = delete;

		inline ref<atlas_t> operator=(cref<atlas_t> other) = delete;
		inline ref<atlas_t> operator=(rval<atlas_t> other) = delete;

		inline ~atlas_t() = default;

		inline void draw(cref<glyph_t> glyph, cref<offset_2d_t> position) const {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				throw std::out_of_range("glyph index out of range!");
			}

			const offset_2d_t pos = position * glyph_size;
			const SDL_Rect dst{ pos.x, pos.y, glyph_size.w, glyph_size.h };

			texture.draw(&rects[glyph.index], &dst, glyph.color);
		}

		inline void draw(cref<glyph_t> glyph, cref<offset_2d_t> position, cref<extent_2d_t> offset) const {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				throw std::out_of_range("glyph index out of range!");
			}

			const offset_2d_t pos = position * glyph_size + offset;
			const SDL_Rect dst{ pos.x, pos.y, glyph_size.w, glyph_size.h };

			texture.draw(&rects[glyph.index], &dst, glyph.color);
		}

		inline void draw(cref<runes_t> runes, cref<offset_2d_t> position) const {
			if (runes.empty()) {
				return;
			}

			offset_2d_t carriage_pos{ 0 };

			for (auto& rune : runes) {
				switch (rune.index) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + Text::HORIZONTAL_TAB_WIDTH - 1) & -Text::HORIZONTAL_TAB_WIDTH;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + Text::VERTICAL_TAB_WIDTH - 1) & -Text::VERTICAL_TAB_WIDTH;
					carriage_pos.x = 0;
					continue;
				default:
					draw(rune, position + carriage_pos);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<runes_t> runes, cref<offset_2d_t> position, cardinal_t alignment) const {
			if (runes.empty()) {
				return;
			}

			const extent_2d_t size{ static_cast<extent_2d_t>(Text::calculate_size(runes)) * alignment };
			const offset_2d_t alignment_offs{ size - size / 2 };

			offset_2d_t carriage_pos{ 0 };

			for (auto& rune : runes) {
				switch (rune.index) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + Text::HORIZONTAL_TAB_WIDTH - 1) & -Text::HORIZONTAL_TAB_WIDTH;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + Text::VERTICAL_TAB_WIDTH - 1) & -Text::VERTICAL_TAB_WIDTH;
					carriage_pos.x = 0;
					continue;
				default:
					draw(rune, position + carriage_pos + alignment_offs);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<runes_t> runes, cref<offset_2d_t> position, cardinal_t alignment, cref<offset_2d_t> offset) const {
			if (runes.empty()) {
				return;
			}

			const extent_2d_t size{ static_cast<extent_2d_t>(Text::calculate_size(runes)) * alignment };
			const extent_2d_t alignment_offs{ size - size / 2 };

			offset_2d_t carriage_pos{ 0 };

			for (auto& rune : runes) {
				switch (rune.index) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + Text::HORIZONTAL_TAB_WIDTH - 1) & -Text::HORIZONTAL_TAB_WIDTH;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + Text::VERTICAL_TAB_WIDTH - 1) & -Text::VERTICAL_TAB_WIDTH;
					carriage_pos.x = 0;
					continue;
				default:
					draw(rune, position + carriage_pos + alignment_offs, offset);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, offset_2d_t position, color_t color) const {
			if (text.empty()) {
				return;
			}

			const extent_2d_t size{ static_cast<extent_2d_t>(Text::calculate_size(text)) };
			const offset_2d_t alignment_offs{ size - size / 2 };

			offset_2d_t carriage_pos{ 0 };

			for (auto& ch : text) {
				switch (ch) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + Text::HORIZONTAL_TAB_WIDTH - 1) & -Text::HORIZONTAL_TAB_WIDTH;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + Text::VERTICAL_TAB_WIDTH - 1) & -Text::VERTICAL_TAB_WIDTH;
					carriage_pos.x = 0;
					continue;
				default:
					draw({ static_cast<u8>(ch), color }, position + carriage_pos + alignment_offs);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, offset_2d_t position, color_t color, cardinal_t alignment) const {
			if (text.empty()) {
				return;
			}

			const extent_2d_t size{ static_cast<extent_2d_t>(Text::calculate_size(text)) * alignment };
			const offset_2d_t alignment_offs{ size - size / 2 };

			offset_2d_t carriage_pos{ 0 };

			for (auto& ch : text) {
				switch (ch) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + Text::HORIZONTAL_TAB_WIDTH - 1) & -Text::HORIZONTAL_TAB_WIDTH;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + Text::VERTICAL_TAB_WIDTH - 1) & -Text::VERTICAL_TAB_WIDTH;
					carriage_pos.x = 0;
					continue;
				default:
					draw({ static_cast<u8>(ch), color }, position + carriage_pos + alignment_offs);
					++carriage_pos.x;
					continue;
				}
			}
		}

	  private:
		static_assert(sizeof(sdl::rect) * Size.w * Size.h <= memory::maximum, "atlas size exceeds memory limit!");

		layer_t<sdl::rect, size> rects;

		texture_t texture;
	};
} // namespace bleak
