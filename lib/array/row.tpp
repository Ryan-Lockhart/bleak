#pragma once

#include "array.hpp"

namespace Bleakdepth {
	template<typename T, usize Width> using row_t = array_t<T, Width>;

    template<typename T, usize Width> constexpr array_t<T, Width>::array_t() : data(new T[size]) {}

    template<typename T, usize Width> constexpr array_t<T, Width>::array_t(std::initializer_list<T> elements) : data { new T[size] } {
        if (elements.size() != size) {
            throw std::invalid_argument("initializer list size mismatch!");
        }

        usize i { 0 };
        for (auto element : elements) {
            data[i++] = element;
        }
    }

    template<typename T, usize Width> constexpr array_t<T, Width>::array_t(cref<array_t> other) : data { new T[size] } {
        for (usize i { 0 }; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    template<typename T, usize Width> constexpr array_t<T, Width>::array_t(rval<array_t> other) noexcept : data { std::move(other.data) } { other.data = nullptr; }

    template<typename T, usize Width> constexpr ref<array_t<T, Width>> array_t<T, Width>::operator=(cref<array_t> other) noexcept {
        if (this == &other) {
            return *this;
        }

        for (usize i { 0 }; i < size; ++i) {
            data[i] = other.data[i];
        }

        return *this;
    }

    template<typename T, usize Width> constexpr ref<array_t<T, Width>> array_t<T, Width>::operator=(rval<array_t> other) noexcept {
        if (this == &other) {
            return *this;
        }

        data = std::move(other.data);
        other.data = nullptr;
        return *this;
    }

    template<typename T, usize Width> constexpr array_t<T, Width>::~array_t() noexcept { delete[] data; }

    template<typename T, usize Width> constexpr ref<T> array_t<T, Width>::operator[](usize index) noexcept { return data[index]; }

    template<typename T, usize Width> constexpr cref<T> array_t<T, Width>::operator[](usize index) const noexcept { return data[index]; }

    template<typename T, usize Width> constexpr bool array_t<T, Width>::valid(usize index) const noexcept { return index < size; }

    template<typename T, usize Width> constexpr ref<T> array_t<T, Width>::at(usize index) {
        if (index >= size) {
            throw std::out_of_range(std::format("index of {} was out of range!", index));
        }

        return data[index];
    }

    template<typename T, usize Width> constexpr cref<T> array_t<T, Width>::at(usize index) const {
        if (index >= size) {
            throw std::out_of_range(std::format("index of {} was out of range!", index));
        }

        return data[index];
    }

    template<typename T, usize Width> constexpr ref<T> array_t<T, Width>::front() noexcept { return data[first]; }

    template<typename T, usize Width> constexpr cref<T> array_t<T, Width>::front() const noexcept { return data[first]; }

    template<typename T, usize Width> constexpr ref<T> array_t<T, Width>::back() noexcept { return data[last]; }

    template<typename T, usize Width> constexpr cref<T> array_t<T, Width>::back() const noexcept { return data[last]; }

    template<typename T, usize Width> constexpr array_t<T, Width>::iterator array_t<T, Width>::begin() noexcept { return iterator { data }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::iterator array_t<T, Width>::end() noexcept { return iterator { data + size }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::const_iterator array_t<T, Width>::begin() const noexcept { return const_iterator { data }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::const_iterator array_t<T, Width>::end() const noexcept { return const_iterator { data + size }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::const_iterator array_t<T, Width>::cbegin() const noexcept { return const_iterator { data }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::const_iterator array_t<T, Width>::cend() const noexcept { return const_iterator { data + size }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::reverse_iterator array_t<T, Width>::rbegin() noexcept { return reverse_iterator { data + size - 1 }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::reverse_iterator array_t<T, Width>::rend() noexcept { return reverse_iterator { data - 1 }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::const_reverse_iterator array_t<T, Width>::rbegin() const noexcept { return const_reverse_iterator { data + size - 1 }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::const_reverse_iterator array_t<T, Width>::rend() const noexcept { return const_reverse_iterator { data - 1 }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::const_reverse_iterator array_t<T, Width>::crbegin() const noexcept { return const_reverse_iterator { data + size - 1 }; }

    template<typename T, usize Width> constexpr array_t<T, Width>::const_reverse_iterator array_t<T, Width>::crend() const noexcept { return const_reverse_iterator { data - 1 }; }
}
