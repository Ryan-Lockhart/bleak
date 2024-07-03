#pragma once

#include "typedef.hpp"

#include <string>

namespace Bleakdepth {
	struct cardinal_t {
		bool north : 1;
		bool south : 1;

		bool east : 1;
		bool west : 1;

		bool up : 1;
		bool down : 1;

	  private:
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
		// this is necessary to align the struct to 1 byte, otherwise you'll get random garbage in the unused bits
		bool unused : 2 { false };
#pragma clang diagnostic pop

	  public:
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

		constexpr cardinal_t() : north { false }, south { false }, east { false }, west { false }, up { false }, down { false } {}

		constexpr cardinal_t(u8 value) :
			north { static_cast<bool>(value & cardinal_t::North) },
			south { static_cast<bool>(value & cardinal_t::South) },
			east { static_cast<bool>(value & cardinal_t::East) },
			west { static_cast<bool>(value & cardinal_t::West) },
			up { static_cast<bool>(value & cardinal_t::Up) },
			down { static_cast<bool>(value & cardinal_t::Down) } {}

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

		constexpr operator u8() const noexcept { return *(cptrc<u8>)this; }

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
} // namespace Bleakdepth
