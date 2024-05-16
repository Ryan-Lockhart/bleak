#pragma once

#include "array.hpp"

namespace Bleakdepth {

    template<typename T> constexpr fwd_iter_t<T>::fwd_iter_t(pointer handle) : handle { handle } {}

    template<typename T> constexpr fwd_iter_t<T>::fwd_iter_t(cref<fwd_iter_t<T>> other) : handle { other.handle } {}

    template<typename T> constexpr fwd_iter_t<T>::fwd_iter_t(rval<fwd_iter_t<T>> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

    template<typename T> constexpr fwd_iter_t<T>::fwd_iter_t(cref<rev_iter_t<T>> other) : handle { other.handle } {}

    template<typename T> constexpr fwd_iter_t<T>::fwd_iter_t(rval<rev_iter_t<T>> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

    template<typename T> constexpr ref<fwd_iter_t<T>> fwd_iter_t<T>::operator=(cref<rev_iter_t<T>> other) {
        if (this == &other) {
            return *this;
        }

        handle = other.handle;
        return *this;
    }

    template<typename T> constexpr ref<fwd_iter_t<T>> fwd_iter_t<T>::operator=(rval<rev_iter_t<T>> other) {
        if (this == &other) {
            return *this;
        }

        handle = std::move(other.handle);
        other.handle = nullptr;
        return *this;
    }

    template<typename T> constexpr ref<fwd_iter_t<T>> fwd_iter_t<T>::operator=(cref<fwd_iter_t> other) {
        if (this == &other) {
            return *this;
        }

        handle = other.handle;
        return *this;
    }

    template<typename T> constexpr ref<fwd_iter_t<T>> fwd_iter_t<T>::operator=(rval<fwd_iter_t> other) {
        if (this == &other) {
            return *this;
        }

        handle = std::move(other.handle);
        other.handle = nullptr;
        return *this;
    }

    template<typename T> constexpr fwd_iter_t<T>::~fwd_iter_t() {};

    template<typename T> constexpr fwd_iter_t<T>::reference fwd_iter_t<T>::operator*() const noexcept { return *handle; }

    template<typename T> constexpr fwd_iter_t<T>::pointer fwd_iter_t<T>::operator->() const noexcept { return handle; }

    template<typename T> constexpr ref<fwd_iter_t<T>> fwd_iter_t<T>::operator++() noexcept {
        ++handle;
        return *this;
    }

    template<typename T> constexpr fwd_iter_t<T> fwd_iter_t<T>::operator++(int) noexcept {
        auto temp { *this };
        ++handle;
        return temp;
    }

    template<typename T> constexpr ref<fwd_iter_t<T>> fwd_iter_t<T>::operator--() noexcept {
        --handle;
        return *this;
    }

    template<typename T> constexpr fwd_iter_t<T> fwd_iter_t<T>::operator--(int) noexcept {
        auto temp { *this };
        --handle;
        return temp;
    }

    template<typename T> constexpr bool fwd_iter_t<T>::operator==(cref<fwd_iter_t> other) const noexcept { return handle == other.handle; }

    template<typename T> constexpr bool fwd_iter_t<T>::operator!=(cref<fwd_iter_t> other) const noexcept { return handle != other.handle; }

    template<typename T> constexpr bool fwd_iter_t<T>::operator==(cref<rev_iter_t<T>> other) const noexcept { return handle == other.handle; }

    template<typename T> constexpr bool fwd_iter_t<T>::operator!=(cref<rev_iter_t<T>> other) const noexcept { return handle != other.handle; }

    template<typename T> constexpr rev_iter_t<T>::rev_iter_t(pointer handle) : handle { handle } {}

    template<typename T> constexpr rev_iter_t<T>::rev_iter_t(cref<rev_iter_t> other) : handle { other.handle } {}

    template<typename T> constexpr rev_iter_t<T>::rev_iter_t(rval<rev_iter_t> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

    template<typename T> constexpr rev_iter_t<T>::rev_iter_t(cref<fwd_iter_t<T>> other) : handle { other.handle } {}

    template<typename T> constexpr rev_iter_t<T>::rev_iter_t(rval<fwd_iter_t<T>> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

    template<typename T> constexpr ref<rev_iter_t<T>> rev_iter_t<T>::operator=(cref<rev_iter_t> other) {
        if (this == &other) {
            return *this;
        }

        handle = other.handle;
        return *this;
    }

    template<typename T> constexpr ref<rev_iter_t<T>> rev_iter_t<T>::operator=(rval<rev_iter_t> other) {
        if (this == &other) {
            return *this;
        }

        handle = std::move(other.handle);
        other.handle = nullptr;
        return *this;
    }

    template<typename T> constexpr ref<rev_iter_t<T>> rev_iter_t<T>::operator=(cref<fwd_iter_t<T>> other) {
        if (this == &other) {
            return *this;
        }

        handle = other.handle;
        return *this;
    }

    template<typename T> constexpr ref<rev_iter_t<T>> rev_iter_t<T>::operator=(rval<fwd_iter_t<T>> other) {
        if (this == &other) {
            return *this;
        }

        handle = std::move(other.handle);
        other.handle = nullptr;
        return *this;
    }

    template<typename T> constexpr rev_iter_t<T>::~rev_iter_t() {};

    template<typename T> constexpr rev_iter_t<T>::reference rev_iter_t<T>::operator*() const { return *handle; }

    template<typename T> constexpr rev_iter_t<T>::pointer rev_iter_t<T>::operator->() const { return handle; }

    template<typename T> constexpr ref<rev_iter_t<T>> rev_iter_t<T>::operator++() {
        --handle;
        return *this;
    }

    template<typename T> constexpr ref<rev_iter_t<T>> rev_iter_t<T>::operator++(int) {
        auto temp { *this };
        --handle;
        return temp;
    }

    template<typename T> constexpr ref<rev_iter_t<T>> rev_iter_t<T>::operator--() {
        ++handle;
        return *this;
    }

    template<typename T> constexpr ref<rev_iter_t<T>> rev_iter_t<T>::operator--(int) {
        auto temp { *this };
        ++handle;
        return temp;
    }

    template<typename T> constexpr bool rev_iter_t<T>::operator==(cref<rev_iter_t> other) const noexcept { return handle == other.handle; }

    template<typename T> constexpr bool rev_iter_t<T>::operator!=(cref<rev_iter_t> other) const noexcept { return handle != other.handle; }

    template<typename T> constexpr bool rev_iter_t<T>::operator==(cref<fwd_iter_t<T>> other) const noexcept { return handle == other.handle; }

    template<typename T> constexpr bool rev_iter_t<T>::operator!=(cref<fwd_iter_t<T>> other) const noexcept { return handle != other.handle; }
}