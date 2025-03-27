#pragma once

#include <bleak.hpp>

#include <necrowarp/ui/base.hpp>
#include <necrowarp/ui/box.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	struct status_t {
		glyph_t active_glyph;
		glyph_t inactive_glyph;
		
		u8 max_value{ 16 };
		u8 current_value;

		constexpr status_t(u8 max_value, cref<glyph_t> active_glyph, cref<glyph_t> inactive_glyph) noexcept : active_glyph{ active_glyph }, inactive_glyph{ inactive_glyph }, max_value{ max_value }, current_value{} {}

		constexpr status_t(u8 max_value, u8 current_value, cref<glyph_t> active_glyph, cref<glyph_t> inactive_glyph) noexcept : active_glyph{ active_glyph }, inactive_glyph{ inactive_glyph }, max_value{ max_value }, current_value{} {}
	};

	template<usize Statuses> struct embedded_status_bar_t {
		std::array<status_t, Statuses> statuses;

		embedded_box_t box;

		extent_t padding;

		constexpr embedded_status_bar_t(std::array<status_t, Statuses> statuses) noexcept : statuses{ statuses } {}

		constexpr embedded_status_bar_t(std::array<status_t, Statuses> statuses, embedded_box_t box, extent_t padding) noexcept : statuses{ statuses }, box{ box }, padding{ padding } {}

		constexpr cref<status_t> operator[](usize index) const noexcept { return statuses[index]; }
		constexpr ref<status_t> operator[](usize index) noexcept { return statuses[index]; }

		constexpr offset_t apply_padding(offset_t position) const noexcept { return position - padding; }

		constexpr usize count() const noexcept { return Statuses; }

		constexpr u8 max_length() const noexcept {
			u8 max_length{};

			for (u8 i{ 0 }; i < Statuses; ++i) {
				max_length = std::max(max_length, statuses[i].max_value);
			}

			return max_length;
		}

		constexpr extent_t internal_size() const noexcept { return extent_t{ max_length(), Statuses }; }

		constexpr extent_t external_size() const noexcept { return extent_t{ max_length(), Statuses } + padding * 2; }

		constexpr extent_t bounding_size() const noexcept { return internal_size() * globals::CellToGlyphRatio + padding * 2; }

		constexpr bool is_hovered(offset_t position) const noexcept { return box.is_hovered(apply_padding(position), bounding_size()); }

		constexpr void draw(ref<renderer_t> renderer, offset_t position) const noexcept {
			box.draw(renderer, apply_padding(position), bounding_size());

			const usize max_len = max_length();

			for (usize i{ 0 }; i < Statuses; ++i) {
				for (usize j{ 0 }; j < max_len; ++j) {
					if (j < statuses[i].max_value) {
						game_atlas.draw(statuses[i].current_value > j ? statuses[i].active_glyph : statuses[i].inactive_glyph, position + offset_t{ j, i }, offset_t{ -8, -8 });
					}
				}
			}
		}
	};
	
	template<usize Statuses> struct status_bar_t : public anchor_t, public embedded_status_bar_t<Statuses> {
		constexpr status_bar_t(std::array<status_t, Statuses> statuses) noexcept : anchor_t{}, embedded_status_bar_t<Statuses>{ statuses } {}

		constexpr status_bar_t(anchor_t anchor, std::array<status_t, Statuses> statuses, embedded_box_t box, extent_t padding) noexcept : anchor_t{ anchor }, embedded_status_bar_t<Statuses>{ statuses, box, padding } {}

		constexpr bool is_hovered() const noexcept {
			return embedded_status_bar_t<Statuses>::is_hovered(anchor_t::get_offset(embedded_status_bar_t<Statuses>::bounding_size()));
		}

		constexpr void draw(ref<renderer_t> renderer) const noexcept { embedded_status_bar_t<Statuses>::draw(renderer, get_offset(embedded_status_bar_t<Statuses>::external_size())); }
	};
}
