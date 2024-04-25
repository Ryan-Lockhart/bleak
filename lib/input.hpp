#pragma once

#include "typedef.hpp"

#include <string>

namespace Bleakdepth {
    struct input_state_t {
    private:
        u8 value;

    public:
        constexpr input_state_t() : value(0) {}

        constexpr input_state_t(u8 value) : value(value) {}

        static const input_state_t Pressed;
        static const input_state_t Released;
        static const input_state_t Down;
        static const input_state_t Up;

        constexpr input_state_t operator~() const { return ~value; }

        constexpr input_state_t operator|(cref<input_state_t> other) const { return value | other.value; }

        constexpr input_state_t operator&(cref<input_state_t> other) const { return value & other.value; }

        constexpr input_state_t operator^(cref<input_state_t> other) const { return value ^ other.value; }

        constexpr input_state_t operator<<(cref<input_state_t> other) const { return value << other.value; }

        constexpr input_state_t operator>>(cref<input_state_t> other) const { return value >> other.value; }

        constexpr ref<input_state_t> operator|=(cref<input_state_t> other) {
            value |= other.value;
            return *this;
        }

        constexpr ref<input_state_t> operator&=(cref<input_state_t> other) {
            value &= other.value;
            return *this;
        }

        constexpr ref<input_state_t> operator^=(cref<input_state_t> other) {
            value ^= other.value;
            return *this;
        }

        constexpr ref<input_state_t> operator<<=(cref<input_state_t> other) {
            value <<= other.value;
            return *this;
        }

        constexpr ref<input_state_t> operator>>=(cref<input_state_t> other) {
            value >>= other.value;
            return *this;
        }

        constexpr operator u8() const { return value; }

        constexpr bool operator==(input_state_t other) const { return value == other.value; }

        constexpr bool operator!=(input_state_t other) const { return value != other.value; }

        constexpr operator std::string() const {
            switch (value) {
                case 0: return "Pressed";
                case 1: return "Released";
                case 2: return "Down";
                case 3: return "Up";
                default: return "Unknown";
            }
        }
    };

    constexpr const input_state_t input_state_t::Pressed{ 0 };
    constexpr const input_state_t input_state_t::Released{ 1 };
    constexpr const input_state_t input_state_t::Down{ 2 };
    constexpr const input_state_t input_state_t::Up{ 3 };
}
