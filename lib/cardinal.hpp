#pragma once

#include "typedef.hpp"

#include <string>

namespace Bleakdepth {
	struct cardinal_t {
	  private:
		u8 value;

	  public:
		constexpr cardinal_t() : value(Central) {}

		constexpr cardinal_t(u8 value) : value(value) {}

		static const cardinal_t Central;

		static const cardinal_t North;
		static const cardinal_t South;

		static const cardinal_t East;
		static const cardinal_t West;

		static const cardinal_t Up;
		static const cardinal_t Down;

		static const cardinal_t Northwest;
		static const cardinal_t Northeast;

		static const cardinal_t Southwest;
		static const cardinal_t Southeast;

		struct Vertical {
			struct Up {
				static const cardinal_t Central;

				static const cardinal_t North;
				static const cardinal_t South;

				static const cardinal_t West;
				static const cardinal_t East;

				static const cardinal_t Northwest;
				static const cardinal_t Northeast;

				static const cardinal_t Southwest;
				static const cardinal_t Southeast;
			};

			struct Down {
				static const cardinal_t Central;

				static const cardinal_t North;
				static const cardinal_t South;

				static const cardinal_t West;
				static const cardinal_t East;

				static const cardinal_t Northwest;
				static const cardinal_t Northeast;

				static const cardinal_t Southwest;
				static const cardinal_t Southeast;
			};
		};

		constexpr cardinal_t operator~() const { return ~value; }

		constexpr cardinal_t operator|(cref<cardinal_t> other) const { return value | other.value; }

		constexpr cardinal_t operator&(cref<cardinal_t> other) const { return value & other.value; }

		constexpr cardinal_t operator^(cref<cardinal_t> other) const { return value ^ other.value; }

		constexpr cardinal_t operator<<(cref<cardinal_t> other) const { return value << other.value; }

		constexpr cardinal_t operator>>(cref<cardinal_t> other) const { return value >> other.value; }

		constexpr ref<cardinal_t> operator|=(cref<cardinal_t> other) {
			value |= other.value;
			return *this;
		}

		constexpr ref<cardinal_t> operator&=(cref<cardinal_t> other) {
			value &= other.value;
			return *this;
		}

		constexpr ref<cardinal_t> operator^=(cref<cardinal_t> other) {
			value ^= other.value;
			return *this;
		}

		constexpr ref<cardinal_t> operator<<=(cref<cardinal_t> other) {
			value <<= other.value;
			return *this;
		}

		constexpr ref<cardinal_t> operator>>=(cref<cardinal_t> other) {
			value >>= other.value;
			return *this;
		}

		constexpr bool operator==(cref<cardinal_t> other) const { return value == other.value; }

		constexpr bool operator!=(cref<cardinal_t> other) const { return value != other.value; }

		constexpr operator u8() const { return value; }

		constexpr operator std::string() const;
	};

	constexpr const cardinal_t cardinal_t::Central { 0 };

	constexpr const cardinal_t cardinal_t::North { 1 << 0 };
	constexpr const cardinal_t cardinal_t::East { 1 << 1 };
	constexpr const cardinal_t cardinal_t::South { 1 << 2 };
	constexpr const cardinal_t cardinal_t::West { 1 << 3 };
	constexpr const cardinal_t cardinal_t::Up { 1 << 4 };
	constexpr const cardinal_t cardinal_t::Down { 1 << 5 };

	constexpr const cardinal_t cardinal_t::Northwest { cardinal_t::North | cardinal_t::West };
	constexpr const cardinal_t cardinal_t::Northeast { cardinal_t::North | cardinal_t::East };

	constexpr const cardinal_t cardinal_t::Southwest { cardinal_t::South | cardinal_t::West };
	constexpr const cardinal_t cardinal_t::Southeast { cardinal_t::South | cardinal_t::East };

	constexpr const cardinal_t cardinal_t::Vertical::Up::Central { cardinal_t::Central | cardinal_t::Up };

	constexpr const cardinal_t cardinal_t::Vertical::Up::North { cardinal_t::North | cardinal_t::Up };
	constexpr const cardinal_t cardinal_t::Vertical::Up::South { cardinal_t::South | cardinal_t::Up };

