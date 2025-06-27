#pragma once

#include <bleak/typedef.hpp>

#include <array>

#include <SDL.h>

#include <bleak/cardinal.hpp>
#include <bleak/lut.hpp>

#include <bleak/constants/keys.hpp>

namespace bleak::bindings {
	constexpr sdl::key nullkey = SDL_SCANCODE_UNKNOWN;

	template<usize Count> using key_collection_t = std::array<sdl::key, Count>;
} // namespace bleak
