#pragma once

#include <bleak/typedef.hpp>

#include <format>

#include <SDL.h>

#include <bleak/color.hpp>
#include <bleak/extent.hpp>
#include <bleak/line.hpp>
#include <bleak/log.hpp>
#include <bleak/offset.hpp>
#include <bleak/primitive.hpp>
#include <bleak/window.hpp>

namespace bleak {
	namespace sdl {
		using texture = SDL_Texture;
		using renderer = SDL_Renderer;
		using renderer_flags = SDL_RendererFlags;

		constexpr renderer_flags RENDERER_FLAGS_NONE{ static_cast<renderer_flags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };

		static inline ptr<renderer> create_renderer(ptr<window> window, renderer_flags flags, i32 driver_index = -1) noexcept {
			ptr<renderer> handle{ SDL_CreateRenderer(window, driver_index, flags) };

			if (handle == nullptr) {
				error_log.add("failed to create renderer: {}", sdl::get_error());
			}

			return handle;
		}

		static inline void destroy_renderer(ref<ptr<renderer>> handle) noexcept {
			if (handle != nullptr) {
				SDL_DestroyRenderer(handle);
				handle = nullptr;
			} else {
				error_log.add("cannot destroy renderer: renderer handle is null!");
			}
		}

		static inline void set_render_target(ptr<renderer> renderer, ptr<texture> texture) noexcept;

		static inline void set_render_draw_color(ptr<renderer> renderer, u8 r, u8 g, u8 b, u8 a) noexcept { SDL_SetRenderDrawColor(renderer, r, g, b, a); }

