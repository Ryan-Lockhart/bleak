#pragma once

#include "bleak/extent/extent_2d.hpp"
#include "bleak/typedef.hpp"

#include <utility>

#include "bleak/renderer.hpp"
#include "bleak/zone.hpp"

namespace bleak {
	struct region_offset_t {
		offset_2d_t zone;
		offset_2d_t cell;
	};

	template<typename T, extent_2d_t RegionSize, extent_2d_t ZoneSize, extent_2d_t ZoneBorder = extent_2d_t{ 0, 0 }> struct region_t {
	  private:
		layer_t<zone_t<T, ZoneSize, ZoneBorder>, RegionSize> zones;

	  public:
		static constexpr extent_2d_t region_size{ RegionSize };

		static constexpr offset_2d_t region_origin{ 0 };
		static constexpr offset_2d_t region_extent{ region_size - 1 };

		static constexpr auto region_area{ region_size.area() };

		static constexpr extent_2d_t zone_size{ ZoneSize };

		static constexpr offset_2d_t zone_origin(cref<offset_2d_t> position) noexcept { return position * zone_size; }

		static constexpr offset_2d_t zone_extent(cref<offset_2d_t> position) noexcept { return zone_origin(position) + zone_size - 1; }

		static constexpr auto zone_area{ zone_size.area() };

		static constexpr extent_2d_t size{ region_size * zone_size };

		static constexpr offset_2d_t origin{ 0 };
		static constexpr offset_2d_t extent{ size - 1 };

		static constexpr auto area{ size.area() };

		constexpr region_t() : zones{} {};

		constexpr region_t(cref<region_t> other) : zones{ other.zones } {}

		constexpr region_t(rval<region_t> other) noexcept : zones{ std::move(other.zones) } {}

		constexpr ref<region_t> operator=(cref<region_t> other) noexcept {
			if (this != &other) {
				zones = other.zones;
			}

			return *this;
		}

		constexpr ref<region_t> operator=(rval<region_t> other) noexcept {
			if (this != &other) {
				zones = std::move(other.zones);
			}

			return *this;
		}

		constexpr ~region_t() noexcept {};

		constexpr ref<zone_t<T, ZoneSize, ZoneBorder>> operator[](cref<offset_1d_t> position) noexcept { return zones[position]; }

		constexpr cref<zone_t<T, ZoneSize, ZoneBorder>> operator[](cref<offset_1d_t> position) const noexcept { return zones[position]; }

		constexpr ref<zone_t<T, ZoneSize, ZoneBorder>> operator[](cref<offset_2d_t> position) noexcept { return zones[position]; }

		constexpr cref<zone_t<T, ZoneSize, ZoneBorder>> operator[](cref<offset_2d_t> position) const noexcept { return zones[position]; }

		constexpr ref<T> operator[](cref<offset_2d_t> zone_position, cref<offset_2d_t> cell_position) noexcept { return zones[zone_position][cell_position]; }

		constexpr cref<T> operator[](cref<offset_2d_t> zone_position, cref<offset_2d_t> cell_position) const noexcept { return zones[zone_position][cell_position]; }

		constexpr ref<T> operator[](cref<region_offset_t> position) noexcept { return zones[position.zone][position.cell]; }

		constexpr cref<T> operator[](cref<region_offset_t> position) const noexcept { return zones[position.zone][position.cell]; }

		template<extent_2d_t AtlasSize>
		constexpr void draw(ref<renderer_t> renderer, cref<atlas_t<AtlasSize>> atlas)
			requires is_drawable<T>::value
		{
			for (extent_2d_t::scalar_t y{ 0 }; y < region_size.h; ++y) {
				for (extent_2d_t::scalar_t x{ 0 }; x < region_size.w; ++x) {
					const offset_2d_t pos{ x, y };
					(*this)[pos].draw(renderer, atlas, pos * region_size);
				}
			}
		}

		template<extent_2d_t AtlasSize>
		constexpr void draw(ref<renderer_t> renderer, cref<atlas_t<AtlasSize>> atlas, cref<offset_2d_t> offset)
			requires is_drawable<T>::value
		{
			for (extent_2d_t::scalar_t y{ 0 }; y < region_size.h; ++y) {
				for (extent_2d_t::scalar_t x{ 0 }; x < region_size.w; ++x) {
					const offset_2d_t pos{ x, y };
					(*this)[pos].draw(renderer, atlas, pos * zone_size + offset);
				}
			}
		}

		template<extent_2d_t AtlasSize>
		constexpr void draw(ref<renderer_t> renderer, cref<atlas_t<AtlasSize>> atlas, cref<offset_2d_t> offset, cref<extent_2d_t> scale)
			requires is_drawable<T>::value
		{
			for (extent_2d_t::scalar_t y{ 0 }; y < region_size.h; ++y) {
				for (extent_2d_t::scalar_t x{ 0 }; x < region_size.w; ++x) {
					const offset_2d_t pos{ x, y };
					(*this)[pos].draw(renderer, atlas, pos * zone_size + offset, scale);
				}
			}
		}
	};
} // namespace bleak
