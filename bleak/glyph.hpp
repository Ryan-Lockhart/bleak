#pragma once

#include "bleak/primitive.hpp"

#include <initializer_list>

#include "bleak/array.hpp"
#include "bleak/color.hpp"
#include "extent/extent_1d.hpp"

namespace bleak {
	struct glyph_t {
		color_t color;
		u16 index;

		constexpr glyph_t() noexcept : color{}, index{} {}

		constexpr glyph_t(u16 index, color_t color) noexcept : color{ color }, index{ index } {}
	};

	template<extent_1d_t Length> struct animated_glyph_t {
	  private:
		row_t<u16, Length> indices;
		extent_1d_t::product_t frame;

		constexpr inline void wrap() noexcept { frame %= length; }

	  public:
		using index_t = extent_1d_t::product_t;

		static constexpr index_t length{ Length };

		static constexpr index_t first{ 0 };
		static constexpr index_t last{ length - 1 };

		using iterator = fwd_iter_t<u16>;
		using const_iterator = fwd_iter_t<const u16>;

		using reverse_iterator = rev_iter_t<u16>;
		using const_reverse_iterator = rev_iter_t<const u16>;

		color_t color;

		constexpr animated_glyph_t() = delete;

		constexpr animated_glyph_t(std::initializer_list<u16> indices) noexcept : indices{ indices }, frame{ 0 }, color{} {}

		constexpr animated_glyph_t(std::initializer_list<u16> indices, color_t color) noexcept : indices{ indices }, frame{ 0 }, color{ color } {}

		constexpr animated_glyph_t(rval<row_t<u16, Length>> indices) noexcept : indices{ indices }, frame{ 0 }, color{} {}

		constexpr animated_glyph_t(rval<row_t<u16, Length>> indices, color_t color) noexcept : indices{ indices }, frame{ 0 }, color{ color } {}

		constexpr animated_glyph_t(cref<animated_glyph_t> other) noexcept : indices{ other.indices }, frame{ other.frame }, color{ other.color } {}

		constexpr animated_glyph_t(rval<animated_glyph_t> other) noexcept : indices{ std::move(other.indices) }, frame{ other.frame }, color{ other.color } {}

		constexpr ref<animated_glyph_t> operator=(cref<animated_glyph_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			indices = other.indices;
			frame = other.frame;
			color = other.color;

			return *this;
		}

		constexpr ref<animated_glyph_t> operator=(rval<animated_glyph_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			indices = std::move(other.indices);
			frame = other.frame;
			color = other.color;

			return *this;
		}

		constexpr ~animated_glyph_t() = default;

		constexpr u16 operator[](index_t index) const noexcept { return indices[index]; }

		constexpr u16 at(index_t index) const {
			if (index >= length) {
				throw std::out_of_range{ "index out of range!" };
			}

			return indices[index];
		}

		constexpr void advance() noexcept {
			++frame;
			wrap();
		}

		constexpr void retreat() noexcept {
			--frame;
			wrap();
		}

		constexpr void reset() noexcept { frame = 0; }

		constexpr void set(index_t index) { frame = index % length; }

		constexpr index_t current_frame() const noexcept { return frame; }

		constexpr glyph_t current() const noexcept { return { indices[frame], color }; }

		constexpr explicit operator glyph_t() const noexcept { return { indices[frame], color }; }

		constexpr u16 front() const noexcept { return indices.front(); }

		constexpr u16 back() const noexcept { return indices.back(); }

		constexpr iterator begin() noexcept { return indices.begin(); }

		constexpr iterator end() noexcept { return indices.end(); }

		constexpr const_iterator begin() const noexcept { return indices.begin(); }

		constexpr const_iterator end() const noexcept { return indices.end(); }

		constexpr const_iterator cbegin() const noexcept { return indices.cbegin(); }

		constexpr const_iterator cend() const noexcept { return indices.cend(); }

		constexpr reverse_iterator rbegin() noexcept { return indices.rbegin(); }

		constexpr reverse_iterator rend() noexcept { return indices.rend(); }

		constexpr const_reverse_iterator rbegin() const noexcept { return indices.rbegin(); }

		constexpr const_reverse_iterator rend() const noexcept { return indices.rend(); }

		constexpr const_reverse_iterator crbegin() const noexcept { return indices.crbegin(); }

		constexpr const_reverse_iterator crend() const noexcept { return indices.crend(); }

		constexpr bool operator==(cref<animated_glyph_t> other) const noexcept = delete;
		constexpr bool operator!=(cref<animated_glyph_t> other) const noexcept = delete;

		constexpr bool operator<(cref<animated_glyph_t> other) const noexcept = delete;
		constexpr bool operator>(cref<animated_glyph_t> other) const noexcept = delete;

		constexpr bool operator<=(cref<animated_glyph_t> other) const noexcept = delete;
		constexpr bool operator>=(cref<animated_glyph_t> other) const noexcept = delete;

		template<u16 Start, u16 End> constexpr static row_t<u16, Length> generate_contiguous_indices() noexcept {
			row_t<u16, Length> indices{};

			static_assert(End - Start + 1 == Length, "lengths of indices and range are not equal!");

			for (index_t i = 0; i < Length; ++i) {
				indices[i] = Start + i;
			}

			return indices;
		}
	};
} // namespace bleak
