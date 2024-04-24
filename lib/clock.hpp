#pragma once

#include "typedef.hpp"

#include <SDL.h>

#include <chrono>
#include <thread>

namespace Bleakdepth {
	struct clock {
	  private:
		static inline usize last;
		static inline usize current;
		static inline f64 delta;

	  public:
		static inline void initialize() {
			std::this_thread::sleep_for(std::chrono::seconds(1));

			current = SDL_GetPerformanceCounter();

			std::this_thread::sleep_for(std::chrono::seconds(1));

			tick();
		}

		static inline void tick() {
			last = current;
			current = SDL_GetPerformanceCounter();
			delta = (current - last) * 1000 / static_cast<f64>(SDL_GetPerformanceFrequency());
		}

		static inline f64 deltaTime() { return delta; }
	};

	static inline clock Clock;
} // namespace Bleakdepth
