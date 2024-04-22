#pragma once

#include "typedef.hpp"

#include <utility>

class Timer
{
private:
	const f64 interval;
	usize total;
	f64 last;

public:
	constexpr Timer() = delete;
	constexpr Timer(f64 interval) : total(0), interval(interval), last(0.0) {}

	constexpr Timer(cref<Timer> other) noexcept = delete;
	constexpr Timer(rval<Timer> other) noexcept = delete;

	constexpr ~Timer() = default;

	constexpr ref<Timer> operator=(cref<Timer> other) noexcept = delete;
	constexpr ref<Timer> operator=(rval<Timer> other) noexcept = delete;

	inline void record() { ++total; last = ray::GetTime(); }

	inline f64 elapsed() const { return ray::GetTime() - last; }
	constexpr usize count() const { return total; }

	inline bool ready() const { return elapsed() >= interval; }

	inline void reset() { total = 0; last = ray::GetTime(); }
};
