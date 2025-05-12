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
		
		offset_t current_position;
		offset_t last_position;

	  public:
		color_t color;

		inline cursor_t() = delete;

		inline cursor_t(ref<renderer_t> renderer, cstr path) : texture{ renderer, path }, current_position{ 0, 0 }, color{ colors::White } {}

		inline cursor_t(ref<renderer_t> renderer, cstr path, color_t color) : texture{ renderer, path }, current_position{ 0, 0 }, color{ color } {}

		inline void update() { current_position = Mouse::get_position(); }

		inline void update(offset_t pos) { current_position = pos; };

		inline void draw() const { texture.draw(current_position, color); }

		inline void draw(offset_t offset) const { texture.draw(current_position + offset, color); }

		inline offset_t get_position() const { return current_position; }
	};

	template<extent_t Size> struct grid_cursor_t {
	  private:
		texture_t texture;
		
	  public:		
		offset_t current_position;

		const extent_t min;
		const extent_t max;

		const bool use_bounds;

		color_t color;

		static constexpr extent_t size{ Size };

		inline grid_cursor_t() = delete;

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path) : texture{ renderer, path }, current_position{}, min{}, max{}, use_bounds{ false }, color{ colors::White } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, color_t color) : texture{ renderer, path }, current_position{}, min{}, max{}, use_bounds{ false }, color{ color } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, offset_t min, offset_t max) : texture{ renderer, path }, current_position{}, min{ min }, max{ max }, use_bounds{ true }, color{ colors::White } {}

		inline grid_cursor_t(ref<renderer_t> renderer, cstr path, color_t color, offset_t min, offset_t max) : texture{ renderer, path }, current_position{}, min{ min }, max{ max }, use_bounds{ true }, color{ color } {}

		inline void update() noexcept {
			const offset_t grid_pos{ Mouse::get_position() / size };

			if (use_bounds) {
				current_position = offset_t::clamp(grid_pos, min, max);
			} else {
				current_position = grid_pos;
			}
		}

		inline void update(rect_t bounds) noexcept {
			const offset_t grid_pos{ Mouse::get_position() / size };

			if (use_bounds) {
				current_position = offset_t::clamp(grid_pos, bounds.origin(), bounds.extent());
			} else {
				current_position = grid_pos;
			}
		}

		inline void update(offset_t offset) {
			const offset_t grid_pos{ Mouse::get_position() / size + offset };

			if (use_bounds) {
				current_position = offset_t::clamp(grid_pos, min, max);
			} else {
				current_position = grid_pos;
			}
		}

		inline void update(cref<camera_t> camera) {
			const offset_t grid_pos{ Mouse::get_position() / size + camera.get_position() };

			if (use_bounds) {
				current_position = offset_t::clamp(grid_pos, min, max);
			} else {
				current_position = grid_pos;
			}
		}

		inline void update(cardinal_t direction) {
			if (direction == cardinal_e::Central) {
				return;
			}

			const offset_t offset_dir{ direction };

			if (use_bounds) {
				current_position = offset_t::clamp(current_position + offset_dir, min, max);
			} else {
				current_position += offset_dir;
			}
		}

		inline void set(offset_t position) { this->current_position = use_bounds ? offset_t::clamp(position, min, max) : position; }

		inline offset_t midpoint() const { return offset_t{ (max - min) / 2 }; }

		inline offset_t lower_third() const { return offset_t{ (max - min) / 3 }; }

		inline offset_t upper_third() const { return offset_t{ (max - min) / 3 * 2 }; }

		inline void draw() const { texture.draw(current_position * size, color); }

		inline void draw(cref<camera_t> camera) const { texture.draw((current_position - camera.get_position()) * size, color); }

		inline void draw(offset_t offset) const { texture.draw(current_position * size + offset, color); }

		inline void draw(cref<camera_t> camera, offset_t offset) const { texture.draw((current_position - camera.get_position()) * size + offset, color); }

		inline offset_t get_position() const { return current_position; }

		template<typename T, extent_t ZoneSize, extent_t BorderSize> inline ref<T> hovered(ref<zone_t<T, ZoneSize, BorderSize>> zone) { return zone[current_position]; };

		template<typename T, extent_t ZoneSize, extent_t BorderSize> inline cref<T> hovered(cref<zone_t<T, ZoneSize, BorderSize>> zone) const { return zone[current_position]; };

		inline quadrant_t get_quadrant() const {
			const auto mid{ midpoint() };
			return quadrant_t{ current_position.y < mid.y, current_position.x < mid.x };
		}

		inline cardinal_t get_nonant() const {
			const auto mid{ midpoint() };
			const auto low{ lower_third() };
			const auto up{ upper_third() };

			return cardinal_t{ current_position.y < low.y ? cardinal_e::North : current_position.y < up.y ? cardinal_e::Central : cardinal_e::South, current_position.x < low.x ? cardinal_e::West : current_position.x < up.x ? cardinal_e::Central : cardinal_e::East };
		}
	};
} // namespace bleak
