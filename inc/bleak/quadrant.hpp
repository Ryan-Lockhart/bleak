#pragma once

#include <bleak/typedef.hpp>

#include <string>

#include <bleak/cardinal.hpp>

extern "C" {
	typedef struct c_quadrant_t {
		union {
			struct {
				bool upper : 1 { false };
				bool left : 1 { false };
				bool : 6;
			};

			bleak::u8 value;
		};
	} c_quadrant_t;

	static_assert(sizeof(c_quadrant_t) == 1, "size of c_quadrant_t must be one byte");
}

namespace bleak {
	struct quadrant_t : public c_quadrant_t {
	  public:
		static constexpr u8 Upper{ 1 << 0 };
		static constexpr u8 Left{ 1 << 1 };

		static constexpr u8 UpperLeft{ quadrant_t::Upper | quadrant_t::Left };
		static constexpr u8 LowerLeft{ quadrant_t::Left };

		static constexpr u8 UpperRight{ quadrant_t::Upper };
		static constexpr u8 LowerRight{ 0 };

		constexpr quadrant_t() noexcept {}

		constexpr quadrant_t(bool upper, bool left) noexcept : c_quadrant_t{ .upper = upper, .left = left } {}

		constexpr quadrant_t(c_quadrant_t quadrant) noexcept : c_quadrant_t{ .value = quadrant.value } {}

		constexpr quadrant_t(cardinal_t direction) noexcept {
			switch (static_cast<u8>(direction)) {
			case cardinal_t::Southeast:
				upper = false;
				left = false;
				break;
			case cardinal_t::Northeast:
				upper = true;
				left = false;
				break;
			case cardinal_t::Southwest:
				upper = false;
				left = true;
				break;
			case cardinal_t::Northwest:
				upper = true;
				left = true;
				break;
			default:
				upper = false;
				left = false;
				break;
			}
		}

		constexpr bool operator==(cref<quadrant_t> other) const noexcept { return value == other.value; }

		constexpr bool operator!=(cref<quadrant_t> other) const noexcept { return value != other.value; }

		constexpr operator u8() const noexcept { return value; }

		constexpr operator cardinal_t() const noexcept {
			if (upper) {
				if (left) {
					return cardinal_t::Northwest;
				} else {
					return cardinal_t::Northeast;
				}
			} else {
				if (left) {
					return cardinal_t::Southwest;
				} else {
					return cardinal_t::Southeast;
				}
			}
		}

		constexpr operator std::string() const noexcept {
			switch (value) {
			case UpperLeft:
				return "upper-left";
			case LowerLeft:
				return "lower-left";
			case UpperRight:
				return "upper-right";
			case LowerRight:
				return "lower-right";
			default:
				return "unknown";
			}
		}
	};
} // namespace bleak
