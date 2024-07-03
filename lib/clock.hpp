#pragma once

#include "typedef.hpp"

#include <SDL.h>

namespace Bleakdepth {
	struct Clock {
	  private:
		static inline usize last;

	  public:
		static inline usize now() { return static_cast<usize>(SDL_GetPerformanceCounter()); }

		static inline usize frequency() { return static_cast<usize>(SDL_GetPerformanceFrequency()); }

		static inline void tick() {
			last = now();
		}

		static inline void tick(f64 interval) {
			last = now();
			if (auto dt = delta_time(); dt < interval) {
				SDL_Delay(static_cast<u32>(interval - dt));
			}
		}

		static inline f64 delta_time() { return ((now() - last) * 1000.0) / frequency(); }

		static inline f64 frame_time() { return 1000.0 / delta_time(); }

		static inline f64 elapsed() { return (f64)now() / frequency(); }
	};
} // namespace Bleakdepth