	constexpr const cardinal_t cardinal_t::Vertical::Up::West { cardinal_t::West | cardinal_t::Up };
	constexpr const cardinal_t cardinal_t::Vertical::Up::East { cardinal_t::East | cardinal_t::Up };

	constexpr const cardinal_t cardinal_t::Vertical::Up::Northwest { cardinal_t::Northwest | cardinal_t::Up };
	constexpr const cardinal_t cardinal_t::Vertical::Up::Northeast { cardinal_t::Northeast | cardinal_t::Up };

	constexpr const cardinal_t cardinal_t::Vertical::Up::Southwest { cardinal_t::Southwest | cardinal_t::Up };
	constexpr const cardinal_t cardinal_t::Vertical::Up::Southeast { cardinal_t::Southeast | cardinal_t::Up };

	constexpr const cardinal_t cardinal_t::Vertical::Down::Central { cardinal_t::Central | cardinal_t::Down };

	constexpr const cardinal_t cardinal_t::Vertical::Down::North { cardinal_t::North | cardinal_t::Down };
	constexpr const cardinal_t cardinal_t::Vertical::Down::South { cardinal_t::South | cardinal_t::Down };

	constexpr const cardinal_t cardinal_t::Vertical::Down::West { cardinal_t::West | cardinal_t::Down };
	constexpr const cardinal_t cardinal_t::Vertical::Down::East { cardinal_t::East | cardinal_t::Down };

	constexpr const cardinal_t cardinal_t::Vertical::Down::Northwest { cardinal_t::Northwest | cardinal_t::Down };
	constexpr const cardinal_t cardinal_t::Vertical::Down::Northeast { cardinal_t::Northeast | cardinal_t::Down };

	constexpr const cardinal_t cardinal_t::Vertical::Down::Southwest { cardinal_t::Southwest | cardinal_t::Down };
	constexpr const cardinal_t cardinal_t::Vertical::Down::Southeast { cardinal_t::Southeast | cardinal_t::Down };

	constexpr inline cardinal_t::operator std::string() const {
		switch (value) {
		case cardinal_t::Central:
			return "Central";
		case cardinal_t::North:
			return "North";
		case cardinal_t::East:
			return "East";
		case cardinal_t::South:
			return "South";
		case cardinal_t::West:
			return "West";
		case cardinal_t::Northwest:
			return "Northwest";
		case cardinal_t::Northeast:
			return "Northeast";
		case cardinal_t::Southwest:
			return "Southwest";
		case cardinal_t::Southeast:
			return "Southeast";
		case cardinal_t::Vertical::Up::Central:
			return "Central Ascending";
		case cardinal_t::Vertical::Up::North:
			return "North Ascending";
		case cardinal_t::Vertical::Up::East:
			return "East Ascending";
		case cardinal_t::Vertical::Up::South:
			return "South Ascending";
		case cardinal_t::Vertical::Up::West:
			return "West Ascending";
		case cardinal_t::Vertical::Up::Northwest:
			return "Northwest Ascending";
		case cardinal_t::Vertical::Up::Northeast:
			return "Northeast Ascending";
		case cardinal_t::Vertical::Up::Southwest:
			return "Southwest Ascending";
		case cardinal_t::Vertical::Up::Southeast:
			return "Southeast Ascending";
		case cardinal_t::Vertical::Down::Central:
			return "Central Descending";
		case cardinal_t::Vertical::Down::North:
			return "North Descending";
		case cardinal_t::Vertical::Down::East:
			return "East Descending";
		case cardinal_t::Vertical::Down::South:
			return "South Descending";
		case cardinal_t::Vertical::Down::West:
			return "West Descending";
		case cardinal_t::Vertical::Down::Northwest:
			return "Northwest Descending";
		case cardinal_t::Vertical::Down::Northeast:
			return "Northeast Descending";
		case cardinal_t::Vertical::Down::Southwest:
			return "Southwest Descending";
		case cardinal_t::Vertical::Down::Southeast:
			return "Southeast Descending";
		default:
			return "Unknown";
		}
	}
} // namespace Bleakdepth
