#pragma once

#include "typedef.hpp"

#include <SDL.h>

namespace Bleakdepth {
	class timer_t {
	  private:
		const f64 interval;
		usize total;
		f64 last;

	  public:
		constexpr timer_t() = delete;

		constexpr timer_t(f64 interval) : interval { interval }, total {}, last {} {}

		constexpr timer_t(cref<timer_t> other) noexcept = delete;
		constexpr timer_t(rval<timer_t> other) noexcept = delete;

		constexpr ~timer_t() = default;

		constexpr ref<timer_t> operator=(cref<timer_t> other) noexcept = delete;
		constexpr ref<timer_t> operator=(rval<timer_t> other) noexcept = delete;

		inline void record() {
			++total;
			last = SDL_GetPerformanceCounter();
		}

		inline f64 elapsed() const { return SDL_GetPerformanceCounter() - last; }

		constexpr usize count() const { return total; }

		inline bool ready() const { return elapsed() >= interval; }

		inline void reset() {
			total = 0;
			last = SDL_GetPerformanceCounter();
		}
	};
} // namespace Bleakdepth
