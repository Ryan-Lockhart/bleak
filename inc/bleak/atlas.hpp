#pragma once

#include <bleak/typedef.hpp>

#include <cctype>
#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>

#include <bleak/array.hpp>
#include <bleak/color.hpp>
#include <bleak/extent.hpp>
#include <bleak/glyph.hpp>
#include <bleak/offset.hpp>
#include <bleak/renderer.hpp>
#include <bleak/text.hpp>
#include <bleak/texture.hpp>

namespace bleak {
	static constexpr offset_t generate_alignment_offset(cref<runes_t> runes, cref<offset_t> position, cref<cardinal_t> alignment) {
		const offset_t size_offs{ static_cast<offset_t>(alignment) * text::calculate_size(runes) };

		return offset_t{ size_offs - size_offs / 2 };
	}

	static constexpr offset_t generate_alignment_offset(cref<std::string> text, cref<offset_t> position, cref<cardinal_t> alignment) {
		const offset_t size_offs{ static_cast<offset_t>(alignment) * text::calculate_size(text) };
		
		return offset_t{ size_offs - size_offs / 2 };
	}

	static constexpr offset_t generate_alignment_offset(cref<runes_t> runes, cref<offset_t> position, cref<cardinal_t> alignment, cref<extent_t> size) {
		const offset_t size_offs{ static_cast<offset_t>(alignment) * size };

		return offset_t{ size_offs - size_offs / 2 };
	}

	static constexpr offset_t generate_alignment_offset(cref<std::string> text, cref<offset_t> position, cref<cardinal_t> alignment, cref<extent_t> size) {
		const offset_t size_offs{ static_cast<offset_t>(alignment) * size };
		
		return offset_t{ size_offs - size_offs / 2 };
	}

	template<extent_t Size> class atlas_t {
	  private:
		array_t<rect_t, Size> rects;

		texture_t texture;

	  public:
		// The size of the atlas in glyphs.
		static constexpr extent_t size{ Size };

		// The size of the image in pixels.
		const extent_t image_size;
		// The size of each glyph in pixels.
		const extent_t glyph_size;

		offset_t universal_offset{};

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

			extent_t::product_t index{ 0 };
			for (int y = 0; y < size.h; ++y) {
				for (int x = 0; x < size.w; ++x) {
					rects[index++] = rect_t{ offset_t{ x, y } * glyph_size, glyph_size };
				}
			}
		}

		inline atlas_t(ref<renderer_t> renderer, cstr path, cref<offset_t> offset) :
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

