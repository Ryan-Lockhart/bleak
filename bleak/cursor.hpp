#pragma once

#include "bleak/offset/offset_2d.hpp"
#include "bleak/typedef.hpp"

#include "bleak/color.hpp"
#include "bleak/extent.hpp"
#include "bleak/mouse.hpp"
#include "bleak/offset.hpp"
#include "bleak/quadrant.hpp"
#include "bleak/texture.hpp"

#include "bleak/constants/colors.hpp"
#include "extent/extent_2d.hpp"

namespace bleak {
	struct cursor_t {
	  private:
		texture_t texture;
		offset_2d_t position;

	  public:
		color_t color;

		inline cursor_t() = delete;

		inline cursor_t(rval<texture_t> texture) : texture{ std::move(texture) }, position{ 0, 0 }, color{ Colors::White } {}

		inline cursor_t(rval<texture_t> texture, cref<color_t> color) : texture{ std::move(texture) }, position{ 0, 0 }, color{ color } {}

		inline void update() { position = Mouse::get_position(); }

		inline void update(cref<offset_2d_t> pos) { position = pos; };

		inline void draw(ref<renderer_t> renderer) const { texture.draw(renderer, position, color); }

		inline cref<offset_2d_t> get_position() const { return position; }
	};

	template<extent_2d_t Size> struct grid_cursor_t {
		texture_t texture;

		offset_2d_t position;
		const offset_2d_t offset;

		const extent_2d_t min;
		const extent_2d_t max;

		const offset_2d_t midpoint;

		const bool use_bounds;

		color_t color;

		static constexpr extent_2d_t size{ Size };

		inline grid_cursor_t() = delete;

		inline grid_cursor_t(rval<texture_t> texture, cref<offset_2d_t> offset) :
			texture{ std::move(texture) },
			position{},
			offset{ offset },
			min{},
			max{},
			midpoint{},
			use_bounds{ false },
			color{ Colors::White } {}

		inline grid_cursor_t(rval<texture_t> texture, cref<offset_2d_t> offset, cref<color_t> color) :
			texture{ std::move(texture) },
			position{},
			offset{ offset },
			min{},
			max{},
			midpoint{},
			use_bounds{ false },
			color{ color } {}

		inline grid_cursor_t(rval<texture_t> texture, cref<offset_2d_t> offset, offset_2d_t min, offset_2d_t max) :
			texture{ std::move(texture) },
			position{},
			offset{ offset },
			min{ min },
			max{ max },
			midpoint{ (max - min) / 2 },
			use_bounds{ true },
			color{ Colors::White } {}

		inline grid_cursor_t(rval<texture_t> texture, cref<offset_2d_t> offset, cref<color_t> color, offset_2d_t min, offset_2d_t max) :
			texture{ std::move(texture) },
			position{},
			offset{ offset },
			min{ min },
			max{ max },
			midpoint{ (max - min) / 2 },
			use_bounds{ true },
			color{ color } {}

		inline void update() {
			offset_2d_t pos{ Mouse::get_position() / size };
			if (use_bounds) {
				position = offset_2d_t::clamp(pos, min, max);
			} else {
				position = pos;
			}
		}

		inline void update(offset_2d_t pos) {
			pos /= size;
			if (use_bounds) {
				position.clamp(pos, min, max);
			} else {
				position = pos;
			}
		};

		inline void draw(ref<renderer_t> renderer) const { texture.draw(renderer, position * size + offset, color); }

		inline offset_2d_t get_position() const { return position; }

		inline offset_2d_t get_screen_position() const { return position + offset; }

		inline quadrant_t get_quadrant() const { return quadrant_t{ position.y < midpoint.y, position.x < midpoint.x }; }
	};
} // namespace bleak
