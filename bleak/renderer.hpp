#pragma once

#include "typedef.hpp"

#include <format>
#include <stdexcept>

#include <SDL.h>

#include "cardinal.hpp"
#include "color.hpp"
#include "line.hpp"
#include "log.hpp"
#include "point.hpp"
#include "rect.hpp"
#include "window.hpp"

namespace Bleakdepth {
	namespace sdl {
		using renderer = ptr<SDL_Renderer>;
		using renderer_flags = SDL_RendererFlags;

		constexpr renderer_flags RENDERER_FLAGS_NONE { static_cast<renderer_flags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };

		static inline renderer create_renderer(window window, renderer_flags flags, int driver_index = -1) {
			renderer handle { SDL_CreateRenderer(window, driver_index, flags) };

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
				error_log.add("cannot destroy renderer: renderer handle is nullptr!");
			}
		}
	} // namespace sdl

	class renderer_t {
	  private:
		ptr<SDL_Renderer> renderer;

		static inline ptr<SDL_Renderer> create(ref<window_t> window, u32 flags) {
			if (!window.handle()) {
				throw std::runtime_error("window is nullptr");
			}

			ptr<SDL_Renderer> renderer_ptr { SDL_CreateRenderer(window.handle(), -1, flags) };

			if (!renderer_ptr) {
				throw std::runtime_error(std::format("failed to create renderer: {}", SDL_GetError()));
			}

			return renderer_ptr;
		}

		static inline void destroy(ptr<SDL_Renderer> renderer) {
			if (renderer) {
				SDL_DestroyRenderer(renderer);
			}
		}

	  public:
		inline renderer_t(ref<window_t> window, u32 flags) : renderer { create(window, flags) } {}

		inline ~renderer_t() { destroy(renderer); }

		constexpr ptr<SDL_Renderer> handle() noexcept { return renderer; }

		constexpr cptr<SDL_Renderer> handle() const noexcept { return renderer; }

		inline void set_draw_color(u8 r, u8 g, u8 b, u8 a) noexcept { SDL_SetRenderDrawColor(renderer, r, g, b, a); }

		inline void set_draw_color(cref<color_t> color) noexcept { SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); }

		inline void clear() noexcept { SDL_RenderClear(renderer); }

		inline void clear(cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderClear(renderer);
		}

		inline void present() noexcept { SDL_RenderPresent(renderer); }

		inline void draw_point(i32 x, i32 y) noexcept { SDL_RenderDrawPoint(renderer, x, y); }

		inline void draw_point(i32 x, i32 y, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderDrawPoint(renderer, x, y);
		}

		inline void draw_point(cref<point_t<i32>> point) noexcept { SDL_RenderDrawPoint(renderer, point.x, point.y); }

		inline void draw_point(cref<point_t<i32>> point, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderDrawPoint(renderer, point.x, point.y);
		}

		inline void draw_line(i32 startX, i32 startY, i32 endX, i32 endY) noexcept { SDL_RenderDrawLine(renderer, startX, startY, endX, endY); }

		inline void drawLine(i32 startX, i32 startY, i32 endX, i32 endY, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
		}

		inline void draw_line(cref<point_t<i32>> start, cref<point_t<i32>> end) noexcept { SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y); }

		inline void draw_line(cref<point_t<i32>> start, cref<point_t<i32>> end, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
		}

		inline void draw_line(cref<line_t<i32>> line) noexcept { SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y); }

		inline void draw_line(cref<line_t<i32>> line, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);
		}

		inline void draw_rect(cref<SDL_Rect> rect) noexcept { SDL_RenderDrawRect(renderer, &rect); }

		inline void draw_rect(cref<SDL_Rect> rect, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderDrawRect(renderer, &rect);
		}

		inline void draw_rect(cref<rect_t<i32>> rect) noexcept { SDL_RenderDrawRect(renderer, (ptr<SDL_Rect>)&rect); }

		inline void draw_rect(cref<rect_t<i32>> rect, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderDrawRect(renderer, (ptr<SDL_Rect>)&rect);
		}

		inline void fill_rect(cref<SDL_Rect> rect) noexcept { SDL_RenderFillRect(renderer, &rect); }

		inline void fillRect(cref<SDL_Rect> rect, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderFillRect(renderer, &rect);
		}

		inline void fill_rect(cref<rect_t<i32>> rect) noexcept { SDL_RenderDrawRect(renderer, (ptr<SDL_Rect>)&rect); }

		inline void fill_rect(cref<rect_t<i32>> rect, cref<color_t> color) noexcept {
			set_draw_color(color);
			SDL_RenderFillRect(renderer, (ptr<SDL_Rect>)&rect);
		}

		inline void draw_rect(cref<rect_t<i32>> rect, cref<color_t> fill, cref<color_t> outline) noexcept {
			fill_rect(rect, fill);
			draw_rect(rect, outline);
		}

		inline void draw_rect(cref<rect_t<i32>> rect, cref<color_t> fill, cref<color_t> outline, i32 outlineThickness) noexcept {
			fill_rect(rect, outline);
			fill_rect(
				rect_t<> { rect.position.x + outlineThickness,
						   rect.position.y + outlineThickness,
						   rect.size.w - outlineThickness * 2,
						   rect.size.h - outlineThickness * 2 },
				fill
			);
		}

		inline void draw_rect(cref<point_t<i32>> position, cref<size_t<i32>> size, cref<color_t> fill, cref<color_t> outline) noexcept {
			fill_rect(rect_t<> { position, size }, fill);
			draw_rect(rect_t<> { position, size }, outline);
		}

		inline void draw_rect(point_t<i32> position, size_t<i32> size, cardinal_t alignment, cref<color_t> fill, cref<color_t> outline) noexcept {
			auto aligned_size { size * alignment };

			position += aligned_size + (-aligned_size - size) / 2;

			fill_rect(rect_t<> { position, size }, fill);
			draw_rect(rect_t<> { position, size }, outline);
		}
	};
} // namespace Bleakdepth
