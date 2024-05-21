#pragma once

#include "array.hpp"
#include "cardinal.hpp"
#include "glyph.hpp"
#include "point.hpp"
#include "typedef.hpp"

#include <algorithm>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

#include "array/layer.tpp"
#include "atlas.hpp"

namespace Bleakdepth {
	struct cell_state_t {
	  private:
		u8 value;

	  public:
		constexpr cell_state_t() : value(0) {}

		constexpr cell_state_t(u8 value) : value(value) {}

		constexpr cell_state_t(cref<cell_state_t> other) noexcept : value { other.value } {}

		constexpr cell_state_t(rval<cell_state_t> other) noexcept : value { std::move(other.value) } {}

		constexpr ref<cell_state_t> operator=(cref<cell_state_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			value = other.value;

			return *this;
		}

		constexpr ref<cell_state_t> operator=(rval<cell_state_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			value = std::move(other.value);

			return *this;
		}

		static const cell_state_t Solid;
		static const cell_state_t Opaque;

		static const cell_state_t Seen;
		static const cell_state_t Explored;

		// static const cell_state_t Occupied;

		// static const cell_state_t Damp;
		// static const cell_state_t Warm;

		constexpr void add(cell_state_t condition) noexcept {
			if (value & condition) {
				return;
			}

			value |= condition;
		}

		constexpr void remove(cell_state_t condition) noexcept {
			if (~value & condition) {
				return;
			}

			value &= ~condition;
		}

		constexpr void toggle(cell_state_t condition) noexcept { value ^= condition; }

		constexpr cell_state_t operator~() const noexcept { return ~value; }

		constexpr cell_state_t operator|(cref<cell_state_t> other) const noexcept { return value | other.value; }

		constexpr cell_state_t operator&(cref<cell_state_t> other) const noexcept { return value & other.value; }

		constexpr cell_state_t operator^(cref<cell_state_t> other) const noexcept { return value ^ other.value; }

		constexpr cell_state_t operator<<(cref<cell_state_t> other) const noexcept { return value << other.value; }

		constexpr cell_state_t operator>>(cref<cell_state_t> other) const noexcept { return value >> other.value; }

		constexpr bool contains(cref<cell_state_t> other) const noexcept { return value & other; }

		constexpr ref<cell_state_t> operator|=(cref<cell_state_t> other) noexcept {
			value |= other.value;
			return *this;
		}

		constexpr ref<cell_state_t> operator&=(cref<cell_state_t> other) noexcept {
			value &= other.value;
			return *this;
		}

		constexpr ref<cell_state_t> operator^=(cref<cell_state_t> other) noexcept {
			value ^= other.value;
			return *this;
		}

		constexpr ref<cell_state_t> operator<<=(cref<cell_state_t> other) noexcept {
			value <<= other.value;
			return *this;
		}

		constexpr ref<cell_state_t> operator>>=(cref<cell_state_t> other) noexcept {
			value >>= other.value;
			return *this;
		}

		constexpr bool operator==(cref<cell_state_t> other) const noexcept { return value == other.value; }

		constexpr bool operator!=(cref<cell_state_t> other) const noexcept { return value != other.value; }

		constexpr bool solid() const noexcept { return value & Solid; }

		constexpr bool open() const noexcept { return !solid(); }

		constexpr bool opaque() const noexcept { return value & Opaque; }

		constexpr bool transperant() const noexcept { return !opaque(); }

		constexpr bool seen() const noexcept { return value & Seen; }

		constexpr bool unseen() const noexcept { return !seen(); }

		constexpr bool explored() const noexcept { return value & Explored; }

		constexpr bool unexplored() const noexcept { return !explored(); }

		constexpr operator u8() const noexcept { return value; }

		inline operator std::string() const {
			return std::format(
				"[{}, {}, {}, {}]",
				solid() ? "Solid" : "Open",
				seen() ? "Opaque" : "Transperant",
				solid() ? "Seen" : "Unseen",
				explored() ? "Explored" : "Unexplored"
			);
		}

		constexpr u8 serialize() const { return value; }

		constexpr bool deserialize(u8 data) {
			value = data;
			return true;
		}
	};

