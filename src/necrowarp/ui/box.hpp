#pragma once

#include <necrowarp/ui/base.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct embedded_box_t {
		color_t background;
		border_t border;

		constexpr embedded_box_t() noexcept : background{ colors::Black }, border{} {}

		constexpr embedded_box_t(color_t background, border_t border) noexcept : background{ background }, border{ border } {}

		constexpr offset_t raw_position(offset_t position) const noexcept { return position * ui_atlas.glyph_size; }

		constexpr extent_t raw_size(extent_t size) const noexcept { return size * ui_atlas.glyph_size; }

		constexpr bool is_hovered(offset_t position, extent_t size) const noexcept { return Mouse::is_inside(raw_position(position), raw_size(size)); }

		constexpr void draw(ref<renderer_t> renderer, offset_t position, extent_t size) const noexcept {
			renderer.draw_composite_rect(raw_position(position), raw_size(size), background, border.color, border.thickness);
		}

		constexpr void draw(ref<renderer_t> renderer, offset_t position, extent_t size, color_t color) const noexcept {
			renderer.draw_composite_rect(raw_position(position), raw_size(size), color, border.color, border.thickness);
		}
	};

	struct box_t : public transform_t, public embedded_box_t {
		constexpr box_t() noexcept : transform_t{}, embedded_box_t{} {}

		constexpr box_t(transform_t transform, color_t background, border_t border) noexcept : transform_t{ transform }, embedded_box_t{ background, border } {}

		constexpr bool is_hovered() const noexcept { return embedded_box_t::is_hovered(get_offset(), size); }

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			embedded_box_t::draw(renderer, get_offset(), size);
		}
	};
}
