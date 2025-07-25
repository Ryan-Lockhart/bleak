#pragma once

#include <bleak/typedef.hpp>

#include <bleak/clock.hpp>

namespace bleak {
	struct timer_t {
	  private:
		usize total;
		usize last;

	  public:
		usize interval;

		constexpr timer_t() = delete;

		constexpr timer_t(usize ms) : total {}, last {}, interval { ms } {}

		constexpr timer_t(cref<timer_t> other) noexcept = delete;
		constexpr timer_t(rval<timer_t> other) noexcept = delete;

		constexpr ~timer_t() = default;

		constexpr ref<timer_t> operator=(cref<timer_t> other) noexcept = delete;
		constexpr ref<timer_t> operator=(rval<timer_t> other) noexcept = delete;

		inline void record() {
			++total;
			last = Clock::now();
		}

		inline f64 elapsed() const { return (Clock::now() - last) * 1000.0 / Clock::frequency(); }

		constexpr usize count() const { return total; }

		inline bool ready() const { return elapsed() >= interval; }

		inline void reset() {
			total = 0;
			last = Clock::now();
		}
	};
} // namespace bleak
