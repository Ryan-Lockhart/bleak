#pragma once

#include "color.hpp"
#include "constants/colors.hpp"
#include "typedef.hpp"

#include "point.hpp"

#include "mouse.hpp"
#include "texture.hpp"

namespace Bleakdepth {
	struct cursor_t {
	  private:
		texture_t texture;
		point_t<i32> position;
		color_t color;

	  public:
		inline cursor_t() = delete;

		inline cursor_t(rval<texture_t> texture) :
			texture { std::move(texture) },
			position { 0, 0 },
			color { Colors::White } {}

		inline cursor_t(rval<texture_t> texture, cref<color_t> color) :
			texture { std::move(texture) },
			position { 0, 0 },
			color { color } {}

		inline void update() { position = Mouse.GetPosition(); }

		inline void draw() const { texture.draw(position, color); }
	};

	template<i32 Width, i32 Height> struct grid_cursor_t {
	  private:
		texture_t texture;
		point_t<i32> position;
		point_t<i32> offset;
		color_t color;

	  public:
		static constexpr point_t<i32> size { Width, Height };

		inline grid_cursor_t() = delete;

		inline grid_cursor_t(rval<texture_t> texture, cref<point_t<i32>> offset) :
			texture { std::move(texture) },
			position { 0, 0 },
			offset { offset },
			color { Colors::White } {}

		inline grid_cursor_t(rval<texture_t> texture, cref<point_t<i32>> offset, cref<color_t> color) :
			texture { std::move(texture) },
			position { 0, 0 },
			offset { offset },
			color { color } {}

		inline void update() { position = Mouse.GetPosition() / size; }

		inline void draw() const { texture.draw(position * size + offset, color); }
	};
} // namespace Bleakdepth
