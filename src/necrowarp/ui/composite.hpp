#pragma once

#include <necrowarp/ui.hpp>

namespace necrowarp {
	using namespace bleak;

	struct labeled_button_t : public anchor_t, public embedded_button_t, public embedded_label_t {
		constexpr labeled_button_t() noexcept : anchor_t{}, embedded_button_t{}, embedded_label_t{} {}

		constexpr labeled_button_t(anchor_t anchor, embedded_label_t label) noexcept : anchor_t{ anchor }, embedded_button_t{}, embedded_label_t{ label } {}

		constexpr bool is_hovered() const noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = apply_padding(get_offset(current_size));

			return embedded_button_t::is_hovered(current_pos, current_size);
		}

		constexpr void update(Mouse::button_t button) noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = apply_padding(get_offset(current_size));

			embedded_button_t::update(button, current_pos, current_size);
		}

		constexpr color_t current_background() const noexcept { return is_active() ? box.background : is_hovered() ? box.background.dimmed(0.75f) : box.background.dimmed(0.5f); }

		constexpr void draw(ref<renderer_t> renderer) const noexcept {
			const extent_t current_size = calculate_size();
			const offset_t current_pos = get_offset(current_size);

			embedded_label_t::draw(renderer, current_pos, current_background());
		}
	};
}
