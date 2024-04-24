#pragma once

#include "typedef.hpp"

#include <SDL.h>

class Timer
{
private:
	const f64 interval;
	usize total;
	f64 last;

public:
	constexpr Timer() = delete;
	constexpr Timer(f64 interval) :  interval{ interval }, total{ }, last{ } { }

	constexpr Timer(cref<Timer> other) noexcept = delete;
	constexpr Timer(rval<Timer> other) noexcept = delete;

	constexpr ~Timer() = default;

	constexpr ref<Timer> operator=(cref<Timer> other) noexcept = delete;
	constexpr ref<Timer> operator=(rval<Timer> other) noexcept = delete;

	inline void record() { ++total; last = SDL_GetPerformanceCounter(); }

	inline f64 elapsed() const { return SDL_GetPerformanceCounter() - last; }
	constexpr usize count() const { return total; }

	inline bool ready() const { return elapsed() >= interval; }

	inline void reset() { total = 0; last = SDL_GetPerformanceCounter(); }
};
