#pragma once

#include "bleak/offset/offset_2d.hpp"
#include "bleak/typedef.hpp"

#include <format>

#include <SDL.h>

#include "bleak/color.hpp"
#include "bleak/extent.hpp"
#include "bleak/line.hpp"
#include "bleak/log.hpp"
#include "bleak/offset.hpp"
#include "bleak/window.hpp"

namespace bleak {
	namespace sdl {
		using renderer = ptr<SDL_Renderer>;
		using const_renderer = cptr<SDL_Renderer>;

		using renderer_flags = SDL_RendererFlags;

		constexpr renderer_flags RENDERER_FLAGS_NONE{ static_cast<renderer_flags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };

		static inline renderer create_renderer(window window, renderer_flags flags, i32 driver_index = -1) noexcept {
			renderer handle{ SDL_CreateRenderer(window, driver_index, flags) };

			if (handle == nullptr) {
				error_log.add("failed to create renderer: {}", sdl::get_error());
			}

			return handle;
		}

		static inline void destroy_renderer(ref<renderer> handle) noexcept {
			if (handle != nullptr) {
				SDL_DestroyRenderer(handle);
				handle = nullptr;
			} else {
				error_log.add("cannot destroy renderer: renderer handle is null!");
			}
		}
	} // namespace sdl

	class renderer_t {
	  private:
		sdl::renderer renderer;

	  public:
		inline renderer_t(ref<window_t> window, sdl::renderer_flags flags) : renderer{ sdl::create_renderer(window.handle(), flags) } {}

		inline ~renderer_t() { sdl::destroy_renderer(renderer); }

		constexpr sdl::renderer handle() noexcept { return renderer; }

		constexpr sdl::const_renderer handle() const noexcept { return renderer; }

		inline void set_draw_color(u8 r, u8 g, u8 b, u8 a) noexcept { SDL_SetRenderDrawColor(renderer, r, g, b, a); }

		inline void set_draw_color(cref<color_t> color) noexcept { SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); }

		inline void clear() noexcept { SDL_RenderClear(renderer); }

		inline void clear(cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderClear(renderer);
		}

		inline void present() noexcept { SDL_RenderPresent(renderer); }

		template<primitive_type_t Primitive, fill_type_t Fill> inline void draw() noexcept;

		inline void draw_point(offset_2d_t::scalar_t x, offset_2d_t::scalar_t y) noexcept { SDL_RenderDrawPoint(renderer, x, y); }

		inline void draw_point(offset_2d_t::scalar_t x, offset_2d_t::scalar_t y, cref<color_t> color) noexcept {
			set_draw_color(color);
			draw_point(x, y);
		}

		inline void draw_point(cref<offset_2d_t> point) noexcept { SDL_RenderDrawPoint(renderer, point.x, point.y); }

		inline void draw_point(cref<offset_2d_t> point, cref<color_t> color) noexcept {
			set_draw_color(color);
			draw_point(point);
		}

		inline void draw_line(cref<offset_2d_t> start, cref<offset_2d_t> end) noexcept { SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y); }

		inline void draw_line(cref<offset_2d_t> start, cref<offset_2d_t> end, cref<color_t> color) noexcept {
			set_draw_color(color);
			draw_line(start, end);
		}

		inline void draw_line(cref<offset_2d_t> start, cref<offset_2d_t> end, extent_2d_t::scalar_t thickness) noexcept {
			auto dst = offset_2d_t::distance<offset_2d_t::product_t>(start, end);
			auto dir = offset_2d_t::direction(start, end);

			auto origin{ start + dir * thickness / 2 };

			sdl::rect rect{ origin.x, origin.y, static_cast<i32>(thickness), static_cast<i32>(dst) };

			SDL_RenderDrawRect(renderer, &rect);
		}

		inline void draw_line(cref<offset_2d_t> start, cref<offset_2d_t> end, cref<color_t> color, extent_2d_t::scalar_t thickness) noexcept {
			set_draw_color(color);
			draw_line(start, end, thickness);
		}

		inline void draw_line(cref<line_2d_t> line) noexcept { draw_line(line.start, line.end); }

		inline void draw_line(cref<line_2d_t> line, cref<color_t> color) noexcept { draw_line(line.start, line.end, color); }

		inline void draw_line(cref<line_2d_t> line, cref<color_t> color, extent_2d_t::scalar_t thickness) noexcept {
			draw_line(line.start, line.end, color, thickness);
		}

		inline void draw_line(cref<line_2d_t> line, extent_2d_t::scalar_t thickness) noexcept { draw_line(line.start, line.end, thickness); }

		inline void draw_outline_rect(cref<offset_2d_t> position, cref<extent_2d_t> size) noexcept {
			sdl::rect sdl_rect{ position.x, position.y, static_cast<int>(size.w), static_cast<int>(size.h) };
			SDL_RenderDrawRect(renderer, &sdl_rect);
		}

		inline void draw_outline_rect(cref<offset_2d_t> position, cref<extent_2d_t> size, cref<color_t> color) noexcept {
			set_draw_color(color);
			draw_outline_rect(position, size);
		}

		inline void draw_outline_rect(cref<rect_t> rect) noexcept { draw_outline_rect(rect.position, rect.size); }

		inline void draw_outline_rect(cref<rect_t> rect, cref<color_t> color) noexcept { draw_outline_rect(rect.position, rect.size, color); }

		inline void draw_fill_rect(cref<offset_2d_t> position, cref<extent_2d_t> size) noexcept {
			sdl::rect sdl_rect{ position.x, position.y, static_cast<int>(size.w), static_cast<int>(size.h) };
			SDL_RenderFillRect(renderer, &sdl_rect);
		}

		inline void draw_fill_rect(cref<offset_2d_t> position, cref<extent_2d_t> size, cref<color_t> color) noexcept {
			set_draw_color(color);
			draw_fill_rect(position, size);
		}

		inline void draw_fill_rect(cref<rect_t> rect) noexcept { draw_fill_rect(rect.position, rect.size); }

		inline void draw_fill_rect(cref<rect_t> rect, cref<color_t> color) noexcept { draw_fill_rect(rect.position, rect.size, color); }

		inline void draw_composite_rect(
			cref<offset_2d_t> position, cref<extent_2d_t> size, cref<color_t> fill_color, cref<color_t> outline_color, extent_2d_t::scalar_t thickness
		) noexcept {
			draw_fill_rect(position, size, outline_color);
			draw_fill_rect({ position + thickness / 2, size - thickness }, fill_color);
		}

		inline void draw_composite_rect(cref<rect_t> rect, cref<color_t> fill_color, cref<color_t> outline_color, extent_2d_t::scalar_t thickness) noexcept {
			draw_fill_rect(rect, outline_color);
			draw_fill_rect({ rect.position + thickness / 2, rect.size - thickness }, fill_color);
		}

		inline void draw_circle(cref<offset_2d_t> position, extent_2d_t::scalar_t radius) {
			error_log.add("draw_circle not implemented!");
		}

		inline void draw_circle(cref<offset_2d_t> position, extent_2d_t::scalar_t radius, cref<color_t> color) {
			set_draw_color(color);
			draw_circle(position, radius);
		}
	};
} // namespace bleak
