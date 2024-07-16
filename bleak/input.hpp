#pragma once

#include <bleak/typedef.hpp>

#include <string>

namespace bleak {
	struct input_state_t {
	  private:
		u8 value { 0 };

	  public:
		constexpr input_state_t() {}

		constexpr input_state_t(u8 value) : value { value } {}

		static constexpr u8 Pressed { 0 };
		static constexpr u8 Released { 1 };
		static constexpr u8 Down { 2 };
		static constexpr u8 Up { 3 };

		constexpr operator u8() const { return value; }

		constexpr bool operator==(input_state_t other) const { return value == other.value; }

		constexpr bool operator==(u8 other) const { return value == other; }

		constexpr bool operator!=(input_state_t other) const { return value != other.value; }

		constexpr bool operator!=(u8 other) const { return value != other; }

		constexpr operator std::string() const {
			switch (value) {
			case 0:
				return "Pressed";
			case 1:
				return "Released";
			case 2:
				return "Down";
			case 3:
				return "Up";
			default:
				return "Unknown";
			}
		}
	};
} // namespace Bleakdepth
