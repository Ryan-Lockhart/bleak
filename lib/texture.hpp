#pragma once

#include "typedef.hpp"

#include <format>
#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>

#include "color.hpp"
#include "point.hpp"
#include "renderer.hpp"

#include "constants/colors.hpp"

namespace Bleakdepth {
	struct texture_t {
	  private:
		struct info_t {
			const point_t<i32> size;
			const i32 access;
			const u32 channels;
		};

		static inline ptr<SDL_Texture> load_texture(ref<renderer_t> renderer, cstr path) {
			ptr<SDL_Texture> texture { IMG_LoadTexture(renderer.handle(), path) };

			if (!texture) {
				throw std::runtime_error(std::format("could not load texture: {}", SDL_GetError()));
			}

			return texture;
		}

		static inline info_t get_texture_info(ptr<SDL_Texture> texture) {
			point_t<i32> size;
			i32 access;
			u32 channels;

			if (SDL_QueryTexture(texture, &channels, &access, &size.x, &size.y) < 0) {
				throw std::runtime_error(std::format("could not get texture info: {}", SDL_GetError()));
			}

			return { size, access, channels };
		}

		ref<renderer_t> renderer;
		ptr<SDL_Texture> texture;

		constexpr inline void copy(cptr<SDL_Rect> src, cptr<SDL_Rect> dst) const { SDL_RenderCopy(renderer.handle(), texture, src, dst); }

		constexpr inline void copy(cptr<SDL_Rect> src, cptr<SDL_FRect> dst) const { SDL_RenderCopyF(renderer.handle(), texture, src, dst); }

	  public:
		const info_t info;

		inline texture_t() = delete;

		constexpr texture_t(cref<texture_t> other) noexcept = delete;

		constexpr texture_t(rval<texture_t> other) noexcept : renderer(other.renderer), texture(std::move(other.texture)), info(other.info) {
			other.texture = nullptr;
			set_blend_mode(SDL_BLENDMODE_BLEND);
			set_color(Colors::White);
		}

		constexpr ref<texture_t> operator=(cref<texture_t> other) noexcept = delete;
		constexpr ref<texture_t> operator=(rval<texture_t> other) noexcept = delete;

		inline texture_t(ref<renderer_t> renderer, cstr path) :
			renderer { renderer },
			texture { load_texture(renderer, path) },
			info { get_texture_info(texture) } {
			if (texture == nullptr) {
				throw std::runtime_error(std::format("failed to load texture!"));
			}

			if (info.size.x <= 0) {
				throw std::runtime_error("texture width must be greater than zero!");
			}
			if (info.size.y <= 0) {
				throw std::runtime_error("texture width must be greater than zero!");
			}

			if (info.channels == SDL_PIXELFORMAT_UNKNOWN) {
				throw std::runtime_error("texture channels must be known!");
			}

			set_blend_mode(SDL_BLENDMODE_BLEND);
			set_color(Colors::White);
		}

		inline ~texture_t() {
			if (texture != nullptr) {
				SDL_DestroyTexture(texture);
			}
		}

		constexpr inline void set_color(cref<color_t> color) const {
			SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
			SDL_SetTextureAlphaMod(texture, color.a);
		}

		constexpr inline void set_blend_mode(SDL_BlendMode mode) const { SDL_SetTextureBlendMode(texture, mode); }

		constexpr ptr<SDL_Texture> handle() { return texture; }

		constexpr cptr<SDL_Texture> handle() const { return texture; }

		constexpr void draw(cref<point_t<i32>> pos) const {
			const SDL_Rect dst { pos.x, pos.y, info.size.x, info.size.y };
			copy(nullptr, &dst);
		}

		constexpr void draw(cref<point_t<f32>> pos) const {
			const SDL_FRect dst { pos.x, pos.y, static_cast<f32>(info.size.x), static_cast<f32>(info.size.y) };
			copy(nullptr, &dst);
		}

		constexpr void draw(cref<point_t<i32>> pos, cref<color_t> color) const {
			const SDL_Rect dst { pos.x, pos.y, info.size.x, info.size.y };
			set_color(color);
			copy(nullptr, &dst);
		}

		constexpr void draw(cref<point_t<f32>> pos, cref<color_t> color) const {
			const SDL_FRect dst { pos.x, pos.y, static_cast<f32>(info.size.x), static_cast<f32>(info.size.y) };
			set_color(color);
			copy(nullptr, &dst);
		}

		constexpr void draw(cptr<SDL_Rect> dst) const { copy(nullptr, (cptr<SDL_Rect>)dst); }

		constexpr void draw(cptr<SDL_FRect> dst) const { copy(nullptr, (cptr<SDL_FRect>)dst); }

		constexpr void draw(cptr<SDL_Rect> dst, cref<color_t> color) const {
			set_color(color);
			copy(nullptr, (cptr<SDL_Rect>)dst);
		}

		constexpr void draw(cptr<SDL_FRect> dst, cref<color_t> color) const {
			set_color(color);
			copy(nullptr, (cptr<SDL_FRect>)dst);
		}

		constexpr void draw(cptr<SDL_Rect> src, cptr<SDL_Rect> dst) const { copy(src, dst); }

		constexpr void draw(cptr<SDL_Rect> src, cptr<SDL_FRect> dst) const { copy(src, dst); }

		constexpr void draw(cptr<SDL_Rect> src, cptr<SDL_Rect> dst, cref<color_t> color) const {
			set_color(color);
			copy(src, dst);
		}

		constexpr void draw(cptr<SDL_Rect> src, cptr<SDL_FRect> dst, cref<color_t> color) const {
			set_color(color);
			copy(src, dst);
		}

		constexpr void draw(cptr<rect_t<i32>> dst) const { copy(nullptr, (cptr<SDL_Rect>)dst); }

		constexpr void draw(cptr<rect_t<f32>> dst) const { copy(nullptr, (cptr<SDL_FRect>)dst); }

		constexpr void draw(cptr<rect_t<i32>> dst, cref<color_t> color) const {
			set_color(color);
			copy(nullptr, (cptr<SDL_Rect>)dst);
		}

		constexpr void draw(cptr<rect_t<f32>> dst, cref<color_t> color) const {
			set_color(color);
			copy(nullptr, (cptr<SDL_FRect>)dst);
		}

		constexpr void draw(cptr<rect_t<i32>> src, cptr<rect_t<i32>> dst) const { copy((cptr<SDL_Rect>)src, (cptr<SDL_Rect>)dst); }

		constexpr void draw(cptr<rect_t<i32>> src, cptr<rect_t<i32>> dst, cref<color_t> color) const {
			set_color(color);
			copy((cptr<SDL_Rect>)src, (cptr<SDL_Rect>)dst);
		}
	};
} // namespace Bleakdepth
