#pragma once

#include "bleak/offset/offset_2d.hpp"
#include "bleak/typedef.hpp"

#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>

#include "bleak/array.hpp"
#include "bleak/color.hpp"
#include "bleak/extent.hpp"
#include "bleak/glyph.hpp"
#include "bleak/offset.hpp"
#include "bleak/text.hpp"
#include "bleak/texture.hpp"
#include "extent/extent_2d.hpp"

namespace bleak {
	template<extent_2d_t Size> class atlas_t {
	  private:
		layer_t<rect_t, Size> rects;

		texture_t texture;

	  public:
		// The size of the atlas in glyphs.
		static constexpr extent_2d_t size{ Size };

		// The size of the image in pixels.
		const extent_2d_t image_size;
		// The size of each glyph in pixels.
		const extent_2d_t glyph_size;

		offset_2d_t universal_offset{};

		inline atlas_t() = delete;

		inline atlas_t(ref<renderer_t> renderer, cstr path) :
			rects{},
			texture{ renderer, path },

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

			extent_2d_t::product_t index{ 0 };
			for (int y = 0; y < size.h; ++y) {
				for (int x = 0; x < size.w; ++x) {
					rects[index] = rect_t{ offset_2d_t{ x, y } * glyph_size, glyph_size };
					++index;
				}
			}
		}

		inline atlas_t(ref<renderer_t> renderer, cstr path, cref<offset_2d_t> offset) :
			rects{},
			texture{ renderer, path },

			image_size{ this->texture.info.size },
			glyph_size{ image_size / size },
			universal_offset{ offset } {
			if (image_size.w <= 0 || image_size.h <= 0) {
				throw std::runtime_error("image size must be greater than zero!");
			}

			if (glyph_size.w <= 0 || glyph_size.h <= 0) {
				throw std::runtime_error("glyph size must be greater than zero!");
			}

			if (image_size.w % size.w != 0 || image_size.h % size.h != 0) {
				throw std::runtime_error("image size must be divisible by the atlas size!");
			}

			extent_2d_t::product_t index{ 0 };
			for (int y = 0; y < size.h; ++y) {
				for (int x = 0; x < size.w; ++x) {
					rects[index] = rect_t{ offset_2d_t{ x, y } * glyph_size, glyph_size };
					++index;
				}
			}
		}

		inline atlas_t(cref<atlas_t> other) = delete;
		inline atlas_t(rval<atlas_t> other) = delete;

		inline ref<atlas_t> operator=(cref<atlas_t> other) = delete;
		inline ref<atlas_t> operator=(rval<atlas_t> other) = delete;

		inline ~atlas_t() = default;

		inline void draw(ref<renderer_t> renderer, cref<glyph_t> glyph, cref<offset_2d_t> position) const noexcept {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				error_log.add("glyph index {} is out of range!", glyph.index);
				return;
			}

			texture.draw(renderer, rects[glyph.index], rect_t{ position * glyph_size, glyph_size } + universal_offset, glyph.color);
		}

		inline void draw(ref<renderer_t> renderer, cref<glyph_t> glyph, cref<offset_2d_t> position, cref<offset_2d_t> offset) const noexcept {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				error_log.add("glyph index {} is out of range!", glyph.index);
				return;
			}

			texture.draw(renderer, rects[glyph.index], rect_t{ position * glyph_size + offset, glyph_size } + universal_offset, glyph.color);
		}

		inline void draw(ref<renderer_t> renderer, cref<runes_t> runes, cref<offset_2d_t> position) const {
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
					draw(renderer, rune, position + carriage_pos);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(ref<renderer_t> renderer, cref<runes_t> runes, cref<offset_2d_t> position, cref<cardinal_t> alignment) const {
			if (runes.empty()) {
				return;
			}

			const extent_2d_t size{Text::calculate_size(runes)};
			const offset_2d_t origin{ position - size / 2 };
			const offset_2d_t size_offs{ static_cast<offset_2d_t>(alignment) * size };
			const offset_2d_t alignment_offs{ size_offs - size_offs / 2 };

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
					draw(renderer, rune, origin + carriage_pos + alignment_offs);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(ref<renderer_t> renderer, cref<runes_t> runes, cref<offset_2d_t> position, cref<cardinal_t> alignment, cref<offset_2d_t> offset) const {
			if (runes.empty()) {
				return;
			}

			const extent_2d_t size{Text::calculate_size(runes)};
			const offset_2d_t origin{ position - size / 2 };
			const offset_2d_t size_offs{ static_cast<offset_2d_t>(alignment) * size };
			const offset_2d_t alignment_offs{ size_offs - size_offs / 2 };

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
					draw(renderer, rune, origin + carriage_pos + alignment_offs, offset);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(ref<renderer_t> renderer, cref<std::string> text, cref<offset_2d_t> position, cref<color_t> color) const {
			if (text.empty()) {
				return;
			}

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
					draw(renderer, glyph_t{ static_cast<u8>(ch), color }, position + carriage_pos);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(ref<renderer_t> renderer, cref<std::string> text, cref<offset_2d_t> position, cref<color_t> color, cref<cardinal_t> alignment) const {
			if (text.empty()) {
				return;
			}

			const extent_2d_t size{Text::calculate_size(text)};
			const offset_2d_t origin{ position - size / 2 };
			const offset_2d_t size_offs{ static_cast<offset_2d_t>(alignment) * size };
			const offset_2d_t alignment_offs{ size_offs - size_offs / 2 };

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
					draw(renderer, glyph_t{ static_cast<u8>(ch), color }, origin + carriage_pos + alignment_offs);
					++carriage_pos.x;
					continue;
				}
			}
		}
	};
} // namespace bleak
