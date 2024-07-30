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
			pair_t<cardinal_t, key_collection_t<4>>{ cardinal_t::North, { keys::Alpha::W, keys::Keypad::Numeric::Seven, keys::Keypad::Numeric::Eight, keys::Keypad::Numeric::Nine } },
			pair_t<cardinal_t, key_collection_t<4>>{ cardinal_t::South, { keys::Alpha::S, keys::Keypad::Numeric::One, keys::Keypad::Numeric::Two, keys::Keypad::Numeric::Three } },
			pair_t<cardinal_t, key_collection_t<4>>{ cardinal_t::West, { keys::Alpha::A, keys::Keypad::Numeric::One, keys::Keypad::Numeric::Four, keys::Keypad::Numeric::Seven } },
			pair_t<cardinal_t, key_collection_t<4>>{ cardinal_t::East, { keys::Alpha::D, keys::Keypad::Numeric::Three, keys::Keypad::Numeric::Six, keys::Keypad::Numeric::Nine } }
		};

		constexpr const std::array<sdl::key, 2> Wait{ keys::Space, keys::Keypad::Numeric::Five };
		
		constexpr const lut_t<cardinal_t, sdl::key, 4> CameraMovement { 
			pair_t<cardinal_t, sdl::key>{ cardinal_t::North, keys::Arrow::Up },
			pair_t<cardinal_t, sdl::key>{ cardinal_t::South, keys::Arrow::Down },
			pair_t<cardinal_t, sdl::key>{ cardinal_t::West, keys::Arrow::Left },
			pair_t<cardinal_t, sdl::key>{ cardinal_t::East, keys::Arrow::Right }
		};

		constexpr const sdl::key Quit{ keys::Escape };

		constexpr const sdl::key CameraLock{ keys::Tab };
		constexpr const sdl::key RevealMap{ keys::Grave };
	} // namespace Bindings
} // namespace bleak
