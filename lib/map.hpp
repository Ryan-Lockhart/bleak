#pragma once

#include "typedef.hpp"

#include <algorithm>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <random>
#include <string>

#include "array.hpp"
#include "array/layer.tpp"
#include "atlas.hpp"
#include "cardinal.hpp"
#include "point.hpp"

namespace Bleakdepth {
	// generator for map randomization is fixed due to lack of support for function template partial specialization
	using MapRandomizer = std::minstd_rand;

	enum class cell_trait_t : u8 { Open, Solid, Transperant, Opaque, Seen, Explored, Unseen, Unexplored, Vacant, Occupied, Dry, Damp, Cold, Warm };

	struct cell_state_t {
	  private:
		bool solid : 1 { false };
		bool opaque : 1 { false };
		bool seen : 1 { false };
		bool explored : 1 { false };
		bool occupied : 1 { false };
		bool damp : 1 { false };
		bool warm : 1 { false };

	  public:
		constexpr cell_state_t() noexcept = default;

		constexpr cell_state_t(cref<cell_state_t> other) noexcept :
			solid { other.solid },
			opaque { other.opaque },
			seen { other.seen },
			explored { other.explored },
			occupied { other.occupied },
			damp { other.damp },
			warm { other.warm } {}

		constexpr cell_state_t(rval<cell_state_t> other) noexcept :
			solid { other.solid },
			opaque { other.opaque },
			seen { other.seen },
			explored { other.explored },
			occupied { other.occupied },
			damp { other.damp },
			warm { other.warm } {}

		constexpr ref<cell_state_t> operator=(cref<cell_state_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			solid = other.solid;
			opaque = other.opaque;
			seen = other.seen;
			explored = other.explored;
			occupied = other.occupied;
			damp = other.damp;
			warm = other.warm;

			return *this;
		}

		constexpr ref<cell_state_t> operator=(rval<cell_state_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			solid = other.solid;
			opaque = other.opaque;
			seen = other.seen;
			explored = other.explored;
			occupied = other.occupied;
			damp = other.damp;
			warm = other.warm;

			return *this;
		}

		template<typename... Traits, typename = cell_trait_t> constexpr cell_state_t(Traits... traits) {
			for (cell_trait_t trait : { traits... }) {
				set(trait);
			}
		}

		constexpr inline cell_state_t operator+(cell_trait_t trait) const noexcept {
			cell_state_t state { *this };

			state.set(trait);

			return state;
		}

