#pragma once

#include <bleak/typedef.hpp>

#include <bleak/extent.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	class camera_t {
	  public:
		constexpr camera_t() noexcept = delete;

		constexpr camera_t(cref<extent_t> size) noexcept : position{}, size{ size }, min{}, max{} {}

		constexpr camera_t(cref<extent_t> size, cref<offset_t> position) noexcept : position{ position }, size{ size }, min{}, max{} {}

		constexpr camera_t(cref<extent_t> size, cref<extent_t> min, cref<extent_t> max) noexcept : position{}, size{ size }, min{ min }, max{ max } {}

		constexpr camera_t(cref<extent_t> size, cref<offset_t> position, cref<extent_t> min, cref<extent_t> max) noexcept : position{ position }, size{ size }, min{ min }, max{ max } {}

		constexpr void constrain() noexcept {
			if (min == max) {
				return;
			}

			position.clamp(min, max);
		}

		constexpr void constrain(cref<extent_t> min, cref<extent_t> max) noexcept {
			if (min == max) {
				return;
			}

			position.clamp(min, max);
		}

		constexpr bool set(cref<offset_t> position) noexcept {
			if (this->position == position) {
				return false;
			}

			const offset_t original_position{ position };

			this->position = position;
			constrain();

			return original_position != position;
		}

		constexpr bool set(cref<offset_t> position, cref<extent_t> min, cref<extent_t> max) noexcept {
			if (this->position == position) {
				return false;
			}

			const offset_t original_position{ position };

			this->position = position;
			constrain(min, max);

			return original_position != position;
		}

		constexpr bool move(cref<offset_t> offset) noexcept {
			if (offset == offset_t::Zero) {
				return false;
			}

			const offset_t original_position{ position };

			set(position + offset);

			return original_position != position;
		}

		constexpr bool move(cref<offset_t> offset, cref<extent_t> min, cref<extent_t> max) noexcept {
			if (offset == offset_t::Zero) {
				return false;
			}

			const offset_t original_position{ position };

			set(position + offset, min, max);

			return original_position != position;
		}

		constexpr bool center_on(cref<offset_t> target) noexcept {
			if (target == position) {
				return false;
			}

			const offset_t original_position{ position };

			position = target - size / 2;
			constrain();

			return original_position != position;
		}

		constexpr bool center_on(cref<offset_t> target, cref<extent_t> min, cref<extent_t> max) noexcept {
			if (target == position) {
				return false;
			}

			const offset_t original_position{ position };

			position = target - size / 2;
			constrain(min, max);

			return original_position != position;
		}

		constexpr offset_t get_position() const noexcept { return position; }

		constexpr extent_t get_size() const noexcept { return size; }

		constexpr extent_t get_min() const noexcept { return min; }

		constexpr extent_t get_max() const noexcept { return max; }

		constexpr offset_t get_center() const noexcept { return position + size / 2; }

		constexpr offset_t get_extent() const noexcept { return position + size; }

	  private:
		offset_t position;

		const extent_t size;

		const extent_t min;
		const extent_t max;
	};
} // namespace bleak
