#pragma once

#include "typedef.hpp"

#include <utility>
#include <raylib.h>

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

	constexpr void record() { ++total; last = GetTime(); }

	constexpr f64 elapsed() const { return GetTime() - last; }
	constexpr usize count() const { return total; }

	constexpr bool ready() const { return elapsed() >= interval; }

	constexpr void reset() { total = 0; last = GetTime(); }
};
