#pragma once

#include "bleak/offset/offset_2d.hpp"
#include "bleak/typedef.hpp"

#include <format>
#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>

#include "bleak/color.hpp"
#include "bleak/extent.hpp"
#include "bleak/renderer.hpp"

#include "bleak/constants/colors.hpp"
#include "extent/extent_2d.hpp"

namespace bleak {
	struct texture_t {
	  private:
		struct info_t {
			const extent_2d_t size;
			const i32 access;
			const u32 channels;
		};

		static inline ptr<SDL_Texture> load_texture(ref<renderer_t> renderer, cstr path) {
			ptr<SDL_Texture> texture{ IMG_LoadTexture(renderer.handle(), path) };

			if (texture != nullptr) {
				throw std::runtime_error(std::format("could not load texture: {}", SDL_GetError()));
			}

			return texture;
		}

		static inline info_t get_texture_info(ptr<SDL_Texture> texture) {
			int w, h;
			i32 access;
			u32 channels;

			if (SDL_QueryTexture(texture, &channels, &access, &w, &h) < 0) {
				throw std::runtime_error(std::format("could not get texture info: {}", SDL_GetError()));
			}

			return { extent_2d_t{ extent_2d_t::scalar_cast(w), extent_2d_t::scalar_cast(h) }, access, channels };
		}

		ref<renderer_t> renderer;
		ptr<SDL_Texture> texture;

		constexpr inline void copy(cptr<sdl::rect> src, cptr<sdl::rect> dst) const { SDL_RenderCopy(renderer.handle(), texture, src, dst); }

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
			renderer{ renderer },
			texture{ load_texture(renderer, path) },
			info{ get_texture_info(texture) } {
			if (texture == nullptr) {
				throw std::runtime_error(std::format("failed to load texture!"));
			}

			if (info.size <= extent_2d_t::zero) {
				throw std::runtime_error("texture size must be greater than zero!");
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

		constexpr void draw(cref<offset_2d_t> pos) const {
			const SDL_Rect dst{ pos.x, pos.y, static_cast<i32>(info.size.w), static_cast<i32>(info.size.h) };
			copy(nullptr, &dst);
		}

		constexpr void draw(cref<offset_2d_t> pos, cref<color_t> color) const {
			const SDL_Rect dst{ pos.x, pos.y, static_cast<i32>(info.size.w), static_cast<i32>(info.size.h) };
			set_color(color);
			copy(nullptr, &dst);
		}

		constexpr void draw(cref<rect_t> dst) const {
			sdl::rect sdl_dst{ (sdl::rect)dst };
			copy(nullptr, &sdl_dst);
		}

		constexpr void draw(cref<rect_t> dst, cref<color_t> color) const {
			set_color(color);
			draw(dst);
		}

		constexpr void draw(cref<rect_t> src, cref<rect_t> dst) const {
			sdl::rect sdl_src{ (sdl::rect)src };
			sdl::rect sdl_dst{ (sdl::rect)dst };
			copy(&sdl_src, &sdl_dst);
		}

		constexpr void draw(cref<rect_t> src, cref<rect_t> dst, cref<color_t> color) const {
			set_color(color);
			draw(src, dst);
		}
	};
} // namespace bleak
