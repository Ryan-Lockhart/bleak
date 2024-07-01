#pragma once

#include "typedef.hpp"

#include "color.hpp"
#include "mouse.hpp"
#include "point.hpp"
#include "texture.hpp"

#include "constants/colors.hpp"

namespace Bleakdepth {
	struct cursor_t {
	  private:
		texture_t texture;
		point_t<i32> position;

	  public:
		color_t color;

		inline cursor_t() = delete;

		inline cursor_t(rval<texture_t> texture) : texture { std::move(texture) }, position { 0, 0 }, color { Colors::White } {}

		inline cursor_t(rval<texture_t> texture, cref<color_t> color) : texture { std::move(texture) }, position { 0, 0 }, color { color } {}

		inline void update() { position = Mouse::get_position(); }

		inline void update(cref<point_t<i32>> pos) { position = pos; };

		inline void draw() const { texture.draw(position, color); }

		inline cref<point_t<i32>> get_position() const { return position; }
	};

	template<i32 Width, i32 Height> struct grid_cursor_t {
	  private:
		texture_t texture;
		point_t<i32> position;
		point_t<i32> offset;

		const point_t<i32> min;
		const point_t<i32> max;

		const bool use_bounds;

	  public:
		color_t color;

		static constexpr point_t<i32> size { Width, Height };

		inline grid_cursor_t() = delete;

		inline grid_cursor_t(rval<texture_t> texture, cref<point_t<i32>> offset) :
			texture { std::move(texture) },
			position {},
			offset { offset },
			min {},
			max {},
			use_bounds { false },
			color { Colors::White } {}

		inline grid_cursor_t(rval<texture_t> texture, cref<point_t<i32>> offset, cref<color_t> color) :
			texture { std::move(texture) },
			position {},
			offset { offset },
			min {},
			max {},
			use_bounds { false },
			color { color } {}

		inline grid_cursor_t(rval<texture_t> texture, cref<point_t<i32>> offset, point_t<i32> min, point_t<i32> max) :
			texture { std::move(texture) },
			position {},
			offset { offset },
			min { min },
			max { max },
			use_bounds { true },
			color { Colors::White } {}

		inline grid_cursor_t(rval<texture_t> texture, cref<point_t<i32>> offset, cref<color_t> color, point_t<i32> min, point_t<i32> max) :
			texture { std::move(texture) },
			position {},
			offset { offset },
			min { min },
			max { max },
			use_bounds { true },
			color { color } {}

		inline void update() {
			point_t<i32> pos { Mouse::get_position() / size };
			if (use_bounds) {
				position.clamp_to(pos, min, max);
			} else {
				position = pos;
			}
		}

		inline void update(point_t<i32> pos) {
			pos /= size;
			if (use_bounds) {
				position.clamp_to(pos, min, max);
			} else {
				position = pos;
			}
		};

		inline void draw() const { texture.draw(position * size + offset, color); }

		inline cref<point_t<i32>> get_position() const { return position; }

		inline point_t<i32> get_screen_position() const { return position + offset; }
	};
} // namespace Bleakdepth
