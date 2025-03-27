#pragma once

#include <bleak/typedef.hpp>

#include <bleak/camera.hpp>
#include <bleak/cardinal.hpp>
#include <bleak/color.hpp>
#include <bleak/extent.hpp>
#include <bleak/mouse.hpp>
#include <bleak/offset.hpp>
#include <bleak/quadrant.hpp>
#include <bleak/texture.hpp>
#include <bleak/zone.hpp>

#include <bleak/constants/colors.hpp>

namespace bleak {
	struct cursor_t {
	  private:
		texture_t texture;
		offset_t position;

	  public:
		color_t color;

		inline cursor_t() = delete;

		inline cursor_t(ref<renderer_t> renderer, cstr path) : texture{ renderer, path }, position{ 0, 0 }, color{ colors::White } {}

		inline cursor_t(ref<renderer_t> renderer, cstr path, cref<color_t> color) : texture{ renderer, path }, position{ 0, 0 }, color{ color } {}

		inline void update() { position = Mouse::get_position(); }

		inline void update(cref<offset_t> pos) { position = pos; };

		inline void draw() const { texture.draw(position, color); }

		inline void draw(cref<offset_t> offset) const { texture.draw(position + offset, color); }

		inline cref<offset_t> get_position() const { return position; }
	};

	template<extent_t Size> struct grid_cursor_t {
		texture_t texture;

		offset_t position;

		const extent_t min;
		const extent_t max;

		const bool use_bounds;

		color_t color;

		static constexpr extent_t size{ Size };

		inline grid_cursor_t() = delete;

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path) : texture{ renderer, path }, position{}, min{}, max{}, use_bounds{ false }, color{ colors::White } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, cref<color_t> color) : texture{ renderer, path }, position{}, min{}, max{}, use_bounds{ false }, color{ color } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, cref<offset_t> min, cref<offset_t> max) : texture{ renderer, path }, position{}, min{ min }, max{ max }, use_bounds{ true }, color{ colors::White } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, cref<color_t> color, cref<offset_t> min, cref<offset_t> max) : texture{ renderer, path }, position{}, min{ min }, max{ max }, use_bounds{ true }, color{ color } {}

		inline void update() {
			const offset_t pos{ Mouse::get_position() / size };
			if (use_bounds) {
				position = offset_t::clamp(pos, min, max);
			} else {
				position = pos;
			}
		}

		inline void update(cref<offset_t> offset) {
			const offset_t pos{ Mouse::get_position() / size + offset };
			if (use_bounds) {
				position = offset_t::clamp(pos, min, max);
			} else {
				position = pos;
			}
		}

		inline void update(cref<cardinal_t> direction) {
			if (direction == cardinal_e::Central) {
				return;
			}

			const offset_t offset_dir{ direction };

			if (use_bounds) {
				position = offset_t::clamp(position + offset_dir, min, max);
			} else {
				position += offset_dir;
			}
		}

		inline void set(cref<offset_t> position) { this->position = use_bounds ? offset_t::clamp(position, min, max) : position; }

		inline offset_t midpoint() const { return offset_t{ (max - min) / 2 }; }

		inline offset_t lower_third() const { return offset_t{ (max - min) / 3 }; }

		inline offset_t upper_third() const { return offset_t{ (max - min) / 3 * 2 }; }

		inline void draw() const { texture.draw(position * size, color); }

		inline void draw(cref<camera_t> camera) const { texture.draw((position - camera.get_position()) * size, color); }

		inline void draw(cref<offset_t> offset) const { texture.draw(position * size + offset, color); }

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const { texture.draw((position + camera.get_offset()) * size + offset, color); }

		inline offset_t get_position() const { return position; }

		template<typename T, extent_t ZoneSize, extent_t BorderSize> inline ref<T> hovered(ref<zone_t<T, ZoneSize, BorderSize>> zone) { return zone[position]; };

		template<typename T, extent_t ZoneSize, extent_t BorderSize> inline cref<T> hovered(cref<zone_t<T, ZoneSize, BorderSize>> zone) const { return zone[position]; };

		inline quadrant_t get_quadrant() const {
			const auto mid{ midpoint() };
			return quadrant_t{ position.y < mid.y, position.x < mid.x };
		}

		inline cardinal_t get_nonant() const {
			const auto mid{ midpoint() };
			const auto low{ lower_third() };
			const auto up{ upper_third() };

			return cardinal_t{ position.y < low.y ? cardinal_e::North : position.y < up.y ? cardinal_e::Central : cardinal_e::South, position.x < low.x ? cardinal_e::West : position.x < up.x ? cardinal_e::Central : cardinal_e::East };
		}
	};
} // namespace bleak
