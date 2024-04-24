#pragma once

#include "typedef.hpp"

#include <stdexcept>
#include <utility>

template<typename Key, typename Value> struct Pair
{
    Key key;
    Value value;

    constexpr Pair() : key{}, value{} {}
    constexpr Pair(cref<Key> key, cref<Value> value) : key{ key }, value{ value } {}
    constexpr Pair(rval<Key> key, rval<Value> value) : key{ std::move(key) }, value{ std::move(value) } {}

    constexpr Pair(cref<Pair> other) : key{ other.key }, value{ other.value } {}
    constexpr Pair(rval<Pair> other) : key{ std::move(other.key) }, value{ std::move(other.value) } {}

    constexpr ref<Pair> operator=(cref<Pair> other) noexcept
    {
        key = other.key;
        value = other.value;

        return *this;
    }

    constexpr ref<Pair> operator=(rval<Pair> other) noexcept
    {
        key = std::move(other.key);
        value = std::move(other.value);

        return *this;
    }

    constexpr ~Pair() = default;

    constexpr bool operator==(cref<Pair> other) const noexcept { return key == other.key && value == other.value; }
    constexpr bool operator!=(cref<Pair> other) const noexcept { return key != other.key || value != other.value; }

    constexpr bool operator<(cref<Pair> other) const noexcept { return value < other.value; }
    constexpr bool operator>(cref<Pair> other) const noexcept { return value > other.value; }

    constexpr bool operator<=(cref<Pair> other) const noexcept { return value <= other.value; }
    constexpr bool operator>=(cref<Pair> other) const noexcept { return value >= other.value; }
};

template<typename Key, typename Value, usize Size, typename Index = usize>
class lut
{
public:
    using KeyIndex = Pair<Key, Index>;
    using KeyValue = Pair<Key, Value>;

private:
    KeyIndex lookup[Size];
    Value values[Size];

public:
    constexpr lut() noexcept = default;

    template<typename... Elements, typename = KeyValue> 
    constexpr lut(Elements... elements) noexcept : lookup{}, values{}
    {
        if (sizeof...(Elements) != Size) throw std::invalid_argument("lut size mismatch!");

        usize i{ 0 };
        for (const auto& element : { elements... })
        {
            lookup[i] = { element.key, i };
            values[i] = element.value;
            ++i;
        }
    }

    constexpr lut(cref<lut> other) noexcept : lookup{ other.lookup }, values{ other.values } {}
    constexpr lut(rval<lut> other) noexcept : lookup{ std::move(other.lookup) }, values{ std::move(other.values) } {}

    constexpr ref<Value> operator[](cref<Key> key)
    {
        for (const auto& [k, i] : lookup)
            if (k == key) return values[i];
        
        throw std::invalid_argument("key not found!");
    }

    constexpr cref<Value> operator[](cref<Key> key) const
    {
        for (const auto& [k, i] : lookup)
            if (k == key) return values[i];
        
        throw std::invalid_argument("key not found!");
    }
};
