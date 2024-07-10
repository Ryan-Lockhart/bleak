#pragma once

#include "bleak/array.hpp"

#include "bleak/extent.hpp"
#include "bleak/offset.hpp"

namespace bleak {
	template<extent_1d_t Size> static constexpr extent_1d_t::product_t flatten(cref<offset_1d_t> offset) noexcept {
		return extent_1d_t::product_cast(offset.x);
	}

	template<extent_1d_t Size> static inline constexpr extent_1d_t::product_t flatten(cref<offset_1d_t::scalar_t> i) noexcept {
		return extent_1d_t::product_cast(i);
	}

	template<extent_2d_t Size> static constexpr extent_2d_t::product_t flatten(cref<offset_2d_t> offset) noexcept {
		return extent_2d_t::product_cast(offset.y * Size.w + offset.x);
	}

	template<extent_2d_t Size> static inline constexpr extent_2d_t::product_t flatten(cref<offset_2d_t::scalar_t> i, cref<offset_2d_t::scalar_t> j) noexcept {
		return extent_2d_t::product_cast(j * Size.w + i);
	}

	template<extent_3d_t Size> static constexpr extent_3d_t::product_t flatten(cref<offset_3d_t> offset) noexcept {
		return extent_3d_t::product_cast(offset.z * Size.area() + offset.y * Size.w + offset.x);
	}

	template<extent_3d_t Size> static inline constexpr extent_3d_t::product_t flatten(cref<offset_3d_t::scalar_t> i, cref<offset_3d_t::scalar_t> j, cref<offset_3d_t::scalar_t> k) noexcept {		
		return extent_3d_t::product_cast(k * Size.area() + j * Size.w + i);
	}

	template<extent_1d_t Size> static constexpr offset_1d_t unflatten(cref<extent_1d_t::product_t> index) noexcept { return offset_1d_t{ index }; }

	template<extent_2d_t Size> static constexpr offset_2d_t unflatten(cref<extent_2d_t::product_t> index) noexcept {
		lldiv_t result{ std::lldiv(index, Size.w) };
		return offset_2d_t{ result.rem, result.quot };
	}

