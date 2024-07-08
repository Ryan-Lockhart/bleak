#pragma once

#include "bleak/typedef.hpp"

#include <string>

extern "C" {
	typedef struct c_cardinal_t {
		union {
			struct {
				bool north : 1 { false };
				bool south : 1 { false };

				bool east : 1 { false };
				bool west : 1 { false };

				bool up : 1 { false };
				bool down : 1 { false };

				bool : 2;
			};

			unsigned char value;
		};
	} c_cardinal_t;

	static_assert(sizeof(c_cardinal_t) == 1, "size of c_cardinal_t must be one byte");
}

namespace bleak {
	struct cardinal_t : public c_cardinal_t {
		static constexpr u8 Central { 0 };

		static constexpr u8 North { 1 << 0 };
		static constexpr u8 South { 1 << 1 };

		static constexpr u8 East { 1 << 2 };
		static constexpr u8 West { 1 << 3 };

		static constexpr u8 Up { 1 << 4 };
		static constexpr u8 Down { 1 << 5 };

		static constexpr u8 Northwest { cardinal_t::North | cardinal_t::West };
		static constexpr u8 Northeast { cardinal_t::North | cardinal_t::East };

		static constexpr u8 Southwest { cardinal_t::South | cardinal_t::West };
		static constexpr u8 Southeast { cardinal_t::South | cardinal_t::East };

		struct Vertical {
			struct Up {
				static constexpr u8 Central { cardinal_t::Up };

				static constexpr u8 North { cardinal_t::Up | cardinal_t::North };
				static constexpr u8 South { cardinal_t::Up | cardinal_t::South };

				static constexpr u8 East { cardinal_t::Up | cardinal_t::East };
				static constexpr u8 West { cardinal_t::Up | cardinal_t::West };

				static constexpr u8 Northwest { cardinal_t::Up | cardinal_t::North | cardinal_t::West };
				static constexpr u8 Northeast { cardinal_t::Up | cardinal_t::North | cardinal_t::East };

				static constexpr u8 Southwest { cardinal_t::Up | cardinal_t::South | cardinal_t::West };
				static constexpr u8 Southeast { cardinal_t::Up | cardinal_t::South | cardinal_t::East };
			};

			struct Down {
				static constexpr u8 Central { cardinal_t::Down };

				static constexpr u8 North { cardinal_t::Down | cardinal_t::North };
				static constexpr u8 South { cardinal_t::Down | cardinal_t::South };

				static constexpr u8 East { cardinal_t::Down | cardinal_t::East };
				static constexpr u8 West { cardinal_t::Down | cardinal_t::West };

				static constexpr u8 Northwest { cardinal_t::Down | cardinal_t::North | cardinal_t::West };
				static constexpr u8 Northeast { cardinal_t::Down | cardinal_t::North | cardinal_t::East };

				static constexpr u8 Southwest { cardinal_t::Down | cardinal_t::South | cardinal_t::West };
				static constexpr u8 Southeast { cardinal_t::Down | cardinal_t::South | cardinal_t::East };
			};
		};

		constexpr cardinal_t() : c_cardinal_t {} {}

		constexpr cardinal_t(u8 value) : c_cardinal_t { .value = value } {}

		constexpr cardinal_t operator-() const noexcept { return cardinal_t { (u8) ~(u8) * this }; }

		constexpr cardinal_t operator+(cref<cardinal_t> other) const noexcept {
			cardinal_t sum { static_cast<cardinal_t>(*this | other) };

			sum.neutralize();

			return sum;
		}

		constexpr cardinal_t operator-(cref<cardinal_t> other) const noexcept {
			cardinal_t diff { static_cast<cardinal_t>(*this & ~other) };

			diff.neutralize();

			return diff;
		}

		constexpr ref<cardinal_t> operator+=(cref<cardinal_t> other) noexcept {
			if (*this == other) {
				return *this;
			}

			return *this = cardinal_t { *this + other };
		}

		constexpr ref<cardinal_t> operator-=(cref<cardinal_t> other) noexcept {
			if (*this == other) {
				return *this;
			}

			return *this = cardinal_t { *this - other };
		}

		constexpr bool operator==(cardinal_t other) const noexcept { return (u8) * this == (u8)other; }

		constexpr bool operator!=(cardinal_t other) const noexcept { return (u8) * this != (u8)other; }

		constexpr bool operator==(u8 other) const noexcept { return (u8) * this == other; }

		constexpr bool operator!=(u8 other) const noexcept { return (u8) * this != other; }

		constexpr bool is_3d() const noexcept { return up || down; }

		constexpr bool is_long_neutral() const noexcept { return north && south; }

		constexpr bool is_lat_neutral() const noexcept { return east && west; }

		constexpr bool is_vert_neutral() const noexcept { return up && down; }

		constexpr void clear_long() noexcept { north = south = false; }

		constexpr void clear_lat() noexcept { east = west = false; }

		constexpr void clear_vert() noexcept { up = down = false; }

		constexpr void neutralize() noexcept {
			if (is_long_neutral()) {
				clear_long();
			}

			if (is_lat_neutral()) {
				clear_lat();
			}

			if (is_vert_neutral()) {
				clear_vert();
			}
		}

		constexpr operator u8() const noexcept { return value; }

		constexpr operator std::string() const noexcept {
			switch (*this) {
			case cardinal_t::Central:
				return "central";
			case cardinal_t::North:
				return "north";
			case cardinal_t::East:
				return "east";
			case cardinal_t::South:
				return "south";
			case cardinal_t::West:
				return "west";
			case cardinal_t::Northwest:
				return "northwest";
			case cardinal_t::Northeast:
				return "northeast";
			case cardinal_t::Southwest:
				return "southwest";
			case cardinal_t::Southeast:
				return "southeast";
			case cardinal_t::Vertical::Up::Central:
				return "ascending";
			case cardinal_t::Vertical::Up::North:
				return "ascending north";
			case cardinal_t::Vertical::Up::East:
				return "ascending east";
			case cardinal_t::Vertical::Up::South:
				return "ascending south";
			case cardinal_t::Vertical::Up::West:
				return "ascending west";
			case cardinal_t::Vertical::Up::Northwest:
				return "ascending northwest";
			case cardinal_t::Vertical::Up::Northeast:
				return "ascending northeast";
			case cardinal_t::Vertical::Up::Southwest:
				return "ascending southwest";
			case cardinal_t::Vertical::Up::Southeast:
				return "ascending southeast";
			case cardinal_t::Vertical::Down::Central:
				return "descending";
			case cardinal_t::Vertical::Down::North:
				return "descending north";
			case cardinal_t::Vertical::Down::East:
				return "descending east";
			case cardinal_t::Vertical::Down::South:
				return "descending south";
			case cardinal_t::Vertical::Down::West:
				return "descending west";
			case cardinal_t::Vertical::Down::Northwest:
				return "descending northwest";
			case cardinal_t::Vertical::Down::Northeast:
				return "descending northeast";
			case cardinal_t::Vertical::Down::Southwest:
				return "descending southwest";
			case cardinal_t::Vertical::Down::Southeast:
				return "descending southeast";
			default:
				return "unknown";
			}
		}
	};
} // namespace bleak
