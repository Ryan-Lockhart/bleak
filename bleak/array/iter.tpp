#pragma once

#include "bleak/array/iter.hpp"

template<typename T> constexpr bleak::fwd_iter_t<T>::fwd_iter_t(pointer handle) : handle { handle } {}

template<typename T> constexpr bleak::fwd_iter_t<T>::fwd_iter_t(cref<bleak::fwd_iter_t<T>> other) : handle { other.handle } {}

template<typename T> constexpr bleak::fwd_iter_t<T>::fwd_iter_t(rval<bleak::fwd_iter_t<T>> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

template<typename T> constexpr bleak::fwd_iter_t<T>::fwd_iter_t(cref<bleak::rev_iter_t<T>> other) : handle { other.handle } {}

template<typename T> constexpr bleak::fwd_iter_t<T>::fwd_iter_t(rval<bleak::rev_iter_t<T>> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

template<typename T> constexpr bleak::ref<bleak::fwd_iter_t<T>> bleak::fwd_iter_t<T>::operator=(cref<bleak::rev_iter_t<T>> other) {
	if (this == &other) {
		return *this;
	}

	handle = other.handle;
	return *this;
}

template<typename T> constexpr bleak::ref<bleak::fwd_iter_t<T>> bleak::fwd_iter_t<T>::operator=(rval<bleak::rev_iter_t<T>> other) {
	if (this == &other) {
		return *this;
	}

	handle = std::move(other.handle);
	other.handle = nullptr;
	return *this;
}

template<typename T> constexpr bleak::ref<bleak::fwd_iter_t<T>> bleak::fwd_iter_t<T>::operator=(cref<fwd_iter_t> other) {
	if (this == &other) {
		return *this;
	}

	handle = other.handle;
	return *this;
}

template<typename T> constexpr bleak::ref<bleak::fwd_iter_t<T>> bleak::fwd_iter_t<T>::operator=(rval<fwd_iter_t> other) {
	if (this == &other) {
		return *this;
	}

	handle = std::move(other.handle);
	other.handle = nullptr;
	return *this;
}

template<typename T> constexpr bleak::fwd_iter_t<T>::~fwd_iter_t() {};

template<typename T> constexpr bleak::fwd_iter_t<T>::reference bleak::fwd_iter_t<T>::operator*() const noexcept { return *handle; }

template<typename T> constexpr bleak::fwd_iter_t<T>::pointer bleak::fwd_iter_t<T>::operator->() const noexcept { return handle; }

template<typename T> constexpr bleak::ref<bleak::fwd_iter_t<T>> bleak::fwd_iter_t<T>::operator++() noexcept {
	++handle;
	return *this;
}

template<typename T> constexpr bleak::fwd_iter_t<T> bleak::fwd_iter_t<T>::operator++(int) noexcept {
	auto temp { *this };
	++handle;
	return temp;
}

template<typename T> constexpr bleak::ref<bleak::fwd_iter_t<T>> bleak::fwd_iter_t<T>::operator--() noexcept {
	--handle;
	return *this;
}

template<typename T> constexpr bleak::fwd_iter_t<T> bleak::fwd_iter_t<T>::operator--(int) noexcept {
	auto temp { *this };
	--handle;
	return temp;
}

template<typename T> constexpr bool bleak::fwd_iter_t<T>::operator==(cref<fwd_iter_t> other) const noexcept { return handle == other.handle; }

template<typename T> constexpr bool bleak::fwd_iter_t<T>::operator!=(cref<fwd_iter_t> other) const noexcept { return handle != other.handle; }

template<typename T> constexpr bool bleak::fwd_iter_t<T>::operator==(cref<bleak::rev_iter_t<T>> other) const noexcept { return handle == other.handle; }

template<typename T> constexpr bool bleak::fwd_iter_t<T>::operator!=(cref<bleak::rev_iter_t<T>> other) const noexcept { return handle != other.handle; }

template<typename T> constexpr bleak::rev_iter_t<T>::rev_iter_t(pointer handle) : handle { handle } {}

template<typename T> constexpr bleak::rev_iter_t<T>::rev_iter_t(cref<rev_iter_t> other) : handle { other.handle } {}

template<typename T> constexpr bleak::rev_iter_t<T>::rev_iter_t(rval<rev_iter_t> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

template<typename T> constexpr bleak::rev_iter_t<T>::rev_iter_t(cref<bleak::fwd_iter_t<T>> other) : handle { other.handle } {}

template<typename T> constexpr bleak::rev_iter_t<T>::rev_iter_t(rval<bleak::fwd_iter_t<T>> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

template<typename T> constexpr bleak::ref<bleak::rev_iter_t<T>> bleak::rev_iter_t<T>::operator=(cref<rev_iter_t> other) {
	if (this == &other) {
		return *this;
	}

	handle = other.handle;
	return *this;
}

template<typename T> constexpr bleak::ref<bleak::rev_iter_t<T>> bleak::rev_iter_t<T>::operator=(rval<rev_iter_t> other) {
	if (this == &other) {
		return *this;
	}

	handle = std::move(other.handle);
	other.handle = nullptr;
	return *this;
}

template<typename T> constexpr bleak::ref<bleak::rev_iter_t<T>> bleak::rev_iter_t<T>::operator=(cref<bleak::fwd_iter_t<T>> other) {
	if (this == &other) {
		return *this;
	}

	handle = other.handle;
	return *this;
}

template<typename T> constexpr bleak::ref<bleak::rev_iter_t<T>> bleak::rev_iter_t<T>::operator=(rval<bleak::fwd_iter_t<T>> other) {
	if (this == &other) {
		return *this;
	}

	handle = std::move(other.handle);
	other.handle = nullptr;
	return *this;
}

template<typename T> constexpr bleak::rev_iter_t<T>::~rev_iter_t() {};

template<typename T> constexpr bleak::rev_iter_t<T>::reference bleak::rev_iter_t<T>::operator*() const { return *handle; }

template<typename T> constexpr bleak::rev_iter_t<T>::pointer bleak::rev_iter_t<T>::operator->() const { return handle; }

template<typename T> constexpr bleak::ref<bleak::rev_iter_t<T>> bleak::rev_iter_t<T>::operator++() {
	--handle;
	return *this;
}

template<typename T> constexpr bleak::ref<bleak::rev_iter_t<T>> bleak::rev_iter_t<T>::operator++(int) {
	auto temp { *this };
	--handle;
	return temp;
}

template<typename T> constexpr bleak::ref<bleak::rev_iter_t<T>> bleak::rev_iter_t<T>::operator--() {
	++handle;
	return *this;
}

template<typename T> constexpr bleak::ref<bleak::rev_iter_t<T>> bleak::rev_iter_t<T>::operator--(int) {
	auto temp { *this };
	++handle;
	return temp;
}

template<typename T> constexpr bool bleak::rev_iter_t<T>::operator==(cref<rev_iter_t> other) const noexcept { return handle == other.handle; }

template<typename T> constexpr bool bleak::rev_iter_t<T>::operator!=(cref<rev_iter_t> other) const noexcept { return handle != other.handle; }

template<typename T> constexpr bool bleak::rev_iter_t<T>::operator==(cref<bleak::fwd_iter_t<T>> other) const noexcept { return handle == other.handle; }

template<typename T> constexpr bool bleak::rev_iter_t<T>::operator!=(cref<bleak::fwd_iter_t<T>> other) const noexcept { return handle != other.handle; }