			extent_t::product_t index{ 0 };
			for (int y = 0; y < size.h; ++y) {
				for (int x = 0; x < size.w; ++x) {
					rects[index++] = rect_t{ offset_t{ x, y } * glyph_size, glyph_size };
				}
			}
		}

		inline atlas_t(cref<atlas_t> other) = delete;
		inline atlas_t(rval<atlas_t> other) = delete;

		inline ref<atlas_t> operator=(cref<atlas_t> other) = delete;
		inline ref<atlas_t> operator=(rval<atlas_t> other) = delete;

		inline ~atlas_t() = default;

		inline void draw(cref<glyph_t> glyph, cref<offset_t> position) const noexcept {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				error_log.add("glyph index {} is out of range!", glyph.index);
				return;
			}

			texture.draw(rects[glyph.index], rect_t{ position * glyph_size, glyph_size } + universal_offset, glyph.color);
		}

		inline void draw(cref<glyph_t> glyph, cref<offset_t> position, cref<offset_t> offset) const noexcept {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				error_log.add("glyph index {} is out of range!", glyph.index);
				return;
			}

			texture.draw(rects[glyph.index], rect_t{ position * glyph_size + offset, glyph_size } + universal_offset, glyph.color);
		}

		inline void draw(cref<runes_t> runes, cref<offset_t> position) const {
			if (runes.empty()) {
				return;
			}

			offset_t carriage_pos{ 0 };

			for (auto& rune : runes) {
				switch (rune.index) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + text::HorizontalTabWidth - 1) & -text::HorizontalTabWidth;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + text::VerticalTabWidth - 1) & -text::VerticalTabWidth;
					carriage_pos.x = 0;
					continue;
				default:
					draw(rune, position + carriage_pos);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<runes_t> runes, cref<offset_t> position, cref<cardinal_t> alignment) const {
			if (runes.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(runes) };
			const offset_t origin{ position - size / 2 };
			const offset_t alignment_offs{ generate_alignment_offset(runes, position, alignment, size) };

			offset_t carriage_pos{ 0 };

			for (auto& rune : runes) {
				switch (rune.index) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + text::HorizontalTabWidth - 1) & -text::HorizontalTabWidth;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + text::VerticalTabWidth - 1) & -text::VerticalTabWidth;
					carriage_pos.x = 0;
					continue;
				case ' ':
					++carriage_pos.x;
					continue;
				default:
					draw(rune, origin + carriage_pos + alignment_offs);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<runes_t> runes, cref<offset_t> position, cref<cardinal_t> alignment, cref<offset_t> offset) const {
			if (runes.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(runes) };
			const offset_t origin{ position - size / 2 };
			const offset_t alignment_offs{ generate_alignment_offset(runes, position, alignment, size) };

			offset_t carriage_pos{ 0 };

			for (cauto rune : runes) {
				switch (rune.index) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + text::HorizontalTabWidth - 1) & -text::HorizontalTabWidth;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + text::VerticalTabWidth - 1) & -text::VerticalTabWidth;
					carriage_pos.x = 0;
					continue;
				case ' ':
					++carriage_pos.x;
					continue;
				default:
					draw(rune, origin + carriage_pos + alignment_offs, offset);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, cref<color_t> color, cref<offset_t> position) const {
			if (text.empty()) {
				return;
			}

			offset_t carriage_pos{ 0 };

			for (cauto ch : text) {
				switch (ch) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + text::HorizontalTabWidth - 1) & -text::HorizontalTabWidth;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + text::VerticalTabWidth - 1) & -text::VerticalTabWidth;
					carriage_pos.x = 0;
					continue;
				case ' ':
					++carriage_pos.x;
					continue;
				default:
					draw(glyph_t{ static_cast<u32>(ch), color }, position + carriage_pos);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, cref<color_t> color, cref<offset_t> position, cref<cardinal_t> alignment) const {
			if (text.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(text) };
			const offset_t origin{ position - size / 2 };
			const offset_t alignment_offs{ generate_alignment_offset(text, position, alignment, size) };

			offset_t carriage_pos{ 0 };

			for (cauto ch : text) {
				switch (ch) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + text::HorizontalTabWidth - 1) & -text::HorizontalTabWidth;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + text::VerticalTabWidth - 1) & -text::VerticalTabWidth;
					carriage_pos.x = 0;
					continue;
				case ' ':
					++carriage_pos.x;
					continue;
				default:
					draw(glyph_t{ static_cast<u32>(ch), color }, origin + carriage_pos + alignment_offs);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, cref<color_t> color, cref<offset_t> position, cref<cardinal_t> alignment, cref<offset_t> offset) const {
			if (text.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(text) };
			const offset_t origin{ position - size / 2 };
			const offset_t alignment_offs{ generate_alignment_offset(text, position, alignment, size) };

			offset_t carriage_pos{ 0 };

			for (cauto ch : text) {
				switch (ch) {
				case '\0':
					return;
				case '\n':
					++carriage_pos.y;
					carriage_pos.x = 0;
					continue;
				case '\t':
					carriage_pos.x += (carriage_pos.x + text::HorizontalTabWidth - 1) & -text::HorizontalTabWidth;
					continue;
				case '\v':
					carriage_pos.y += (carriage_pos.y + text::VerticalTabWidth - 1) & -text::VerticalTabWidth;
					carriage_pos.x = 0;
					continue;
				case ' ':
					++carriage_pos.x;
					continue;
				default:
					draw(glyph_t{ static_cast<u32>(ch), color }, origin + carriage_pos + alignment_offs, offset);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw_label(ref<renderer_t> renderer, cref<runes_t> runes, cref<offset_t> position, cref<extent_t> padding, cref<color_t> background, cref<color_t> outline) const {
			if (runes.empty()) {
				return;
			}

			renderer.draw_composite_rect(rect_t{ position * glyph_size, (text::calculate_size(runes) + padding * 2) * glyph_size }, background, outline, 1);
			
			draw(runes, position);
		}

		inline void draw_label(ref<renderer_t> renderer, cref<runes_t> runes, cref<offset_t> position, cref<cardinal_t> alignment, cref<extent_t> padding, cref<color_t> background, cref<color_t> outline) const {
			if (runes.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(runes) };
			const offset_t origin{ position - size / 2 };
			const offset_t size_offs{ static_cast<offset_t>(alignment) * size };
			const offset_t alignment_offs{ size_offs - size_offs / 2 };

			renderer.draw_composite_rect(rect_t{ (origin + alignment_offs) * glyph_size, (size + padding * 2) * glyph_size }, background, outline, 1);
			
			draw(runes, position, alignment);
		}

		inline void draw_label(ref<renderer_t> renderer, cref<runes_t> runes, cref<offset_t> position, cref<cardinal_t> alignment, cref<offset_t> offset, cref<extent_t> padding, cref<color_t> background, cref<color_t> outline) const {
			if (runes.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(runes) };
			const offset_t origin{ position - size / 2 };
			const offset_t size_offs{ static_cast<offset_t>(alignment) * size };
			const offset_t alignment_offs{ size_offs - size_offs / 2 };

			renderer.draw_composite_rect(rect_t{ (origin + alignment_offs) * glyph_size + offset, (size + padding * 2) * glyph_size }, background, outline, 1);
			
			draw(runes, position, alignment, offset);
		}

		inline void draw_label(ref<renderer_t> renderer, cref<std::string> text, cref<color_t> color, cref<offset_t> position, cref<extent_t> padding, cref<color_t> background, cref<color_t> outline) const {
			if (text.empty()) {
				return;
			}

			renderer.draw_composite_rect(rect_t{ position * glyph_size, text::calculate_size(text) * glyph_size }, background, outline, 1);
			
			draw(text, position, color);
		}

		inline void draw_label(ref<renderer_t> renderer, cref<std::string> text, cref<color_t> color, cref<offset_t> position, cref<cardinal_t> alignment, cref<extent_t> padding, cref<color_t> background, cref<color_t> outline) const {
			if (text.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(text) };
			const offset_t origin{ position - size / 2 };
			const offset_t size_offs{ static_cast<offset_t>(alignment) * size };
			const offset_t alignment_offs{ size_offs - size_offs / 2 };

			renderer.draw_composite_rect(rect_t{ (origin + alignment_offs) * glyph_size, size * glyph_size }, background, outline, 1);
			
			draw(text, position, color, alignment);
		}

		inline void draw_label(ref<renderer_t> renderer, cref<std::string> text, cref<color_t> color, cref<offset_t> position, cref<cardinal_t> alignment, cref<offset_t> offset, cref<extent_t> padding, cref<color_t> background, cref<color_t> outline) const {
			if (text.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(text) };
			const offset_t origin{ position - size / 2 };
			const offset_t size_offs{ static_cast<offset_t>(alignment) * size };
			const offset_t alignment_offs{ size_offs - size_offs / 2 };

			renderer.draw_composite_rect(rect_t{ (origin + alignment_offs) * glyph_size + offset, size * glyph_size }, background, outline, 1);
			
			draw(text, position, color, alignment, offset);
		}
	};
} // namespace bleak
