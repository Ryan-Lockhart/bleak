#pragma once

#include <bleak/typedef.hpp>

#include <array>
#include <initializer_list>
#include <type_traits>

#include <bleak/concepts.hpp>
#include <bleak/extent.hpp>
#include <bleak/hash.hpp>
#include <bleak/iter.hpp>
#include <bleak/log.hpp>
#include <bleak/offset.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<typename T, extent_t Size> struct array_t {
	  private:
		std::array<T, Size.area()> data;

	  public:
		static constexpr extent_t size{ Size };

		static constexpr usize area{ Size.size() };

		static constexpr usize byte_size{ area * sizeof(T) };

		static_assert(area > 0, "array size must have a size greater than zero!");
		static_assert(area * sizeof(T) <= memory::Maximum, "array size must not exceed the maximum!");

		static constexpr usize first{ 0 };
		static constexpr usize last{ area - 1 };

		static constexpr extent_t::scalar_t width{ size.w };
		static constexpr extent_t::scalar_t height{ size.h };

		static inline constexpr usize flatten(offset_t offset) noexcept { return static_cast<usize>(offset.y) * Size.w + offset.x; }

		static inline constexpr usize flatten(offset_t::scalar_t i, offset_t::scalar_t j) noexcept { return static_cast<usize>(j) * Size.w + i; }

		static inline constexpr offset_t unflatten(usize index) noexcept { return offset_t{ index / Size.w, index % Size.w }; }

		using iterator = fwd_iter_t<T>;
		using const_iterator = fwd_iter_t<const T>;

		using reverse_iterator = rev_iter_t<T>;
		using const_reverse_iterator = rev_iter_t<const T>;

		inline constexpr iterator begin() noexcept { return data.begin(); }

		inline constexpr iterator end() noexcept { return data.end(); }

		inline constexpr const_iterator begin() const noexcept { return data.begin(); }

		inline constexpr const_iterator end() const noexcept { return data.end(); }

		inline constexpr const_iterator cbegin() const noexcept { return data.cbegin(); }

		inline constexpr const_iterator cend() const noexcept { return data.cend(); }

		inline constexpr reverse_iterator rbegin() noexcept { return data.rbegin(); }

		inline constexpr reverse_iterator rend() noexcept { return data.rend(); }

		inline constexpr const_reverse_iterator rbegin() const noexcept { return data.rbegin(); }

		inline constexpr const_reverse_iterator rend() const noexcept { return data.rend(); }

		inline constexpr const_reverse_iterator crbegin() const noexcept { return data.crbegin(); }

		inline constexpr const_reverse_iterator crend() const noexcept { return data.crend(); }

		inline constexpr ref<T> front() noexcept { return data[first]; }

		inline constexpr cref<T> front() const noexcept { return data[first]; }

		inline constexpr ref<T> back() noexcept { return data[last]; }

		inline constexpr cref<T> back() const noexcept { return data[last]; }

		inline constexpr array_t() : data{} {}

		inline constexpr array_t(std::array<T, area> elements) : data{} {
			for (usize i{ 0 }; i < area; ++i) {
				data[i] = elements[i];
			}
		}

		template<typename U>
			requires std::is_convertible<U, T>::value
		inline constexpr explicit array_t(std::array<U, area> elements) : data{} {
			for (usize i{ 0 }; i < area; ++i) {
				data[i] = elements[i];
			}
		}

		inline constexpr array_t(std::initializer_list<T> elements) : data{} {
			if (elements.size() != area) {
				error_log.add("ERROR: initializer list size mismatch!");
			}

			usize i{ 0 };
			for (auto element : elements) {
				data[i++] = element;
			}
		}

		template<typename... Params>
			requires is_homogeneous<T, Params...>::value && is_plurary<Params...>::value && (sizeof...(Params) <= Size.size())
		inline constexpr array_t(cref<Params>... elements) : data{} {
			usize i{ 0 };
			((data[i++] = elements), ...);
		}

		template<typename... Params>
			requires is_homogeneous<T, Params...>::value && is_plurary<Params...>::value && (sizeof...(Params) <= Size.size())
		inline constexpr array_t(rval<Params>... elements) : data{} {
			usize i{ 0 };
			((data[i++] = std::move(elements)), ...);
		}

		inline constexpr array_t(cref<array_t> other) : data{} {
			for (usize i{ 0 }; i < area; ++i) {
				data[i] = other.data[i];
			}
		}

		inline constexpr array_t(rval<array_t> other) noexcept : data{ std::move(other.data) } {}

		inline constexpr ref<array_t> operator=(cref<array_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			for (usize i{ 0 }; i < area; ++i) {
				data[i] = other.data[i];
			}

			return *this;
		}

		inline constexpr ref<array_t> operator=(rval<array_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			data = std::move(other.data);

			return *this;
		}

		inline constexpr ~array_t() noexcept {}

		inline constexpr ref<T> operator[](offset_t offset) noexcept { return data[first + flatten(offset)]; }

		inline constexpr cref<T> operator[](offset_t offset) const noexcept { return data[first + flatten(offset)]; }

		inline constexpr ref<T> operator[](offset_t::product_t index) noexcept { return data[first + index]; }

		inline constexpr cref<T> operator[](offset_t::product_t index) const noexcept { return data[first + index]; }

		inline constexpr ref<T> operator[](offset_t::scalar_t i, offset_t::scalar_t j) noexcept { return data[first + flatten(i, j)]; }

		inline constexpr cref<T> operator[](offset_t::scalar_t i, offset_t::scalar_t j) const noexcept { return data[first + flatten(i, j)]; }

		inline constexpr bool is_valid(offset_t offset) const noexcept { return between<offset_t::product_t>(flatten(offset), 0, area); }

		inline constexpr bool is_valid(offset_t::product_t index) const noexcept { return between<offset_t::product_t>(index, 0, area); }

		inline constexpr bool is_valid(offset_t::scalar_t i, offset_t::scalar_t j) const noexcept { return between<offset_t::product_t>(flatten(i, j), 0, area); }

		inline constexpr ref<T> at(offset_t offset) {
			if (!is_valid(offset)) {
				error_log.add("ERROR: offset out of range!");
			}

			return data[first + flatten(offset)];
		}

		inline constexpr cref<T> at(offset_t offset) const {
			if (!is_valid(offset)) {
				error_log.add("ERROR: offset out of range!");
			}

			return data[first + flatten(offset)];
		}

		inline constexpr ref<T> at(offset_t::product_t index) {
			if (!is_valid(index)) {
				error_log.add("ERROR: offset out of range!");
			}

			return data[first + index];
		}

		inline constexpr cref<T> at(offset_t::product_t index) const {
			if (!is_valid(index)) {
				error_log.add("ERROR: offset out of range!");
			}

			return data[first + index];
		}

		inline constexpr ref<T> at(offset_t::scalar_t i, offset_t::scalar_t j) {
			if (!is_valid(i, j)) {
				error_log.add("ERROR: indices out of range!");
			}

			return data[first + flatten(i, j)];
		}

		inline constexpr cref<T> at(offset_t::scalar_t i, offset_t::scalar_t j) const {
			if (!is_valid(i, j)) {
				error_log.add("ERROR: indices out of range!");
			}

			return data[first + flatten(i, j)];
		}

		constexpr ptr<T> data_ptr() noexcept { return data; }

		constexpr cptr<T> data_ptr() const noexcept { return data; }

		constexpr bool operator==(cref<array_t> other) const noexcept {
			for (usize i{0}; i < area; ++i) {
				if (data[i] != other.data[i]) {
					return false;
				}
			}

			return true;
		}

		constexpr bool operator!=(cref<array_t> other) const noexcept {
			for (usize i{0}; i < area; ++i) {
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
