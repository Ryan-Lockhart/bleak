#pragma once

#include "typedef.hpp"

#include <stdexcept>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

struct Subsystem
{
private:
	static bool _initialized;
public:
	static inline void initialize()
	{
		if (_initialized) return;

		try {
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw std::runtime_error("failed to initialize sdl: " + std::string(SDL_GetError()));
			if (IMG_Init(IMG_INIT_PNG) < 0) throw std::runtime_error("failed to initialize sdl-image: " + std::string(IMG_GetError()));
			if (Mix_Init(MIX_INIT_MOD | MIX_INIT_OGG) < 0) throw std::runtime_error("failed to initialize sdl-mixer: " + std::string(Mix_GetError()));
			if (SDLNet_Init() < 0) throw std::runtime_error("failed to initialize sdl-net: " + std::string(SDLNet_GetError()));
			if (TTF_Init() < 0) throw std::runtime_error("failed to initialize sdl-ttf: " + std::string(TTF_GetError()));
		} catch (ref<std::runtime_error> e) {
			
		}
		

		_initialized = true;
	}

	static inline void terminate()
	{
		if (!_initialized) return;

		TTF_Quit();
		SDLNet_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();

		_initialized = false;
	}

	static inline bool initialized() { return _initialized; }
};