		constexpr inline ref<cell_state_t> operator+=(cell_trait_t trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr inline cell_state_t operator-(cell_trait_t trait) const noexcept {
			cell_state_t state { *this };

			state.unset(trait);

			return state;
		}

		constexpr inline ref<cell_state_t> operator-=(cell_trait_t trait) noexcept {
			unset(trait);

			return *this;
		}

		constexpr void set(cell_trait_t trait) noexcept {
			switch (trait) {
			case cell_trait_t::Open:
				solid = false;
				break;
			case cell_trait_t::Solid:
				solid = true;
				break;
			case cell_trait_t::Transperant:
				opaque = false;
				break;
			case cell_trait_t::Opaque:
				opaque = true;
				break;
			case cell_trait_t::Unseen:
				seen = false;
				break;
			case cell_trait_t::Seen:
				seen = true;
				break;
			case cell_trait_t::Unexplored:
				explored = false;
				break;
			case cell_trait_t::Explored:
				explored = true;
				break;
			case cell_trait_t::Vacant:
				occupied = false;
				break;
			case cell_trait_t::Occupied:
				occupied = true;
				break;
			case cell_trait_t::Dry:
				damp = false;
				break;
			case cell_trait_t::Damp:
				damp = true;
				break;
			case cell_trait_t::Cold:
				warm = false;
				break;
			case cell_trait_t::Warm:
				warm = true;
				break;
			default:
				break;
			}
		}

		constexpr void unset(cell_trait_t trait) noexcept {
			switch (trait) {
			case cell_trait_t::Open:
				solid = true;
				break;
			case cell_trait_t::Solid:
				solid = false;
				break;
			case cell_trait_t::Transperant:
				opaque = true;
				break;
			case cell_trait_t::Opaque:
				opaque = false;
				break;
			case cell_trait_t::Unseen:
				seen = true;
				break;
			case cell_trait_t::Seen:
				seen = false;
				break;
			case cell_trait_t::Unexplored:
				explored = true;
				break;
			case cell_trait_t::Explored:
				explored = false;
				break;
			case cell_trait_t::Vacant:
				occupied = true;
				break;
			case cell_trait_t::Occupied:
				occupied = false;
				break;
			case cell_trait_t::Dry:
				damp = true;
				break;
			case cell_trait_t::Damp:
				damp = false;
				break;
			case cell_trait_t::Cold:
				warm = true;
				break;
			case cell_trait_t::Warm:
				warm = false;
				break;
			default:
				break;
			}
		}

		constexpr bool operator==(cref<cell_state_t> other) const noexcept {
			return solid == other.solid && opaque == other.opaque && seen == other.seen && explored == other.explored && occupied == other.occupied
				   && damp == other.damp && warm == other.warm;
		}

		constexpr bool operator!=(cref<cell_state_t> other) const noexcept {
			return solid != other.solid || opaque != other.opaque || seen != other.seen || explored != other.explored || occupied != other.occupied
				   || damp != other.damp || warm != other.warm;
		}

		constexpr bool contains(cell_trait_t trait) const noexcept {
			switch (trait) {
			case cell_trait_t::Open:
				return !solid;
			case cell_trait_t::Solid:
				return solid;
			case cell_trait_t::Transperant:
				return !opaque;
			case cell_trait_t::Opaque:
				return opaque;
			case cell_trait_t::Seen:
				return seen;
			case cell_trait_t::Unseen:
				return !seen;
			case cell_trait_t::Explored:
				return explored;
			case cell_trait_t::Unexplored:
				return !explored;
			case cell_trait_t::Occupied:
				return occupied;
			case cell_trait_t::Vacant:
				return !occupied;
			case cell_trait_t::Damp:
				return damp;
			case cell_trait_t::Dry:
				return !damp;
			case cell_trait_t::Warm:
				return warm;
			case cell_trait_t::Cold:
				return !warm;
			default:
				return false;
			}
		}

		inline operator std::string() const {
			return std::format(
				"[{}, {}, {}, {}, {}, {}, {}]",
				solid ? "Solid" : "Open",
				seen ? "Opaque" : "Transperant",
				solid ? "Seen" : "Unseen",
				explored ? "Explored" : "Unexplored",
				occupied ? "Occupied" : "Vacant",
				damp ? "Damp" : "Dry",
				warm ? "Warm" : "Cold"
			);
		}
	};

	enum class map_region_t : u8 { None = 0, Interior = 1 << 0, Border = 1 << 1, All = Interior | Border };

