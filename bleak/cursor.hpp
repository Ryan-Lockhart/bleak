#pragma once

#include "bleak/typedef.hpp"

#include "bleak/cardinal.hpp"
#include "bleak/color.hpp"
#include "bleak/extent.hpp"
#include "bleak/mouse.hpp"
#include "bleak/offset.hpp"
#include "bleak/quadrant.hpp"
#include "bleak/texture.hpp"

#include "bleak/constants/colors.hpp"

namespace bleak {
	struct cursor_t {
	  private:
		texture_t texture;
		offset_2d_t position;

	  public:
		color_t color;

		inline cursor_t() = delete;

		inline cursor_t(ref<renderer_t> renderer, cstr path) : texture{ renderer, path }, position{ 0, 0 }, color{ Colors::White } {}

		inline cursor_t(ref<renderer_t> renderer, cstr path, cref<color_t> color) : texture{ renderer, path }, position{ 0, 0 }, color{ color } {}

		inline void update() { position = Mouse::get_position(); }

		inline void update(cref<offset_2d_t> pos) { position = pos; };

		inline void draw(ref<renderer_t> renderer) const { texture.draw(renderer, position, color); }

		inline cref<offset_2d_t> get_position() const { return position; }
	};

	template<extent_2d_t Size> struct grid_cursor_t {
		texture_t texture;

		offset_2d_t position;

		const extent_2d_t min;
		const extent_2d_t max;

		const bool use_bounds;

		color_t color;

		static constexpr extent_2d_t size{ Size };

		inline grid_cursor_t() = delete;

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path) : texture{ renderer, path }, position{}, min{}, max{}, use_bounds{ false }, color{ Colors::White } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, cref<color_t> color) : texture{ renderer, path }, position{}, min{}, max{}, use_bounds{ false }, color{ color } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, offset_2d_t min, offset_2d_t max) : texture{ renderer, path }, position{}, min{ min }, max{ max }, use_bounds{ true }, color{ Colors::White } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, cref<color_t> color, offset_2d_t min, offset_2d_t max) : texture{ renderer, path }, position{}, min{ min }, max{ max }, use_bounds{ true }, color{ color } {}

		inline void update() {
			const offset_2d_t pos{ Mouse::get_position() / size };
			if (use_bounds) {
				position = offset_2d_t::clamp(pos, min, max);
			} else {
				position = pos;
			}
		}

		inline void update(cref<offset_2d_t> offset) {
			const offset_2d_t pos{ (Mouse::get_position() - offset) / size };
			if (use_bounds) {
				position = offset_2d_t::clamp(pos, min, max);
			} else {
				position = pos;
			}
		}

		inline offset_2d_t midpoint() const { return offset_2d_t{ (max - min) / 2 }; }

		inline offset_2d_t lower_third() const { return offset_2d_t{ (max - min) / 3 }; }

		inline offset_2d_t upper_third() const { return offset_2d_t{ (max - min) / 3 * 2 }; }

		inline void draw(ref<renderer_t> renderer) const { texture.draw(renderer, position * size, color); }

		inline void draw(ref<renderer_t> renderer, cref<offset_2d_t> offset) const { texture.draw(renderer, position * size + offset, color); }

		inline offset_2d_t get_position() const { return position; }

		inline quadrant_t get_quadrant() const {
			const auto mid{ midpoint() };
			return quadrant_t{ position.y < mid.y, position.x < mid.x };
		}

		inline cardinal_t get_nonant() const {
			const auto mid{ midpoint() };
			const auto low{ lower_third() };
			const auto up{ upper_third() };

			return cardinal_t{ position.y < low.y ? cardinal_t::North : position.y < up.y ? cardinal_t::Central : cardinal_t::South, position.x < low.x ? cardinal_t::West : position.x < up.x ? cardinal_t::Central : cardinal_t::East };
		}
	};
} // namespace bleak
