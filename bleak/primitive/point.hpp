#include "bleak/primitive.hpp"

#include "bleak/offset.hpp"
#include "bleak/extent.hpp"

#include "bleak/color.hpp"

namespace bleak {
    template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Outline> {
        offset_2d_t position;

        extent_2d_t::product_t thickness;
        
        color_t color;
    };

	template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Fill> {
        offset_2d_t position;

        extent_2d_t::product_t thickness;

        color_t color;
    };

	template<> struct primitive_t<primitive_type_t::Point, fill_type_t::Composite> {
        offset_2d_t position;

        extent_2d_t::product_t thickness;

        color_t outline_color;
        color_t fill_color;
    };
}
