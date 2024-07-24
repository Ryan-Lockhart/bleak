#pragma once

#include <bleak/typedef.hpp>

#include <array>

#include <SDL.h>

#include <bleak/cardinal.hpp>
#include <bleak/lut.hpp>

#include <bleak/constants/keys.hpp>

namespace bleak {
	namespace bindings {
		constexpr sdl::key nullkey = SDL_SCANCODE_UNKNOWN;
		template<usize Count> using key_collection_t = std::array<sdl::key, Count>;

		constexpr const lut_t<cardinal_t, key_collection_t<4>, 4> CharacterMovement { 
			pair_t<cardinal_t, key_collection_t<4>>{ cardinal_t::North, { Keys::Alpha::W, Keys::Keypad::Numeric::Seven, Keys::Keypad::Numeric::Eight, Keys::Keypad::Numeric::Nine } },
			pair_t<cardinal_t, key_collection_t<4>>{ cardinal_t::South, { Keys::Alpha::S, Keys::Keypad::Numeric::One, Keys::Keypad::Numeric::Two, Keys::Keypad::Numeric::Three } },
			pair_t<cardinal_t, key_collection_t<4>>{ cardinal_t::West, { Keys::Alpha::A, Keys::Keypad::Numeric::One, Keys::Keypad::Numeric::Four, Keys::Keypad::Numeric::Seven } },
			pair_t<cardinal_t, key_collection_t<4>>{ cardinal_t::East, { Keys::Alpha::D, Keys::Keypad::Numeric::Three, Keys::Keypad::Numeric::Six, Keys::Keypad::Numeric::Nine } }
		};
		
		constexpr const lut_t<cardinal_t, sdl::key, 4> CameraMovement { 
			pair_t<cardinal_t, sdl::key>{ cardinal_t::North, Keys::Arrow::Up },
			pair_t<cardinal_t, sdl::key>{ cardinal_t::South, Keys::Arrow::Down },
			pair_t<cardinal_t, sdl::key>{ cardinal_t::West, Keys::Arrow::Left },
			pair_t<cardinal_t, sdl::key>{ cardinal_t::East, Keys::Arrow::Right }
		};

		constexpr const sdl::key Wait{ Keys::Space };
		constexpr const sdl::key Quit{ Keys::Escape };
	} // namespace Bindings
} // namespace bleak
