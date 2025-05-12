#pragma once

#include <bleak/primitive.hpp>

#include <bleak/color.hpp>
#include <bleak/extent.hpp>
#include <bleak/line.hpp>
#include <bleak/offset.hpp>
#include <bleak/rect.hpp>

namespace bleak {
	template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Outline> {
		offset_t position;

		extent_t::product_t thickness;

		color_t color;
	};

	template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Fill> {
		offset_t position;

		extent_t::product_t thickness;

		color_t color;
	};

	template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Composite> {
		offset_t position;

		extent_t::product_t thickness;

		color_t outline_color;
		color_t fill_color;
	};

	template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Outline> {
		line_t line;

		extent_t::product_t thickness;

		color_t color;
	};

	template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Fill> {
		line_t line;

		color_t color;
	};

	template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Composite> {
		line_t line;

		extent_t::product_t thickness;

		color_t outline_color;
		color_t fill_color;
	};

	template<> struct primitive_t<primitive_type_t::Rectangle, fill_type_t::Outline> {
		rect_t rect;

		extent_t::product_t thickness;

		color_t color;
	};

	template<> struct primitive_t<primitive_type_t::Rectangle, fill_type_t::Fill> {
		rect_t rect;

		color_t color;
	};

	template<> struct primitive_t<primitive_type_t::Rectangle, fill_type_t::Composite> {
		rect_t rect;

		extent_t::product_t thickness;

		color_t outline_color;
		color_t fill_color;
	};
} // namespace bleak
