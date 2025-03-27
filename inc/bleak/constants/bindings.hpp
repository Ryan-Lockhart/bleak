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
			pair_t<cardinal_t, key_collection_t<4>>{
				cardinal_e::North,
				key_collection_t<4>{
					keys::Alpha::W,
					keys::Keypad::Numeric::Seven,
					keys::Keypad::Numeric::Eight,
					keys::Keypad::Numeric::Nine
				}
			}, pair_t<cardinal_t, key_collection_t<4>>{
				cardinal_e::South,
				key_collection_t<4>{
					keys::Alpha::S,
					keys::Keypad::Numeric::One,
					keys::Keypad::Numeric::Two,
					keys::Keypad::Numeric::Three
				}
			}, pair_t<cardinal_t, key_collection_t<4>>{
				cardinal_e::West,
				key_collection_t<4>{
					keys::Alpha::A,
					keys::Keypad::Numeric::One,
					keys::Keypad::Numeric::Four,
					keys::Keypad::Numeric::Seven
				}
			}, pair_t<cardinal_t, key_collection_t<4>>{
				cardinal_e::East,
				key_collection_t<4>{
					keys::Alpha::D,
					keys::Keypad::Numeric::Three,
					keys::Keypad::Numeric::Six,
					keys::Keypad::Numeric::Nine
				}
			}
		};

		constexpr const std::array<sdl::key, 2> Wait{
			keys::Space,
			keys::Keypad::Numeric::Five
		};
		
		constexpr const lut_t<cardinal_t, sdl::key, 4> CameraMovement { 
			pair_t<cardinal_t, sdl::key>{
				cardinal_e::North,
				keys::Arrow::Up
			}, pair_t<cardinal_t, sdl::key>{
				cardinal_e::South,
				keys::Arrow::Down
			}, pair_t<cardinal_t, sdl::key>{
				cardinal_e::West,
				keys::Arrow::Left
			}, pair_t<cardinal_t, sdl::key>{
				cardinal_e::East,
				keys::Arrow::Right
			}
		};

		constexpr const sdl::key Quit{ keys::Escape };
		
		constexpr const sdl::key ToggleFullscreen{ keys::Function::Ten };
		constexpr const sdl::key ToggleControls{ keys::Function::One };

		constexpr const sdl::key CameraLock{ keys::Tab };
		constexpr const sdl::key RevealMap{ keys::Grave };

		constexpr const sdl::key RandomWarp{ keys::Alpha::Q };
		constexpr const sdl::key TargetWarp{ keys::Alpha::E };
		
		constexpr const sdl::key CalciticInvocation{ keys::Numeric::One };
		constexpr const sdl::key SpectralInvocation{ keys::Numeric::Two };
		constexpr const sdl::key SanguineInvocation{ keys::Numeric::Three };
		constexpr const sdl::key NecromanticAscendance{ keys::Numeric::Four };
	} // namespace Bindings
} // namespace bleak
