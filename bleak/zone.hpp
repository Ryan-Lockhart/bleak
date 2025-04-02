#pragma once

#include <bleak/typedef.hpp>

#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <bleak/applicator.hpp>
#include <bleak/array.hpp>
#include <bleak/atlas.hpp>
#include <bleak/camera.hpp>
#include <bleak/cardinal.hpp>
#include <bleak/concepts.hpp>
#include <bleak/creeper.hpp>
#include <bleak/extent.hpp>
#include <bleak/log.hpp>
#include <bleak/octant.hpp>
#include <bleak/offset.hpp>
#include <bleak/primitive.hpp>
#include <bleak/random.hpp>
#include <bleak/renderer.hpp>

#include <bleak/constants/numeric.hpp>

namespace bleak {
	template<typename T, extent_t RegionSize, extent_t ZoneSize, extent_t ZoneBorder = extent_t::Zero> struct region_t;

	enum class zone_region_t : u8 { None = 0, Interior = 1 << 0, Border = 1 << 1, All = Interior | Border };

	enum class neighbourhood_solver_t : u8 { Moore, VonNeumann, Extended, MarchingSquares, Melded };

	template<typename T, extent_t Size, extent_t BorderSize = extent_t{ 0, 0 }> class zone_t {
		static_assert(Size > extent_t::Zero, "Map size must be greater than zero.");
		static_assert(Size >= BorderSize, "Map size must be greater than or equal to border size.");

	  private:
		array_t<T, Size> cells;

	  public:
		static constexpr extent_t zone_size{ Size };
		static constexpr extent_t border_size{ BorderSize };

		static constexpr offset_t zone_origin{ 0 };
		static constexpr offset_t zone_extent{ zone_size - 1 };

		static constexpr offset_t interior_origin{ zone_origin + border_size };
		static constexpr offset_t interior_extent{ zone_extent - border_size };

		static constexpr auto zone_area{ zone_size.area() };

		static constexpr extent_t::product_t interior_area{ (zone_size - border_size).area() };
		static constexpr extent_t::product_t border_area{ zone_area - interior_area };

		static constexpr usize byte_size{ zone_area * sizeof(T) };

		static constexpr bool interior_safe{ border_size.w > 0 && border_size.h > 0 };

		constexpr zone_t() : cells{} {}

		constexpr zone_t(cref<std::string> path) : cells{} {
			std::ifstream file{};

			try {
				file.open(path, std::ios::in | std::ios::binary);

				file.seekg(0, std::ios::end);

				if (file.tellg() != byte_size) {
					throw std::runtime_error{ "byte size mismatch between file and map!" };
				}

				file.seekg(0, std::ios::beg);

				file.read(reinterpret_cast<str>(cells.data_ptr()), cells.byte_size);

				file.close();
			} catch (cref<std::exception> e) {
				error_log.add(e.what(), __TIME_FILE_LINE__);
			}
		}

		constexpr zone_t(cref<zone_t<T, Size, BorderSize>> other) : cells{ other.cells } {};

		constexpr zone_t(rval<zone_t<T, Size, BorderSize>> other) : cells{ std::move(other.cells) } {}

		constexpr ref<zone_t<T, Size, BorderSize>> operator=(cref<zone_t<T, Size, BorderSize>> other) noexcept {
			if (this != &other) {
				cells = other.cells;
			}

			return *this;
		}

		constexpr ref<zone_t<T, Size, BorderSize>> operator=(rval<zone_t<T, Size, BorderSize>> other) noexcept {
			if (this != &other) {
				cells = std::move(other.cells);
			}

			return *this;
		}

		constexpr ~zone_t() noexcept {}

		constexpr cref<array_t<T, Size>> data() const noexcept { return cells; }

		constexpr cptr<array_t<T, Size>> data_ptr() const noexcept { return &cells; }

		constexpr array_t<cref<T>, Size> view() const {
			array_t<cref<T>, Size> view{};

			for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
				view[i] = cells[i];
			}

			return view;
		}

		constexpr array_t<ref<T>, Size> proxy() noexcept {
			array_t<ref<T>, Size> proxy{};

			for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
				proxy[i] = ref<T>(cells[i]);
			}

