#pragma once

#include "coord.hpp"
#include "typedef.hpp"

#include <string>

namespace Bleakdepth {
	struct cardinal_t {
	  private:
		u8 value;

		static const cardinal_t Central;

		static const cardinal_t North;
		static const cardinal_t East;
		static const cardinal_t South;
		static const cardinal_t West;
		static const cardinal_t Up;
		static const cardinal_t Down;

	  public:
		constexpr cardinal_t() : value(Central) {}

		constexpr cardinal_t(u8 value) : value(value) {}

		struct Horizontal {
			static const cardinal_t Central;

			static const cardinal_t North;
			static const cardinal_t East;
			static const cardinal_t South;
			static const cardinal_t West;
			static const cardinal_t Up;
			static const cardinal_t Down;

			static const cardinal_t Northwest;
			static const cardinal_t Northeast;

			static const cardinal_t Southwest;
			static const cardinal_t Southeast;
		};

		struct Vertical {
			struct Up {
				static const cardinal_t Central;

				static const cardinal_t North;
				static const cardinal_t East;
				static const cardinal_t South;
				static const cardinal_t West;

				static const cardinal_t Northwest;
				static const cardinal_t Northeast;

				static const cardinal_t Southwest;
				static const cardinal_t Southeast;
			};

			struct Down {
				static const cardinal_t Central;

				static const cardinal_t North;
				static const cardinal_t East;
				static const cardinal_t South;
				static const cardinal_t West;

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

	constexpr inline cardinal_t::operator std::string() const {
		switch (value) {
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
		default:
			return "Central";
		}
	}
} // namespace Bleakdepth
