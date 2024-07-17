#pragma once

#include <bleak/typedef.hpp>

#include <format>
#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>

#include <bleak/color.hpp>
#include <bleak/extent.hpp>
#include <bleak/log.hpp>
#include <bleak/renderer.hpp>

#include <bleak/constants/colors.hpp>

namespace bleak {
	namespace sdl {
		using texture = ptr<SDL_Texture>;
		using const_texture = cptr<SDL_Texture>;

		using blend_mode = SDL_BlendMode;

		struct texture_info {
			const extent_2d_t size;

			const i32 access;
			const u32 channels;
		};

		static inline texture_info get_texture_info(texture texture) noexcept {
			int w, h;
			i32 access;
			u32 channels;

			if (SDL_QueryTexture(texture, &channels, &access, &w, &h) < 0) {
				error_log.add("could not get texture info: {}", sdl::get_error());
			}

			return { extent_2d_t{ w, h }, access, channels };
		}

		static inline void destroy_texture(ref<texture> texture) noexcept {
			if (texture != nullptr) {
				SDL_DestroyTexture(texture);
				texture = nullptr;
			}
		}

		namespace img {
			static inline texture load_texture(sdl::renderer renderer, cstr path) noexcept {
				texture texture{ IMG_LoadTexture(renderer, path) };

				if (texture == nullptr) {
					error_log.add("could not load texture: {}", sdl::get_error());
				}

				return texture;
			}
		} // namespace img
	} // namespace sdl

	struct texture_t {
		inline texture_t() = delete;

		constexpr texture_t(cref<texture_t> other) noexcept = delete;

		constexpr texture_t(rval<texture_t> other) noexcept : renderer{ other.renderer }, texture(std::move(other.texture)), info(other.info) {
			other.texture = nullptr;
			set_blend_mode(SDL_BLENDMODE_BLEND);
			set_color(Colors::White);
		}

		constexpr ref<texture_t> operator=(cref<texture_t> other) noexcept = delete;
		constexpr ref<texture_t> operator=(rval<texture_t> other) noexcept = delete;

		inline texture_t(ref<renderer_t> renderer, cstr path) : renderer{ renderer }, texture{ sdl::img::load_texture(renderer.handle(), path) }, info{ sdl::get_texture_info(texture) } {
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

		inline ~texture_t() noexcept { sdl::destroy_texture(texture); }

		constexpr inline void set_color(cref<color_t> color) const {
			SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
			SDL_SetTextureAlphaMod(texture, color.a);
		}

		constexpr inline void set_blend_mode(sdl::blend_mode mode) const { SDL_SetTextureBlendMode(texture, mode); }

		constexpr inline void copy(cptr<sdl::rect> src, cptr<sdl::rect> dst) const { SDL_RenderCopy(renderer.handle(), texture, src, dst); }

		constexpr sdl::texture handle() { return texture; }

		constexpr sdl::const_texture handle() const { return texture; }

		constexpr void draw(cref<offset_2d_t> pos) const {
			const SDL_Rect dst{ static_cast<i32>(pos.x), static_cast<i32>(pos.y), static_cast<i32>(info.size.w), static_cast<i32>(info.size.h) };
			copy(nullptr, &dst);
		}

		constexpr void draw(cref<offset_2d_t> pos, cref<color_t> color) const {
			set_color(color);
			draw(pos);
		}

		constexpr void draw(cref<offset_2d_t> pos, cref<extent_2d_t> size) const {
			const SDL_Rect dst{ static_cast<i32>(pos.x), static_cast<i32>(pos.y), static_cast<i32>(size.w), static_cast<i32>(size.h) };
			copy(nullptr, &dst);
		}

		constexpr void draw(cref<offset_2d_t> pos, cref<extent_2d_t> size, cref<color_t> color) const {
			set_color(color);
			draw(pos, size);
		}

		constexpr void draw(cref<rect_t> dst) const {
			sdl::rect sdl_dst{ static_cast<sdl::rect>(dst) };
			copy(nullptr, &sdl_dst);
		}

		constexpr void draw(cref<rect_t> dst, cref<color_t> color) const {
			set_color(color);
			draw(dst);
		}

		constexpr void draw(cref<rect_t> src, cref<rect_t> dst) const {
			sdl::rect sdl_src{ static_cast<sdl::rect>(src) };
			sdl::rect sdl_dst{ static_cast<sdl::rect>(dst) };
			copy(&sdl_src, &sdl_dst);
		}

		constexpr void draw(cref<rect_t> src, cref<rect_t> dst, cref<color_t> color) const {
			set_color(color);
			draw(src, dst);
		}

	  private:
		ref<renderer_t> renderer;

		sdl::texture texture;

	  public:
		const sdl::texture_info info;
	};
} // namespace bleak
