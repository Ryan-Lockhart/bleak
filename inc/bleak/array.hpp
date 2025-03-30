#pragma once

#include <bleak/typedef.hpp>

#include <initializer_list>
#include <type_traits>

#include <bleak/concepts.hpp>
#include <bleak/extent.hpp>
#include <bleak/hash.hpp>
#include <bleak/iter.hpp>
#include <bleak/offset.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<typename T, extent_t Size> class array_t {
	  private:
		ptr<T> data;

	  public:
		static constexpr usize size{ Size.size() };

		static constexpr usize byte_size{ size * sizeof(T) };

		static_assert(size > 0, "array size must have a size greater than zero!");
		static_assert(size * sizeof(T) <= memory::Maximum, "array size must not exceed the maximum!");

		static constexpr usize first{ 0 };
		static constexpr usize last{ size - 1 };

		static constexpr extent_t::scalar_t width{ Size.w };
		static constexpr extent_t::scalar_t height{ Size.h };

		static inline constexpr usize flatten(offset_t offset) noexcept { return static_cast<usize>(offset.y) * Size.w + offset.x; }

		static inline constexpr usize flatten(offset_t::scalar_t i, offset_t::scalar_t j) noexcept { return static_cast<usize>(j) * Size.w + i; }

		static inline constexpr offset_t unflatten(usize index) noexcept { return offset_t{ index / Size.w, index % Size.w }; }

		using iterator = fwd_iter_t<T>;
		using const_iterator = fwd_iter_t<const T>;

		using reverse_iterator = rev_iter_t<T>;
		using const_reverse_iterator = rev_iter_t<const T>;

		inline constexpr iterator begin() noexcept { return iterator{ data }; }

		inline constexpr iterator end() noexcept { return iterator{ data + size }; }

		inline constexpr const_iterator begin() const noexcept { return const_iterator{ data }; }

		inline constexpr const_iterator end() const noexcept { return const_iterator{ data + size }; }

		inline constexpr const_iterator cbegin() const noexcept { return const_iterator{ data }; }

		inline constexpr const_iterator cend() const noexcept { return const_iterator{ data + size }; }

		inline constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{ data + size - 1 }; }

		inline constexpr reverse_iterator rend() noexcept { return reverse_iterator{ data - 1 }; }

		inline constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{ data + size - 1 }; }

		inline constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator{ data - 1 }; }

		inline constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{ data + size - 1 }; }

		inline constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{ data - 1 }; }

		inline constexpr ref<T> front() noexcept { return data[first]; }

		inline constexpr cref<T> front() const noexcept { return data[first]; }

		inline constexpr ref<T> back() noexcept { return data[last]; }

		inline constexpr cref<T> back() const noexcept { return data[last]; }

		inline constexpr array_t() : data{ new T[size] } {}

		inline constexpr array_t(std::array<T, size> elements) : data{ new T[size] } {
			for (usize i{ 0 }; i < size; ++i) {
				data[i] = elements[i];
			}
		}

		template<typename U>
			requires std::is_convertible<U, T>::value
		inline constexpr explicit array_t(std::array<U, size> elements) : data{ new T[size] } {
			for (usize i{ 0 }; i < size; ++i) {
				data[i] = elements[i];
			}
		}

		inline constexpr array_t(std::initializer_list<T> elements) : data{ new T[size] } {
			if (elements.size() != size) {
				throw std::invalid_argument("initializer list size mismatch!");
			}

			usize i{ 0 };
			for (auto element : elements) {
				data[i++] = element;
			}
		}

		template<typename... Params>
			requires is_homogeneous<T, Params...>::value && is_plurary<Params...>::value && (sizeof...(Params) <= Size.size())
		inline constexpr array_t(cref<Params>... elements) : data{ new T[size] } {
			usize i{ 0 };
			((data[i++] = elements), ...);
		}

		template<typename... Params>
			requires is_homogeneous<T, Params...>::value && is_plurary<Params...>::value && (sizeof...(Params) <= Size.size())
		inline constexpr array_t(rval<Params>... elements) : data{ new T[size] } {
			usize i{ 0 };
			((data[i++] = std::move(elements)), ...);
		}

		inline constexpr array_t(cref<array_t> other) : data{ new T[size] } {
			for (usize i{ 0 }; i < size; ++i) {
				data[i] = other.data[i];
			}
		}

		inline constexpr array_t(rval<array_t> other) noexcept : data{ std::move(other.data) } { other.data = nullptr; }

		inline constexpr ref<array_t> operator=(cref<array_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			for (usize i{ 0 }; i < size; ++i) {
				data[i] = other.data[i];
			}

			return *this;
		}

		inline constexpr ref<array_t> operator=(rval<array_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			data = std::move(other.data);
			other.data = nullptr;

			return *this;
		}

		inline constexpr ~array_t() noexcept {
			if (data != nullptr) {
				delete[] data;
				data = nullptr;
			}
		}

		inline constexpr ref<T> operator[](offset_t offset) noexcept { return data[first + flatten(offset)]; }

		inline constexpr cref<T> operator[](offset_t offset) const noexcept { return data[first + flatten(offset)]; }

		inline constexpr ref<T> operator[](offset_t::product_t index) noexcept { return data[first + index]; }

		inline constexpr cref<T> operator[](offset_t::product_t index) const noexcept { return data[first + index]; }

		inline constexpr ref<T> operator[](offset_t::scalar_t i, offset_t::scalar_t j) noexcept { return data[first + flatten(i, j)]; }

		inline constexpr cref<T> operator[](offset_t::scalar_t i, offset_t::scalar_t j) const noexcept { return data[first + flatten(i, j)]; }

		inline constexpr bool valid(offset_t offset) const noexcept { return flatten(offset) < size; }

		inline constexpr bool valid(offset_t::product_t index) const noexcept { return index < size; }

		inline constexpr bool valid(offset_t::scalar_t i, offset_t::scalar_t j) const noexcept { return flatten(i, j) < size; }

		inline constexpr ref<T> at(offset_t offset) {
			if (!valid(offset)) {
				throw std::out_of_range("offset out of range!");
			}

			return data[first + flatten(offset)];
		}

		inline constexpr cref<T> at(offset_t offset) const {
			if (!valid(offset)) {
				throw std::out_of_range("offset out of range!");
			}

			return data[first + flatten(offset)];
		}

		inline constexpr ref<T> at(offset_t::product_t index) {
			if (!valid(index)) {
				throw std::out_of_range("offset out of range!");
			}

			return data[first + index];
		}

		inline constexpr cref<T> at(offset_t::product_t index) const {
			if (!valid(index)) {
				throw std::out_of_range("offset out of range!");
			}

			return data[first + index];
		}

		inline constexpr ref<T> at(offset_t::scalar_t i, offset_t::scalar_t j) {
			if (!valid(i, j)) {
				throw std::out_of_range("indices out of range!");
			}

			return data[first + flatten(i, j)];
		}

		inline constexpr cref<T> at(offset_t::scalar_t i, offset_t::scalar_t j) const {
			if (!valid(i, j)) {
				throw std::out_of_range("indices out of range!");
			}

			return data[first + flatten(i, j)];
		}

		constexpr ptr<T> data_ptr() noexcept { return data; }

		constexpr cptr<T> data_ptr() const noexcept { return data; }

		constexpr bool operator==(cref<array_t> other) const noexcept {
			for (usize i{0}; i < size; ++i) {
				if (data[i] != other.data[i]) {
					return false;
				}
			}

			return true;
		}

		constexpr bool operator!=(cref<array_t> other) const noexcept {
			for (usize i{0}; i < size; ++i) {
				if (data[i] != other.data[i]) {
					return true;
				}
			}

			return false;
		}

		struct hasher {
			static constexpr usize operator()(cref<array_t<T, Size>> array) { return hash_array(array.begin(), array.end()); }
		};
	};
} // namespace bleak
