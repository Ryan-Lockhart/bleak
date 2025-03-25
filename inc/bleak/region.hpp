#pragma once

#include <bleak/typedef.hpp>

#include <cstring>
#include <fstream>
#include <utility>

#include <bleak/extent.hpp>
#include <bleak/log.hpp>
#include <bleak/offset.hpp>
#include <bleak/renderer.hpp>
#include <bleak/zone.hpp>

namespace bleak {
	struct region_offset_t {
		offset_t zone;
		offset_t cell;
	};

	template<typename T, extent_t RegionSize, extent_t ZoneSize, extent_t ZoneBorder> struct region_t {
	  private:
		array_t<zone_t<T, ZoneSize, ZoneBorder>, RegionSize> zones;

	  public:
		using zone_type = zone_t<T, ZoneSize, ZoneBorder>;

		static constexpr extent_t region_size{ RegionSize };

		static constexpr offset_t region_origin{ 0 };
		static constexpr offset_t region_extent{ region_size - 1 };

		static constexpr auto region_area{ region_size.area() };

		static constexpr extent_t zone_size{ ZoneSize };

		static constexpr offset_t zone_origin(cref<offset_t> position) noexcept { return position * zone_size; }

		static constexpr offset_t zone_extent(cref<offset_t> position) noexcept { return zone_origin(position) + zone_size - 1; }

		static constexpr auto zone_area{ zone_size.area() };

		static constexpr extent_t size{ region_size * zone_size };

		static constexpr offset_t origin{ 0 };
		static constexpr offset_t extent{ size - 1 };

		static constexpr auto area{ size.area() };

		static constexpr usize byte_size{ region_area * zone_type::byte_size };

		constexpr region_t() : zones{} {};

		constexpr region_t(cref<std::string> path) noexcept : zones{} {
			str buffer = static_cast<str>(std::malloc(zone_type::byte_size));

			if (buffer == nullptr) {
				error_log.add("failed to allocate buffer for reading region file.", __TIME_FILE_LINE__);
				return;
			}

			std::ifstream file{};

			file.open(path, std::ios::in | std::ios::binary);

			for (extent_t::product_t i{ 0 }; i < region_area; ++i) {
				file.read(buffer, zone_type::byte_size);
				zones[i].deserialize(buffer);
			}

			file.close();

			std::free(buffer);
			buffer = nullptr;
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

		constexpr ref<zone_t<T, ZoneSize, ZoneBorder>> operator[](offset_t::product_t position) noexcept { return zones[position]; }

		constexpr cref<zone_t<T, ZoneSize, ZoneBorder>> operator[](offset_t::product_t position) const noexcept { return zones[position]; }

		constexpr ref<zone_t<T, ZoneSize, ZoneBorder>> operator[](cref<offset_t> position) noexcept { return zones[position]; }

		constexpr cref<zone_t<T, ZoneSize, ZoneBorder>> operator[](cref<offset_t> position) const noexcept { return zones[position]; }

		constexpr ref<zone_t<T, ZoneSize, ZoneBorder>> operator[](offset_t::product_t zone_position, offset_t::product_t cell_position) noexcept { return zones[zone_position][cell_position]; }

		constexpr cref<zone_t<T, ZoneSize, ZoneBorder>> operator[](offset_t::product_t zone_position, offset_t::product_t cell_position) const noexcept { return zones[zone_position][cell_position]; }

		constexpr ref<T> operator[](cref<offset_t> zone_position, cref<offset_t> cell_position) noexcept { return zones[zone_position][cell_position]; }

		constexpr cref<T> operator[](cref<offset_t> zone_position, cref<offset_t> cell_position) const noexcept { return zones[zone_position][cell_position]; }

		constexpr ref<T> operator[](cref<region_offset_t> position) noexcept { return zones[position.zone][position.cell]; }

		constexpr cref<T> operator[](cref<region_offset_t> position) const noexcept { return zones[position.zone][position.cell]; }

		constexpr zone_t<T, RegionSize * ZoneSize, ZoneBorder> compile() const noexcept {
			zone_t<T, RegionSize * ZoneSize, ZoneBorder> zone{};

			for (extent_t::scalar_t region_y{ 0 }; region_y < region_size.h; ++region_y) {
				for (extent_t::scalar_t region_x{ 0 }; region_x < region_size.w; ++region_x) {
					const offset_t region_pos{ region_x, region_y };
					for (extent_t::scalar_t zone_y{ 0 }; zone_y < zone_size.h; ++zone_y) {
						for (extent_t::scalar_t zone_x{ 0 }; zone_x < zone_size.w; ++zone_x) {
							const offset_t zone_pos{ zone_x, zone_y };
							zone[region_pos * zone_size + zone_pos] = zones[region_pos][zone_pos];
						}
					}
				}
			}

			return zone;
		}

		constexpr void compile(ref<zone_t<T, RegionSize * ZoneSize, ZoneBorder>> zone) const noexcept {
			for (extent_t::scalar_t region_y{ 0 }; region_y < region_size.h; ++region_y) {
				for (extent_t::scalar_t region_x{ 0 }; region_x < region_size.w; ++region_x) {
					const offset_t region_pos{ region_x, region_y };
					for (extent_t::scalar_t zone_y{ 0 }; zone_y < zone_size.h; ++zone_y) {
						for (extent_t::scalar_t zone_x{ 0 }; zone_x < zone_size.w; ++zone_x) {
							const offset_t zone_pos{ zone_x, zone_y };
							zone[region_pos * zone_size + zone_pos] = zones[region_pos][zone_pos];
						}
					}
				}
			}
		}

		template<extent_t AtlasSize>
			requires is_drawable<T>::value
		constexpr void draw(ref<renderer_t> renderer, cref<atlas_t<AtlasSize>> atlas) const noexcept {
			for (extent_t::scalar_t y{ 0 }; y < region_size.h; ++y) {
				for (extent_t::scalar_t x{ 0 }; x < region_size.w; ++x) {
					const offset_t pos{ x, y };
					(*this)[pos].draw(renderer, atlas, pos * region_size);
				}
			}
		}

		template<extent_t AtlasSize>
			requires is_drawable<T>::value
		constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<offset_t> offset) const noexcept {
			for (extent_t::scalar_t y{ 0 }; y < region_size.h; ++y) {
				for (extent_t::scalar_t x{ 0 }; x < region_size.w; ++x) {
					const offset_t pos{ x, y };
					(*this)[pos].draw(atlas, pos * zone_size + offset);
				}
			}
		}

		template<extent_t AtlasSize>
			requires is_drawable<T>::value
		constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<offset_t> offset, cref<extent_t> scale) const noexcept {
			for (extent_t::scalar_t y{ 0 }; y < region_size.h; ++y) {
				for (extent_t::scalar_t x{ 0 }; x < region_size.w; ++x) {
					const offset_t pos{ x, y };
					(*this)[pos].draw(atlas, pos * zone_size + offset, scale);
				}
			}
		}

		constexpr bool serialize(cref<std::string> path) const noexcept {
			std::ofstream file{};

			try {
				file.open(path, std::ios::in | std::ios::binary);

				for (extent_t::product_t i{ 0 }; i < region_area; ++i) {
					file.write(zones[i].serialize(), zone_type::byte_size);
				}

				file.close();
			} catch (std::exception e) {
				error_log.add(e.what(), __TIME_FILE_LINE__);
			}

			return true;
		}

		constexpr void deserialize(cstr buffer) noexcept {
			for (extent_t::product_t i{ 0 }; i < region_area; ++i) {
				zones[i].deserialize(buffer + i * zone_type::byte_size);
			}
		}
	};
} // namespace bleak