		static inline void set_render_draw_color(ptr<renderer> renderer, color_t color) noexcept { SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); }
	} // namespace sdl

	struct target_texture_t;

	class renderer_t {
	  private:
		ptr<sdl::renderer> renderer;

	  public:
		inline renderer_t(ref<window_t> window, sdl::renderer_flags flags) : renderer{ sdl::create_renderer(window.handle(), flags) } {}

		inline ~renderer_t() { sdl::destroy_renderer(renderer); }

		constexpr ptr<sdl::renderer> handle() noexcept { return renderer; }

		constexpr cptr<sdl::renderer> handle() const noexcept { return renderer; }

		inline void set_target(ref<target_texture_t> target) noexcept;

		inline void unset_target() noexcept;

		inline void set_draw_color(u8 r, u8 g, u8 b, u8 a) noexcept { sdl::set_render_draw_color(renderer, r, g, b, a); }

		inline void set_draw_color(color_t color) noexcept { sdl::set_render_draw_color(renderer, color); }

		inline void clear() noexcept { SDL_RenderClear(renderer); }

		inline void clear(color_t color) noexcept {
			set_draw_color(color);
			SDL_RenderClear(renderer);
		}

		inline void present() noexcept { SDL_RenderPresent(renderer); }

		template<primitive_type_t Primitive, fill_type_t Fill> inline void draw(cref<primitive_t<Primitive, Fill>> primitive) noexcept {
			error_log.add("draw not implemented for primitive: {} {}", (std::string)Primitive, (std::string)Fill);
			return;

			if constexpr (Primitive == primitive_type_t::Point) {
				if constexpr (Fill == fill_type_t::Outline) {

				} else if constexpr (Fill == fill_type_t::Fill) {

				} else if constexpr (Fill == fill_type_t::Composite) {
				}
			} else if constexpr (Primitive == primitive_type_t::Line) {
				if constexpr (Fill == fill_type_t::Outline) {

				} else if constexpr (Fill == fill_type_t::Fill) {

				} else if constexpr (Fill == fill_type_t::Composite) {
				}
			} else if constexpr (Primitive == primitive_type_t::Triangle) {
				if constexpr (Fill == fill_type_t::Outline) {

				} else if constexpr (Fill == fill_type_t::Fill) {

				} else if constexpr (Fill == fill_type_t::Composite) {
				}
			} else if constexpr (Primitive == primitive_type_t::Rectangle) {
				if constexpr (Fill == fill_type_t::Outline) {

				} else if constexpr (Fill == fill_type_t::Fill) {

				} else if constexpr (Fill == fill_type_t::Composite) {
				}
			} else if constexpr (Primitive == primitive_type_t::Circle) {
				if constexpr (Fill == fill_type_t::Outline) {

				} else if constexpr (Fill == fill_type_t::Fill) {

				} else if constexpr (Fill == fill_type_t::Composite) {
				}
			} else if constexpr (Primitive == primitive_type_t::Ellipse) {
				if constexpr (Fill == fill_type_t::Outline) {

				} else if constexpr (Fill == fill_type_t::Fill) {

				} else if constexpr (Fill == fill_type_t::Composite) {
				}
			} else if constexpr (Primitive == primitive_type_t::Capsule) {
				if constexpr (Fill == fill_type_t::Outline) {

				} else if constexpr (Fill == fill_type_t::Fill) {

				} else if constexpr (Fill == fill_type_t::Composite) {
				}
			}
		}

		inline void draw_point(offset_t::scalar_t x, offset_t::scalar_t y) noexcept { SDL_RenderDrawPoint(renderer, x, y); }

		inline void draw_point(offset_t::scalar_t x, offset_t::scalar_t y, color_t color) noexcept {
			set_draw_color(color);
			draw_point(x, y);
		}

		inline void draw_point(offset_t point) noexcept { SDL_RenderDrawPoint(renderer, point.x, point.y); }

		inline void draw_point(offset_t point, color_t color) noexcept {
			set_draw_color(color);
			draw_point(point);
		}

		inline void draw_line(offset_t start, offset_t end) noexcept { SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y); }

		inline void draw_line(offset_t start, offset_t end, color_t color) noexcept {
			set_draw_color(color);
			draw_line(start, end);
		}

		inline void draw_line(offset_t start, offset_t end, extent_t::scalar_t thickness) noexcept {
			offset_t::product_t dst{ offset_t::distance<distance_function_t::Chebyshev>(start, end) };
			offset_t dir{ offset_t::direction(start, end) };

			offset_t origin{ start + dir * thickness / 2 };

			sdl::rect rect{ static_cast<i32>(origin.x), static_cast<i32>(origin.y), static_cast<i32>(thickness), static_cast<i32>(dst) };

			SDL_RenderDrawRect(renderer, &rect);
		}

		inline void draw_line(offset_t start, offset_t end, color_t color, extent_t::scalar_t thickness) noexcept {
			set_draw_color(color);
			draw_line(start, end, thickness);
		}

		inline void draw_line(cref<line_t> line) noexcept { draw_line(line.start, line.end); }

		inline void draw_line(cref<line_t> line, color_t color) noexcept { draw_line(line.start, line.end, color); }

		inline void draw_line(cref<line_t> line, color_t color, extent_t::scalar_t thickness) noexcept { draw_line(line.start, line.end, color, thickness); }

		inline void draw_line(cref<line_t> line, extent_t::scalar_t thickness) noexcept { draw_line(line.start, line.end, thickness); }

		inline void draw_outline_rect(offset_t position, extent_t size) noexcept {
			sdl::rect sdl_rect{ static_cast<i32>(position.x), static_cast<i32>(position.y), static_cast<i32>(size.w), static_cast<i32>(size.h) };
			SDL_RenderDrawRect(renderer, &sdl_rect);
		}

		inline void draw_outline_rect(offset_t position, extent_t size, extent_t thickness) noexcept {
			extent_t dbl_thickness{ thickness * 2 };

			extent_t vertical_size{ size.w, dbl_thickness.h };
			extent_t horizontal_size{ dbl_thickness.w, size.h - dbl_thickness.h };

			draw_fill_rect(position, vertical_size);																			 // top
			draw_fill_rect(offset_t{ position.x, position.y + size.h - dbl_thickness.h }, vertical_size);					 // bottom
			draw_fill_rect(offset_t{ position.x, position.y + dbl_thickness.h }, horizontal_size);							 // left
			draw_fill_rect(offset_t{ position.x + size.w - dbl_thickness.w, position.y + dbl_thickness.h }, horizontal_size); // right
		}

		inline void draw_outline_rect(offset_t position, extent_t size, extent_t thickness, color_t color) noexcept {
			set_draw_color(color);
			draw_outline_rect(position, size, thickness);
		}

		inline void draw_outline_rect(offset_t position, extent_t size, color_t color) noexcept {
			set_draw_color(color);
			draw_outline_rect(position, size);
		}

		inline void draw_outline_rect(cref<rect_t> rect) noexcept { draw_outline_rect(rect.position, rect.size); }

		inline void draw_outline_rect(cref<rect_t> rect, color_t color) noexcept { draw_outline_rect(rect.position, rect.size, color); }

		inline void draw_fill_rect(offset_t position, extent_t size) noexcept {
			sdl::rect sdl_rect{ static_cast<i32>(position.x), static_cast<i32>(position.y), static_cast<i32>(size.w), static_cast<i32>(size.h) };
			SDL_RenderFillRect(renderer, &sdl_rect);
		}

		inline void draw_fill_rect(offset_t position, extent_t size, color_t color) noexcept {
			set_draw_color(color);
			draw_fill_rect(position, size);
		}

		inline void draw_fill_rect(cref<rect_t> rect) noexcept { draw_fill_rect(rect.position, rect.size); }

		inline void draw_fill_rect(cref<rect_t> rect, color_t color) noexcept { draw_fill_rect(rect.position, rect.size, color); }

		inline void draw_composite_rect(offset_t position, extent_t size, color_t fill_color, color_t outline_color, extent_t::scalar_t thickness) noexcept {
			draw_fill_rect(position, size, outline_color);
			draw_fill_rect(position + thickness, size - thickness * 2, fill_color);
		}

		inline void draw_composite_rect(cref<rect_t> rect, color_t fill_color, color_t outline_color, extent_t::scalar_t thickness) noexcept {
			draw_fill_rect(rect, outline_color);
			draw_fill_rect(rect.position + thickness, rect.size - thickness * 2, fill_color);
		}

		inline void draw_composite_rect(offset_t position, extent_t size, color_t fill_color, color_t outline_color, extent_t::scalar_t thickness, bool) noexcept {
			draw_fill_rect(position - thickness, size + thickness * 2, outline_color);
			draw_fill_rect(position, size, fill_color);
		}

		inline void draw_composite_rect(cref<rect_t> rect, color_t fill_color, color_t outline_color, extent_t::scalar_t thickness, bool) noexcept {
			draw_fill_rect(rect.position - thickness, rect.size + thickness * 2, outline_color);
			draw_fill_rect(rect, fill_color);
		}

		inline void draw_circle(offset_t position, extent_t::scalar_t radius) { error_log.add("draw_circle not implemented!"); }

		inline void draw_circle(offset_t position, extent_t::scalar_t radius, color_t color) {
			set_draw_color(color);
			draw_circle(position, radius);
		}
	};
} // namespace bleak
