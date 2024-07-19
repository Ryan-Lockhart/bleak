#pragma once

#include <bleak/typedef.hpp>

#include <cassert>
#include <initializer_list>

#include <bleak/extent.hpp>
#include <bleak/iter.hpp>
#include <bleak/offset.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<extent_t Size> static inline constexpr extent_t::product_t flatten(cref<offset_t> offset) noexcept;
	template<extent_t Size> static inline constexpr extent_t::product_t flatten(cref<offset_t::scalar_t> i, cref<offset_t::scalar_t> j) noexcept;

	template<extent_t Size> static inline constexpr offset_t unflatten(cref<extent_t::product_t> index) noexcept;

	template<typename T, extent_t Size> class array_t {
	  private:
		ptr<T> data;

	  public:
	  	static constexpr extent_t::scalar_t width{ Size.w };
		static constexpr extent_t::scalar_t height{ Size.h };

		static constexpr usize size{ Size.size() };

		static constexpr usize byte_size{ size * sizeof(T) };

		constexpr ptr<T> data_ptr() noexcept { return data; }

		constexpr cptr<T> data_ptr() const noexcept { return data; }

		static_assert(size > 0, "array must have a size greater than zero!");
		static_assert(size * sizeof(T) <= memory::maximum, "array must not exceed the maximum size of an array!");

		static constexpr usize first{ 0 };
		static constexpr usize last{ size - 1 };

		using iterator = fwd_iter_t<T>;
		using const_iterator = fwd_iter_t<const T>;

		using reverse_iterator = rev_iter_t<T>;
		using const_reverse_iterator = rev_iter_t<const T>;

		constexpr array_t();

		constexpr array_t(std::initializer_list<T> elements);

		constexpr array_t(cref<array_t> other);

		constexpr array_t(rval<array_t> other) noexcept;

		constexpr ref<array_t> operator=(cref<array_t> other) noexcept;

		constexpr ref<array_t> operator=(rval<array_t> other) noexcept;

		constexpr ~array_t() noexcept;

		constexpr ref<T> operator[](cref<offset_t> offset) noexcept;

		constexpr cref<T> operator[](cref<offset_t> offset) const noexcept;

		constexpr ref<T> operator[](cref<offset_t::product_t> index) noexcept;

		constexpr cref<T> operator[](cref<offset_t::product_t> index) const noexcept;

		constexpr ref<T> operator[](offset_t::scalar_t i, offset_t::scalar_t j) noexcept;

		constexpr cref<T> operator[](offset_t::scalar_t i, offset_t::scalar_t j) const noexcept;

		constexpr bool valid(cref<offset_t> offset) const noexcept;

		constexpr bool valid(cref<offset_t::product_t> index) const noexcept;

		constexpr bool valid(offset_t::scalar_t i, offset_t::scalar_t j) const noexcept;

		constexpr ref<T> at(cref<offset_t> offset);

		constexpr cref<T> at(cref<offset_t> offset) const;

		constexpr ref<T> at(cref<offset_t::product_t> index);

		constexpr cref<T> at(cref<offset_t::product_t> index) const;

		constexpr ref<T> at(offset_t::scalar_t i, offset_t::scalar_t j);

		constexpr cref<T> at(offset_t::scalar_t i, offset_t::scalar_t j) const;

		constexpr ref<T> front() noexcept;

		constexpr cref<T> front() const noexcept;

		constexpr ref<T> back() noexcept;

		constexpr cref<T> back() const noexcept;

		constexpr iterator begin() noexcept;

		constexpr iterator end() noexcept;

		constexpr const_iterator begin() const noexcept;

		constexpr const_iterator end() const noexcept;

		constexpr const_iterator cbegin() const noexcept;

		constexpr const_iterator cend() const noexcept;

		constexpr reverse_iterator rbegin() noexcept;

		constexpr reverse_iterator rend() noexcept;

		constexpr const_reverse_iterator rbegin() const noexcept;

		constexpr const_reverse_iterator rend() const noexcept;

		constexpr const_reverse_iterator crbegin() const noexcept;

		constexpr const_reverse_iterator crend() const noexcept;
	};

	template<extent_t Size> static constexpr extent_t::product_t flatten(cref<offset_t> offset) noexcept { return extent_t::product_cast(offset.y * Size.w + offset.x); }

	template<extent_t Size> static inline constexpr extent_t::product_t flatten(cref<offset_t::scalar_t> i, cref<offset_t::scalar_t> j) noexcept { return extent_t::product_cast(j * Size.w + i); }

	template<extent_t Size> static constexpr offset_t unflatten(cref<extent_t::product_t> index) noexcept {
		lldiv_t result{ std::lldiv(index, Size.w) };
		return offset_t{ result.rem, result.quot };
	}

	template<typename T, extent_t Size> constexpr array_t<T, Size>::array_t() : data{ new T[size] } {}

	template<typename T, extent_t Size> constexpr array_t<T, Size>::array_t(std::initializer_list<T> elements) : data{ new T[size] } {
		if (elements.size() != size) {
			throw std::invalid_argument("initializer list size mismatch!");
		}

		usize i{ 0 };
		for (auto element : elements) {
			assert(i < size);
			data[i++] = element;
		}
	}

	template<typename T, extent_t Size> constexpr array_t<T, Size>::array_t(cref<array_t> other) : data{ new T[size] } {
		for (usize i{ 0 }; i < size; ++i) {
			assert(i < size);
			data[i] = other.data[i];
		}
	}

	template<typename T, extent_t Size> constexpr array_t<T, Size>::array_t(rval<array_t> other) noexcept : data{ std::move(other.data) } { other.data = nullptr; }

	template<typename T, extent_t Size> constexpr ref<array_t<T, Size>> array_t<T, Size>::operator=(cref<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		for (usize i{ 0 }; i < size; ++i) {
			assert(i < size);
			data[i] = other.data[i];
		}

		return *this;
	}

	template<typename T, extent_t Size> constexpr ref<array_t<T, Size>> array_t<T, Size>::operator=(rval<array_t> other) noexcept {
		if (this == &other) {
			return *this;
		}

		data = std::move(other.data);
		other.data = nullptr;

		return *this;
	}

	template<typename T, extent_t Size> constexpr array_t<T, Size>::~array_t() noexcept {
		if (data != nullptr) {
			delete[] data;
			data = nullptr;
		}
	}

	template<typename T, extent_t Size> constexpr ref<T> array_t<T, Size>::operator[](cref<offset_t> offset) noexcept {
		assert(flatten<Size>(offset) < size);
		return data[first + flatten<Size>(offset)];
	}

	template<typename T, extent_t Size> constexpr cref<T> array_t<T, Size>::operator[](cref<offset_t> offset) const noexcept {
		assert(flatten<Size>(offset) < size);
		return data[first + flatten<Size>(offset)];
	}

	template<typename T, extent_t Size>
	constexpr ref<T> array_t<T, Size>::operator[](cref<offset_t::product_t> index) noexcept
	{
		assert(valid(index));
		return data[first + index];
	}

	template<typename T, extent_t Size>
	constexpr cref<T> array_t<T, Size>::operator[](cref<offset_t::product_t> index) const noexcept
	{
		assert(valid(index));
		return data[first + index];
	}

	template<typename T, extent_t Size>
	constexpr ref<T> array_t<T, Size>::operator[](offset_t::scalar_t i, offset_t::scalar_t j) noexcept
	{
		assert(valid(flatten<Size>(i, j)));
		return data[first + flatten<Size>(i, j)];
	}

	template<typename T, extent_t Size>
	constexpr cref<T> array_t<T, Size>::operator[](offset_t::scalar_t i, offset_t::scalar_t j) const noexcept
	{
		assert(valid(flatten<Size>(i, j)));
		return data[first + flatten<Size>(i, j)];
	}

	template<typename T, extent_t Size> constexpr bool array_t<T, Size>::valid(cref<offset_t> offset) const noexcept { return flatten<Size>(offset) < size; }

	template<typename T, extent_t Size>
	constexpr bool array_t<T, Size>::valid(cref<offset_t::product_t> index) const noexcept
	{
		return index < size;
	}

	template<typename T, extent_t Size>
	constexpr bool array_t<T, Size>::valid(offset_t::scalar_t i, offset_t::scalar_t j) const noexcept
	{
		return flatten<Size>(i, j) < size;
	}

	template<typename T, extent_t Size> constexpr ref<T> array_t<T, Size>::at(cref<offset_t> offset) {
		if (!valid(offset)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + flatten<Size>(offset)];
	}

	template<typename T, extent_t Size> constexpr cref<T> array_t<T, Size>::at(cref<offset_t> offset) const {
		if (!valid(offset)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + flatten<Size>(offset)];
	}

	template<typename T, extent_t Size>
	constexpr ref<T> array_t<T, Size>::at(cref<offset_t::product_t> index)
	{
		if (!valid(index)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + index];
	}

	template<typename T, extent_t Size>
	constexpr cref<T> array_t<T, Size>::at(cref<offset_t::product_t> index) const
	{
		if (!valid(index)) {
			throw std::out_of_range("offset out of range!");
		}

		return data[first + index];
	}

	template<typename T, extent_t Size>
	constexpr ref<T> array_t<T, Size>::at(offset_t::scalar_t i, offset_t::scalar_t j)
	{
		if (!valid(i, j)) {
			throw std::out_of_range("indices out of range!");
		}

		return data[first + flatten<Size>(i, j)];
	}

	template<typename T, extent_t Size>
	constexpr cref<T> array_t<T, Size>::at(offset_t::scalar_t i, offset_t::scalar_t j) const
	{
		if (!valid(i, j)) {
			throw std::out_of_range("indices out of range!");
		}

		return data[first + flatten<Size>(i, j)];
	}

	template<typename T, extent_t Size> inline constexpr ref<T> array_t<T, Size>::front() noexcept { return data[first]; }

	template<typename T, extent_t Size> inline constexpr cref<T> array_t<T, Size>::front() const noexcept { return data[first]; }

	template<typename T, extent_t Size> inline constexpr ref<T> array_t<T, Size>::back() noexcept { return data[last]; }

	template<typename T, extent_t Size> inline constexpr cref<T> array_t<T, Size>::back() const noexcept { return data[last]; }

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::iterator array_t<T, Size>::begin() noexcept {
		return iterator{ data };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::iterator array_t<T, Size>::end() noexcept {
		return iterator{ data + size };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::const_iterator array_t<T, Size>::begin() const noexcept {
		return const_iterator{ data };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::const_iterator array_t<T, Size>::end() const noexcept {
		return const_iterator{ data + size };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::const_iterator array_t<T, Size>::cbegin() const noexcept {
		return const_iterator{ data };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::const_iterator array_t<T, Size>::cend() const noexcept {
		return const_iterator{ data + size };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::reverse_iterator array_t<T, Size>::rbegin() noexcept {
		return reverse_iterator{ data + size - 1 };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::reverse_iterator array_t<T, Size>::rend() noexcept {
		return reverse_iterator{ data - 1 };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::const_reverse_iterator array_t<T, Size>::rbegin() const noexcept {
		return const_reverse_iterator{ data + size - 1 };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::const_reverse_iterator array_t<T, Size>::rend() const noexcept {
		return const_reverse_iterator{ data - 1 };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::const_reverse_iterator array_t<T, Size>::crbegin() const noexcept {
		return const_reverse_iterator{ data + size - 1 };
	}

	template<typename T, extent_t Size> inline constexpr array_t<T, Size>::const_reverse_iterator array_t<T, Size>::crend() const noexcept {
		return const_reverse_iterator{ data - 1 };
	}
} // namespace bleak
