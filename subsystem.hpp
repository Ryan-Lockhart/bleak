#pragma once

#include "typedef.hpp"

#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>

class Subsystem
{
public:
	inline Subsystem()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw std::runtime_error("failed to initialize sdl: " + std::string(SDL_GetError()));
		if (IMG_Init(IMG_INIT_PNG) < 0) throw std::runtime_error("failed to initialize sdl-image: " + std::string(IMG_GetError()));
		if (Mix_Init(MIX_INIT_MOD | MIX_INIT_OGG) < 0) throw std::runtime_error("failed to initialize sdl-mixer: " + std::string(Mix_GetError()));
		if (SDLNet_Init() < 0) throw std::runtime_error("failed to initialize sdl-net: " + std::string(SDLNet_GetError()));
		if (TTF_Init() < 0) throw std::runtime_error("failed to initialize sdl-ttf: " + std::string(TTF_GetError()));
	}

	inline ~Subsystem()
	{
		TTF_Quit();
		SDLNet_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}
};