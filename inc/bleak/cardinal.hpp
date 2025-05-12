#pragma once

#include <bleak/typedef.hpp>

#include <string>

namespace bleak {
	enum struct cardinal_e : u8 {
		Central = 0,

		North = 1 << 0,
		South = 1 << 1,

		East = 1 << 2,
		West = 1 << 3,

		Northwest = North | West,
		Northeast = North | East,

		Southwest = South | West,
		Southeast = South | East,
	};

	constexpr cardinal_e operator~(cardinal_e value) noexcept { return static_cast<cardinal_e>(~static_cast<u8>(value)); }

	constexpr cardinal_e operator|(cardinal_e lhs, cardinal_e rhs) noexcept { return static_cast<cardinal_e>(static_cast<u8>(lhs) | static_cast<u8>(rhs)); }

	constexpr cardinal_e operator&(cardinal_e lhs, cardinal_e rhs) noexcept { return static_cast<cardinal_e>(static_cast<u8>(lhs) & static_cast<u8>(rhs)); }

	constexpr cardinal_e operator^(cardinal_e lhs, cardinal_e rhs) noexcept { return static_cast<cardinal_e>(static_cast<u8>(lhs) ^ static_cast<u8>(rhs)); }

	constexpr cardinal_e operator+(cardinal_e lhs, cardinal_e rhs) noexcept { return static_cast<cardinal_e>(static_cast<u8>(lhs) + static_cast<u8>(rhs)); }

	constexpr cardinal_e operator-(cardinal_e lhs, cardinal_e rhs) noexcept { return static_cast<cardinal_e>(static_cast<u8>(lhs) - static_cast<u8>(rhs)); }

	constexpr ref<cardinal_e> operator|=(ref<cardinal_e> lhs, cardinal_e rhs) noexcept { return lhs = lhs | rhs; }

	constexpr ref<cardinal_e> operator&=(ref<cardinal_e> lhs, cardinal_e rhs) noexcept { return lhs = lhs & rhs; }

	constexpr ref<cardinal_e> operator^=(ref<cardinal_e> lhs, cardinal_e rhs) noexcept { return lhs = lhs ^ rhs; }

	constexpr ref<cardinal_e> operator+=(ref<cardinal_e> lhs, cardinal_e rhs) noexcept { return lhs = lhs + rhs; }

	constexpr ref<cardinal_e> operator-=(ref<cardinal_e> lhs, cardinal_e rhs) noexcept { return lhs = lhs - rhs; }

	constexpr bool operator==(cardinal_e lhs, cardinal_e rhs) noexcept { return static_cast<u8>(lhs) == static_cast<u8>(rhs); }

	constexpr bool operator!=(cardinal_e lhs, cardinal_e rhs) noexcept { return static_cast<u8>(lhs) != static_cast<u8>(rhs); }
}

extern "C" {
	typedef struct c_cardinal_t {
		union {
			struct {
				bool north : 1 { false };
				bool south : 1 { false };

				bool east : 1 { false };
				bool west : 1 { false };

				bool : 4;
			};

			bleak::cardinal_e value;
		};
	} c_cardinal_t;

	static_assert(sizeof(c_cardinal_t) == 1, "size of c_cardinal_t must be one byte");
}

namespace bleak {
	struct cardinal_t : public c_cardinal_t {
		constexpr cardinal_t() : c_cardinal_t {} {}

		constexpr cardinal_t(cardinal_e value) : c_cardinal_t { .value = value } {}

		constexpr cardinal_t operator-() const noexcept { return cardinal_t { static_cast<cardinal_e>(~value) }; }

		constexpr cardinal_t operator+(cardinal_t other) const noexcept {
			cardinal_t sum { static_cast<cardinal_e>(value | other.value) };

			sum.neutralize();

			return sum;
		}

		constexpr cardinal_t operator-(cardinal_t other) const noexcept {
			cardinal_t diff { static_cast<cardinal_e>(value & ~other.value) };

			diff.neutralize();

			return diff;
		}

		constexpr ref<cardinal_t> operator+=(cardinal_t other) noexcept {
			if (*this == other) {
				return *this;
			}

			value = static_cast<cardinal_e>(value + other.value);

			neutralize();

			return *this;
		}

		constexpr ref<cardinal_t> operator-=(cardinal_t other) noexcept {
			if (*this == other) {
				return *this;
			}

			value = static_cast<cardinal_e>(value - other.value);

			neutralize();

			return *this;
		}

		constexpr bool operator==(cardinal_t other) const noexcept { return value == other.value; }

		constexpr bool operator!=(cardinal_t other) const noexcept { return value != other.value; }

		constexpr bool operator==(cardinal_e other) const noexcept { return value == other; }

		constexpr bool operator!=(cardinal_e other) const noexcept { return value != other; }

		constexpr bool is_long_neutral() const noexcept { return (north && south) || (!north && !south); }

		constexpr bool is_lat_neutral() const noexcept { return (east && west) || (!east && !west); }

		constexpr ref<cardinal_t> clear_long() noexcept {
			north = south = false;

			return *this;
		}

		constexpr ref<cardinal_t> clear_lat() noexcept {
			east = west = false;

			return *this;
		}

		constexpr ref<cardinal_t> neutralize() noexcept {
			if (is_long_neutral()) {
				clear_long();
			}

			if (is_lat_neutral()) {
				clear_lat();
			}

			return *this;
		}

		constexpr operator cardinal_e() const noexcept { return value; }

		constexpr explicit operator std::string() const noexcept {
			switch (value) {
			case cardinal_e::Central:
				return "central";
			case cardinal_e::North:
				return "north";
			case cardinal_e::East:
				return "east";
			case cardinal_e::South:
				return "south";
			case cardinal_e::West:
				return "west";
			case cardinal_e::Northwest:
				return "northwest";
			case cardinal_e::Northeast:
				return "northeast";
			case cardinal_e::Southwest:
				return "southwest";
			case cardinal_e::Southeast:
				return "southeast";
			default:
				return "unknown";
			}
		}
	};
} // namespace bleak
