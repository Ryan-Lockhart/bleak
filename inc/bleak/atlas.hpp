#pragma once

#include <bleak/typedef.hpp>

#include <SDL.h>
#include <SDL_image.h>

#include <bleak/array.hpp>
#include <bleak/color.hpp>
#include <bleak/extent.hpp>
#include <bleak/glyph.hpp>
#include <bleak/keyframe.hpp>
#include <bleak/offset.hpp>
#include <bleak/renderer.hpp>
#include <bleak/text.hpp>
#include <bleak/texture.hpp>

namespace bleak {
	static constexpr offset_t generate_alignment_offset(cref<runes_t> runes, offset_t position, cardinal_t alignment) {
		const offset_t size_offs{ static_cast<offset_t>(alignment) * text::calculate_size(runes) };

		return offset_t{ size_offs - size_offs / 2 };
	}

	static constexpr offset_t generate_alignment_offset(cref<std::string> text, offset_t position, cardinal_t alignment) {
		const offset_t size_offs{ static_cast<offset_t>(alignment) * text::calculate_size(text) };
		
		return offset_t{ size_offs - size_offs / 2 };
	}

	static constexpr offset_t generate_alignment_offset(cref<runes_t> runes, offset_t position, cardinal_t alignment, extent_t size) {
		const offset_t size_offs{ static_cast<offset_t>(alignment) * size };

		return offset_t{ size_offs - size_offs / 2 };
	}

	static constexpr offset_t generate_alignment_offset(cref<std::string> text, offset_t position, cardinal_t alignment, extent_t size) {
		const offset_t size_offs{ static_cast<offset_t>(alignment) * size };
		
		return offset_t{ size_offs - size_offs / 2 };
	}

