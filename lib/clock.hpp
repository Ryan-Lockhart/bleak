#pragma once

#include "typedef.hpp"

#include <SDL.h>

#include <SDL_timer.h>
#include <chrono>
#include <thread>

namespace Bleakdepth {
	struct clock {
	  private:
		static inline usize last;

	  public:
		static inline usize now() { return static_cast<usize>(SDL_GetPerformanceCounter()); }

		static inline usize frequency() { return static_cast<usize>(SDL_GetPerformanceFrequency()); }

		static inline void initialize() {
			std::this_thread::sleep_for(std::chrono::seconds(1));

			tick();

			std::this_thread::sleep_for(std::chrono::seconds(1));

			tick();
		}

		static inline void tick() {
			last = now();
		}

		static inline void tick(f32 interval) {	
			last = now();
			if (auto dt = deltaTime(); dt < interval) {
				SDL_Delay(static_cast<Bleakdepth::u32>(interval - dt));
			}
		}

		static inline f64 deltaTime() { return ((now() - last) * 1000.0) / frequency(); }

		static inline f64 frameTime() { return 1000.0 / deltaTime(); }

		static inline f64 elapsed() { return (f64)now() / frequency(); }
	};

	static inline clock Clock;
} // namespace Bleakdepth
