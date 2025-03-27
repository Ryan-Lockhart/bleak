#pragma once

#include <bleak.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct anchor_t {
		offset_t position;
		cardinal_t alignment;

		constexpr anchor_t() noexcept : position{ 0, 0 }, alignment{ cardinal_e::Northwest } {}

		constexpr anchor_t(offset_t position, cardinal_t alignment) noexcept : position{ position }, alignment{ alignment } {}

		constexpr offset_t get_offset(extent_t size) const noexcept {
			const offset_t half_size{ size / 2 };

			offset_t offs{ position - (static_cast<offset_t>(alignment) - offset_t::Northwest) * half_size };

			if (size.w % 2 == 0 && alignment.east) {
				++offs.x;
			}

			if (size.h % 2 == 0 && alignment.south) {
				++offs.y;
			}
			
			return offs;
		}
	};

	struct transform_t {
		anchor_t anchor;
		extent_t size;

		constexpr transform_t() noexcept : anchor{}, size{} {}

		constexpr transform_t(anchor_t anchor, extent_t size) noexcept : anchor{ anchor }, size{ size } {}

		constexpr offset_t get_offset() const noexcept { return anchor.get_offset(size); }
	};

	struct border_t {
		color_t color;
		extent_t::scalar_t thickness;

		constexpr border_t() noexcept : color{ colors::White }, thickness{ 1 } {}

		constexpr border_t(color_t color, extent_t::scalar_t thickness) noexcept : color{ color }, thickness{ thickness } {}
	};
} // namespace necrowarp
