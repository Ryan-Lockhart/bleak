#pragma once

#include "array.hpp"
#include "point.hpp"

namespace Bleakdepth {
	template<typename T, usize Width, usize Height> using layer_t = array_t<T, Width, Height>;

	template<typename T, usize Width, usize Height> inline constexpr usize array_t<T, Width, Height>::flatten(uhalf i, uhalf j) { return j * Width + i; }

	template<typename T, usize Width, usize Height> inline constexpr usize array_t<T, Width, Height>::flatten(cref<point_t<uhalf>> position) {
		return position.y * Width + position.x;
	}

	template<typename T, usize Width, usize Height> inline constexpr point_t<uhalf> array_t<T, Width, Height>::unflatten(usize index) {
		return { index % Width, index / Width };
	}

	template<typename T, usize Width, usize Height> inline constexpr void array_t<T, Width, Height>::unflatten(usize index, ref<uhalf> i, ref<uhalf> j) {
		i = index % Width;
		j = index / Width;
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::array_t() : data(new T[size]) {}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::array_t(std::initializer_list<T> elements) : data { new T[size] } {
		if (elements.size() != size) {
			throw std::invalid_argument("initializer list size mismatch!");
		}

		usize i { 0 };
		for (auto element : elements) {
			data[i++] = element;
		}
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::array_t(cref<array_t> other) : data { new T[size] } {
		for (usize i { 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::array_t(rval<array_t> other) : data { std::move(other.data) } {
		other.data = nullptr;
	}

	template<typename T, usize Width, usize Height> constexpr ref<array_t<T, Width, Height>> array_t<T, Width, Height>::operator=(cref<array_t> other) {
		if (this == &other) {
			return *this;
		}

		for (usize i { 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}

		return *this;
	}

	template<typename T, usize Width, usize Height> constexpr ref<array_t<T, Width, Height>> array_t<T, Width, Height>::operator=(rval<array_t> other) {
		if (this == &other) {
			return *this;
		}

		data = std::move(other.data);
		other.data = nullptr;
		return *this;
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::~array_t() { delete[] data; }

	template<typename T, usize Width, usize Height> constexpr ref<T> array_t<T, Width, Height>::operator[](usize index) noexcept { return data[index]; }

	template<typename T, usize Width, usize Height> constexpr cref<T> array_t<T, Width, Height>::operator[](usize index) const noexcept { return data[index]; }

	template<typename T, usize Width, usize Height> constexpr ref<T> array_t<T, Width, Height>::operator[](uhalf i, uhalf j) noexcept {
		return data[flatten(i, j)];
	}

	template<typename T, usize Width, usize Height> constexpr cref<T> array_t<T, Width, Height>::operator[](uhalf i, uhalf j) const noexcept {
		return data[flatten(i, j)];
	}

	template<typename T, usize Width, usize Height>
	inline constexpr ref<T> array_t<T, Width, Height>::operator[](cref<point_t<uhalf>> position) noexcept {
		return data[flatten(position)];
	}

	template<typename T, usize Width, usize Height>
	inline constexpr cref<T> array_t<T, Width, Height>::operator[](cref<point_t<uhalf>> position) const noexcept {
		return data[flatten(position)];
	}

	template<typename T, usize Width, usize Height> constexpr bool array_t<T, Width, Height>::valid(usize index) const noexcept { return index < size; }

	template<typename T, usize Width, usize Height> constexpr ref<T> array_t<T, Width, Height>::at(usize index) {
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of {} was out of range!", index));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height> constexpr cref<T> array_t<T, Width, Height>::at(usize index) const {
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of {} was out of range!", index));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height> constexpr bool array_t<T, Width, Height>::valid(uhalf i, uhalf j) const noexcept {
		return i < Width && j < Height;
	}

	template<typename T, usize Width, usize Height> constexpr ref<T> array_t<T, Width, Height>::at(uhalf i, uhalf j) {
		usize index { flatten(i, j) };
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", i, j));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height> constexpr cref<T> array_t<T, Width, Height>::at(uhalf i, uhalf j) const {
		usize index { flatten(i, j) };
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", i, j));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height> constexpr bool array_t<T, Width, Height>::valid(cref<point_t<uhalf>> position) const noexcept {
		return position.x < Width && position.y < Height;
	}

	template<typename T, usize Width, usize Height> constexpr ref<T> array_t<T, Width, Height>::at(cref<point_t<uhalf>> position) {
		usize index { flatten(position) };
		if (index >= size) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", position.x, position.y));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height> constexpr cref<T> array_t<T, Width, Height>::at(cref<point_t<uhalf>> position) const {
		usize index { flatten(position) };
		if (index >= size) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", position.x, position.y));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height> constexpr ref<T> array_t<T, Width, Height>::front() { return data[first]; }

	template<typename T, usize Width, usize Height> constexpr cref<T> array_t<T, Width, Height>::front() const { return data[first]; }

	template<typename T, usize Width, usize Height> constexpr ref<T> array_t<T, Width, Height>::back() { return data[last]; }

	template<typename T, usize Width, usize Height> constexpr cref<T> array_t<T, Width, Height>::back() const { return data[last]; }

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::iterator array_t<T, Width, Height>::begin() {
		return iterator { data };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::iterator array_t<T, Width, Height>::end() {
		return iterator { data + size };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::const_iterator array_t<T, Width, Height>::begin() const {
		return const_iterator { data };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::const_iterator array_t<T, Width, Height>::end() const {
		return const_iterator { data + size };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::const_iterator array_t<T, Width, Height>::cbegin() const {
		return const_iterator { data };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::const_iterator array_t<T, Width, Height>::cend() const {
		return const_iterator { data + size };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::reverse_iterator array_t<T, Width, Height>::rbegin() {
		return reverse_iterator { data + size - 1 };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::reverse_iterator array_t<T, Width, Height>::rend() {
		return reverse_iterator { data - 1 };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::const_reverse_iterator array_t<T, Width, Height>::rbegin() const {
		return const_reverse_iterator { data + size - 1 };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::const_reverse_iterator array_t<T, Width, Height>::rend() const {
		return const_reverse_iterator { data - 1 };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::const_reverse_iterator array_t<T, Width, Height>::crbegin() const {
		return const_reverse_iterator { data + size - 1 };
	}

	template<typename T, usize Width, usize Height> constexpr array_t<T, Width, Height>::const_reverse_iterator array_t<T, Width, Height>::crend() const {
		return const_reverse_iterator { data - 1 };
	}
} // namespace Bleakdepth
