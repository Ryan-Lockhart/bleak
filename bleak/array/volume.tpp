#pragma once

#include "array.hpp"

namespace Bleakdepth {
	template<typename T, usize Width, usize Height, usize Depth> using volume_t = array_t<T, Width, Height, Depth>;

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr usize array_t<T, Width, Height, Depth>::flatten(uhalf i, uhalf j, uhalf k) noexcept {
		return k * area + j * Width + i;
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr usize array_t<T, Width, Height, Depth>::flatten(cref<coord_t<uhalf>> position) noexcept {
		return position.z * area + position.y * Width + position.x;
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr coord_t<uhalf> array_t<T, Width, Height, Depth>::unflatten(usize index) noexcept {
		usize div { index / Width };
		return coord_t<uhalf> { (uhalf)(index % Width), (uhalf)(div % Height), (uhalf)(div / Height) };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr void array_t<T, Width, Height, Depth>::unflatten(usize index, ref<uhalf> i, ref<uhalf> j, ref<uhalf> k) noexcept {
		usize div { index / Width };

		i = index % Width;
		j = div % Height;
		k = div / Height;
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr array_t<T, Width, Height, Depth>::array_t() : data(new T[size]) {}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::array_t(std::initializer_list<T> elements) : data { new T[size] } {
		if (elements.size() != size) {
			throw std::invalid_argument("initializer list size mismatch!");
		}

		usize i { 0 };
		for (auto element : elements) {
			data[i++] = element;
		}
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::array_t(cref<array_t> other) : data { new T[size] } {
		for (usize i { 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::array_t(rval<array_t> other) noexcept : data { std::move(other.data) } {
		other.data = nullptr;
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr ref<array_t<T, Width, Height, Depth>> array_t<T, Width, Height, Depth>::operator=(cref<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		for (usize i { 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}

		return *this;
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr ref<array_t<T, Width, Height, Depth>> array_t<T, Width, Height, Depth>::operator=(rval<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		data = std::move(other.data);
		other.data = nullptr;
		return *this;
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr array_t<T, Width, Height, Depth>::~array_t() noexcept { delete[] data; }

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr ref<T> array_t<T, Width, Height, Depth>::operator[](usize index) noexcept {
		return data[index];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr cref<T> array_t<T, Width, Height, Depth>::operator[](usize index) const noexcept {
		return data[index];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr ref<T> array_t<T, Width, Height, Depth>::operator[](uhalf i, uhalf j, uhalf k) noexcept {
		return data[flatten(i, j, k)];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr cref<T> array_t<T, Width, Height, Depth>::operator[](uhalf i, uhalf j, uhalf k) const noexcept {
		return data[flatten(i, j, k)];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr ref<T> array_t<T, Width, Height, Depth>::operator[](cref<coord_t<uhalf>> position) noexcept {
		return data[flatten(position)];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr cref<T> array_t<T, Width, Height, Depth>::operator[](cref<coord_t<uhalf>> position) const noexcept {
		return data[flatten(position)];
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr bool array_t<T, Width, Height, Depth>::valid(usize index) const noexcept {
		return index < size;
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr ref<T> array_t<T, Width, Height, Depth>::at(usize index) {
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of {} was out of range!", index));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr cref<T> array_t<T, Width, Height, Depth>::at(usize index) const {
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of {} was out of range!", index));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr bool array_t<T, Width, Height, Depth>::valid(uhalf i, uhalf j, uhalf k) const noexcept {
		return i < Width && j < Height && k < Depth;
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr ref<T> array_t<T, Width, Height, Depth>::at(uhalf i, uhalf j, uhalf k) {
		usize index { flatten(i, j, k) };
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", i, j));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr cref<T> array_t<T, Width, Height, Depth>::at(uhalf i, uhalf j, uhalf k) const {
		usize index { flatten(i, j, k) };
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of [{}, {}, {}] was out of range!", i, j, k));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr bool array_t<T, Width, Height, Depth>::valid(cref<coord_t<uhalf>> position) const noexcept {
		return position.x < Width && position.y < Height && position.z < Depth;
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr ref<T> array_t<T, Width, Height, Depth>::at(cref<coord_t<uhalf>> position) {
		usize index { flatten(position) };
		if (index >= size) {
			throw std::out_of_range(std::format("index of [{}, {}, {}] was out of range!", position.x, position.y, position.z));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr cref<T> array_t<T, Width, Height, Depth>::at(cref<coord_t<uhalf>> position) const {
		usize index { flatten(position) };
		if (index >= size) {
			throw std::out_of_range(std::format("index of [{}, {}, {}] was out of range!", position.x, position.y, position.z));
		}

		return data[index];
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr ref<T> array_t<T, Width, Height, Depth>::front() noexcept { return data[first]; }

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr cref<T> array_t<T, Width, Height, Depth>::front() const noexcept {
		return data[first];
	}

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr ref<T> array_t<T, Width, Height, Depth>::back() noexcept { return data[last]; }

	template<typename T, usize Width, usize Height, usize Depth> inline constexpr cref<T> array_t<T, Width, Height, Depth>::back() const noexcept { return data[last]; }

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::iterator array_t<T, Width, Height, Depth>::begin() noexcept {
		return iterator { data };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::iterator array_t<T, Width, Height, Depth>::end() noexcept {
		return iterator { data + size };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::const_iterator array_t<T, Width, Height, Depth>::begin() const noexcept {
		return const_iterator { data };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::const_iterator array_t<T, Width, Height, Depth>::end() const noexcept {
		return const_iterator { data + size };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::const_iterator array_t<T, Width, Height, Depth>::cbegin() const noexcept {
		return const_iterator { data };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::const_iterator array_t<T, Width, Height, Depth>::cend() const noexcept {
		return const_iterator { data + size };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::reverse_iterator array_t<T, Width, Height, Depth>::rbegin() noexcept {
		return reverse_iterator { data + size - 1 };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::reverse_iterator array_t<T, Width, Height, Depth>::rend() noexcept {
		return reverse_iterator { data - 1 };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::const_reverse_iterator array_t<T, Width, Height, Depth>::rbegin() const noexcept {
		return const_reverse_iterator { data + size - 1 };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::const_reverse_iterator array_t<T, Width, Height, Depth>::rend() const noexcept {
		return const_reverse_iterator { data - 1 };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::const_reverse_iterator array_t<T, Width, Height, Depth>::crbegin() const noexcept {
		return const_reverse_iterator { data + size - 1 };
	}

	template<typename T, usize Width, usize Height, usize Depth>
	inline constexpr array_t<T, Width, Height, Depth>::const_reverse_iterator array_t<T, Width, Height, Depth>::crend() const noexcept {
		return const_reverse_iterator { data - 1 };
	}
} // namespace Bleakdepth
