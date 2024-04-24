#pragma once

#include "typedef.hpp"

#include <cmath>
#include <numbers>

namespace Bleakdepth {
	struct wave_t {
	  private:
		f64 value;

	  public:
		enum class type_t { Sine, Square, Triangle, Sawtooth };
		f64 frequency;
		f64 amplitude;
		f64 phase;

		constexpr wave_t() : value {}, frequency { 1.0 }, amplitude { 1.0 }, phase { 0.0 } {}

		constexpr wave_t(f64 frequency, f64 amplitude, f64 phase) :
			value {},
			frequency { frequency },
			amplitude { amplitude },
			phase { phase } {}

		template<type_t W> constexpr void update(f64 time);

		template<> constexpr void update<type_t::Sine>(f64 time) {
			value = amplitude * (phase + std::sin(2.0 * std::numbers::pi * frequency * time));
		}

		template<> constexpr void update<type_t::Square>(f64 time) {
			value = amplitude * (phase + std::copysign(1.0, std::sin(2.0 * std::numbers::pi * frequency * time)));
		}

		template<> constexpr void update<type_t::Triangle>(f64 time) {
			value = amplitude * (phase + std::asin(std::sin(2.0 * std::numbers::pi * frequency * time)));
		}

		template<> constexpr void update<type_t::Sawtooth>(f64 time) {
			value = amplitude * (phase + std::fmod(2.0 * frequency * time, 2.0) - 1.0);
		}
	};
} // namespace Bleakdepth
