#pragma once

#include <bleak/typedef.hpp> // IWYU pragma: export

#include <SDL.h>

extern "C" {
	struct c_cardinal_t;
	struct c_quadrant_t;

	struct c_offset_t;
	struct c_extent_t;

	struct c_line_t;
	struct c_rect_t;

	struct c_color_t;
	struct c_glyph_t;
}

namespace bleak {
	namespace sdl {
		using point = SDL_Point;
		using fpoint = SDL_FPoint;

		using rect = SDL_Rect;
		using frect = SDL_FRect;

		using color = SDL_Color;
	} // namespace sdl

	struct cardinal_t;
	struct quadrant_t;

	struct offset_t;
	struct extent_t;

	struct line_t;
	struct rect_t;

	struct color_t;
	struct glyph_t;

	enum class primitive_type_t {
		Point,
		Line,
		Triangle,
		Rectangle,
		Circle,
		Ellipse,
		Capsule,
	};

	enum class fill_type_t {
		Outline,
		Fill,
		Composite,
	};

	template<primitive_type_t Primitive, fill_type_t Fill> struct primitive_t;

	template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Outline>;
	using outline_point_t = primitive_t<primitive_type_t::Point, fill_type_t::Outline>;

	template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Fill>;
	using fill_point_t = primitive_t<primitive_type_t::Point, fill_type_t::Fill>;

	template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Composite>;
	using composite_point_t = primitive_t<primitive_type_t::Point, fill_type_t::Composite>;

	template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Outline>;
	using outline_line_t = primitive_t<primitive_type_t::Line, fill_type_t::Outline>;

	template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Fill>;
	using fill_line_t = primitive_t<primitive_type_t::Line, fill_type_t::Fill>;

	template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Composite>;
	using composite_line_t = primitive_t<primitive_type_t::Line, fill_type_t::Composite>;

	template<> struct primitive_t<primitive_type_t::Triangle, fill_type_t::Outline>;
	using outline_triangle_t = primitive_t<primitive_type_t::Triangle, fill_type_t::Outline>;

	template<> struct primitive_t<primitive_type_t::Triangle, fill_type_t::Fill>;
	using fill_triangle_t = primitive_t<primitive_type_t::Triangle, fill_type_t::Fill>;

	template<> struct primitive_t<primitive_type_t::Triangle, fill_type_t::Composite>;
	using composite_triangle_t = primitive_t<primitive_type_t::Triangle, fill_type_t::Composite>;

	template<> struct primitive_t<primitive_type_t::Rectangle, fill_type_t::Outline>;
	using outline_rect_t = primitive_t<primitive_type_t::Rectangle, fill_type_t::Outline>;

	template<> struct primitive_t<primitive_type_t::Rectangle, fill_type_t::Fill>;
	using fill_rect_t = primitive_t<primitive_type_t::Rectangle, fill_type_t::Fill>;

	template<> struct primitive_t<primitive_type_t::Rectangle, fill_type_t::Composite>;
	using composite_rect_t = primitive_t<primitive_type_t::Rectangle, fill_type_t::Composite>;

	template<> struct primitive_t<primitive_type_t::Circle, fill_type_t::Outline>;
	using outline_circle_t = primitive_t<primitive_type_t::Circle, fill_type_t::Outline>;

	template<> struct primitive_t<primitive_type_t::Circle, fill_type_t::Fill>;
	using fill_circle_t = primitive_t<primitive_type_t::Circle, fill_type_t::Fill>;

	template<> struct primitive_t<primitive_type_t::Circle, fill_type_t::Composite>;
	using composite_circle_t = primitive_t<primitive_type_t::Circle, fill_type_t::Composite>;

	template<> struct primitive_t<primitive_type_t::Ellipse, fill_type_t::Outline>;
	using outline_ellipse_t = primitive_t<primitive_type_t::Ellipse, fill_type_t::Outline>;

	template<> struct primitive_t<primitive_type_t::Ellipse, fill_type_t::Fill>;
	using fill_ellipse_t = primitive_t<primitive_type_t::Ellipse, fill_type_t::Fill>;

	template<> struct primitive_t<primitive_type_t::Ellipse, fill_type_t::Composite>;
	using composite_ellipse_t = primitive_t<primitive_type_t::Ellipse, fill_type_t::Composite>;

	template<> struct primitive_t<primitive_type_t::Capsule, fill_type_t::Outline>;
	using outline_capsule_t = primitive_t<primitive_type_t::Capsule, fill_type_t::Outline>;

	template<> struct primitive_t<primitive_type_t::Capsule, fill_type_t::Fill>;
	using fill_capsule_t = primitive_t<primitive_type_t::Capsule, fill_type_t::Fill>;

	template<> struct primitive_t<primitive_type_t::Capsule, fill_type_t::Composite>;
	using composite_capsule_t = primitive_t<primitive_type_t::Capsule, fill_type_t::Composite>;
} // namespace bleak
