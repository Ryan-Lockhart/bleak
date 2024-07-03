#pragma once

#include <stdexcept>
#include <utility>

#include <SDL.h>
#include <SDL_image.h>

#include "array/layer.tpp"
#include "color.hpp"
#include "glyph.hpp"
#include "point.hpp"
#include "text.hpp"
#include "texture.hpp"

namespace Bleakdepth {
	template<usize Width, usize Height> class atlas_t {
	  private:
		static_assert(Width > 0, "atlas width must be greater than zero!");
		static_assert(Height > 0, "atlas height must be greater than zero!");

		static_assert(sizeof(SDL_Rect) * Width * Height <= Memory::Maximum, "atlas size exceeds memory limit!");

		layer_t<SDL_Rect, Width, Height> rects;

		texture_t texture;

	  public:
		// The size of the image in pixels.
		const size_t<i32> image_size;
		// The size of each glyph in pixels.
		const size_t<i32> glyph_size;

		size_t<i32> universal_offset { 0 };
		size_t<f32> universal_foffset { 0.0f };

		// The size of the atlas in glyphs.
		static constexpr size_t<i32> size { Width, Height };

		inline atlas_t() = delete;

		inline atlas_t(rval<texture_t> texture) :
			rects {},
			texture { std::move(texture) },

			image_size { this->texture.info.size },
			glyph_size { image_size / size } {
			if (image_size.x <= 0 || image_size.y <= 0) {
				throw std::runtime_error("image size must be greater than zero!");
			}
			if (glyph_size.x <= 0 || glyph_size.y <= 0) {
				throw std::runtime_error("glyph size must be greater than zero!");
			}

			if (image_size.x % size.w != 0 || image_size.y % size.h != 0) {
				throw std::runtime_error("image size must be divisible by the atlas size!");
			}

			for (int y = 0; y < size.y; ++y) {
				for (int x = 0; x < size.x; ++x) {
					rects[x, y] = { x * glyph_size.x, y * glyph_size.y, glyph_size.w, glyph_size.h };
				}
			}
		}

		inline atlas_t(cref<atlas_t> other) = delete;
		inline atlas_t(rval<atlas_t> other) = delete;

		inline ref<atlas_t> operator=(cref<atlas_t> other) = delete;
		inline ref<atlas_t> operator=(rval<atlas_t> other) = delete;

		inline ~atlas_t() = default;

		inline void draw(cref<glyph_t> glyph, cref<point_t<i32>> position) const {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				throw std::out_of_range("glyph index out of range!");
			}

			const point_t pos = position * glyph_size;
			const SDL_Rect dst { pos.x + universal_offset.x, pos.y + universal_offset.y, glyph_size.x, glyph_size.y };

			texture.draw(&rects[glyph.index], &dst, glyph.color);
		}

		inline void draw(cref<glyph_t> glyph, cref<point_t<i32>> position, cref<point_t<i32>> offset) const {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				throw std::out_of_range("glyph index out of range!");
			}

			const point_t pos = position * glyph_size + offset;
			const SDL_Rect dst { pos.x + universal_offset.x, pos.y + universal_offset.y, glyph_size.x, glyph_size.y };

			texture.draw(&rects[glyph.index], &dst, glyph.color);
		}

		inline void draw(cref<glyph_t> glyph, cref<point_t<f32>> position) const {
			if (glyph.index < 0 || glyph.index >= rects.size) {
				throw std::out_of_range("glyph index out of range!");
			}

			const SDL_FRect dst {
				position.x + universal_foffset.x, position.y + universal_foffset.y, static_cast<f32>(glyph_size.x), static_cast<f32>(glyph_size.y)
			};

			texture.draw(&rects[glyph.index], &dst, glyph.color);
		}

		inline void draw(cref<runes_t> runes, cref<point_t<i32>> position) const {
			if (runes.empty()) {
				return;
			}

			point_t<i32> carriage_pos { 0 };

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

		inline void draw(cref<runes_t> runes, cref<point_t<f32>> position) const {
			if (runes.empty()) {
				return;
			}

			point_t<i32> carriage_pos { 0 };

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
					draw(rune, position + static_cast<point_t<f32>>(carriage_pos));
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<runes_t> runes, cref<point_t<i32>> position, cardinal_t alignment) const {
			if (runes.empty()) {
				return;
			}

			const point_t<i32> size { static_cast<point_t<i32>>(Text::calculate_size(runes)) * alignment };
			const point_t<i32> alignment_offs { size - size / 2 };

			point_t<i32> carriage_pos { 0 };

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

		inline void draw(cref<runes_t> runes, cref<point_t<i32>> position, cardinal_t alignment, cref<point_t<i32>> offset) const {
			if (runes.empty()) {
				return;
			}

			const point_t<i32> size { static_cast<point_t<i32>>(Text::calculate_size(runes)) * alignment };
			const point_t<i32> alignment_offs { size - size / 2 };

			point_t<i32> carriage_pos { 0 };

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

		inline void draw(cref<runes_t> runes, point_t<f32> position, cardinal_t alignment) const {
			if (runes.empty()) {
				return;
			}

			const point_t<i32> size { static_cast<point_t<i32>>(Text::calculate_size(runes)) * alignment };
			position += static_cast<point_t<f32>>(size - size / 2);

			point_t<i32> carriage_pos { 0 };

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
					draw(rune, position + static_cast<point_t<f32>>(carriage_pos));
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, point_t<i32> position, color_t color) const {
			if (text.empty()) {
				return;
			}

			const point_t<i32> size { static_cast<point_t<i32>>(Text::calculate_size(text)) };
			const point_t<i32> alignment_offs { size - size / 2 };

			point_t<i32> carriage_pos { 0 };

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

		inline void draw(cref<std::string> text, point_t<f32> position, color_t color) const {
			if (text.empty()) {
				return;
			}

			const point_t<i32> size { static_cast<point_t<i32>>(Text::calculate_size(text)) };
			const point_t<i32> alignment_offs { size - size / 2 };

			point_t<i32> carriage_pos { 0 };

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
					draw(
						{ static_cast<u8>(ch), color },
						position + static_cast<point_t<f32>>(carriage_pos + alignment_offs) * static_cast<point_t<f32>>(glyph_size)
					);
					++carriage_pos.x;
					continue;
				}
			}
		}

		inline void draw(cref<std::string> text, point_t<i32> position, color_t color, cardinal_t alignment) const {
			if (text.empty()) {
				return;
			}

			const point_t<i32> size { static_cast<point_t<i32>>(Text::calculate_size(text)) * alignment };
			const point_t<i32> alignment_offs { size - size / 2 };

			point_t<i32> carriage_pos { 0 };

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

		inline void draw(cref<std::string> text, point_t<f32> position, color_t color, cardinal_t alignment) const {
			if (text.empty()) {
				return;
			}

			const point_t<i32> size { static_cast<point_t<i32>>(Text::calculate_size(text)) * alignment };
			const point_t<i32> alignment_offs { size - size / 2 };

			point_t<i32> carriage_pos { 0 };

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
					draw(
						{ static_cast<u8>(ch), color },
						position + static_cast<point_t<f32>>(carriage_pos + alignment_offs) * static_cast<point_t<f32>>(glyph_size)
					);
					++carriage_pos.x;
					continue;
				}
			}
		}
	};
} // namespace Bleakdepth
