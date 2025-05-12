#pragma once

#include <bleak/typedef.hpp>

#include <bleak/extent.hpp>
#include <bleak/offset.hpp>
#include <bleak/rect.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	class camera_t {
	  public:
		constexpr camera_t() noexcept = delete;

		constexpr camera_t(extent_t size) noexcept : position{}, size{ size }, min{}, max{} {}

		constexpr camera_t(extent_t size, offset_t position) noexcept : position{ position }, size{ size }, min{}, max{} {}

		constexpr camera_t(extent_t size, extent_t min, extent_t max) noexcept : position{}, size{ size }, min{ min }, max{ max } {}

		constexpr camera_t(extent_t size, offset_t position, extent_t min, extent_t max) noexcept : position{ position }, size{ size }, min{ min }, max{ max } {}

		constexpr void constrain() noexcept {
			if (min == max) {
				return;
			}

			position.clamp(min, max);
		}

		constexpr void constrain(extent_t min, extent_t max) noexcept {
			if (min == max) {
				return;
			}

			position.clamp(min, max);
		}

		constexpr void constrain_width() noexcept {
			if (min.w == max.w) {
				return;
			}

			position.x = clamp<offset_t::scalar_t>(position.x, min.w, max.w);
		}

		constexpr void constrain_height() noexcept {
			if (min.h == max.h) {
				return;
			}

			position.y = clamp<offset_t::scalar_t>(position.y, min.h, max.h);
		}

		constexpr bool set(offset_t position) noexcept {
			if (this->position == position) {
				return false;
			}

			const offset_t original_position{ position };

			this->position = position;
			constrain();

			return original_position != position;
		}

		constexpr bool set(offset_t position, extent_t min, extent_t max) noexcept {
			if (this->position == position) {
				return false;
			}

			const offset_t original_position{ position };

			this->position = position;
			constrain(min, max);

			return original_position != position;
		}

		constexpr bool move(offset_t offset) noexcept {
			if (offset == offset_t::Zero) {
				return false;
			}

			const offset_t original_position{ position };

			set(position + offset);

			return original_position != position;
		}

		constexpr bool move(offset_t offset, extent_t min, extent_t max) noexcept {
			if (offset == offset_t::Zero) {
				return false;
			}

			const offset_t original_position{ position };

			set(position + offset, min, max);

			return original_position != position;
		}

		template<bool Force = false> constexpr bool center_on(offset_t target) noexcept {
			const offset_t target_position{ target - half_size() };
			
			if (position == target_position) {
				return false;
			}

			const offset_t original_position{ position };

			position = target_position;
			if constexpr (!Force) {
				constrain();
			}

			return original_position != position;
		}

		template<bool Force = false> constexpr bool center_on(offset_t target, extent_t min, extent_t max) noexcept {
			const offset_t target_position{ target - half_size() };

			if (position == target_position) {
				return false;
			}

			const offset_t original_position{ position };

			position = target_position;
			if constexpr (!Force) {
				constrain(min, max);
			}

			return original_position != position;
		}

		constexpr bool center_on(bool force_width, offset_t::scalar_t target_x, bool force_height, offset_t::scalar_t target_y) noexcept {
			const offset_t target_position{ offset_t{ target_x, target_y } - half_size() };
			
			if (position == target_position) {
				return false;
			}

			const offset_t original_position{ position };

			position = target_position;

			if (!force_width && !force_height) {
				constrain();
			} else {
				if (!force_width) {
					constrain_width();
				}

				if (!force_height) {
					constrain_height();
				}
			}

			return original_position != position;
		}

		constexpr offset_t get_position() const noexcept { return position; }

		constexpr offset_t get_offset() const noexcept { return -position; }

		constexpr extent_t get_size() const noexcept { return size; }

		constexpr extent_t get_min() const noexcept { return min; }

		constexpr extent_t get_max() const noexcept { return max; }

		constexpr offset_t get_center() const noexcept { return position + size / 2; }

		constexpr offset_t get_extent() const noexcept { return position + size; }

		constexpr rect_t get_viewport() const noexcept { return rect_t{ position, size }; }

	  private:
		offset_t position;

		const extent_t size;

		constexpr inline extent_t half_size() const noexcept { return size / 2; }

		const extent_t min;
		const extent_t max;
	};
} // namespace bleak