	template<usize Width, usize Height> class map_t {
	  private:
		layer_t<cell_state_t, Width, Height> state;

	  public:
		static constexpr usize width { Width };
		static constexpr usize height { Height };

		static constexpr usize area { Width * Height };

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
		inline cptr<array_t<cell_state_t, Width, Height>> data_ptr() const noexcept { return &state;}

		inline ref<cell_state_t> operator[](usize index) noexcept { return state[index]; }

		inline cref<cell_state_t> operator[](usize index) const noexcept { return state[index]; }

		inline ref<cell_state_t> operator[](uhalf x, uhalf y) noexcept { return state[x, y]; }

		inline cref<cell_state_t> operator[](uhalf x, uhalf y) const noexcept { return state[x, y]; }

		inline ref<cell_state_t> operator[](cref<point_t<uhalf>> position) noexcept { return state[position]; }

		inline cref<cell_state_t> operator[](cref<point_t<uhalf>> position) const noexcept { return state[position]; }

		inline bool on_x_edge(cref<point_t<uhalf>> position) const noexcept { return position.x == 0 || position.x == Width - 1; }

		inline bool on_y_edge(cref<point_t<uhalf>> position) const noexcept { return position.y == 0 || position.y == Height - 1; }

		inline bool on_edge(cref<point_t<uhalf>> position) const noexcept { return on_x_edge(position) || on_y_edge(position); }

		inline cardinal_t edge_state(cref<point_t<uhalf>> position) const noexcept {
			cardinal_t state { cardinal_t::Central };

			if (!on_edge(position)) {
				return state;
			}

			if (position.x == 0) {
				state |= cardinal_t::West;
			} else if (position.x == Width - 1) {
				state |= cardinal_t::East;
			}

			if (position.y == 0) {
				state |= cardinal_t::North;
			} else if (position.y == Height - 1) {
				state |= cardinal_t::South;
			}

			return state;
		}

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

		template<usize AtlasWidth, usize AtlasHeight> inline void draw(cref<atlas_t<AtlasWidth, AtlasHeight>> atlas) const {
			for (uhalf y { 0 }; y < Height; ++y) {
				for (uhalf x { 0 }; x < Width; ++x) {
					const usize idx { state.flatten(x, y) };
					const cell_state_t cell_state { operator[](idx) };

					if (cell_state.unexplored()) {
						continue;
					}

					const u8 rgb { cell_state.solid() ? u8 { 0xC0 } : u8 { 0x40 } };
					const u8 alpha { cell_state.seen() ? u8 { 0xFF } : u8 { 0x80 } };
					const u8 glyph { cell_state.solid() ? u8 { 0xB2 } : u8 { 0xB0 } };

					atlas.draw({ glyph, { rgb, rgb, rgb, alpha } }, { static_cast<i32>(x), static_cast<i32>(y) });
				}
			}
		}

		template<usize AtlasWidth, usize AtlasHeight> inline void draw(cref<atlas_t<AtlasWidth, AtlasHeight>> atlas, cref<point_t<i32>> offset) const {
			for (uhalf y { 0 }; y < Height; ++y) {
				for (uhalf x { 0 }; x < Width; ++x) {
					const usize idx { state.flatten(x, y) };
					const cell_state_t cell_state { operator[](idx) };

					if (cell_state.unexplored()) {
						continue;
					}

					const u8 rgb { cell_state.solid() ? u8 { 0xC0 } : u8 { 0x40 } };
					const u8 alpha { cell_state.seen() ? u8 { 0xFF } : u8 { 0x80 } };
					const u8 glyph { cell_state.solid() ? u8 { 0xB2 } : u8 { 0xB0 } };

					atlas.draw({ glyph, { rgb, rgb, rgb, alpha } }, point_t<i32> { static_cast<i32>(x), static_cast<i32>(y) } + offset);
				}
			}
		}

		inline bool serialize(cref<std::string> path, cref<std::string> name) const {
			std::ofstream file{};

			try {
				file.open(std::format("{}\\{}.map.bin", path, name), std::ios::out | std::ios::binary);

				file.write(reinterpret_cast<cstr>(state.data_ptr()), state.byte_size);

				file.close();
			}
			catch (std::exception e) {
				std::cerr << e.what() << std::endl;
				return false;
			}			

			return true;
		} 
	};

	constexpr const cell_state_t cell_state_t::Solid { 1 << 0 };
	constexpr const cell_state_t cell_state_t::Opaque { 1 << 1 };

	constexpr const cell_state_t cell_state_t::Seen { 1 << 2 };
	constexpr const cell_state_t cell_state_t::Explored { 1 << 3 };
} // namespace Bleakdepth