	template<usize Width, usize Height, usize BorderWidth = 0, usize BorderHeight = 0> class map_t {
	  private:
		layer_t<cell_state_t, Width, Height> state;

	  public:
		static constexpr usize width { Width };
		static constexpr usize height { Height };

		static constexpr point_t<uhalf> origin { 0 };
		static constexpr point_t<uhalf> extent { width - 1, height - 1 };

		static constexpr usize border_width { BorderWidth };
		static constexpr usize border_height { BorderHeight };

		static constexpr point_t<uhalf> border_origin { origin.x + border_width, origin.y + border_height };
		static constexpr point_t<uhalf> border_extent { extent.x - border_width, extent.y - border_height };

		static constexpr usize area { Width * Height };

		static constexpr usize interior_area { (width - border_width * 2) * (height - border_height * 2) };
		static constexpr usize border_area { area - interior_area };

		static constexpr usize size { area };

		inline map_t() : state {} {}

		inline map_t(cref<map_t<Width, Height>> other) : state { other.state } {}

		inline map_t(rval<map_t<Width, Height>> other) : state { std::move(other.state) } {}

		inline ref<map_t<Width, Height>> operator=(cref<map_t<Width, Height>> other) noexcept {
			if (this != &other) {
				state = other.state;
			}

			return *this;
		}

		inline ref<map_t<Width, Height>> operator=(rval<map_t<Width, Height>> other) noexcept {
			if (this != &other) {
				state = std::move(other.state);
			}

			return *this;
		}

		inline ~map_t() noexcept {}

		inline cref<array_t<cell_state_t, Width, Height>> data() const noexcept { return state; }

		inline cptr<array_t<cell_state_t, Width, Height>> data_ptr() const noexcept { return &state; }

		inline ref<cell_state_t> operator[](usize index) noexcept { return state[index]; }

		inline cref<cell_state_t> operator[](usize index) const noexcept { return state[index]; }

		inline ref<cell_state_t> operator[](uhalf x, uhalf y) noexcept { return state[x, y]; }

		inline cref<cell_state_t> operator[](uhalf x, uhalf y) const noexcept { return state[x, y]; }

		inline ref<cell_state_t> operator[](cref<point_t<uhalf>> position) noexcept { return state[position]; }

		inline cref<cell_state_t> operator[](cref<point_t<uhalf>> position) const noexcept { return state[position]; }

		inline bool on_x_edge(cref<point_t<uhalf>> position) const noexcept { return position.x == origin.x || position.x == extent.x; }

		inline bool on_y_edge(cref<point_t<uhalf>> position) const noexcept { return position.y == origin.y || position.y == extent.y; }

		inline bool on_edge(cref<point_t<uhalf>> position) const noexcept { return on_x_edge(position) || on_y_edge(position); }

		inline cardinal_t edge_state(cref<point_t<uhalf>> position) const noexcept {
			cardinal_t state { cardinal_t::Central };

			if (!on_edge(position)) {
				return state;
			}

			if (position.x == origin.x) {
				state |= cardinal_t::West;
			} else if (position.x == extent.x) {
				state |= cardinal_t::East;
			}

			if (position.y == origin.y) {
				state |= cardinal_t::North;
			} else if (position.y == extent.y) {
				state |= cardinal_t::South;
			}

			return state;
		}

		template<map_region_t Region> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> set(cell_state_t cell_state);

		template<map_region_t Region> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> randomize(ref<MapRandomizer> generator, f64 fill_percent, cell_state_t true_state, cell_state_t false_state);

		template<> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> set<map_region_t::All>(cell_state_t cell_state) {
			for (usize i { 0 }; i < area; ++i) {
				state[i].set(cell_state);
			}

			return *this;
		}

		template<> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> set<map_region_t::Interior>(cell_state_t cell_state) {
			for (uhalf y { border_origin.y }; y <= border_extent.y; ++y) {
				for (uhalf x { border_origin.x }; x <= border_extent.x; ++x) {
					state[x, y] = cell_state;
				}
			}

			return *this;
		}

		template<> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> set<map_region_t::Border>(cell_state_t cell_state) {
			for (int y { 0 }; y < height; ++y) {
				if (y < border_origin.y || y > border_extent.y) {
					for (int x { 0 }; x < width; ++x) {
						state[x, y] = cell_state;
					}
				} else {
					for (int i { 0 }; i < border_width; ++i) {
						state[i, y] = cell_state;
						state[extent.x - i, y] = cell_state;
					}
				}
			}

			return *this;
		}

		template<> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> set<map_region_t::None>(cell_state_t cell_state) { return *this; }

		template<> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> randomize<map_region_t::All>(ref<MapRandomizer> generator, f64 fill_percent, cell_state_t true_state, cell_state_t false_state) {
			auto dis{std::bernoulli_distribution { fill_percent }};
			
			for (usize i{0}; i < area; ++i) {
				state[i] = dis(generator) ? true_state : false_state;
			}
			
			return *this;
		}

		template<> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> randomize<map_region_t::Interior>(ref<MapRandomizer> generator, f64 fill_percent, cell_state_t true_state, cell_state_t false_state) {
			auto dis{std::bernoulli_distribution { fill_percent }};
			
			for (uhalf y{border_origin.y}; y <= border_extent.y; ++y) {
				for (uhalf x{border_origin.x}; x <= border_extent.x; ++x) {
					state[x, y] = dis(generator) ? true_state : false_state;
				}
			}
			
			return *this;
		}

		template<> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> randomize<map_region_t::Border>(ref<MapRandomizer> generator, f64 fill_percent, cell_state_t true_state, cell_state_t false_state) {
			auto dis{std::bernoulli_distribution { fill_percent }};
			
			for (int y{0}; y < height; ++y) {
				if (y < border_origin.y || y > border_extent.y) {
					for (int x{0}; x < width; ++x) {
						state[x, y] = dis(generator) ? true_state : false_state;
					}
				} else {
					for (int i{0}; i < border_width; ++i) {
						state[i, y] = dis(generator) ? true_state : false_state;
						state[extent.x - i, y] = dis(generator) ? true_state : false_state;
					}
				}
			}
			
			return *this;
		}

		template<> inline ref<map_t<Width, Height, BorderWidth, BorderHeight>> randomize<map_region_t::None>(ref<MapRandomizer> generator, f64 fill_percent, cell_state_t true_state, cell_state_t false_state) { return *this; }

		inline u8 neighbour_count(cref<point_t<uhalf>> position, cell_state_t mask) const noexcept {
			u8 count { 0 };
			cardinal_t edge { edge_state(position) };

			if (edge != cardinal_t::Northwest && state[position + point_t<uhalf>::Northwest].contains(mask)) {
				++count;
			}

			if (edge != cardinal_t::North && state[position + point_t<uhalf>::North].contains(mask)) {
				++count;
			}

			if (edge != cardinal_t::Northeast && state[position + point_t<uhalf>::Northeast].contains(mask)) {
				++count;
			}

			if (edge != cardinal_t::West && state[position + point_t<uhalf>::West].contains(mask)) {
				++count;
			}

			if (edge != cardinal_t::East && state[position + point_t<uhalf>::East].contains(mask)) {
				++count;
			}

			if (edge != cardinal_t::Southwest && state[position + point_t<uhalf>::Southwest].contains(mask)) {
				++count;
			}

			if (edge != cardinal_t::South && state[position + point_t<uhalf>::South].contains(mask)) {
				++count;
			}

			if (edge != cardinal_t::Southeast && state[position + point_t<uhalf>::Southeast].contains(mask)) {
				++count;
			}

			return count;
		}

		template<typename Generator> inline std::optional<point_t<uhalf>> find_random_cell(ref<Generator> generator, cell_trait_t trait) const {
			std::uniform_int_distribution<uhalf> x_dis { 0, width - 1 };
			std::uniform_int_distribution<uhalf> y_dis { 0, height - 1 };

			for (usize i { 0 }; i < area; ++i) {
				const point_t<uhalf> pos { x_dis(generator), y_dis(generator) };

				if (state[pos].contains(trait)) {
					return pos;
				}
			}

			return std::nullopt;
		}

		template<typename Generator> inline std::optional<point_t<uhalf>> find_random_cell_interior(ref<Generator> generator, cell_trait_t trait) const {
			std::uniform_int_distribution<uhalf> x_dis { border_width, width - 1 - border_width };
			std::uniform_int_distribution<uhalf> y_dis { border_height, height - 1 - border_height };

			for (usize i { 0 }; i < area - border_area; ++i) {
				const point_t<uhalf> pos { x_dis(generator), y_dis(generator) };

				if (state[pos].contains(trait)) {
					return pos;
				}
			}

			return std::nullopt;
		}

		template<usize AtlasWidth, usize AtlasHeight> inline void draw(cref<atlas_t<AtlasWidth, AtlasHeight>> atlas) const {
			for (uhalf y { 0 }; y < Height; ++y) {
				for (uhalf x { 0 }; x < Width; ++x) {
					const usize idx { state.flatten(x, y) };
					const cell_state_t cell_state { operator[](idx) };

					if (cell_state.contains(cell_trait_t::Unexplored)) {
						continue;
					}

					const bool is_solid { cell_state.contains(cell_trait_t::Solid) };
					const bool is_seen { cell_state.contains(cell_trait_t::Seen) };

					const u8 rgb { is_solid ? u8 { 0xC0 } : u8 { 0x40 } };
					const u8 alpha { is_seen ? u8 { 0xFF } : u8 { 0x80 } };
					const u8 glyph { is_solid ? u8 { 0xB2 } : u8 { 0xB0 } };

					atlas.draw({ glyph, { rgb, rgb, rgb, alpha } }, { static_cast<i32>(x), static_cast<i32>(y) });
				}
			}
		}

		template<usize AtlasWidth, usize AtlasHeight> inline void draw(cref<atlas_t<AtlasWidth, AtlasHeight>> atlas, cref<point_t<i32>> offset) const {
			for (uhalf y { 0 }; y < Height; ++y) {
				for (uhalf x { 0 }; x < Width; ++x) {
					const usize idx { state.flatten(x, y) };
					const cell_state_t cell_state { operator[](idx) };

					if (cell_state.contains(cell_trait_t::Unexplored)) {
						continue;
					}

					const bool is_solid { cell_state.contains(cell_trait_t::Solid) };
					const bool is_seen { cell_state.contains(cell_trait_t::Seen) };

					const u8 rgb { is_solid ? u8 { 0xC0 } : u8 { 0x40 } };
					const u8 alpha { is_seen ? u8 { 0xFF } : u8 { 0x80 } };
					const u8 glyph { is_solid ? u8 { 0xB2 } : u8 { 0xB0 } };

					atlas.draw({ glyph, { rgb, rgb, rgb, alpha } }, point_t<i32> { static_cast<i32>(x), static_cast<i32>(y) } + offset);
				}
			}
		}

		inline bool serialize(cref<std::string> path, cref<std::string> name) const {
			std::ofstream file {};

			try {
				file.open(std::format("{}\\{}.map.bin", path, name), std::ios::out | std::ios::binary);

				file.write(reinterpret_cast<cstr>(state.data_ptr()), state.byte_size);

				file.close();
			} catch (std::exception e) {
				std::cerr << e.what() << std::endl;
				return false;
			}

			return true;
		}
	};
} // namespace Bleakdepth
