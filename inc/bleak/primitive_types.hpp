#pragma once

#include <bleak/primitive.hpp>

#include <bleak/color.hpp>
#include <bleak/extent.hpp>
#include <bleak/line.hpp>
#include <bleak/offset.hpp>
#include <bleak/rect.hpp>

namespace bleak {
	template<> struct primitive_t<primitive_e::Point, fill_e::Outline> {
		offset_t position;

		extent_t::product_t thickness;

		color_t color;
	};

	template<> struct primitive_t<primitive_e::Point, fill_e::Fill> {
		offset_t position;

		extent_t::product_t thickness;

		color_t color;
	};

	template<> struct primitive_t<primitive_e::Point, fill_e::Composite> {
		offset_t position;

		extent_t::product_t thickness;

		color_t outline_color;
		color_t fill_color;
	};

	template<> struct primitive_t<primitive_e::Line, fill_e::Outline> {
		line_t line;

		extent_t::product_t thickness;

		color_t color;
	};

	template<> struct primitive_t<primitive_e::Line, fill_e::Fill> {
		line_t line;

		color_t color;
	};

	template<> struct primitive_t<primitive_e::Line, fill_e::Composite> {
		line_t line;

		extent_t::product_t thickness;

		color_t outline_color;
		color_t fill_color;
	};

	template<> struct primitive_t<primitive_e::Rectangle, fill_e::Outline> {
		rect_t rect;

		extent_t::product_t thickness;

		color_t color;
	};

	template<> struct primitive_t<primitive_e::Rectangle, fill_e::Fill> {
		rect_t rect;

		color_t color;
	};

	template<> struct primitive_t<primitive_e::Rectangle, fill_e::Composite> {
		rect_t rect;

		extent_t::product_t thickness;

		color_t outline_color;
		color_t fill_color;
	};
} // namespace bleak
