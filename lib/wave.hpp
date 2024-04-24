#pragma once

#include "typedef.hpp"

#include <cmath>
#include <numbers>

enum class WaveType
{
    Sine,
    Square,
    Triangle,
    Sawtooth
};

struct Wave
{
private:
    f64 value;

public:
    f64 frequency;
    f64 amplitude;
    f64 phase;

    constexpr Wave() : value{ }, frequency{ 1.0 }, amplitude{ 1.0 }, phase{ 0.0 } { }
    constexpr Wave(f64 frequency, f64 amplitude, f64 phase) :
        value{ },
        frequency{ frequency },
        amplitude{ amplitude },
        phase{ phase }
    { }

    template<WaveType W> constexpr void update(f64 time);

    template<> constexpr void update<WaveType::Sine>(f64 time) { value = amplitude * (phase + std::sin(2.0 * std::numbers::pi * frequency * time)); }
    template<> constexpr void update<WaveType::Square>(f64 time) { value = amplitude * (phase + std::copysign(1.0, std::sin(2.0 * std::numbers::pi * frequency * time))); }
    template<> constexpr void update<WaveType::Triangle>(f64 time) { value = amplitude * (phase + std::asin(std::sin(2.0 * std::numbers::pi * frequency * time))); }
    template<> constexpr void update<WaveType::Sawtooth>(f64 time) { value = amplitude * (phase + std::fmod(2.0 * frequency * time, 2.0) - 1.0); }
};