	template<extent_3d_t Size> static constexpr offset_3d_t unflatten(cref<extent_3d_t::product_t> index) noexcept {
		lldiv_t outer{ std::lldiv(index, Size.w) };
		lldiv_t inner{ std::lldiv(outer.quot, Size.area()) };
		return offset_3d_t{ outer.rem, inner.rem, inner.quot };
	}

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
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](cref<offset_t> offset) noexcept {
		return data[first + flatten<Size>(offset)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](cref<offset_t> offset) const noexcept {
		return data[first + flatten<Size>(offset)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](cref<index_t> index) noexcept
		requires(is_2d<OffsetType> && is_2d<ExtentType>) || (is_3d<OffsetType> && is_3d<ExtentType>)
	{
		return data[first + index];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](cref<index_t> index) const noexcept
		requires(is_2d<OffsetType> && is_2d<ExtentType>) || (is_3d<OffsetType> && is_3d<ExtentType>)
	{
		return data[first + index];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](offset_1d_t::scalar_t i) noexcept
		requires is_1d<OffsetType> && is_1d<ExtentType>
	{
		return data[first + flatten<Size>(i)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](offset_2d_t::scalar_t i, offset_2d_t::scalar_t j) noexcept
		requires is_2d<OffsetType> && is_2d<ExtentType>
	{
		return data[first + flatten<Size>(i, j)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](offset_3d_t::scalar_t i, offset_3d_t::scalar_t j, offset_3d_t::scalar_t k) noexcept
		requires is_3d<OffsetType> && is_3d<ExtentType>
	{
		return data[first + flatten<Size>(i, j, k)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](offset_1d_t::scalar_t i) const noexcept
		requires is_1d<OffsetType> && is_1d<ExtentType>
	{
		return data[first + flatten<Size>(i)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::operator[](offset_2d_t::scalar_t i, offset_2d_t::scalar_t j) const noexcept
		requires is_2d<OffsetType> && is_2d<ExtentType>
	{
		return data[first + flatten<Size>(i, j)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T>
	array_t<T, OffsetType, ExtentType, Size>::operator[](offset_3d_t::scalar_t i, offset_3d_t::scalar_t j, offset_3d_t::scalar_t k) const noexcept
		requires is_3d<OffsetType> && is_3d<ExtentType>
	{
		return data[first + flatten<Size>(i, j, k)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr bool array_t<T, OffsetType, ExtentType, Size>::valid(cref<offset_t> offset) const noexcept {
		return flatten<Size>(offset) < size;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr bool array_t<T, OffsetType, ExtentType, Size>::valid(cref<index_t> index) const noexcept
		requires(is_2d<OffsetType> && is_2d<ExtentType>) || (is_3d<OffsetType> && is_3d<ExtentType>)
	{
		return index < size;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr bool array_t<T, OffsetType, ExtentType, Size>::valid(offset_1d_t::scalar_t i) const noexcept
		requires is_1d<OffsetType> && is_1d<ExtentType>
	{
		return flatten<Size>(i) < size;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr bool array_t<T, OffsetType, ExtentType, Size>::valid(offset_2d_t::scalar_t i, offset_2d_t::scalar_t j) const noexcept
		requires is_2d<OffsetType> && is_2d<ExtentType>
	{
		return flatten<Size>(i, j) < size;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr bool array_t<T, OffsetType, ExtentType, Size>::valid(offset_3d_t::scalar_t i, offset_3d_t::scalar_t j, offset_3d_t::scalar_t k) const noexcept
		requires is_3d<OffsetType> && is_3d<ExtentType>
	{
		return flatten<Size>(i, j, k) < size;
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::at(cref<offset_t> offset) {
		if (!valid(offset)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + flatten<Size>(offset)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::at(cref<offset_t> offset) const {
		if (!valid(offset)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + flatten<Size>(offset)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::at(cref<index_t> index)
		requires(is_2d<OffsetType> && is_2d<ExtentType>) || (is_3d<OffsetType> && is_3d<ExtentType>)
	{
		if (!valid(index)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + index];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::at(cref<index_t> index) const
		requires(is_2d<OffsetType> && is_2d<ExtentType>) || (is_3d<OffsetType> && is_3d<ExtentType>)
	{
		if (!valid(index)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + index];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::at(offset_1d_t::scalar_t i)
		requires is_1d<OffsetType> && is_1d<ExtentType>
	{
		if (!valid(i)) {
			throw std::out_of_range("indices out of range!");
		}

		return data[first + flatten<Size>(i)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::at(offset_2d_t::scalar_t i, offset_2d_t::scalar_t j)
		requires is_2d<OffsetType> && is_2d<ExtentType>
	{
		if (!valid(i, j)) {
			throw std::out_of_range("indices out of range!");
		}

		return data[first + flatten<Size>(i, j)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr ref<T> array_t<T, OffsetType, ExtentType, Size>::at(offset_3d_t::scalar_t i, offset_3d_t::scalar_t j, offset_3d_t::scalar_t k)
		requires is_3d<OffsetType> && is_3d<ExtentType>
	{
		if (!valid(i, j, k)) {
			throw std::out_of_range("indices out of range!");
		}

		return data[first + flatten<Size>(i, j, k)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::at(offset_1d_t::scalar_t i) const
		requires is_1d<OffsetType> && is_1d<ExtentType>
	{
		if (!valid(i)) {
			throw std::out_of_range("indices out of range!");
		}

		return data[first + flatten<Size>(i)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::at(offset_2d_t::scalar_t i, offset_2d_t::scalar_t j) const
		requires is_2d<OffsetType> && is_2d<ExtentType>
	{
		if (!valid(i, j)) {
			throw std::out_of_range("indices out of range!");
		}

		return data[first + flatten<Size>(i, j)];
	}

	template<typename T, typename OffsetType, typename ExtentType, ExtentType Size>
	constexpr cref<T> array_t<T, OffsetType, ExtentType, Size>::at(offset_3d_t::scalar_t i, offset_3d_t::scalar_t j, offset_3d_t::scalar_t k) const
		requires is_3d<OffsetType> && is_3d<ExtentType>
	{
		if (!valid(i, j, k)) {
			throw std::out_of_range("indices out of range!");
		}

		return data[first + flatten<Size>(i, j, k)];
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
