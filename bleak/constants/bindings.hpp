#pragma once

#include <array>

#include "SDL_scancode.h"
#include "bleak/cardinal.hpp"
#include "bleak/lut.hpp"

#include "bleak/constants/keys.hpp"

namespace bleak {
	namespace Bindings {
		constexpr Keys::key_t nullkey = SDL_SCANCODE_UNKNOWN;
		template<usize Count> using key_collection_t = std::array<Keys::key_t, Count>;

		constexpr const lut_t<cardinal_t, key_collection_t<4>, 4> CharacterMovement { 
			{ cardinal_t::North, { Keys::Alpha::W, Keys::Keypad::Numeric::Seven, Keys::Keypad::Numeric::Eight, Keys::Keypad::Numeric::Nine } },
			{ cardinal_t::South, { Keys::Alpha::S, Keys::Keypad::Numeric::One, Keys::Keypad::Numeric::Two, Keys::Keypad::Numeric::Three } },
			{ cardinal_t::West, { Keys::Alpha::A, Keys::Keypad::Numeric::One, Keys::Keypad::Numeric::Four, Keys::Keypad::Numeric::Seven } },
			{ cardinal_t::East, { Keys::Alpha::D, Keys::Keypad::Numeric::Three, Keys::Keypad::Numeric::Six, Keys::Keypad::Numeric::Nine } }
		};
		
		constexpr const lut_t<cardinal_t, Keys::key_t, 4> CameraMovement { 
			{ cardinal_t::North, Keys::Arrow::Up },
			{ cardinal_t::South, Keys::Arrow::Down },
			{ cardinal_t::West, Keys::Arrow::Left },
			{ cardinal_t::East, Keys::Arrow::Right }
		};

		constexpr const Keys::key_t Wait{ Keys::Space };
		constexpr const Keys::key_t Quit{ Keys::Escape };
	} // namespace Bindings
} // namespace bleak
