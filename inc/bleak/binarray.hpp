#pragma once

#include <bleak/typedef.hpp>

#include <bitset>
#include <initializer_list>
#include <type_traits>

#include <bleak/concepts.hpp>
#include <bleak/extent.hpp>
#include <bleak/hash.hpp>
#include <bleak/iter.hpp>
#include <bleak/offset.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<extent_t Size> class binarray_t {
	  private:
		std::bitset<Size.size()> data;

	  public:
	  	using bit_ref = std::bitset<Size.size()>::reference;

		static constexpr usize size{ Size.size() };

		static constexpr usize byte_size{ sizeof(data) };

		static_assert(size > 0, "array size must have a size greater than zero!");
		static_assert(sizeof(data) <= memory::Maximum, "array size must not exceed the maximum!");

		static constexpr usize first{ 0 };
		static constexpr usize last{ size - 1 };

		static constexpr extent_t::scalar_t width{ Size.w };
		static constexpr extent_t::scalar_t height{ Size.h };

		static inline constexpr extent_t::product_t flatten(cref<offset_t> offset) noexcept { return extent_t::product_cast(offset.y) * Size.w + offset.x; }

		static inline constexpr extent_t::product_t flatten(cref<offset_t::scalar_t> i, cref<offset_t::scalar_t> j) noexcept { return extent_t::product_cast(j) * Size.w + i; }

		static inline constexpr offset_t unflatten(cref<extent_t::product_t> index) noexcept { return offset_t{ index / Size.w, index % Size.w }; }

		inline constexpr bit_ref front() noexcept { return data[first]; }

		inline constexpr bool front() const noexcept { return data[first]; }

		inline constexpr bit_ref back() noexcept { return data[last]; }

		inline constexpr bool back() const noexcept { return data[last]; }

		inline constexpr binarray_t() : data{} {}

		inline constexpr binarray_t(std::array<bool, size> elements) : data{} {
			for (usize i{ 0 }; i < size; ++i) {
				data[i] = elements[i];
			}
		}

		template<typename U>
			requires std::is_convertible<U, bool>::value
		inline constexpr explicit binarray_t(std::array<U, size> elements) : data{} {
			for (usize i{ 0 }; i < size; ++i) {
				data[i] = elements[i];
			}
		}

		inline constexpr binarray_t(std::initializer_list<bool> elements) : data{} {
			if (elements.size() != size) {
				throw std::invalid_argument("initializer list size mismatch!");
			}

			usize i{ 0 };
			for (auto element : elements) {
				data[i++] = element;
			}
		}

		template<typename... Params>
			requires is_homogeneous<bool, Params...>::value && is_plurary<Params...>::value && (sizeof...(Params) <= Size.size())
		inline constexpr binarray_t(cref<Params>... elements) : data{} {
			usize i{ 0 };
			((data[i++] = elements), ...);
		}

		template<typename... Params>
			requires is_homogeneous<bool, Params...>::value && is_plurary<Params...>::value && (sizeof...(Params) <= Size.size())
		inline constexpr binarray_t(rval<Params>... elements) : data{} {
			usize i{ 0 };
			((data[i++] = std::move(elements)), ...);
		}

		inline constexpr binarray_t(cref<binarray_t> other) : data{ other.data } {}

		inline constexpr binarray_t(rval<binarray_t> other) noexcept : data{ std::move(other.data) } {}

		inline constexpr ref<binarray_t> operator=(cref<binarray_t> other) noexcept {
			if (this == &other) {
				return *this;
			}
			
			data = other.data;

			return *this;
		}

		inline constexpr ref<binarray_t> operator=(rval<binarray_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			data = std::move(other.data);

			return *this;
		}

		inline constexpr ~binarray_t() noexcept {}

		inline constexpr bit_ref operator[](cref<offset_t> offset) noexcept { return data[first + flatten(offset)]; }

		inline constexpr bool operator[](cref<offset_t> offset) const noexcept { return data[first + flatten(offset)]; }

		inline constexpr bit_ref operator[](cref<offset_t::product_t> index) noexcept { return data[first + index]; }

		inline constexpr bool operator[](cref<offset_t::product_t> index) const noexcept { return data[first + index]; }

		inline constexpr bit_ref operator[](offset_t::scalar_t i, offset_t::scalar_t j) noexcept { return data[first + flatten(i, j)]; }

		inline constexpr bool operator[](offset_t::scalar_t i, offset_t::scalar_t j) const noexcept { return data[first + flatten(i, j)]; }

		inline constexpr bool valid(cref<offset_t> offset) const noexcept { return flatten(offset) < size; }

		inline constexpr bool valid(cref<offset_t::product_t> index) const noexcept { return index < size; }

		inline constexpr bool valid(offset_t::scalar_t i, offset_t::scalar_t j) const noexcept { return flatten(i, j) < size; }

		inline constexpr bit_ref at(cref<offset_t> offset) {
			if (!valid(offset)) {
				throw std::out_of_range("offset out of range!");
			}

			return data[first + flatten(offset)];
		}

		inline constexpr bool at(cref<offset_t> offset) const {
			if (!valid(offset)) {
				throw std::out_of_range("offset out of range!");
			}

			return data[first + flatten(offset)];
		}

		inline constexpr bit_ref at(cref<offset_t::product_t> index) {
			if (!valid(index)) {
				throw std::out_of_range("offset out of range!");
			}

			return data[first + index];
		}

		inline constexpr bool at(cref<offset_t::product_t> index) const {
			if (!valid(index)) {
				throw std::out_of_range("offset out of range!");
			}

			return data[first + index];
		}

		inline constexpr bit_ref at(offset_t::scalar_t i, offset_t::scalar_t j) {
			if (!valid(i, j)) {
				throw std::out_of_range("indices out of range!");
			}

			return data[first + flatten(i, j)];
		}

		inline constexpr bool at(offset_t::scalar_t i, offset_t::scalar_t j) const {
			if (!valid(i, j)) {
				throw std::out_of_range("indices out of range!");
			}

			return data[first + flatten(i, j)];
		}

		constexpr bool operator==(cref<binarray_t> other) const noexcept {
			return data == other.data;
		}

		constexpr bool operator!=(cref<binarray_t> other) const noexcept {
			return data != other.data;
		}

		struct hasher {
			static constexpr usize operator()(cref<binarray_t<Size>> array) {
				usize seed{ 0 };

				for (usize i{ 0 }; Size.size(); ++i) {
					hash_combine(seed, array[i]);
				}

				return seed;
			}
		};
	};
} // namespace bleak
