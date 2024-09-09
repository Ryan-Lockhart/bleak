#pragma once

#include <bleak/primitive.hpp>

#include <cassert>
#include <initializer_list>
#include <array>

#include <bleak/array.hpp>
#include <bleak/color.hpp>
#include <bleak/concepts.hpp>
#include <bleak/hash.hpp>

namespace bleak {
	struct glyph_t {
		color_t color;
		u32 index;

		constexpr glyph_t() noexcept : color{}, index{} {}

		constexpr glyph_t(u32 index, color_t color) noexcept : color{ color }, index{ index } {}

		struct hasher {
			static constexpr size_t operator()(cref<glyph_t> glyph) noexcept { return hash_combine(glyph.color, glyph.index); }
		};
	};

	template<extent_t::product_t Length> struct animated_glyph_t {
	  private:
		static constexpr extent_t size{ Length, 1 };

		array_t<u32, size> indices;
		extent_t::product_t frame;

		constexpr inline void wrap() noexcept { frame %= length; }

	  public:
		using index_t = extent_t::product_t;

		static constexpr index_t length{ Length };

		static constexpr index_t first{ 0 };
		static constexpr index_t last{ length - 1 };

		using iterator = fwd_iter_t<u32>;
		using const_iterator = fwd_iter_t<const u32>;

		using reverse_iterator = rev_iter_t<u32>;
		using const_reverse_iterator = rev_iter_t<const u32>;

		color_t color;

		constexpr animated_glyph_t() = delete;

		constexpr animated_glyph_t(std::array<u32, Length> indices) noexcept : indices{ indices }, frame{ 0 }, color{} {}

		constexpr animated_glyph_t(std::array<u32, Length> indices, color_t color) noexcept : indices{ indices }, frame{ 0 }, color{ color } {}

		template<Integer IndexType> constexpr animated_glyph_t(std::array<IndexType, Length> indices) noexcept : indices{ indices }, frame{ 0 }, color{} {}

		template<Integer IndexType> constexpr animated_glyph_t(std::array<IndexType, Length> indices, color_t color) noexcept : indices{ indices }, frame{ 0 }, color{ color } {}

		constexpr animated_glyph_t(std::initializer_list<u32> indices) noexcept : indices{ indices }, frame{ 0 }, color{} {}

		constexpr animated_glyph_t(std::initializer_list<u32> indices, color_t color) noexcept : indices{ indices }, frame{ 0 }, color{ color } {}

		constexpr animated_glyph_t(rval<array_t<u32, size>> indices) noexcept : indices{ std::move(indices) }, frame{ 0 }, color{} {}

		constexpr animated_glyph_t(rval<array_t<u32, size>> indices, color_t color) noexcept : indices{ std::move(indices) }, frame{ 0 }, color{ color } {}

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

		constexpr u32 operator[](index_t index) const noexcept { return indices[index]; }

		constexpr u32 at(index_t index) const {
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

		constexpr u32 front() const noexcept { return indices.front(); }

		constexpr u32 back() const noexcept { return indices.back(); }

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

		struct hasher {
			static constexpr size_t operator()(cref<animated_glyph_t> glyph) noexcept { return hash_combine(glyph.color, glyph.frame, glyph.indices); }
		};
	};
} // namespace bleak
