#pragma once
#include <bleak/typedef.hpp>

#include <fstream>
#include <utility>

#include <bleak/extent.hpp>
#include <bleak/offset.hpp>
#include <bleak/renderer.hpp>
#include <bleak/zone.hpp>

namespace bleak {
	struct region_offset_t {
		offset_2d_t zone;
		offset_2d_t cell;
	};

	template<typename T, extent_2d_t RegionSize, extent_2d_t ZoneSize, extent_2d_t ZoneBorder = extent_2d_t{ 0, 0 }> struct region_t {
	  private:
		layer_t<zone_t<T, ZoneSize, ZoneBorder>, RegionSize> zones;

	  public:
		using zone_type = zone_t<T, ZoneSize, ZoneBorder>;

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

		static constexpr usize byte_size{ region_area * zone_type::byte_size };

		constexpr region_t() : zones{} {};

		constexpr region_t(cref<std::string> path) : zones{} {
			std::ifstream file{};
			str buffer;

			try {
				file.open(path, std::ios::in | std::ios::binary);

				buffer = static_cast<str>(std::malloc(zone_type::byte_size));

				for (extent_2d_t::product_t i{ 0 }; i < region_area; ++i) {
					const extent_1d_t index{ i };
					file.read(buffer, zone_type::byte_size);

					zones[index].deserialize(buffer);
				}

				std::free(buffer);
				buffer = nullptr;

				file.close();
			} catch (std::exception e) {
				error_log.add(e.what(), __TIME_FILE_LINE__);

				if (buffer != nullptr) {
					std::free(buffer);
					buffer = nullptr;
				}
			}
		}

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

		constexpr bool serialize(cref<std::string> path) const noexcept {
			std::ofstream file{};

			try {
				file.open(path, std::ios::in | std::ios::binary);

				for (extent_2d_t::product_t i{ 0 }; i < region_area; ++i) {
					file.write(zones[extent_1d_t{ i }].serialize(), zone_type::byte_size);
				}

				file.close();
			} catch (std::exception e) {
				error_log.add(e.what(), __TIME_FILE_LINE__);
			}

			return true;
		}
	};
} // namespace bleak
