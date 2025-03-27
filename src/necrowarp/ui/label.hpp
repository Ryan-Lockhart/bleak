#pragma once

#include <necrowarp/ui/base.hpp>

#include <necrowarp/ui/box.hpp>
#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct embedded_label_t {
		runes_t text;
		embedded_box_t box;
		extent_t padding;

		constexpr embedded_label_t() noexcept : text{}, box{}, padding{ 1, 1 } {}

		constexpr embedded_label_t(cref<runes_t> text, embedded_box_t box, extent_t padding) noexcept : text{ text }, box{ box }, padding{ padding } {}

		constexpr offset_t apply_padding(offset_t position) const noexcept { return position - padding; }

		constexpr extent_t calculate_size() const noexcept { return text::calculate_size(text) + padding * 2; }

		constexpr bool is_hovered(offset_t position) const noexcept { return box.is_hovered(apply_padding(position), calculate_size()); }

		constexpr void draw(ref<renderer_t> renderer, offset_t position) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size());
			ui_atlas.draw(text, position);
		}

		constexpr void draw(ref<renderer_t> renderer, offset_t position, color_t background) const noexcept {
			box.draw(renderer, apply_padding(position), calculate_size(), background);
			ui_atlas.draw(text, position);
		}
	};

	struct label_t : public anchor_t, public embedded_label_t {
		constexpr label_t() noexcept : anchor_t{}, embedded_label_t{} {}

		constexpr label_t(anchor_t anchor, embedded_label_t label) noexcept : anchor_t{ anchor }, embedded_label_t{ label } {}

		constexpr bool is_hovered() const noexcept {
			return embedded_label_t::is_hovered(anchor_t::get_offset(embedded_label_t::calculate_size()));
		}

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			embedded_label_t::draw(renderer, anchor_t::get_offset(embedded_label_t::calculate_size()));
		}

		constexpr void draw(ref<renderer_t> renderer, color_t background) const noexcept {
			embedded_label_t::draw(renderer, anchor_t::get_offset(embedded_label_t::calculate_size()), background);
		}
	};
}
