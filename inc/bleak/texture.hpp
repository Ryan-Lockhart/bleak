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
		using texture = SDL_Texture;
		using blend_mode = SDL_BlendMode;

		using texture_access = SDL_TextureAccess;
		using pixel_format = SDL_PixelFormatEnum;

		struct texture_info {
			const extent_t size;

			const texture_access access;
			const pixel_format channels;
		};

		static inline texture_info get_texture_info(ptr<texture> texture) noexcept {
			int width, height;
			u32 channels;
			i32 access;

			if (SDL_QueryTexture(texture, &channels, &access, &width, &height) < 0) {
				error_log.add("could not get texture info: {}", sdl::get_error());
			}

			return sdl::texture_info{
				extent_t{ width, height },
				static_cast<sdl::texture_access>(access),
				static_cast<sdl::pixel_format>(channels)
			};
		}

		static inline ptr<sdl::texture> create_texture(ptr<sdl::renderer> renderer, extent_t size, sdl::pixel_format format, sdl::texture_access access) noexcept {
			ptr<sdl::texture> texture = SDL_CreateTexture(renderer, format, access, size.w, size.h);

			if (texture == nullptr) {
				error_log.add("could not create texture: {}", sdl::get_error());
			}

			return texture;
		}

		static inline void destroy_texture(ref<ptr<texture>> texture) noexcept {
			if (texture != nullptr) {
				SDL_DestroyTexture(texture);
				texture = nullptr;
			}
		}

		namespace img {
			static inline ptr<texture> load_texture(ptr<sdl::renderer> renderer, cstr path) noexcept {
				ptr<texture> texture{ IMG_LoadTexture(renderer, path) };

				if (texture == nullptr) {
					error_log.add("could not load texture: {}", sdl::get_error());
				}

				return texture;
			}
		} // namespace img
		
		static inline void set_render_target(ptr<renderer> renderer, ptr<texture> texture) noexcept { SDL_SetRenderTarget(renderer, texture); }
	} // namespace sdl

	struct texture_t {
		inline texture_t() = delete;

		constexpr texture_t(cref<texture_t> other) noexcept = delete;

		constexpr texture_t(rval<texture_t> other) noexcept : renderer{ other.renderer }, texture(std::move(other.texture)), info(other.info) {
			other.texture = nullptr;
			set_blend_mode(SDL_BLENDMODE_BLEND);
			set_color(colors::White);
		}

		constexpr ref<texture_t> operator=(cref<texture_t> other) noexcept = delete;
		constexpr ref<texture_t> operator=(rval<texture_t> other) noexcept = delete;

		inline texture_t(ref<renderer_t> renderer, cstr path) : renderer{ renderer }, texture{ sdl::img::load_texture(renderer.handle(), path) }, info{ sdl::get_texture_info(texture) } {
			if (texture == nullptr) {
				throw std::runtime_error(std::format("failed to load texture!"));
			}

			if (info.size <= extent_t::Zero) {
				throw std::runtime_error("texture size must be greater than zero!");
			}

			if (info.channels == SDL_PIXELFORMAT_UNKNOWN) {
				throw std::runtime_error("texture channels must be known!");
			}

			set_blend_mode(SDL_BLENDMODE_BLEND);
			set_color(colors::White);
		}

		inline ~texture_t() noexcept { sdl::destroy_texture(texture); }

		constexpr inline void set_color(color_t color) const {
			SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
			SDL_SetTextureAlphaMod(texture, color.a);
		}

		constexpr inline void set_blend_mode(sdl::blend_mode mode) const { SDL_SetTextureBlendMode(texture, mode); }

		constexpr inline void copy(cptr<sdl::rect> src, cptr<sdl::rect> dst) const { SDL_RenderCopy(renderer.handle(), texture, src, dst); }

		constexpr ptr<sdl::texture> handle() { return texture; }

		constexpr cptr<sdl::texture> handle() const { return texture; }

		constexpr void draw(offset_t pos) const {
			const SDL_Rect dst{ static_cast<i32>(pos.x), static_cast<i32>(pos.y), static_cast<i32>(info.size.w), static_cast<i32>(info.size.h) };
			copy(nullptr, &dst);
		}

		constexpr void draw(offset_t pos, color_t color) const {
			set_color(color);
			draw(pos);
		}

		constexpr void draw(offset_t pos, extent_t size) const {
			const SDL_Rect dst{ static_cast<i32>(pos.x), static_cast<i32>(pos.y), static_cast<i32>(size.w), static_cast<i32>(size.h) };
			copy(nullptr, &dst);
		}

		constexpr void draw(offset_t pos, extent_t size, color_t color) const {
			set_color(color);
			draw(pos, size);
		}

		constexpr void draw(cref<rect_t> dst) const {
			sdl::rect sdl_dst{ static_cast<sdl::rect>(dst) };
			copy(nullptr, &sdl_dst);
		}

		constexpr void draw(cref<rect_t> dst, color_t color) const {
			set_color(color);
			draw(dst);
		}

		constexpr void draw(cref<rect_t> src, cref<rect_t> dst) const {
			sdl::rect sdl_src{ static_cast<sdl::rect>(src) };
			sdl::rect sdl_dst{ static_cast<sdl::rect>(dst) };
			copy(&sdl_src, &sdl_dst);
		}

		constexpr void draw(cref<rect_t> src, cref<rect_t> dst, color_t color) const {
			set_color(color);
			draw(src, dst);
		}

	  private:
		ref<renderer_t> renderer;

		ptr<sdl::texture> texture;

	  public:
		const sdl::texture_info info;
	};
	
	struct target_texture_t {
		inline target_texture_t() = delete;

		constexpr target_texture_t(cref<target_texture_t> other) noexcept = delete;

		constexpr target_texture_t(rval<target_texture_t> other) noexcept : renderer{ other.renderer }, texture(std::move(other.texture)), info(other.info) {
			other.texture = nullptr;
			set_blend_mode(SDL_BLENDMODE_BLEND);
			set_color(colors::White);
		}

		constexpr ref<target_texture_t> operator=(cref<target_texture_t> other) noexcept = delete;
		constexpr ref<target_texture_t> operator=(rval<target_texture_t> other) noexcept = delete;

		inline target_texture_t(ref<renderer_t> renderer, extent_t size, sdl::pixel_format format, sdl::texture_access access) : renderer{ renderer }, texture{ sdl::create_texture(renderer.handle(), size, format, access) }, info{ sdl::get_texture_info(texture) } {
			if (texture == nullptr) {
				throw std::runtime_error(std::format("failed to create texture!"));
			}

			if (info.size <= extent_t::Zero) {
				throw std::runtime_error("texture size must be greater than zero!");
			}

			if (info.channels == SDL_PIXELFORMAT_UNKNOWN) {
				throw std::runtime_error("texture channels must be known!");
			}

			set_blend_mode(SDL_BLENDMODE_BLEND);
			set_color(colors::White);
		}

		inline ~target_texture_t() noexcept { sdl::destroy_texture(texture); }

		constexpr inline void set_color(color_t color) const {
			SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
			SDL_SetTextureAlphaMod(texture, color.a);
		}

		constexpr inline void set_blend_mode(sdl::blend_mode mode) const { SDL_SetTextureBlendMode(texture, mode); }

		constexpr inline void copy(cptr<sdl::rect> src, cptr<sdl::rect> dst) const { SDL_RenderCopy(renderer.handle(), texture, src, dst); }

		constexpr ptr<sdl::texture> handle() { return texture; }

		constexpr cptr<sdl::texture> handle() const { return texture; }

		constexpr void draw(offset_t pos) const {
			const SDL_Rect dst{ static_cast<i32>(pos.x), static_cast<i32>(pos.y), static_cast<i32>(info.size.w), static_cast<i32>(info.size.h) };
			copy(nullptr, &dst);
		}

		constexpr void draw(offset_t pos, color_t color) const {
			set_color(color);
			draw(pos);
		}

		constexpr void draw(offset_t pos, extent_t size) const {
			const SDL_Rect dst{ static_cast<i32>(pos.x), static_cast<i32>(pos.y), static_cast<i32>(size.w), static_cast<i32>(size.h) };
			copy(nullptr, &dst);
		}

		constexpr void draw(offset_t pos, extent_t size, color_t color) const {
			set_color(color);
			draw(pos, size);
		}

		constexpr void draw(cref<rect_t> dst) const {
			sdl::rect sdl_dst{ static_cast<sdl::rect>(dst) };
			copy(nullptr, &sdl_dst);
		}

		constexpr void draw(cref<rect_t> dst, color_t color) const {
			set_color(color);
			draw(dst);
		}

		constexpr void draw(cref<rect_t> src, cref<rect_t> dst) const {
			sdl::rect sdl_src{ static_cast<sdl::rect>(src) };
			sdl::rect sdl_dst{ static_cast<sdl::rect>(dst) };
			copy(&sdl_src, &sdl_dst);
		}

		constexpr void draw(cref<rect_t> src, cref<rect_t> dst, color_t color) const {
			set_color(color);
			draw(src, dst);
		}

	  private:
		ref<renderer_t> renderer;

		ptr<sdl::texture> texture;

	  public:
		const sdl::texture_info info;
	};

	inline void renderer_t::set_target(ref<target_texture_t> target) noexcept { sdl::set_render_target(renderer, target.handle()); }
	
	inline void renderer_t::unset_target() noexcept { sdl::set_render_target(renderer, nullptr); }
} // namespace bleak
