#pragma once

#include "bleak/array.hpp"

#include "bleak/extent.hpp"
#include "bleak/offset.hpp"

namespace bleak {
	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::array_t() : data{ new T[size] } {}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::array_t(std::initializer_list<T> elements) : data{ new T[size] } {
		if (elements.size() != size) {
			throw std::invalid_argument("initializer list size mismatch!");
		}

		usize i{ 0 };
		for (auto element : elements) {
			data[i++] = element;
		}
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::array_t(cref<array_t> other) : data{ new T[size] } {
		for (usize i{ 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::array_t(rval<array_t> other) noexcept : data{ std::move(other.data) } {
		other.data = nullptr;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<array_t<T, OffsetType, ExtentType, Size>> array_t<T, OffsetType, ExtentType, Size>::operator=(cref<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		for (usize i{ 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}

		return *this;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<array_t<T, OffsetType, ExtentType, Size>> array_t<T, OffsetType, ExtentType, Size>::operator=(rval<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		data = std::move(other.data);
		other.data = nullptr;

		return *this;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size> constexpr array_t<T, OffsetType, ExtentType, Size>::~array_t() noexcept {
		if (data != nullptr) {
			delete[] data;
			data = nullptr;
		}
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::index_t array_t<T, OffsetType, ExtentType, Size>::flatten(offset_t offset) noexcept {
		return OffsetType::flatten<Size>(offset);
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::offset_t array_t<T, OffsetType, ExtentType, Size>::unflatten(index_t index) noexcept {
		return OffsetType::unflatten<Size>(index);
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](offset_t offset) noexcept {
		return data[first + flatten(offset)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](offset_t offset) const noexcept {
		return data[first + flatten(offset)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr bool array_t<T, OffsetType, ExtentType, Size>::valid(offset_t offset) const noexcept {
		return offset_t::flatten<Size>(offset) < size;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::at(offset_t offset) {
		if (!valid(offset)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + flatten(offset)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::at(offset_t offset) const {
		if (!valid(offset)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + flatten(offset)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::front() noexcept {
		return data[first];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr cref<T> array_t<T, OffsetType, ExtentType, Extent>::front() const noexcept {
		return data[first];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::back() noexcept {
		return data[last];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr cref<T> array_t<T, OffsetType, ExtentType, Extent>::back() const noexcept {
		return data[last];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::iterator array_t<T, OffsetType, ExtentType, Extent>::begin() noexcept {
		return iterator{ data };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::iterator array_t<T, OffsetType, ExtentType, Extent>::end() noexcept {
		return iterator{ data + size };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_iterator array_t<T, OffsetType, ExtentType, Extent>::begin() const noexcept {
		return const_iterator{ data };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_iterator array_t<T, OffsetType, ExtentType, Extent>::end() const noexcept {
		return const_iterator{ data + size };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_iterator array_t<T, OffsetType, ExtentType, Extent>::cbegin() const noexcept {
		return const_iterator{ data };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_iterator array_t<T, OffsetType, ExtentType, Extent>::cend() const noexcept {
		return const_iterator{ data + size };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::rbegin() noexcept {
		return reverse_iterator{ data + size - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::rend() noexcept {
		return reverse_iterator{ data - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::rbegin() const noexcept {
		return const_reverse_iterator{ data + size - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::rend() const noexcept {
		return const_reverse_iterator{ data - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::crbegin() const noexcept {
		return const_reverse_iterator{ data + size - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::crend() const noexcept {
		return const_reverse_iterator{ data - 1 };
	}
} // namespace bleak
