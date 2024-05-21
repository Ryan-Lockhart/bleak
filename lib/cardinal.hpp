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

		constexpr cardinal_t operator~() const noexcept { return ~value; }

		constexpr cardinal_t operator|(cref<cardinal_t> other) const noexcept { return value | other.value; }

		constexpr cardinal_t operator&(cref<cardinal_t> other) const noexcept { return value & other.value; }

		constexpr cardinal_t operator^(cref<cardinal_t> other) const noexcept { return value ^ other.value; }

		constexpr cardinal_t operator<<(cref<cardinal_t> other) const noexcept { return value << other.value; }

		constexpr cardinal_t operator>>(cref<cardinal_t> other) const noexcept { return value >> other.value; }

		constexpr bool contains(cref<cardinal_t> other) const noexcept { return value & other; }

		constexpr ref<cardinal_t> operator|=(cref<cardinal_t> other) noexcept {
			value |= other.value;
			return *this;
		}

		constexpr ref<cardinal_t> operator&=(cref<cardinal_t> other) noexcept {
			value &= other.value;
			return *this;
		}

		constexpr ref<cardinal_t> operator^=(cref<cardinal_t> other) noexcept {
			value ^= other.value;
			return *this;
		}

		constexpr ref<cardinal_t> operator<<=(cref<cardinal_t> other) noexcept {
			value <<= other.value;
			return *this;
		}

		constexpr ref<cardinal_t> operator>>=(cref<cardinal_t> other) noexcept {
			value >>= other.value;
			return *this;
		}

		constexpr bool operator==(cref<cardinal_t> other) const noexcept { return value == other.value; }

		constexpr bool operator!=(cref<cardinal_t> other) const noexcept { return value != other.value; }

		constexpr bool is_3d() const noexcept { return (value & East) || (value & West); }

		constexpr bool is_long_neutral() const noexcept { return (value & North) && (value & South); }

		constexpr bool is_lat_neutral() const noexcept { return (value & East) && (value & West); }

		constexpr bool is_vert_neutral() const noexcept { return (value & Up) && (value & Down); }

		constexpr void clear_long() noexcept { value &= ~(North | South); }

		constexpr void clear_lat() noexcept { value &= ~(East | West); }

		constexpr void clear_vert() noexcept { value &= ~(Up | Down); }

		constexpr operator u8() const noexcept { return value; }

		constexpr operator std::string() const noexcept;
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

	constexpr inline cardinal_t::operator std::string() const noexcept {
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
			return "Ascending";
		case cardinal_t::Vertical::Up::North:
			return "Ascending North";
		case cardinal_t::Vertical::Up::East:
			return "Ascending East";
		case cardinal_t::Vertical::Up::South:
			return "Ascending South";
		case cardinal_t::Vertical::Up::West:
			return "Ascending West";
		case cardinal_t::Vertical::Up::Northwest:
			return "Ascending Northwest";
		case cardinal_t::Vertical::Up::Northeast:
			return "Ascending Northeast";
		case cardinal_t::Vertical::Up::Southwest:
			return "Ascending Southwest";
		case cardinal_t::Vertical::Up::Southeast:
			return "Ascending Southeast";
		case cardinal_t::Vertical::Down::Central:
			return "Descending";
		case cardinal_t::Vertical::Down::North:
			return "Descending North";
		case cardinal_t::Vertical::Down::East:
			return "Descending East";
		case cardinal_t::Vertical::Down::South:
			return "Descending South";
		case cardinal_t::Vertical::Down::West:
			return "Descending West";
		case cardinal_t::Vertical::Down::Northwest:
			return "Descending Northwest";
		case cardinal_t::Vertical::Down::Northeast:
			return "Descending Northeast";
		case cardinal_t::Vertical::Down::Southwest:
			return "Descending Southwest";
		case cardinal_t::Vertical::Down::Southeast:
			return "Descending Southeast";
		default:
			return "Unknown";
		}
	}
} // namespace Bleakdepth
