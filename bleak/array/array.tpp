#pragma once

#include "bleak/array/array.hpp"

#include "bleak/extent/extent.hpp"
#include "bleak/offset/offset.hpp"

namespace bleak {
	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::array_t() : data { new T[size] } {}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::array_t(std::initializer_list<T> elements) : data { new T[size] } {
		if (elements.size() != size) {
			throw std::invalid_argument("initializer list size mismatch!");
		}

		usize i { 0 };
		for (auto element : elements) {
			data[i++] = element;
		}
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::array_t(cref<array_t> other) : data { new T[size] } {
		for (usize i { 0 }; i < size; ++i) {
			data[i] = other.data[i];
		}
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr array_t<T, OffsetType, ExtentType, Size>::array_t(rval<array_t> other) noexcept : data { std::move(other.data) } {
		other.data = nullptr;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<array_t<T, OffsetType, ExtentType, Size>> array_t<T, OffsetType, ExtentType, Size>::operator=(cref<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		for (usize i { 0 }; i < size; ++i) {
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

	template<typename T, extent_1d_t Size> constexpr row_t<T, Size>::index_t row_t<T, Size>::flatten(offset_1d_t offset) noexcept {
		return offset.x;
	}

	template<typename T, extent_2d_t Size> constexpr extent_2d_t::product_t array_t<T, offset_2d_t, extent_2d_t, Size>::flatten(offset_2d_t offset) noexcept {
		return offset.y * Extent.width + offset.x;
	}

	template<typename T, extent_3d_t Size> constexpr extent_3d_t::product_t array_t<T, offset_3d_t, extent_3d_t, Size>::flatten(offset_3d_t offset) noexcept {
		return offset.z * Size.area() + offset.y * Size.width + offset.x;
	}

	template<typename T, extent_1d_t Size>
	constexpr array_t<T, offset_1d_t, extent_1d_t, Size>::index_t array_t<T, offset_1d_t, extent_1d_t, Size>::flatten(position_t i) noexcept
		requires OneDimensional<offset_1d_t> && OneDimensional<extent_1d_t>
	{}

	template<typename T, extent_2d_t Size>
	constexpr array_t<T, offset_2d_t, extent_2d_t, Size>::index_t array_t<T, offset_2d_t, extent_2d_t, Size>::flatten(position_t i, position_t j) noexcept
		requires TwoDimensional<offset_2d_t> && TwoDimensional<extent_2d_t>
	{}

	template<typename T, extent_3d_t Size>
	constexpr array_t<T, offset_3d_t, extent_3d_t, Size>::index_t
	array_t<T, offset_3d_t, extent_3d_t, Size>::flatten(position_t i, position_t j, position_t k) noexcept
		requires ThreeDimensional<offset_3d_t> && ThreeDimensional<extent_3d_t>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr array_t<T, OffsetType, ExtentType, Extent>::offset_t array_t<T, OffsetType, ExtentType, Extent>::unflatten(index_t index) noexcept {}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::operator[](offset_t offset) noexcept {}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::operator[](index_t i) noexcept
		requires OneDimensional<OffsetType> && OneDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::operator[](index_t i, index_t j) noexcept
		requires TwoDimensional<OffsetType> && TwoDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::operator[](index_t i, index_t j, index_t k) noexcept
		requires ThreeDimensional<OffsetType> && ThreeDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Extent>::operator[](offset_t offset) const noexcept {}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Extent>::operator[](index_t i) const noexcept
		requires OneDimensional<OffsetType> && OneDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Extent>::operator[](index_t i, index_t j) const noexcept
		requires TwoDimensional<OffsetType> && TwoDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Extent>::operator[](index_t i, index_t j, index_t k) const noexcept
		requires ThreeDimensional<OffsetType> && ThreeDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr bool array_t<T, OffsetType, ExtentType, Extent>::valid(offset_t offset) const noexcept {}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr bool array_t<T, OffsetType, ExtentType, Extent>::valid(index_t i) const noexcept
		requires OneDimensional<OffsetType> && OneDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr bool array_t<T, OffsetType, ExtentType, Extent>::valid(index_t i, index_t j) const noexcept
		requires TwoDimensional<OffsetType> && TwoDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr bool array_t<T, OffsetType, ExtentType, Extent>::valid(index_t i, index_t j, index_t k) const noexcept
		requires ThreeDimensional<OffsetType> && ThreeDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::at(offset_t offset) {}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::at(index_t i) noexcept
		requires OneDimensional<OffsetType> && OneDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::at(index_t i, index_t j) noexcept
		requires TwoDimensional<OffsetType> && TwoDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::at(index_t i, index_t j, index_t k) noexcept
		requires ThreeDimensional<OffsetType> && ThreeDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Extent>::at(offset_t offset) const {}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::at(index_t i) const noexcept
		requires OneDimensional<OffsetType> && OneDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::at(index_t i, index_t j) const noexcept
		requires TwoDimensional<OffsetType> && TwoDimensional<ExtentType>
	{}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Extent>::at(index_t i, index_t j, index_t k) const noexcept
		requires ThreeDimensional<OffsetType> && ThreeDimensional<ExtentType>
	{}

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
		return iterator { data };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::iterator array_t<T, OffsetType, ExtentType, Extent>::end() noexcept {
		return iterator { data + size };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_iterator array_t<T, OffsetType, ExtentType, Extent>::begin() const noexcept {
		return const_iterator { data };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_iterator array_t<T, OffsetType, ExtentType, Extent>::end() const noexcept {
		return const_iterator { data + size };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_iterator array_t<T, OffsetType, ExtentType, Extent>::cbegin() const noexcept {
		return const_iterator { data };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_iterator array_t<T, OffsetType, ExtentType, Extent>::cend() const noexcept {
		return const_iterator { data + size };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::rbegin() noexcept {
		return reverse_iterator { data + size - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::rend() noexcept {
		return reverse_iterator { data - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::rbegin() const noexcept {
		return const_reverse_iterator { data + size - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::rend() const noexcept {
		return const_reverse_iterator { data - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::crbegin() const noexcept {
		return const_reverse_iterator { data + size - 1 };
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Extent>
	inline constexpr array_t<T, OffsetType, ExtentType, Extent>::const_reverse_iterator array_t<T, OffsetType, ExtentType, Extent>::crend() const noexcept {
		return const_reverse_iterator { data - 1 };
	}
} // namespace bleak