			return proxy;
		}

		constexpr array_t<cref<T>, Size> proxy() const noexcept {
			array_t<ref<T>, Size> proxy{};

			for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
				proxy[i] = ref<T>(cells[i]);
			}

			return proxy;
		}

		constexpr ref<T> operator[](extent_t::product_t index) noexcept { return cells[index]; }

		constexpr cref<T> operator[](extent_t::product_t index) const noexcept { return cells[index]; }

		constexpr ref<T> operator[](extent_t::scalar_t x, extent_t::scalar_t y) noexcept { return cells[x, y]; }

		constexpr cref<T> operator[](extent_t::scalar_t x, extent_t::scalar_t y) const noexcept { return cells[x, y]; }

		constexpr ref<T> operator[](cref<offset_t> position) noexcept { return cells[position]; }

		constexpr cref<T> operator[](cref<offset_t> position) const noexcept { return cells[position]; }

		constexpr bool on_x_edge(cref<offset_t> position) const noexcept { return position.x == zone_origin.x || position.x == zone_extent.x; }

		constexpr bool on_y_edge(cref<offset_t> position) const noexcept { return position.y == zone_origin.y || position.y == zone_extent.y; }

		constexpr bool on_edge(cref<offset_t> position) const noexcept { return on_x_edge(position) || on_y_edge(position); }

		constexpr cardinal_t edge_state(cref<offset_t> position) const noexcept {
			cardinal_t state{ cardinal_e::Central };

			if (!on_edge(position)) {
				return state;
			}

			if (position.x == zone_origin.x) {
				state += cardinal_e::West;
			} else if (position.x == zone_extent.x) {
				state += cardinal_e::East;
			}

			if (position.y == zone_origin.y) {
				state += cardinal_e::North;
			} else if (position.y == zone_extent.y) {
				state += cardinal_e::South;
			}

			return state;
		}

		template<zone_region_t Region> constexpr bool within(cref<offset_t> position) const noexcept {
			if constexpr (Region == zone_region_t::All) {
				return cells.valid(position);
			} else if constexpr (Region == zone_region_t::Interior) {
				return position.x >= interior_origin.x && position.x <= interior_extent.x && position.y >= interior_origin.y && position.y <= interior_extent.y;
			} else if constexpr (Region == zone_region_t::Border) {
				return position.x < interior_origin.x || position.x > interior_extent.x || position.y < interior_origin.y || position.y > interior_extent.y;
			}

			return false;
		}

		template<zone_region_t Region> constexpr ref<zone_t<T, Size, BorderSize>> set(cref<T> value) noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] = value;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] = value;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] = value;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] = value;
							cells[zone_extent.x - i, y] = value;
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> set(cref<U> value) noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] = value;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] = value;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] = value;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] = value;
							cells[zone_extent.x - i, y] = value;
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region> constexpr ref<zone_t<T, Size, BorderSize>> reset() noexcept {
			set<Region>(T{});

			return *this;
		}

		template<zone_region_t Region>
			requires is_operable_unary<T, operator_t::Addition>::value
		constexpr ref<zone_t<T, Size, BorderSize>> apply(cref<T> value) noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] += value;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] += value;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] += value;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] += value;
							cells[zone_extent.x - i, y] += value;
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires is_operable<T, U, operator_t::Addition>::value
		constexpr ref<zone_t<T, Size, BorderSize>> apply(cref<U> value) noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					assert(i < zone_area);
					cells[i] += value;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] += value;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] += value;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] += value;
							cells[zone_extent.x - i, y] += value;
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename... Params>
			requires(is_operable<T, Params, operator_t::Addition>::value, ...) && is_plurary<Params...>::value
		constexpr ref<zone_t<T, Size, BorderSize>> apply(cref<Params>... values) noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					for (auto value : { values... }) {
						cells[i] += value;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						for (auto value : { values... }) {
							cells[x, y] += value;
						}
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							for (auto value : { values... }) {
								cells[x, y] += value;
							}
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							for (auto value : { values... }) {
								cells[i, y] += value;
								cells[zone_extent.x - i, y] += value;
							}
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region>
			requires is_operable_unary<T, operator_t::Subtraction>::value
		constexpr ref<zone_t<T, Size, BorderSize>> repeal(cref<T> value) noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] -= value;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] -= value;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] -= value;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] -= value;
							cells[zone_extent.x - i, y] -= value;
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires is_operable<T, U, operator_t::Subtraction>::value
		constexpr ref<zone_t<T, Size, BorderSize>> repeal(cref<U> value) noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] -= value;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] -= value;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] -= value;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] -= value;
							cells[zone_extent.x - i, y] -= value;
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename... Params>
			requires(is_operable<T, Params, operator_t::Subtraction>::value, ...) && is_plurary<Params...>::value
		constexpr ref<zone_t<T, Size, BorderSize>> repeal(cref<Params>... values) noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					for (auto value : { values... }) {
						cells[i] -= value;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						for (auto value : { values... }) {
							cells[x, y] -= value;
						}
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							for (auto value : { values... }) {
								cells[x, y] -= value;
							}
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							for (auto value : { values... }) {
								cells[i, y] -= value;
								cells[zone_extent.x - i, y] -= value;
							}
						}
					}
				}
			}

			return *this;
		}

		constexpr void swap(ref<array_t<T, Size>> buffer) noexcept { std::swap(cells, buffer); }

		constexpr void sync(cref<array_t<T, Size>> buffer) noexcept {
			for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
				cells[i] = buffer[i];
			}
		}

		template<typename U>
			requires std::is_assignable<T, U>::value
		constexpr void sync(cref<array_t<U, Size>> buffer) noexcept {
			for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
				cells[i] = buffer[i];
			}
		}

		template<zone_region_t Region, typename U, typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr ref<zone_t<T, Size, BorderSize>> randomize(ref<Randomizer> generator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] = T::template randomizer<Randomizer>::template operator()<U>(generator);
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] = T::template randomizer<Randomizer>::template operator()<U>(generator);
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] = T::template randomizer<Randomizer>::template operator()<U>(generator);
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] = T::template randomizer<Randomizer>::template operator()<U>(generator);
							cells[zone_extent.x - i, y] = T::template randomizer<Randomizer>::template operator()<U>(generator);
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr ref<zone_t<T, Size, BorderSize>> randomize(ref<Randomizer> generator, f64 fill_percent, cref<T> true_value, cref<T> false_value) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			auto dis{ std::bernoulli_distribution{ fill_percent } };

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] = dis(generator) ? true_value : false_value;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] = dis(generator) ? true_value : false_value;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] = dis(generator) ? true_value : false_value;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] = dis(generator) ? true_value : false_value;
							cells[zone_extent.x - i, y] = dis(generator) ? true_value : false_value;
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename Randomizer, typename U>
			requires is_random_engine<Randomizer>::value && std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> randomize(ref<Randomizer> generator, f64 fill_percent, cref<U> true_value, cref<U> false_value) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			auto dis{ std::bernoulli_distribution{ fill_percent } };

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] = dis(generator) ? true_value : false_value;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] = dis(generator) ? true_value : false_value;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] = dis(generator) ? true_value : false_value;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] = dis(generator) ? true_value : false_value;
							cells[zone_extent.x - i, y] = dis(generator) ? true_value : false_value;
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr ref<zone_t<T, Size, BorderSize>> randomize(ref<Randomizer> generator, f64 fill_percent, cref<binary_applicator_t<T>> applicator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			auto dis{ std::bernoulli_distribution{ fill_percent } };

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] = applicator(generator, dis);
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] = applicator(generator, dis);
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] = applicator(generator, dis);
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] = applicator(generator, dis);
							cells[zone_extent.x - i, y] = applicator(generator, dis);
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename Randomizer, typename U>
			requires is_random_engine<Randomizer>::value && std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> randomize(ref<Randomizer> generator, f64 fill_percent, cref<binary_applicator_t<U>> applicator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			auto dis{ std::bernoulli_distribution{ fill_percent } };

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					cells[i] = applicator(generator, dis);
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						cells[x, y] = applicator(generator, dis);
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							cells[x, y] = applicator(generator, dis);
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							cells[i, y] = applicator(generator, dis);
							cells[zone_extent.x - i, y] = applicator(generator, dis);
						}
					}
				}
			}

			return *this;
		}

		template<bool Safe = false> constexpr u8 neighbour_count(cref<offset_t> position, cref<T> value) const noexcept {
			u8 count{ 0 };

			if constexpr (Safe) {
				if (cells[position + offset_t::Northwest] == value) {
					++count;
				}
				if (cells[position + offset_t::North] == value) {
					++count;
				}
				if (cells[position + offset_t::Northeast] == value) {
					++count;
				}
				if (cells[position + offset_t::West] == value) {
					++count;
				}
				if (cells[position + offset_t::East] == value) {
					++count;
				}
				if (cells[position + offset_t::Southwest] == value) {
					++count;
				}
				if (cells[position + offset_t::South] == value) {
					++count;
				}
				if (cells[position + offset_t::Southeast] == value) {
					++count;
				}
			} else {
				cardinal_t edge{ edge_state(position) };

				if (edge.north || edge.west || cells[position + offset_t::Northwest] == value) {
					++count;
				}

				if (edge.north || cells[position + offset_t::North] == value) {
					++count;
				}

				if (edge.north || edge.east || cells[position + offset_t::Northeast] == value) {
					++count;
				}

				if (edge.west || cells[position + offset_t::West] == value) {
					++count;
				}

				if (edge.east || cells[position + offset_t::East] == value) {
					++count;
				}

				if (edge.south || edge.west || cells[position + offset_t::Southwest] == value) {
					++count;
				}

				if (edge.south || cells[position + offset_t::South] == value) {
					++count;
				}

				if (edge.south || edge.east || cells[position + offset_t::Southeast] == value) {
					++count;
				}
			}

			return count;
		}

		template<bool Safe = false, typename U>
			requires is_equatable<T, U>::value
		constexpr u8 neighbour_count(cref<offset_t> position, cref<U> value) const noexcept {
			u8 count{ 0 };

			if constexpr (Safe) {
				if (cells[position + offset_t::Northwest] == value) {
					++count;
				}
				if (cells[position + offset_t::North] == value) {
					++count;
				}
				if (cells[position + offset_t::Northeast] == value) {
					++count;
				}
				if (cells[position + offset_t::West] == value) {
					++count;
				}
				if (cells[position + offset_t::East] == value) {
					++count;
				}
				if (cells[position + offset_t::Southwest] == value) {
					++count;
				}
				if (cells[position + offset_t::South] == value) {
					++count;
				}
				if (cells[position + offset_t::Southeast] == value) {
					++count;
				}
			} else {
				cardinal_t edge{ edge_state(position) };

				if (edge.north || edge.west || cells[position + offset_t::Northwest] == value) {
					++count;
				}

				if (edge.north || cells[position + offset_t::North] == value) {
					++count;
				}

				if (edge.north || edge.east || cells[position + offset_t::Northeast] == value) {
					++count;
				}

				if (edge.west || cells[position + offset_t::West] == value) {
					++count;
				}

				if (edge.east || cells[position + offset_t::East] == value) {
					++count;
				}

				if (edge.south || edge.west || cells[position + offset_t::Southwest] == value) {
					++count;
				}

				if (edge.south || cells[position + offset_t::South] == value) {
					++count;
				}

				if (edge.south || edge.east || cells[position + offset_t::Southeast] == value) {
					++count;
				}
			}

			return count;
		}

		template<neighbourhood_solver_t Solver, bool Safe = false> constexpr u8 calculate_index(cref<offset_t> position, cref<T> value) const noexcept {
			u8 index{ 0 };

			if constexpr (Solver == neighbourhood_solver_t::Melded) {
				if constexpr (Safe) {
					const bool nw{ cells[position + offset_t::Northwest] == value };
					const bool n{ cells[position + offset_t::North] == value };
					const bool ne{ cells[position + offset_t::Northeast] == value };

					const bool w{ cells[position + offset_t::West] == value };
					const bool e{ cells[position + offset_t::East] == value };

					const bool sw{ cells[position + offset_t::Southwest] == value };
					const bool s{ cells[position + offset_t::South] == value };
					const bool se{ cells[position + offset_t::Southeast] == value };

					if (nw && n && w) {
						index += 1 << 3;
					}

					if (n && ne && e) {
						index += 1 << 2;
					}

					if (e && se && s) {
						index += 1 << 1;
					}

					if (w && sw && s) {
						index += 1 << 0;
					}
				} else {
					if (!within<zone_region_t::All>(position)) {
						return (1 << 4) - 1;
					}

					cardinal_t edge{ edge_state(position) };

					const bool nw{ edge.north || edge.west || cells[position + offset_t::Northwest] == value };
					const bool n{ edge.north || cells[position + offset_t::North] == value };
					const bool ne{ edge.north || edge.east || cells[position + offset_t::Northeast] == value };

					const bool w{ edge.west || cells[position + offset_t::West] == value };
					const bool e{ edge.east || cells[position + offset_t::East] == value };

					const bool sw{ edge.south || edge.west || cells[position + offset_t::Southwest] == value };
					const bool s{ edge.south || cells[position + offset_t::South] == value };
					const bool se{ edge.south || edge.east || cells[position + offset_t::Southeast] == value };

					if (nw && n && w) {
						index += 1 << 3;
					}

					if (n && ne && e) {
						index += 1 << 2;
					}

					if (e && se && s) {
						index += 1 << 1;
					}

					if (w && sw && s) {
						index += 1 << 0;
					}
				}
			} else if constexpr (Solver == neighbourhood_solver_t::MarchingSquares) {
				if constexpr (Safe) {
					if (cells[position] == value) {
						index += 1 << 3;
					}
					if (cells[position + offset_t::East] == value) {
						index += 1 << 2;
					}
					if (cells[position + offset_t::Southeast] == value) {
						index += 1 << 1;
					}
					if (cells[position + offset_t::South] == value) {
						index += 1 << 0;
					}
				} else {
					if (!within<zone_region_t::All>(position)) {
						return (1 << 4) - 1;
					}

					cardinal_t edge{ edge_state(position) };

					if (cells[position] == value) {
						index += 1 << 3;
					}

					if (edge == cardinal_e::East || cells[position + offset_t::East] == value) {
						index += 1 << 2;
					}

					if (edge == cardinal_e::Southeast || cells[position + offset_t::Southeast] == value) {
						index += 1 << 1;
					}

					if (edge == cardinal_e::South || cells[position + offset_t::South] == value) {
						index += 1 << 0;
					}
				}
			}

			return index;
		}

		template<neighbourhood_solver_t Solver, bool Safe = false, typename U>
			requires is_equatable<T, U>::value
		constexpr u8 calculate_index(cref<offset_t> position, cref<U> value) const noexcept {
			u8 index{ 0 };

			if constexpr (Solver == neighbourhood_solver_t::Melded) {
				if constexpr (Safe) {
					const bool nw{ cells[position + offset_t::Northwest] == value };
					const bool n{ cells[position + offset_t::North] == value };
					const bool ne{ cells[position + offset_t::Northeast] == value };

					const bool w{ cells[position + offset_t::West] == value };
					const bool e{ cells[position + offset_t::East] == value };

					const bool sw{ cells[position + offset_t::Southwest] == value };
					const bool s{ cells[position + offset_t::South] == value };
					const bool se{ cells[position + offset_t::Southeast] == value };

					if (nw && n && w) {
						index += 1 << 3;
					}

					if (n && ne && e) {
						index += 1 << 2;
					}

					if (e && se && s) {
						index += 1 << 1;
					}

					if (w && sw && s) {
						index += 1 << 0;
					}
				} else {
					if (!within<zone_region_t::All>(position)) {
						return (1 << 4) - 1;
					}

					cardinal_t edge{ edge_state(position) };

					const bool nw{ edge.north || edge.west || cells[position + offset_t::Northwest] == value };
					const bool n{ edge.north || cells[position + offset_t::North] == value };
					const bool ne{ edge.north || edge.east || cells[position + offset_t::Northeast] == value };

					const bool w{ edge.west || cells[position + offset_t::West] == value };
					const bool e{ edge.east || cells[position + offset_t::East] == value };

					const bool sw{ edge.south || edge.west || cells[position + offset_t::Southwest] == value };
					const bool s{ edge.south || cells[position + offset_t::South] == value };
					const bool se{ edge.south || edge.east || cells[position + offset_t::Southeast] == value };

					if (nw && n && w) {
						index += 1 << 3;
					}

					if (n && ne && e) {
						index += 1 << 2;
					}

					if (e && se && s) {
						index += 1 << 1;
					}

					if (w && sw && s) {
						index += 1 << 0;
					}
				}
			} else if constexpr (Solver == neighbourhood_solver_t::MarchingSquares) {
				if constexpr (Safe) {
					if (cells[position] == value) {
						index += 1 << 3;
					}
					if (cells[position + offset_t::East] == value) {
						index += 1 << 2;
					}
					if (cells[position + offset_t::Southeast] == value) {
						index += 1 << 1;
					}
					if (cells[position + offset_t::South] == value) {
						index += 1 << 0;
					}
				} else {
					if (!within<zone_region_t::All>(position)) {
						return (1 << 4) - 1;
					}

					cardinal_t edge{ edge_state(position) };

					if (cells[position] == value) {
						index += 1 << 3;
					}

					if (edge == cardinal_e::East || cells[position + offset_t::East] == value) {
						index += 1 << 2;
					}

					if (edge == cardinal_e::Southeast || cells[position + offset_t::Southeast] == value) {
						index += 1 << 1;
					}

					if (edge == cardinal_e::South || cells[position + offset_t::South] == value) {
						index += 1 << 0;
					}
				}
			}

			return index;
		}

		template<zone_region_t Region> constexpr ref<zone_t<T, Size, BorderSize>> collapse(cref<T> value, usize index, cref<T> collapse_to) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			array_t<T, Size> buffer{ cells };

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t y{ 0 }; y < zone_area; ++y) {
					for (extent_t::product_t x{ 0 }; x < zone_area; ++x) {
						const offset_t position{ x, y };

						if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded>(position, value) != index) {
							continue;
						}

						buffer[position] = collapse_to;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						const offset_t position{ x, y };

						if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded, interior_safe>(position, value) != index) {
							continue;
						}

						buffer[position] = collapse_to;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							const offset_t position{ x, y };

							if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded>(position, value) != index) {
								continue;
							}

							buffer[position] = collapse_to;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							const offset_t inner_position{ i, y };

							if (cells[inner_position] != value || calculate_index<neighbourhood_solver_t::Melded>(inner_position, value) != index) {
								goto outer_pos;
							}

							buffer[inner_position] = collapse_to;

						outer_pos:
							const offset_t outer_position{ zone_extent.x - i, y };

							if (cells[outer_position] != value || calculate_index<neighbourhood_solver_t::Melded>(outer_position, value) != index) {
								continue;
							}

							buffer[outer_position] = collapse_to;
						}
					}
				}
			}

			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires is_equatable<T, U>::value && std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> collapse(cref<U> value, usize index, cref<U> collapse_to) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			array_t<T, Size> buffer{ cells };

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t y{ 0 }; y < zone_area; ++y) {
					for (extent_t::product_t x{ 0 }; x < zone_area; ++x) {
						const offset_t position{ x, y };

						if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded>(position, value) != index) {
							continue;
						}

						buffer[position] = collapse_to;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						const offset_t position{ x, y };

						if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded, interior_safe>(position, value) != index) {
							continue;
						}

						buffer[position] = collapse_to;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							const offset_t position{ x, y };

							if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded>(position, value) != index) {
								continue;
							}

							buffer[position] = collapse_to;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							const offset_t inner_position{ i, y };

							if (cells[inner_position] != value || calculate_index<neighbourhood_solver_t::Melded>(inner_position, value) != index) {
								goto outer_pos;
							}

							buffer[inner_position] = collapse_to;

						outer_pos:
							const offset_t outer_position{ zone_extent.x - i, y };

							if (cells[outer_position] != value || calculate_index<neighbourhood_solver_t::Melded>(outer_position, value) != index) {
								continue;
							}

							buffer[outer_position] = collapse_to;
						}
					}
				}
			}

			swap(buffer);

			return *this;
		}

		template<zone_region_t Region> constexpr ref<zone_t<T, Size, BorderSize>> collapse(ref<array_t<T, Size>> buffer, cref<T> value, usize index, cref<T> collapse_to) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			buffer = cells;

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t y{ 0 }; y < zone_area; ++y) {
					for (extent_t::product_t x{ 0 }; x < zone_area; ++x) {
						const offset_t position{ x, y };

						if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded>(position, value) != index) {
							continue;
						}

						buffer[position] = collapse_to;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						const offset_t position{ x, y };

						if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded, interior_safe>(position, value) != index) {
							continue;
						}

						buffer[position] = collapse_to;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							const offset_t position{ x, y };

							if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded>(position, value) != index) {
								continue;
							}

							buffer[position] = collapse_to;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							const offset_t inner_position{ i, y };

							if (cells[inner_position] != value || calculate_index<neighbourhood_solver_t::Melded>(inner_position, value) != index) {
								goto outer_pos;
							}

							buffer[inner_position] = collapse_to;

						outer_pos:
							const offset_t outer_position{ zone_extent.x - i, y };

							if (cells[outer_position] != value || calculate_index<neighbourhood_solver_t::Melded>(outer_position, value) != index) {
								continue;
							}

							buffer[outer_position] = collapse_to;
						}
					}
				}
			}

			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires is_equatable<T, U>::value && std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> collapse(ref<array_t<T, Size>> buffer, cref<U> value, usize index, cref<U> collapse_to) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			buffer = cells;

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t y{ 0 }; y < zone_area; ++y) {
					for (extent_t::product_t x{ 0 }; x < zone_area; ++x) {
						const offset_t position{ x, y };

						if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded>(position, value) != index) {
							continue;
						}

						buffer[position] = collapse_to;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						const offset_t position{ x, y };

						if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded, U, interior_safe>(position, value) != index) {
							continue;
						}

						buffer[position] = collapse_to;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							const offset_t position{ x, y };

							if (cells[position] != value || calculate_index<neighbourhood_solver_t::Melded>(position, value) != index) {
								continue;
							}

							buffer[position] = collapse_to;
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							const offset_t inner_position{ i, y };

							if (cells[inner_position] != value || calculate_index<neighbourhood_solver_t::Melded>(inner_position, value) != index) {
								goto outer_pos;
							}

							buffer[inner_position] = collapse_to;

						outer_pos:
							const offset_t outer_position{ zone_extent.x - i, y };

							if (cells[outer_position] != value || calculate_index<neighbourhood_solver_t::Melded>(outer_position, value) != index) {
								continue;
							}

							buffer[outer_position] = collapse_to;
						}
					}
				}
			}

			swap(buffer);

			return *this;
		}

		template<bool Safe = false> constexpr void modulate(ref<array_t<T, Size>> buffer, cref<offset_t> position, u8 threshold, cref<T> true_state, cref<T> false_state) const noexcept {
			u8 neighbours{ neighbour_count<Safe>(position, true_state) };

			if (neighbours > threshold) {
				buffer[position] = true_state;
			} else if (neighbours < threshold) {
				buffer[position] = false_state;
			}
		}

		template<bool Safe = false, typename U>
			requires std::is_assignable<T, U>::value
		constexpr void modulate(ref<array_t<T, Size>> buffer, cref<offset_t> position, u8 threshold, cref<U> true_state, cref<U> false_state) const noexcept {
			u8 neighbours{ neighbour_count<Safe>(position, true_state) };

			if (neighbours > threshold) {
				buffer[position] = true_state;
			} else if (neighbours < threshold) {
				buffer[position] = false_state;
			}
		}

		template<bool Safe = false> constexpr void modulate(ref<array_t<T, Size>> buffer, cref<offset_t> position, u8 threshold, cref<binary_applicator_t<T>> applicator) const noexcept {
			u8 neighbours{ neighbour_count<Safe>(position, applicator.true_value) };

			if (neighbours > threshold) {
				buffer[position] = applicator.true_value;
			} else if (neighbours < threshold) {
				buffer[position] = applicator.false_value;
			}
		}

		template<bool Safe = false, typename U>
			requires std::is_assignable<T, U>::value
		constexpr void modulate(ref<array_t<T, Size>> buffer, cref<offset_t> position, u8 threshold, cref<binary_applicator_t<U>> applicator) const noexcept {
			u8 neighbours{ neighbour_count<Safe>(position, applicator.true_value) };

			if (neighbours > threshold) {
				buffer[position] = applicator.true_value;
			} else if (neighbours < threshold) {
				buffer[position] = applicator.false_value;
			}
		}

		template<zone_region_t Region> constexpr cref<zone_t<T, Size, BorderSize>> automatize(ref<array_t<T, Size>> buffer, u8 threshold, cref<T> true_value, cref<T> false_state) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::scalar_t y{ zone_origin.y }; y <= zone_extent.y; ++y) {
					for (extent_t::scalar_t x{ zone_origin.x }; x <= zone_extent.x; ++x) {
						modulate(buffer, offset_t{ x, y }, threshold, true_value, false_state);
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						modulate<interior_safe>(buffer, offset_t{ x, y }, threshold, true_value, false_state);
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							modulate(buffer, offset_t{ x, y }, threshold, true_value, false_state);
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							modulate(buffer, offset_t{ i, y }, threshold, true_value, false_state);
							modulate(buffer, offset_t{ zone_extent.x - i, y }, threshold, true_value, false_state);
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires std::is_assignable<T, U>::value
		constexpr cref<zone_t<T, Size, BorderSize>> automatize(ref<array_t<T, Size>> buffer, u8 threshold, cref<U> true_value, cref<U> false_state) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::scalar_t y{ zone_origin.y }; y <= zone_extent.y; ++y) {
					for (extent_t::scalar_t x{ zone_origin.x }; x <= zone_extent.x; ++x) {
						modulate(buffer, offset_t{ x, y }, threshold, true_value, false_state);
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						modulate<interior_safe>(buffer, offset_t{ x, y }, threshold, true_value, false_state);
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							modulate(buffer, offset_t{ x, y }, threshold, true_value, false_state);
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							modulate(buffer, offset_t{ i, y }, threshold, true_value, false_state);
							modulate(buffer, offset_t{ zone_extent.x - i, y }, threshold, true_value, false_state);
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region> constexpr cref<zone_t<T, Size, BorderSize>> automatize(ref<array_t<T, Size>> buffer, u8 threshold, cref<binary_applicator_t<T>> applicator) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::scalar_t y{ zone_origin.y }; y <= zone_extent.y; ++y) {
					for (extent_t::scalar_t x{ zone_origin.x }; x <= zone_extent.x; ++x) {
						modulate(buffer, offset_t{ x, y }, threshold, applicator);
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						modulate<interior_safe>(buffer, offset_t{ x, y }, threshold, applicator);
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							modulate(buffer, offset_t{ x, y }, threshold, applicator);
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							modulate(buffer, offset_t{ i, y }, threshold, applicator);
							modulate(buffer, offset_t{ zone_extent.x - i, y }, threshold, applicator);
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires std::is_assignable<T, U>::value
		constexpr cref<zone_t<T, Size, BorderSize>> automatize(ref<array_t<T, Size>> buffer, u8 threshold, cref<binary_applicator_t<U>> applicator) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::scalar_t y{ zone_origin.y }; y <= zone_extent.y; ++y) {
					for (extent_t::scalar_t x{ zone_origin.x }; x <= zone_extent.x; ++x) {
						modulate(buffer, offset_t{ x, y }, threshold, applicator);
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						modulate<interior_safe>(buffer, offset_t{ x, y }, threshold, applicator);
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							modulate(buffer, offset_t{ x, y }, threshold, applicator);
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							modulate(buffer, offset_t{ i, y }, threshold, applicator);
							modulate(buffer, offset_t{ zone_extent.x - i, y }, threshold, applicator);
						}
					}
				}
			}

			return *this;
		}

		template<zone_region_t Region> constexpr ref<zone_t<T, Size, BorderSize>> automatize(ref<array_t<T, Size>> buffer, u32 iterations, u8 threshold, cref<T> true_value, cref<T> false_state) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			for (u32 i{ 0 }; i < iterations; ++i) {
				automatize<Region>(buffer, threshold, true_value, false_state);
				swap(buffer);
			}

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> automatize(ref<array_t<T, Size>> buffer, u32 iterations, u8 threshold, cref<U> true_value, cref<U> false_state) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			for (u32 i{ 0 }; i < iterations; ++i) {
				automatize<Region>(buffer, threshold, true_value, false_state);
				swap(buffer);
			}

			return *this;
		}

		template<zone_region_t Region> constexpr ref<zone_t<T, Size, BorderSize>> automatize(ref<array_t<T, Size>> buffer, u32 iterations, u8 threshold, cref<binary_applicator_t<T>> applicator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			for (u32 i{ 0 }; i < iterations; ++i) {
				automatize<Region>(buffer, threshold, applicator);
				swap(buffer);
			}

			return *this;
		}

		template<zone_region_t Region, typename U>
			requires std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> automatize(ref<array_t<T, Size>> buffer, u32 iterations, u8 threshold, cref<binary_applicator_t<U>> applicator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			for (u32 i{ 0 }; i < iterations; ++i) {
				automatize<Region>(buffer, threshold, applicator);
				swap(buffer);
			}

			return *this;
		}

		template<zone_region_t Region, typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr ref<zone_t<T, Size, BorderSize>> generate(ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<T> true_value, cref<T> false_state) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, true_value, false_state);

			array_t<T, Size> buffer{ cells };

			automatize<Region>(buffer, iterations, threshold, true_value, false_state);
			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename Randomizer, typename U>
			requires is_random_engine<Randomizer>::value && std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> generate(ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<U> true_value, cref<U> false_state) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, true_value, false_state);

			array_t<T, Size> buffer{ cells };

			automatize<Region>(buffer, iterations, threshold, true_value, false_state);
			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr ref<zone_t<T, Size, BorderSize>> generate(ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<binary_applicator_t<T>> applicator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, applicator);

			array_t<T, Size> buffer{ cells };

			automatize<Region>(buffer, iterations, threshold, applicator);
			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename Randomizer, typename U>
			requires is_random_engine<Randomizer>::value && std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> generate(ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<binary_applicator_t<U>> applicator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, applicator);

			array_t<T, Size> buffer{ cells };

			automatize<Region>(buffer, iterations, threshold, applicator);
			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr ref<zone_t<T, Size, BorderSize>> generate(ref<array_t<T, Size>> buffer, ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<T> true_value, cref<T> false_state) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, true_value, false_state);

			buffer = cells;

			automatize<Region>(buffer, iterations, threshold, true_value, false_state);
			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename Randomizer, typename U>
			requires is_random_engine<Randomizer>::value && std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> generate(ref<array_t<T, Size>> buffer, ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<U> true_value, cref<U> false_state) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, true_value, false_state);

			buffer = cells;

			automatize<Region>(buffer, iterations, threshold, true_value, false_state);
			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr ref<zone_t<T, Size, BorderSize>> generate(ref<array_t<T, Size>> buffer, ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<binary_applicator_t<T>> applicator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, applicator);

			buffer = cells;

			automatize<Region>(buffer, iterations, threshold, applicator);
			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename Randomizer, typename U>
			requires is_random_engine<Randomizer>::value && std::is_assignable<T, U>::value
		constexpr ref<zone_t<T, Size, BorderSize>> generate(ref<array_t<T, Size>> buffer, ref<Randomizer> generator, f64 fill_percent, u32 iterations, u8 threshold, cref<binary_applicator_t<U>> applicator) noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			randomize<Region>(generator, fill_percent, applicator);

			buffer = cells;

			automatize<Region>(buffer, iterations, threshold, applicator);
			swap(buffer);

			return *this;
		}

		template<zone_region_t Region, typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr std::optional<offset_t> find_random(ref<Randomizer> generator, cref<T> value) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (cells[pos] == value) {
						return pos;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ interior_origin.x, interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ interior_origin.y, interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (cells[pos] == value) {
						return pos;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, border_size.h * 2 - 1 };

				for (extent_t::product_t i{ 0 }; i < border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < border_size.w ? x : zone_extent.x - x;
					y = y < border_size.h ? y : zone_extent.y - y;

					const offset_t pos{ x, y };

					if (cells[pos] == value) {
						return pos;
					}
				}
			}

			return std::nullopt;
		}

		template<zone_region_t Region, typename Randomizer, typename U>
			requires is_random_engine<Randomizer>::value && is_equatable<T, U>::value
		constexpr std::optional<offset_t> find_random(ref<Randomizer> generator, cref<U> value) const noexcept {
			if constexpr (Region == zone_region_t::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (cells[pos] == value) {
						return pos;
					}
				}

				return std::nullopt;
			} else if constexpr (Region == zone_region_t::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ interior_origin.x, interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ interior_origin.y, interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (cells[pos] == value) {
						return pos;
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, border_size.h * 2 - 1 };

				for (extent_t::product_t i{ 0 }; i < border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < border_size.w ? x : zone_extent.x - x;
					y = y < border_size.h ? y : zone_extent.y - y;

					const offset_t pos{ x, y };

					if (cells[pos] == value) {
						return pos;
					}
				}
			}

			return std::nullopt;
		}

		template<zone_region_t Region, typename Randomizer, SparseBlockage Blockage>
			requires is_random_engine<Randomizer>::value
		constexpr std::optional<offset_t> find_random(ref<Randomizer> generator, cref<T> value, cref<Blockage> sparse_blockage) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (cells[pos] != value || sparse_blockage.contains(pos)) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ interior_origin.x, interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ interior_origin.y, interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (cells[pos] != value || sparse_blockage.contains(pos)) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_t::Border) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, border_size.h * 2 - 1 };

				for (extent_t::product_t i{ 0 }; i < border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < border_size.w ? x : zone_extent.x - x;
					y = y < border_size.h ? y : zone_extent.y - y;

					const offset_t pos{ x, y };

					if (cells[pos] != value || sparse_blockage.contains(pos)) {
						continue;
					}

					return pos;
				}
			}

			return std::nullopt;
		}

		template<zone_region_t Region, typename Randomizer, typename U, SparseBlockage Blockage>
			requires is_random_engine<Randomizer>::value && is_equatable<T, U>::value
		constexpr std::optional<offset_t> find_random(ref<Randomizer> generator, cref<U> value, cref<Blockage> sparse_blockage) const noexcept {
			if constexpr (Region == zone_region_t::None) {
				return *this;
			}

			if constexpr (Region == zone_region_t::All) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, zone_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, zone_extent.y };

				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (cells[pos] != value || sparse_blockage.contains(pos)) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ interior_origin.x, interior_extent.x };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ interior_origin.y, interior_extent.y };

				for (extent_t::product_t i{ 0 }; i < interior_area; ++i) {
					const offset_t pos{ x_dis(generator), y_dis(generator) };

					if (cells[pos] != value || sparse_blockage.contains(pos)) {
						continue;
					}

					return pos;
				}
			} else if constexpr (Region == zone_region_t::Border) {
				std::uniform_int_distribution<offset_t::scalar_t> x_dis{ 0, border_size.w * 2 - 1 };
				std::uniform_int_distribution<offset_t::scalar_t> y_dis{ 0, border_size.h * 2 - 1 };

				for (extent_t::product_t i{ 0 }; i < border_area; ++i) {
					auto x{ x_dis(generator) };
					auto y{ y_dis(generator) };

					x = x < border_size.w ? x : zone_extent.x - x;
					y = y < border_size.h ? y : zone_extent.y - y;

					const offset_t pos{ x, y };

					if (cells[pos] != value || sparse_blockage.contains(pos)) {
						continue;
					}

					return pos;
				}
			}

			return std::nullopt;
		}

		constexpr bool linear_blockage(cref<offset_t> origin, cref<offset_t> target, cref<T> value) const noexcept {
			if (cells[origin] == value || cells[target] == value) {
				return true;
			}

			if (origin == target) {
				return false;
			}

			offset_t delta{ std::abs(target.x - origin.x), std::abs(target.y - origin.y) };

			offset_t step{ origin.x < target.x ? 1 : -1, origin.y < target.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			offset_t current_position{ origin };

			for (;;) {
				if (current_position == target) {
					return true;
				}

				if (cells[current_position] == value) {
					return true;
				}

				i32 e2 = 2 * err;

				if (e2 > -delta.y) {
					err -= delta.y;
					current_position.x += step.x;
				}

				if (e2 < delta.x) {
					err += delta.x;
					current_position.y += step.y;
				}
			}
		}

		template<zone_region_t Region> constexpr u32 count(cref<T> value) const noexcept {
			u32 total{ 0 };

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					if (cells[i] == value) {
						++total;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						if (cells[x, y] == value) {
							++total;
						}
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							if (cells[x, y] == value) {
								++total;
							}
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							if (cells[i, y] == value) {
								++total;
							}
							if (cells[zone_extent.x - i, y] == value) {
								++total;
							}
						}
					}
				}
			} else if constexpr (Region == zone_region_t::None) {
				return 0;
			}

			return total;
		}

		template<zone_region_t Region, typename U>
			requires is_equatable<T, U>::value
		constexpr u32 count(cref<U> value) const noexcept {
			u32 total{ 0 };

			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					if (cells[i] == value) {
						++total;
					}
				}
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						if (cells[x, y] == value) {
							++total;
						}
					}
				}
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							if (cells[x, y] == value) {
								++total;
							}
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							if (cells[i, y] == value) {
								++total;
							}
							if (cells[zone_extent.x - i, y] == value) {
								++total;
							}
						}
					}
				}
			} else if constexpr (Region == zone_region_t::None) {
				return 0;
			}

			return total;
		}

		template<zone_region_t Region> constexpr u32 contains(cref<T> value) const noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					if (cells[i] == value) {
						return true;
					}
				}

				return false;
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						if (cells[x, y] == value) {
							return true;
						}
					}
				}

				return false;
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							if (cells[x, y] == value) {
								return true;
							}
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							if (cells[i, y] == value) {
								return true;
							}
							if (cells[zone_extent.x - i, y] == value) {
								return true;
							}
						}
					}
				}

				return false;
			} else if constexpr (Region == zone_region_t::None) {
				return false;
			}

			return false;
		}

		template<zone_region_t Region, typename U>
			requires is_equatable<T, U>::value
		constexpr u32 contains(cref<U> value) const noexcept {
			if constexpr (Region == zone_region_t::All) {
				for (extent_t::product_t i{ 0 }; i < zone_area; ++i) {
					if (cells[i] == value) {
						return true;
					}
				}

				return false;
			} else if constexpr (Region == zone_region_t::Interior) {
				for (extent_t::scalar_t y{ interior_origin.y }; y <= interior_extent.y; ++y) {
					for (extent_t::scalar_t x{ interior_origin.x }; x <= interior_extent.x; ++x) {
						if (cells[x, y] == value) {
							return true;
						}
					}
				}

				return false;
			} else if constexpr (Region == zone_region_t::Border) {
				for (extent_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
					if (y < interior_origin.y || y > interior_extent.y) {
						for (extent_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
							if (cells[x, y] == value) {
								return true;
							}
						}
					} else {
						for (extent_t::scalar_t i{ 0 }; i < border_size.w; ++i) {
							if (cells[i, y] == value) {
								return true;
							}
							if (cells[zone_extent.x - i, y] == value) {
								return true;
							}
						}
					}
				}

				return false;
			} else if constexpr (Region == zone_region_t::None) {
				return false;
			}

			return false;
		}

		constexpr bool linear_blockage(cref<offset_t> origin, cref<offset_t> target, cref<T> value, u32 distance) const noexcept {
			if (cells[origin] == value || cells[target] == value) {
				return true;
			}

			if (origin == target) {
				return false;
			}

			offset_t delta{ std::abs(target.x - origin.x), std::abs(target.y - origin.y) };

			offset_t step{ origin.x < target.x ? 1 : -1, origin.y < target.y ? 1 : -1 };

			i32 err = delta.x - delta.y;

			creeper_t<u32> creeper{ origin, 0 };

			for (;;) {
				if (creeper.position == target) {
					return true;
				}

				if (cells[creeper.position] == value) {
					return true;
				}

				i32 e2 = 2 * err;

				if (e2 > -delta.y || e2 < delta.x) {
					if (creeper.distance >= distance) {
						return false;
					}

					if (e2 > -delta.y) {
						err -= delta.y;
						creeper.position.x += step.x;
					}

					if (e2 < delta.x) {
						err += delta.x;
						creeper.position.y += step.y;
					}

					++creeper.distance;
				}
			}
		}

		template<bool Simple = false, extent_t AtlasSize>
		constexpr void draw(cref<atlas_t<AtlasSize>> atlas) const noexcept
			requires is_drawable<T>::value
		{
			for (offset_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
				for (offset_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
					const offset_t pos{ x, y };

					if constexpr (Simple) {
						(*this)[pos].draw(atlas, pos);
					} else {
						(*this)[pos].draw(atlas, *this, pos);
					}
				}
			}
		}

		template<bool Simple = false, extent_t AtlasSize>
		constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<offset_t> offset) const noexcept
			requires is_drawable<T>::value
		{
			for (offset_t::scalar_t y{ 0 }; y < zone_size.h; ++y) {
				for (offset_t::scalar_t x{ 0 }; x < zone_size.w; ++x) {
					const offset_t pos{ x, y };

					if constexpr (Simple) {
						(*this)[pos].draw(atlas, pos, offset);
					} else {
						(*this)[pos].draw(atlas, *this, pos, offset);
					}
				}
			}
		}

		template<bool Simple = false, extent_t AtlasSize>
		constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<camera_t> camera) const noexcept
			requires is_drawable<T>::value
		{
			const offset_t origin{ camera.get_position() };
			const extent_t camera_extent{ camera.get_extent() };

			if (camera_extent.w == 0 || camera_extent.h == 0 || origin.x > zone_extent.x || origin.y > zone_extent.y) {
				return;
			}

			for (offset_t::scalar_t y{ max(offset_t::scalar_t{ 0 }, origin.y) }; y < min(zone_size.h, camera_extent.h); ++y) {
				for (offset_t::scalar_t x{ max(offset_t::scalar_t{ 0 }, origin.x) }; x < min(zone_size.w, camera_extent.w); ++x) {
					const offset_t pos{ x, y };

					if constexpr (Simple) {
						(*this)[pos].draw(atlas, pos, -origin);
					} else {
						(*this)[pos].draw(atlas, *this, pos, -origin);
					}
				}
			}
		}

		template<bool Simple = false, extent_t AtlasSize>
		constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<offset_t> offset, cref<offset_t> origin, cref<extent_t> size) const noexcept
			requires is_drawable<T>::value
		{
			const extent_t camera_extent{ origin + size };

			if (camera_extent.w == 0 || camera_extent.h == 0 || origin.x > zone_extent.x || origin.y > zone_extent.y) {
				return;
			}

			for (offset_t::scalar_t y{ max(offset_t::scalar_t{ 0 }, origin.y) }; y < min(zone_size.h, camera_extent.h); ++y) {
				for (offset_t::scalar_t x{ max(offset_t::scalar_t{ 0 }, origin.x) }; x < min(zone_size.w, camera_extent.w); ++x) {
					const offset_t pos{ x, y };

					if constexpr (Simple) {
						(*this)[pos].draw(atlas, pos, offset);
					} else {
						(*this)[pos].draw(atlas, *this, pos, offset);
					}
				}
			}
		}

		constexpr cstr serialize() const noexcept { return reinterpret_cast<cstr>(cells.data_ptr()); }

		constexpr bool serialize(cref<std::string> path) const noexcept {
			std::ofstream file{};

			try {
				file.open(path, std::ios::out | std::ios::binary);

				file.write(reinterpret_cast<cstr>(cells.data_ptr()), cells.byte_size);

				file.close();
			} catch (cref<std::exception> e) {
				error_log.add(e.what(), __TIME_FILE_LINE__);
				return false;
			}

			return true;
		}

		constexpr void deserialize(cstr binary_data) noexcept { std::memcpy(reinterpret_cast<str>(cells.data_ptr()), binary_data, cells.byte_size); }
	};
} // namespace bleak
