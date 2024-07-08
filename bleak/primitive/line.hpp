#include "bleak/primitive.hpp"

#include "bleak/line.hpp"
#include "bleak/extent.hpp"

#include "bleak/color.hpp"

namespace bleak {
    template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Outline> {
        line_2d_t line;

        extent_2d_t::product_t thickness;
        
        color_t color;
    };

	template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Fill> {
        line_2d_t line;

        extent_2d_t::product_t thickness;

        color_t color;
    };

	template<> struct primitive_t<primitive_type_t::Line, fill_type_t::Composite> {
        line_2d_t line;

        extent_2d_t::product_t thickness;

        color_t outline_color;
        color_t fill_color;
    };
}
