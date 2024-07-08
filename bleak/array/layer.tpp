#pragma once

#include "array.hpp"
#include "extent.hpp"
#include "offset.hpp"

namespace Bleakdepth {
	template<typename T, extent_2d_t Extents> using layer_t = array_t<T, Extents>;

	template<typename T, extent_2d_t Extents> inline constexpr usize array_t<T, Extents>::flatten(uhalf i, uhalf j) noexcept {
		return static_cast<usize>(j) * Width + static_cast<usize>(i);
	}

	template<typename T, extent_2d_t Extents> inline constexpr usize array_t<T, Extents>::flatten(cref<point_t<uhalf>> position) noexcept {
		return static_cast<usize>(position.y) * Width + static_cast<usize>(position.x);
	}

	template<typename T, extent_2d_t Extents> inline constexpr point_t<uhalf> array_t<T, Extents>::unflatten(usize index) noexcept {
		return { static_cast<uhalf>(index % Width), static_cast<uhalf>(index / Width) };
	}

	template<typename T, extent_2d_t Extents> inline constexpr void array_t<T, Extents>::unflatten(usize index, ref<uhalf> i, ref<uhalf> j) noexcept {
		i = static_cast<uhalf>(index % Width);
		j = static_cast<uhalf>(index / Width);
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::array_t() : data(new T[size]) {}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::array_t(std::initializer_list<T> elements) : data { new T[size] } {
		if (elements.size() != size) {
			throw std::invalid_argument("initializer list size mismatch!");
		}

		usize i { 0 };
		for (auto element : elements) {
			data[i++] = element;
		}
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::array_t(cref<array_t> other) : data { new T[size] } {
		for (usize i { 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::array_t(rval<array_t> other) noexcept : data { std::move(other.data) } {
		other.data = nullptr;
	}

	template<typename T, extent_2d_t Extents> constexpr ref<array_t<T, Extents>> array_t<T, Extents>::operator=(cref<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		for (usize i { 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}

		return *this;
	}

	template<typename T, extent_2d_t Extents> constexpr ref<array_t<T, Extents>> array_t<T, Extents>::operator=(rval<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		data = std::move(other.data);
		other.data = nullptr;
		return *this;
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::~array_t() noexcept { delete[] data; }

	template<typename T, extent_2d_t Extents> constexpr ref<T> array_t<T, Extents>::operator[](usize index) noexcept { return data[index]; }

	template<typename T, extent_2d_t Extents> constexpr cref<T> array_t<T, Extents>::operator[](usize index) const noexcept { return data[index]; }

	template<typename T, extent_2d_t Extents> constexpr ref<T> array_t<T, Extents>::operator[](uhalf i, uhalf j) noexcept {
		return data[flatten(i, j)];
	}

	template<typename T, extent_2d_t Extents> constexpr cref<T> array_t<T, Extents>::operator[](uhalf i, uhalf j) const noexcept {
		return data[flatten(i, j)];
	}

	template<typename T, extent_2d_t Extents> inline constexpr ref<T> array_t<T, Extents>::operator[](cref<point_t<uhalf>> position) noexcept {
		return data[flatten(position)];
	}

	template<typename T, extent_2d_t Extents>
	inline constexpr cref<T> array_t<T, Extents>::operator[](cref<point_t<uhalf>> position) const noexcept {
		return data[flatten(position)];
	}

	template<typename T, extent_2d_t Extents> constexpr bool array_t<T, Extents>::valid(usize index) const noexcept { return index < size; }

	template<typename T, extent_2d_t Extents> constexpr ref<T> array_t<T, Extents>::at(usize index) {
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of {} was out of range!", index));
		}

		return data[index];
	}

	template<typename T, extent_2d_t Extents> constexpr cref<T> array_t<T, Extents>::at(usize index) const {
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of {} was out of range!", index));
		}

		return data[index];
	}

	template<typename T, extent_2d_t Extents> constexpr bool array_t<T, Extents>::valid(uhalf i, uhalf j) const noexcept {
		return i < Width && j < Height;
	}

	template<typename T, extent_2d_t Extents> constexpr ref<T> array_t<T, Extents>::at(uhalf i, uhalf j) {
		usize index { flatten(i, j) };
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", i, j));
		}

		return data[index];
	}

	template<typename T, extent_2d_t Extents> constexpr cref<T> array_t<T, Extents>::at(uhalf i, uhalf j) const {
		usize index { flatten(i, j) };
		if (!valid(index)) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", i, j));
		}

		return data[index];
	}

	template<typename T, extent_2d_t Extents> constexpr bool array_t<T, Extents>::valid(cref<point_t<uhalf>> position) const noexcept {
		return position.x < Width && position.y < Height;
	}

	template<typename T, extent_2d_t Extents> constexpr ref<T> array_t<T, Extents>::at(cref<point_t<uhalf>> position) {
		usize index { flatten(position) };
		if (index >= size) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", position.x, position.y));
		}

		return data[index];
	}

	template<typename T, extent_2d_t Extents> constexpr cref<T> array_t<T, Extents>::at(cref<point_t<uhalf>> position) const {
		usize index { flatten(position) };
		if (index >= size) {
			throw std::out_of_range(std::format("index of [{}, {}] was out of range!", position.x, position.y));
		}

		return data[index];
	}

	template<typename T, extent_2d_t Extents> constexpr ref<T> array_t<T, Extents>::front() noexcept { return data[first]; }

	template<typename T, extent_2d_t Extents> constexpr cref<T> array_t<T, Extents>::front() const noexcept { return data[first]; }

	template<typename T, extent_2d_t Extents> constexpr ref<T> array_t<T, Extents>::back() noexcept { return data[last]; }

	template<typename T, extent_2d_t Extents> constexpr cref<T> array_t<T, Extents>::back() const noexcept { return data[last]; }

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::iterator array_t<T, Extents>::begin() noexcept {
		return iterator { data };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::iterator array_t<T, Extents>::end() noexcept {
		return iterator { data + size };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::const_iterator array_t<T, Extents>::begin() const noexcept {
		return const_iterator { data };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::const_iterator array_t<T, Extents>::end() const noexcept {
		return const_iterator { data + size };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::const_iterator array_t<T, Extents>::cbegin() const noexcept {
		return const_iterator { data };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::const_iterator array_t<T, Extents>::cend() const noexcept {
		return const_iterator { data + size };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::reverse_iterator array_t<T, Extents>::rbegin() noexcept {
		return reverse_iterator { data + size - 1 };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::reverse_iterator array_t<T, Extents>::rend() noexcept {
		return reverse_iterator { data - 1 };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::const_reverse_iterator array_t<T, Extents>::rbegin() const noexcept {
		return const_reverse_iterator { data + size - 1 };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::const_reverse_iterator array_t<T, Extents>::rend() const noexcept {
		return const_reverse_iterator { data - 1 };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::const_reverse_iterator array_t<T, Extents>::crbegin() const noexcept {
		return const_reverse_iterator { data + size - 1 };
	}

	template<typename T, extent_2d_t Extents> constexpr array_t<T, Extents>::const_reverse_iterator array_t<T, Extents>::crend() const noexcept {
		return const_reverse_iterator { data - 1 };
	}
} // namespace Bleakdepth
