#pragma once

#include <bleak/typedef.hpp>

#include <string>
#include <vector>

#include <bleak/color.hpp>
#include <bleak/extent.hpp>
#include <bleak/glyph.hpp>
#include <bleak/offset.hpp>

#include <bleak/constants/colors.hpp>

namespace bleak {
	class runes_t : public std::vector<glyph_t> {
	  public:
		static constexpr color_t DEFAULT_COLOR{ Colors::White };

		constexpr runes_t(cref<std::string> text) {
			for (auto& ch : text) {
				emplace_back(ch, DEFAULT_COLOR);
			}
		}

		constexpr runes_t(cref<std::string> text, color_t color) {
			for (auto& ch : text) {
				emplace_back(ch, color);
			}
		}

		constexpr ref<runes_t> operator+=(cref<glyph_t> glyph) {
			push_back(glyph);

			return *this;
		}

		constexpr ref<runes_t> operator+=(rval<glyph_t> glyph) {
			push_back(std::move(glyph));

			return *this;
		}

		constexpr ref<runes_t> operator+=(u8 index) {
			emplace_back(index, empty() ? DEFAULT_COLOR : back().color);

			return *this;
		}

		constexpr ref<runes_t> operator+=(char ch) {
			emplace_back(static_cast<u8>(ch), empty() ? DEFAULT_COLOR : back().color);

			return *this;
		}

		// convenience cast, use sparingly as rune strings are not inherently strings
		constexpr explicit operator std::string() const {
			std::string casted{};

			for (auto iter{ cbegin() }; iter != cend(); ++iter) {
				casted += iter->index;
			}

			return casted;
		}
	};

	namespace Text {
		static constexpr isize HORIZONTAL_TAB_WIDTH{ 4 };
		static_assert(
			HORIZONTAL_TAB_WIDTH && ((HORIZONTAL_TAB_WIDTH & (HORIZONTAL_TAB_WIDTH - 1)) == 0 && HORIZONTAL_TAB_WIDTH > 0),
			"horizontal tab width must be a positive power-of-two!"
		);

		static constexpr isize VERTICAL_TAB_WIDTH{ 4 };
		static_assert(
			VERTICAL_TAB_WIDTH && ((VERTICAL_TAB_WIDTH & (VERTICAL_TAB_WIDTH - 1)) == 0 && VERTICAL_TAB_WIDTH > 0),
			"vertical tab width must be a positive power-of-two!"
		);

		static constexpr extent_t calculate_size(cref<runes_t> runes) noexcept {
			if (runes.empty()) {
				return extent_t::Zero;
			}

			usize current_width{ 0 };
			extent_t size{ 0, 1 };

			for (auto& rune : runes) {
				switch (rune.index) {
				case '\0':
					goto superbreak;
				case '\n':
					++size.h;
					if (current_width > size.w) {
						size.w = current_width;
					}
					current_width = 0;
					continue;
				case '\t':
					current_width += (current_width + HORIZONTAL_TAB_WIDTH - 1) & -HORIZONTAL_TAB_WIDTH;
					continue;
				case '\v':
					size.h += (size.h + VERTICAL_TAB_WIDTH - 1) & -VERTICAL_TAB_WIDTH;
					if (current_width > size.w) {
						size.w = current_width;
					}
					current_width = 0;
					continue;
				default:
					++current_width;
					continue;
				}
			}

		superbreak:
			if (current_width > size.w) {
				size.w = current_width;
			}

			return size;
		}

		static constexpr extent_t calculate_size(cref<std::string> text) noexcept {
			return calculate_size(runes_t{ text });
		}
	} // namespace Text
} // namespace bleak