	template<extent_t Size> struct atlas_t {
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

		// The size of the rendered glyph in pixels
		const extent_t override_size;

		inline atlas_t() noexcept = delete;

		inline atlas_t(ref<renderer_t> renderer, cstr path) :
			rects{},
			texture{ renderer, path },

			image_size{ this->texture.info.size },
			glyph_size{ image_size / size },
			override_size{ glyph_size } {
			if (image_size.w <= 0 || image_size.h <= 0) {
				error_log.add("[ERROR]: failed to initialize atlas! (image size must be greater than zero!)");
			}

			if (glyph_size.w <= 0 || glyph_size.h <= 0) {
				error_log.add("[ERROR]: failed to initialize atlas! (glyph size must be greater than zero!)");
			}

			if (override_size.w <= 0 || override_size.h <= 0) {
				error_log.add("[ERROR]: failed to initialize atlas! (override size must be greater than zero!)");
			}

			if (image_size.w % size.w != 0 || image_size.h % size.h != 0) {
				error_log.add("[ERROR]: failed to initialize atlas! (image size must be divisible by the atlas size!)");
			}

			extent_t::product_t index{ 0 };
			for (int y = 0; y < size.h; ++y) {
				for (int x = 0; x < size.w; ++x) {
					rects[index++] = rect_t{ offset_t{ x, y } * glyph_size, glyph_size };
				}
			}
		}

		inline atlas_t(ref<renderer_t> renderer, cstr path, extent_t override_size) :
			rects{},
			texture{ renderer, path },

			image_size{ this->texture.info.size },
			glyph_size{ image_size / size },
			override_size{ override_size } {
			if (image_size.w <= 0 || image_size.h <= 0) {
				error_log.add("[ERROR]: failed to initialize atlas! (image size must be greater than zero!)");
			}

			if (glyph_size.w <= 0 || glyph_size.h <= 0) {
				error_log.add("[ERROR]: failed to initialize atlas! (glyph size must be greater than zero!)");
			}

			if (override_size.w <= 0 || override_size.h <= 0) {
				error_log.add("[ERROR]: failed to initialize atlas! (override size must be greater than zero!)");
			}

			if (image_size.w % size.w != 0 || image_size.h % size.h != 0) {
				error_log.add("[ERROR]: failed to initialize atlas! (image size must be divisible by the atlas size!)");
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

		inline ~atlas_t() noexcept = default;

		inline bool has_override() const noexcept { return glyph_size != override_size; }

		inline extent_t get_glyph_size() const noexcept { return has_override() ? override_size : glyph_size; }

		template<bool UseOverride = true> inline void draw(glyph_t glyph, offset_t position) const noexcept {
			if (glyph.index < 0 || glyph.index >= rects.area) {
				error_log.add("[WARNING]: glyph index {} is out of range!", glyph.index);
				return;
			}

			if constexpr (UseOverride) {
				texture.draw(rects[glyph.index], rect_t{ position * override_size, override_size }, glyph.color);
			} else {
				texture.draw(rects[glyph.index], rect_t{ position * glyph_size, glyph_size }, glyph.color);
			}
		}

		template<bool UseOverride = true> inline void draw(glyph_t glyph, offset_t position, offset_t offset) const noexcept {
			if (glyph.index < 0 || glyph.index >= rects.area) {
				error_log.add("[WARNING]: glyph index {} is out of range!", glyph.index);
				return;
			}

			if constexpr (UseOverride) {
				texture.draw(rects[glyph.index], rect_t{ position * override_size + offset, override_size }, glyph.color);
			} else {
				texture.draw(rects[glyph.index], rect_t{ position * glyph_size + offset, glyph_size }, glyph.color);
			}
		}

		template<bool UseOverride = true> inline void draw(keyframe_t keyframe, color_t color, offset_t position) const noexcept {
			const usize index{ keyframe.index * keyframe_t::length + keyframe.current_frame() };

			if (index < 0 || index >= rects.area) {
				error_log.add("[WARNING]: keyframe index {} is out of range!", index);
				return;
			}

			if constexpr (UseOverride) {
				texture.draw(rects[index], rect_t{ position * override_size, override_size }, color);
			} else {
				texture.draw(rects[index], rect_t{ position * glyph_size, glyph_size }, color);
			}
		}

		template<bool UseOverride = true> inline void draw(keyframe_t keyframe, color_t color, offset_t position, offset_t offset) const noexcept {
			const usize index{ keyframe.index * keyframe_t::length + keyframe.current_frame() };

			if (index < 0 || index >= rects.area) {
				error_log.add("[WARNING]: glyph index {} is out of range!", index);
				return;
			}

			if constexpr (UseOverride) {
				texture.draw(rects[index], rect_t{ position * override_size + offset, override_size }, color);
			} else {
				texture.draw(rects[index], rect_t{ position * glyph_size + offset, glyph_size }, color);
			}
		}

		inline void draw(cref<runes_t> runes, offset_t position) const {
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

		inline void draw(cref<runes_t> runes, offset_t position, cardinal_t alignment) const {
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

		inline void draw(cref<runes_t> runes, offset_t position, cardinal_t alignment, offset_t offset) const {
			if (runes.empty()) {
				return;
			}

			const extent_t size{ text::calculate_size(runes) };
			const offset_t origin{ position - size / 2 };
			const offset_t alignment_offs{ generate_alignment_offset(runes, position, alignment, size) };

			offset_t carriage_pos{ 0 };

			for (crauto rune : runes) {
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

		inline void draw(cref<std::string> text, color_t color, offset_t position) const {
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
					draw(glyph_t{ static_cast<glyph_t::index_t>(ch), color }, position + carriage_pos);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, color_t color, offset_t position, cardinal_t alignment) const {
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
					draw(glyph_t{ static_cast<glyph_t::index_t>(ch), color }, origin + carriage_pos + alignment_offs);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, color_t color, offset_t position, cardinal_t alignment, offset_t offset) const {
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
					draw(glyph_t{ static_cast<glyph_t::index_t>(ch), color }, origin + carriage_pos + alignment_offs, offset);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw_label(ref<renderer_t> renderer, cref<runes_t> runes, offset_t position, extent_t padding, color_t background, color_t outline) const {
			if (runes.empty()) {
				return;
			}

			renderer.draw_composite_rect(rect_t{ position * glyph_size, (text::calculate_size(runes) + padding * 2) * glyph_size }, background, outline, 1);
			
			draw(runes, position);
		}

		inline void draw_label(ref<renderer_t> renderer, cref<runes_t> runes, offset_t position, cardinal_t alignment, extent_t padding, color_t background, color_t outline) const {
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

		inline void draw_label(ref<renderer_t> renderer, cref<runes_t> runes, offset_t position, cardinal_t alignment, offset_t offset, extent_t padding, color_t background, color_t outline) const {
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

		inline void draw_label(ref<renderer_t> renderer, cref<std::string> text, color_t color, offset_t position, extent_t padding, color_t background, color_t outline) const {
			if (text.empty()) {
				return;
			}

			renderer.draw_composite_rect(rect_t{ position * glyph_size, text::calculate_size(text) * glyph_size }, background, outline, 1);
			
			draw(text, position, color);
		}

		inline void draw_label(ref<renderer_t> renderer, cref<std::string> text, color_t color, offset_t position, cardinal_t alignment, extent_t padding, color_t background, color_t outline) const {
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

		inline void draw_label(ref<renderer_t> renderer, cref<std::string> text, color_t color, offset_t position, cardinal_t alignment, offset_t offset, extent_t padding, color_t background, color_t outline) const {
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
