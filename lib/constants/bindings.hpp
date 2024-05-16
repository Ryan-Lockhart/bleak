#pragma once

#include "lut.hpp"

#include "keys.hpp"

#include "cardinal.hpp"
#include <array>

namespace Bleakdepth {
	namespace Bindings {
		constexpr const lut_t<cardinal_t, std::array<Keys::key_t, 2>, 4> CharacterMovement {
			{ cardinal_t::North, { Keys::Alpha::W, Keys::Keypad::Numeric::Eight } },
			{ cardinal_t::South, { Keys::Alpha::S, Keys::Keypad::Numeric::Two } },
			{ cardinal_t::West, { Keys::Alpha::A, Keys::Keypad::Numeric::Four } },
			{ cardinal_t::East, { Keys::Alpha::D, Keys::Keypad::Numeric::Six } }
		};
		constexpr const lut_t<cardinal_t, Keys::key_t, 4> CameraMovement {
			{ cardinal_t::North, Keys::Arrow::Up },
			{ cardinal_t::South, Keys::Arrow::Down },
			{ cardinal_t::West, Keys::Arrow::Left },
			{ cardinal_t::East, Keys::Arrow::Right }
		};
	}
} // namespace Bleakdepth
