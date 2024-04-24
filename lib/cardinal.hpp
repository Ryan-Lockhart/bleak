#pragma once

#include "typedef.hpp"

#include <string>

struct Cardinal
{
private:
    u8 value;

public:
    constexpr Cardinal() : value(0) {}
    constexpr Cardinal(u8 value) : value(value) {}

    static const Cardinal Central;

    static const Cardinal North;
    static const Cardinal East;
    static const Cardinal South;
    static const Cardinal West;

    static const Cardinal Northwest;
    static const Cardinal Northeast;

    static const Cardinal Southwest;
    static const Cardinal Southeast;

    constexpr Cardinal operator~() const { return ~value; }
    constexpr Cardinal operator|(cref<Cardinal> other) const { return value | other.value; }
    constexpr Cardinal operator&(cref<Cardinal> other) const { return value & other.value; }
    constexpr Cardinal operator^(cref<Cardinal> other) const { return value ^ other.value; }

    constexpr Cardinal operator<<(cref<Cardinal> other) const { return value << other.value; }
    constexpr Cardinal operator>>(cref<Cardinal> other) const { return value >> other.value; }

    constexpr ref<Cardinal> operator|=(cref<Cardinal> other) { value |= other.value; return *this; }
    constexpr ref<Cardinal> operator&=(cref<Cardinal> other) { value &= other.value; return *this; }
    constexpr ref<Cardinal> operator^=(cref<Cardinal> other) { value ^= other.value; return *this; }

    constexpr ref<Cardinal> operator<<=(cref<Cardinal> other) { value <<= other.value; return *this; }
    constexpr ref<Cardinal> operator>>=(cref<Cardinal> other) { value >>= other.value; return *this; }

    constexpr bool operator==(cref<Cardinal> other) const { return value == other.value; }
    constexpr bool operator!=(cref<Cardinal> other) const { return value != other.value; }

    constexpr operator u8() const { return value; }

    constexpr operator std::string() const;
};

constexpr const Cardinal Cardinal::Central{ 0 };

constexpr const Cardinal Cardinal::North{ 1 << 0 };
constexpr const Cardinal Cardinal::East{ 1 << 1 };
constexpr const Cardinal Cardinal::South{ 1 << 2 };
constexpr const Cardinal Cardinal::West{ 1 << 3 };

constexpr const Cardinal Cardinal::Northwest{ Cardinal::North | Cardinal::West };
constexpr const Cardinal Cardinal::Northeast{ Cardinal::North | Cardinal::East };

constexpr const Cardinal Cardinal::Southwest{ Cardinal::South | Cardinal::West };
constexpr const Cardinal Cardinal::Southeast{ Cardinal::South | Cardinal::East };

constexpr inline Cardinal::operator std::string() const
{
    switch (value)
    {
    case Cardinal::North:
        return "North";
    case Cardinal::East:
        return "East";
    case Cardinal::South:
        return "South";
    case Cardinal::West:
        return "West";
    case Cardinal::Northwest:
        return "Northwest";
    case Cardinal::Northeast:
        return "Northeast";
    case Cardinal::Southwest:
        return "Southwest";
    case Cardinal::Southeast:
        return "Southeast";
    default:
        return "Central";
    }
}
