#pragma once

#include <bleak/typedef.hpp>

#include <SDL.h>

namespace bleak {
	namespace sdl {
		static inline void delay(u32 ms) noexcept { SDL_Delay(ms); }

		static inline usize get_performance_counter() noexcept { return SDL_GetPerformanceCounter(); }

		static inline usize get_performance_frequency() noexcept { return SDL_GetPerformanceFrequency(); }
	} // namespace sdl

	struct Clock {
	  private:
		static inline usize last;

	  public:
		static inline usize now() { return sdl::get_performance_counter(); }

		static inline usize frequency() { return sdl::get_performance_frequency(); }

		static inline void tick() { last = now(); }

		static inline void tick(f64 interval) {
			last = now();
			if (auto dt = delta_time(); dt < interval) {
				sdl::delay(static_cast<u32>(interval - dt));
			}
		}

		static inline f64 delta_time() { return ((now() - last) * 1000.0) / frequency(); }

		static inline f64 frame_time() { return 1000.0 / delta_time(); }

		static inline f64 elapsed() { return static_cast<f64>(now()) / frequency(); }
	};
} // namespace bleak
