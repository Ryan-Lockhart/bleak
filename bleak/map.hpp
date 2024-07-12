#pragma once

#include "bleak/typedef.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "bleak/array.hpp"
#include "bleak/atlas.hpp"
#include "bleak/cardinal.hpp"
#include "bleak/extent.hpp"
#include "bleak/log.hpp"
#include "bleak/offset.hpp"
#include "bleak/random.hpp"
#include "bleak/renderer.hpp"

namespace bleak {
	enum class map_region_t : u8 { None = 0, Interior = 1 << 0, Border = 1 << 1, All = Interior | Border };

	template<typename T> struct binary_applicator_t {
		const T true_value;
		const T false_value;
		
		constexpr binary_applicator_t() noexcept = delete;

		constexpr binary_applicator_t(cref<T> true_value, cref<T> false_value) noexcept : true_value{ true_value }, false_value{ false_value } {}

		constexpr binary_applicator_t(rval<T> true_value, rval<T> false_value) noexcept : true_value{ std::move(true_value) }, false_value{ std::move(false_value) } {}

		constexpr ref<binary_applicator_t<T>> operator=(cref<binary_applicator_t<T>> other) noexcept = delete;
		constexpr ref<binary_applicator_t<T>> operator=(rval<binary_applicator_t<T>> other) noexcept = delete;

		constexpr ~binary_applicator_t() noexcept {}

		constexpr T operator()(bool condition) const noexcept { return condition ? true_value : false_value; }

		template<typename Randomizer>
		constexpr T operator()(ref<Randomizer> generator, f64 probability) const noexcept
			requires is_random_engine<Randomizer>::value
		{
			return std::bernoulli_distribution{ probability }(generator) ? true_value : false_value;
		}

		template<typename Randomizer, typename Distribution>
		constexpr T operator()(ref<Randomizer> generator, ref<Distribution> distribution) const noexcept
			requires is_random_engine<Randomizer>::value && is_random_distribution<Distribution>::value
		{
			return distribution(generator) ? true_value : false_value;
		}
	};

