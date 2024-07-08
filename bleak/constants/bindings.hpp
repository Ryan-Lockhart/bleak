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

		constexpr const lut_t<cardinal_t, key_collection_t<2>, 4> CharacterMovement { 
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
	} // namespace Bindings
} // namespace bleak
