#pragma once

#include "typedef.hpp"

#include <SDL2/SDL.h>

class DeltaClock
{
private:
	usize last;
	usize current;
	usize delta;

public:
	inline DeltaClock()
	{
		current = SDL_GetPerformanceFrequency();
		last = SDL_GetPerformanceCounter
	}
};