	template<typename T, extent_2d_t MapSize, extent_2d_t BorderSize = { 0, 0 }> class map_t {

		static_assert(is_drawable<T>::value, "Map value type must be drawable.");

		static_assert(MapSize > extent_2d_t::zero, "Map size must be greater than zero.");
		static_assert(MapSize >= BorderSize, "Map size must be greater than or equal to border size.");

	  private:
		layer_t<T, MapSize> state{};
		layer_t<T, MapSize> buffer{};

	  public:
		static constexpr extent_2d_t map_size{ MapSize };
		static constexpr extent_2d_t border_size{ BorderSize };

		static constexpr offset_2d_t map_origin{ 0 };
		static constexpr offset_2d_t map_extent{ map_size - 1 };

		static constexpr offset_2d_t interior_origin{ map_origin + border_size };
		static constexpr offset_2d_t interior_extent{ map_extent - border_size };

		static constexpr auto map_area{ map_size.area() };

		static constexpr usize interior_area{ (map_size - border_size).area() };
		static constexpr usize border_area{ map_area - interior_area };

		inline map_t() {}

		inline map_t(cref<std::string> path) {
			std::ifstream file{};

			try {
				file.open(path, std::ios::in | std::ios::binary);

				file.read(reinterpret_cast<str>(state.data_ptr()), state.byte_size);

				file.close();
			} catch (std::exception e) {
				error_log.add(e.what(), __TIME_FILE_LINE__);
			}
		}

		inline map_t(cref<map_t<T, MapSize, BorderSize>> other) : state{ other.state }, buffer{} {}

		inline map_t(rval<map_t<T, MapSize, BorderSize>> other) : state{ std::move(other.state) }, buffer{} {}

		inline ref<map_t<T, MapSize, BorderSize>> operator=(cref<map_t<T, MapSize, BorderSize>> other) noexcept {
			if (this != &other) {
				state = other.state;
				buffer = {};
			}

			return *this;
		}

		inline ref<map_t<T, MapSize, BorderSize>> operator=(rval<map_t<T, MapSize, BorderSize>> other) noexcept {
			if (this != &other) {
				state = std::move(other.state);
				buffer = {};
			}

			return *this;
		}

		inline ~map_t() noexcept {}

		inline cref<layer_t<T, MapSize>> data() const noexcept { return state; }

		inline cptr<layer_t<T, MapSize>> data_ptr() const noexcept { return &state; }

		inline layer_t<cref<T>, MapSize> view() const {
			layer_t<cref<T>, MapSize> view{};

			for (extent_2d_t::product_t i{ 0 }; i < map_area; ++i) {
				view[i] = state[i];
			}

			return view;
		}

		inline layer_t<ref<T>, MapSize> proxy() {
			layer_t<ref<T>, MapSize> proxy{};

			for (extent_2d_t::product_t i{ 0 }; i < map_area; ++i) {
				proxy[i] = ref<T>(state[i]);
			}

			return proxy;
		}

		inline ref<T> operator[](extent_2d_t::product_t index) noexcept { return state[index]; }

		inline cref<T> operator[](extent_2d_t::product_t index) const noexcept { return state[index]; }

		inline ref<T> operator[](extent_2d_t::scalar_t x, extent_2d_t::scalar_t y) noexcept { return state[x, y]; }

		inline cref<T> operator[](extent_2d_t::scalar_t x, extent_2d_t::scalar_t y) const noexcept { return state[x, y]; }

		inline ref<T> operator[](cref<offset_2d_t> position) noexcept { return state[position]; }

		inline cref<T> operator[](cref<offset_2d_t> position) const noexcept { return state[position]; }

		inline bool on_x_edge(cref<offset_2d_t> position) const noexcept { return position.x == map_origin.x || position.x == map_extent.x; }

		inline bool on_y_edge(cref<offset_2d_t> position) const noexcept { return position.y == map_origin.y || position.y == map_extent.y; }

		inline bool on_edge(cref<offset_2d_t> position) const noexcept { return on_x_edge(position) || on_y_edge(position); }

		inline cardinal_t edge_state(cref<offset_2d_t> position) const noexcept {
			cardinal_t state{ cardinal_t::Central };

			if (!on_edge(position)) {
				return state;
			}

			if (position.x == map_origin.x) {
				state += cardinal_t::West;
			} else if (position.x == map_extent.x) {
				state += cardinal_t::East;
			}

			if (position.y == map_origin.y) {
				state += cardinal_t::North;
			} else if (position.y == map_extent.y) {
				state += cardinal_t::South;
			}

			return state;
		}

		template<map_region_t Region> inline bool within(cref<offset_2d_t> position) const noexcept {
			if constexpr (Region == map_region_t::All) {
				return state.valid(position);
			} else if constexpr (Region == map_region_t::Interior) {
				return position.x >= interior_origin.x && position.x <= interior_extent.x && position.y >= interior_origin.y && position.y <= interior_extent.y;
			} else if constexpr (Region == map_region_t::Border) {
				return position.x < interior_origin.x || position.x > interior_extent.x || position.y < interior_origin.y || position.y > interior_extent.y;
			}

			return false;
		}

		template<map_region_t Region> inline ref<map_t<T, MapSize, BorderSize>> set(cref<T> value) noexcept {
			if constexpr (Region == map_region_t::All) {
				for (extent_2d_t::product_t i{ 0 }; i < map_area; ++i) {
					state[i] = value;
				}
			} else if constexpr (Region == map_region_t::Interior) {
				for (extent_2d_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_2d_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						state[x, y] = value;
					}
				}
			} else if constexpr (Region == map_region_t::Border) {
				for (extent_2d_t::scalar_t y{ 0 }; y < map_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_2d_t::scalar_t x{ 0 }; x < map_size.w; ++x) {
							state[x, y] = value;
						}
					} else {
						for (extent_2d_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							state[i, y] = value;
							state[map_extent.x - i, y] = value;
						}
					}
				}
			}

			return *this;
		}

		inline void swap_buffers() noexcept { std::swap(state, buffer); }

		template<bool Buffer = true> inline void synchronize() noexcept {
			for (extent_2d_t::product_t i{ 0 }; i < map_area; ++i) {
				if constexpr (Buffer) {
					buffer[i] = state[i];
				} else {
					state[i] = buffer[i];
				}
			}
		}

		template<map_region_t Region, typename Randomizer>
		inline ref<map_t<T, MapSize, BorderSize>> randomize(ref<Randomizer> generator, f64 fill_percent, cref<T> true_value, cref<T> false_value) noexcept
			requires is_random_engine<Randomizer>::value
		{
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			auto dis{ std::bernoulli_distribution{ fill_percent } };

			if constexpr (Region == map_region_t::All) {
				for (extent_2d_t::product_t i{ 0 }; i < map_area; ++i) {
					state[i] = dis(generator) ? true_value : false_value;
				}
			} else if constexpr (Region == map_region_t::Interior) {
				for (extent_2d_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_2d_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						state[x, y] = dis(generator) ? true_value : false_value;
					}
				}
			} else if constexpr (Region == map_region_t::Border) {
				for (extent_2d_t::scalar_t y{ 0 }; y < map_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_2d_t::scalar_t x{ 0 }; x < map_size.w; ++x) {
							state[x, y] = dis(generator) ? true_value : false_value;
						}
					} else {
						for (extent_2d_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							state[i, y] = dis(generator) ? true_value : false_value;
							state[map_extent.x - i, y] = dis(generator) ? true_value : false_value;
						}
					}
				}
			}

			return *this;
		}

		template<map_region_t Region, typename Randomizer>
		inline ref<map_t<T, MapSize, BorderSize>> randomize(ref<Randomizer> generator, f64 fill_percent, cref<binary_applicator_t<T>> applicator) noexcept
			requires is_random_engine<Randomizer>::value
		{
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			auto dis{ std::bernoulli_distribution{ fill_percent } };

			if constexpr (Region == map_region_t::All) {
				for (extent_2d_t::product_t i{ 0 }; i < map_area; ++i) {
					state[i] = applicator(generator, dis);
				}
			} else if constexpr (Region == map_region_t::Interior) {
				for (extent_2d_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_2d_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						state[x, y] = applicator(generator, dis);
					}
				}
			} else if constexpr (Region == map_region_t::Border) {
				for (extent_2d_t::scalar_t y{ 0 }; y < map_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_2d_t::scalar_t x{ 0 }; x < map_size.w; ++x) {
							state[x, y] = applicator(generator, dis);
						}
					} else {
						for (extent_2d_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							state[i, y] = applicator(generator, dis);
							state[map_extent.x - i, y] = applicator(generator, dis);
						}
					}
				}
			}

			return *this;
		}

		template<bool Safe = true> inline u8 neighbour_count(cref<offset_2d_t> position, cref<T> value) const noexcept {
			u8 count{ 0 };

			if constexpr (Safe) {
				cardinal_t edge{ edge_state(position) };

				if (edge == cardinal_t::Northwest || state[position + offset_2d_t::northwest] == value) {
					++count;
				}

				if (edge == cardinal_t::North || state[position + offset_2d_t::north] == value) {
					++count;
				}

				if (edge == cardinal_t::Northeast || state[position + offset_2d_t::northeast] == value) {
					++count;
				}

				if (edge == cardinal_t::West || state[position + offset_2d_t::west] == value) {
					++count;
				}

				if (edge == cardinal_t::East || state[position + offset_2d_t::east] == value) {
					++count;
				}

				if (edge == cardinal_t::Southwest || state[position + offset_2d_t::southwest] == value) {
					++count;
				}

				if (edge == cardinal_t::South || state[position + offset_2d_t::south] == value) {
					++count;
				}

				if (edge == cardinal_t::Southeast || state[position + offset_2d_t::southeast] == value) {
					++count;
				}
			} else {
				if (state[position + offset_2d_t::northwest] == value) {
					++count;
				}
				if (state[position + offset_2d_t::north] == value) {
					++count;
				}
				if (state[position + offset_2d_t::northeast] == value) {
					++count;
				}
				if (state[position + offset_2d_t::west] == value) {
					++count;
				}
				if (state[position + offset_2d_t::east] == value) {
					++count;
				}
				if (state[position + offset_2d_t::southwest] == value) {
					++count;
				}
				if (state[position + offset_2d_t::south] == value) {
					++count;
				}
				if (state[position + offset_2d_t::southeast] == value) {
					++count;
				}
			}

			return count;
		}

		template<bool Safe = true> inline void modulate(cref<offset_2d_t> position, u8 threshold, cref<T> true_state, cref<T> false_state) noexcept {
			u8 neighbours{ neighbour_count<Safe>(position, true_state) };

			if (neighbours > threshold) {
				buffer[position] = true_state;
			} else if (neighbours < threshold) {
				buffer[position] = false_state;
			}
		}

		template<bool Safe = true> inline void modulate(cref<offset_2d_t> position, u8 threshold, cref<binary_applicator_t<T>> applicator) noexcept {
			u8 neighbours{ neighbour_count<Safe>(position, applicator.true_value) };

			if (neighbours > threshold) {
				buffer[position] = applicator.true_value;
			} else if (neighbours < threshold) {
				buffer[position] = applicator.false_value;
			}
		}

		template<map_region_t Region> inline ref<map_t<T, MapSize, BorderSize>> automatize(u8 threshold, cref<T> true_value, cref<T> false_state) noexcept {
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			if constexpr (Region == map_region_t::All) {
				for (extent_2d_t::scalar_t y{ map_origin.y }; y <= map_extent.y; ++y) {
					for (extent_2d_t::scalar_t x{ map_origin.x }; x <= map_extent.x; ++x) {
						modulate({ x, y }, threshold, true_value, false_state);
					}
				}
			} else if constexpr (Region == map_region_t::Interior) {
				constexpr bool is_not_safe{ border_size.w > 0 && border_size.h > 0 };

				for (extent_2d_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_2d_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						modulate<is_not_safe>({ x, y }, threshold, true_value, false_state);
					}
				}
			} else if constexpr (Region == map_region_t::Border) {
				for (extent_2d_t::scalar_t y{ 0 }; y < map_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_2d_t::scalar_t x{ 0 }; x < map_size.w; ++x) {
							modulate({ x, y }, threshold, true_value, false_state);
						}
					} else {
						for (extent_2d_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							modulate({ i, y }, threshold, true_value, false_state);
							modulate({ map_extent.x - i, y }, threshold, true_value, false_state);
						}
					}
				}
			}

			return *this;
		}

		template<map_region_t Region> inline ref<map_t<T, MapSize, BorderSize>> automatize(u8 threshold, cref<binary_applicator_t<T>> applicator) noexcept {
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			if constexpr (Region == map_region_t::All) {
				for (extent_2d_t::scalar_t y{ map_origin.y }; y <= map_extent.y; ++y) {
					for (extent_2d_t::scalar_t x{ map_origin.x }; x <= map_extent.x; ++x) {
						modulate({ x, y }, threshold, applicator);
					}
				}
			} else if constexpr (Region == map_region_t::Interior) {
				constexpr bool is_not_safe{ border_size.w > 0 && border_size.h > 0 };

				for (extent_2d_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_2d_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						modulate<is_not_safe>({ x, y }, threshold, applicator);
					}
				}
			} else if constexpr (Region == map_region_t::Border) {
				for (extent_2d_t::scalar_t y{ 0 }; y < map_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_2d_t::scalar_t x{ 0 }; x < map_size.w; ++x) {
							modulate({ x, y }, threshold, applicator);
						}
					} else {
						for (extent_2d_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							modulate({ i, y }, threshold, applicator);
							modulate({ map_extent.x - i, y }, threshold, applicator);
						}
					}
				}
			}

			return *this;
		}

		template<map_region_t Region> inline ref<map_t<T, MapSize, BorderSize>> automatize(u32 iterations, u8 threshold, cref<T> true_value, cref<T> false_state) noexcept {
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			for (u32 i{ 0 }; i < iterations; ++i) {
				automatize<Region>(threshold, true_value, false_state);
				swap_buffers();
			}

			return *this;
		}

		template<map_region_t Region> inline ref<map_t<T, MapSize, BorderSize>> automatize(u32 iterations, u8 threshold, cref<binary_applicator_t<T>> applicator) noexcept {
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			for (u32 i{ 0 }; i < iterations; ++i) {
				automatize<Region>(threshold, applicator);
				swap_buffers();
			}

			return *this;
		}

		template<map_region_t Region, typename Randomizer>
		inline ref<map_t<T, MapSize, BorderSize>> generate(ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<T> true_value, cref<T> false_state) noexcept
			requires is_random_engine<Randomizer>::value
		{
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, true_value, false_state);
			synchronize();

			automatize<Region>(iterations, threshold, true_value, false_state);
			swap_buffers();

			return *this;
		}

		template<map_region_t Region, typename Randomizer>
		inline ref<map_t<T, MapSize, BorderSize>> generate(ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<binary_applicator_t<T>> applicator) noexcept
			requires is_random_engine<Randomizer>::value
		{
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, applicator);
			synchronize();

			automatize<Region>(iterations, threshold, applicator);
			swap_buffers();

			return *this;
		}

		template<map_region_t Region, typename Randomizer>
		inline std::optional<offset_2d_t> find_random(ref<Randomizer> generator, cref<T> value) const noexcept
			requires is_random_engine<Randomizer>::value
		{
			if constexpr (Region == map_region_t::None) {
				return *this;
			}

			if constexpr (Region == map_region_t::All) {
				std::uniform_int_distribution<offset_2d_t::scalar_t> x_dis{ 0, map_extent.x };
				std::uniform_int_distribution<offset_2d_t::scalar_t> y_dis{ 0, map_extent.y };

				for (extent_2d_t::product_t i{ 0 }; i < map_area; ++i) {
					const offset_2d_t pos{ x_dis(generator), y_dis(generator) };

					if (state[pos] == value) {
						return pos;
					}
				}

				return std::nullopt;
			} else if constexpr (Region == map_region_t::Interior) {
				std::uniform_int_distribution<offset_2d_t::scalar_t> x_dis{ interior_origin.x, interior_extent.x };
				std::uniform_int_distribution<offset_2d_t::scalar_t> y_dis{ interior_origin.y, interior_extent.y };

				for (extent_2d_t::product_t i{ 0 }; i < interior_area; ++i) {
					const offset_2d_t pos{ x_dis(generator), y_dis(generator) };

					if (state[pos] == value) {
						return pos;
					}
				}

				return std::nullopt;
			} else if constexpr (Region == map_region_t::Border) {
				std::uniform_int_distribution<offset_2d_t::scalar_t> x_dis{ 0, border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_2d_t::scalar_t> y_dis{ 0, border_size.h * 2 - 1 };

				for (extent_2d_t::product_t i{ 0 }; i < border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < border_size.w ? x : map_extent.x - x;
					y = y < border_size.h ? y : map_extent.y - y;

					const offset_2d_t pos{ x, y };

					if (state[pos] == value) {
						return pos;
					}
				}

				return std::nullopt;
			}

			return std::nullopt;
		}

		template<map_region_t Region, typename Randomizer, typename U>
		inline std::optional<offset_2d_t> find_random(ref<Randomizer> generator, cref<U> value) const noexcept
			requires is_random_engine<Randomizer>::value && is_equatable<T, U>::value
		{
			if constexpr (Region == map_region_t::All) {
				std::uniform_int_distribution<offset_2d_t::scalar_t> x_dis{ 0, map_extent.x };
				std::uniform_int_distribution<offset_2d_t::scalar_t> y_dis{ 0, map_extent.y };

				for (extent_2d_t::product_t i{ 0 }; i < map_area; ++i) {
					const offset_2d_t pos{ x_dis(generator), y_dis(generator) };

					if (state[pos] == value) {
						return pos;
					}
				}

				return std::nullopt;
			} else if constexpr (Region == map_region_t::Interior) {
				std::uniform_int_distribution<offset_2d_t::scalar_t> x_dis{ interior_origin.x, interior_extent.x };
				std::uniform_int_distribution<offset_2d_t::scalar_t> y_dis{ interior_origin.y, interior_extent.y };

				for (extent_2d_t::product_t i{ 0 }; i < interior_area; ++i) {
					const offset_2d_t pos{ x_dis(generator), y_dis(generator) };

					if (state[pos] == value) {
						return pos;
					}
				}

				return std::nullopt;
			} else if constexpr (Region == map_region_t::Border) {
				std::uniform_int_distribution<offset_2d_t::scalar_t> x_dis{ 0, border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_2d_t::scalar_t> y_dis{ 0, border_size.h * 2 - 1 };

				for (extent_2d_t::product_t i{ 0 }; i < border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < border_size.w ? x : map_extent.x - x;
					y = y < border_size.h ? y : map_extent.y - y;

					const offset_2d_t pos{ x, y };

					if (state[pos] == value) {
						return pos;
					}
				}

				return std::nullopt;
			}

			return std::nullopt;
		}

		template<extent_2d_t AtlasSize> inline void draw(ref<renderer_t> renderer, cref<atlas_t<AtlasSize>> atlas) const {
			for (extent_2d_t::scalar_t y{ 0 }; y < map_size.h; ++y) {
				for (extent_2d_t::scalar_t x{ 0 }; x < map_size.w; ++x) {
					const offset_2d_t pos{ x, y };
					(*this)[pos].draw(renderer, atlas, pos);
				}
			}
		}

		template<extent_2d_t AtlasSize> inline void draw(ref<renderer_t> renderer, cref<atlas_t<AtlasSize>> atlas, cref<offset_2d_t> offset) const {
			for (extent_2d_t::scalar_t y{ 0 }; y < map_size.h; ++y) {
				for (extent_2d_t::scalar_t x{ 0 }; x < map_size.w; ++x) {
					const offset_2d_t pos{ x, y };
					(*this)[pos].draw(renderer, atlas, pos + offset);
				}
			}
		}

		inline bool serialize(cref<std::string> path) const noexcept {
			std::ofstream file{};

			try {
				file.open(path, std::ios::out | std::ios::binary);

				file.write(reinterpret_cast<cstr>(state.data_ptr()), state.byte_size);

				file.close();
			} catch (std::exception e) {
				error_log.add(e.what(), __TIME_FILE_LINE__);
				return false;
			}

			return true;
		}
	};
} // namespace bleak
