#pragma once

#include "typedef.hpp"

#include <initializer_list>
#include <iterator>

#include "coord.hpp"
#include "point.hpp"

namespace Bleakdepth {
	namespace Memory {
		// size in bytes of one byte
		constexpr const usize Byte { usize { 1 } << 0 };
		// size in bytes of one kilobyte
		constexpr const usize Kilobyte { usize { 1 } << 10 };
		// size in bytes of one megabyte
		constexpr const usize Megabyte { usize { 1 } << 20 };
		// size in bytes of one gigabyte
		constexpr const usize Gigabyte { usize { 1 } << 30 };
		// size in bytes of one terabyte
		constexpr const usize Terabyte { usize { 1 } << 40 };
		// size in bytes of one petabyte
		constexpr const usize Petabyte { usize { 1 } << 50 };
		// size in bytes of one exabyte
		constexpr const usize Exabyte { usize { 1 } << 60 };

		// size in bytes of one zettabyte minus one byte
		constexpr const usize Limit { usize { 0 } - 1 };
		// size in bytes of the maximum size of an array
		constexpr const usize Maximum { Gigabyte * 4 };
	}; // namespace Memory

	template<typename T> struct fwd_iter_t;
	template<typename T> struct rev_iter_t;

	template<typename T, usize... Dimensions> class array_t;

	template<typename T> struct fwd_iter_t {
		using iterator_category = std::contiguous_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = ptr<value_type>;
		using reference = ref<value_type>;

		constexpr fwd_iter_t() = delete;

		constexpr fwd_iter_t(pointer handle);

		constexpr fwd_iter_t(cref<fwd_iter_t> other);

		constexpr fwd_iter_t(rval<fwd_iter_t> other);

		constexpr explicit fwd_iter_t(cref<rev_iter_t<T>> other);

		constexpr explicit fwd_iter_t(rval<rev_iter_t<T>> other);

		constexpr ref<fwd_iter_t> operator=(cref<rev_iter_t<T>> other);

		constexpr ref<fwd_iter_t> operator=(rval<rev_iter_t<T>> other);

		constexpr ref<fwd_iter_t> operator=(cref<fwd_iter_t> other);

		constexpr ref<fwd_iter_t> operator=(rval<fwd_iter_t> other);

		constexpr ~fwd_iter_t();

		constexpr reference operator*() const noexcept;

		constexpr pointer operator->() const noexcept;

		constexpr ref<fwd_iter_t> operator++() noexcept;

		constexpr fwd_iter_t operator++(int) noexcept;

		constexpr ref<fwd_iter_t> operator--() noexcept;

		constexpr fwd_iter_t operator--(int) noexcept;

		constexpr bool operator==(cref<fwd_iter_t> other) const noexcept;

		constexpr bool operator!=(cref<fwd_iter_t> other) const noexcept;

		constexpr bool operator==(cref<rev_iter_t<T>> other) const noexcept;

		constexpr bool operator!=(cref<rev_iter_t<T>> other) const noexcept;

	  private:
		pointer handle;
	};

	template<typename T> struct rev_iter_t {
		using iterator_category = std::contiguous_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = ptr<value_type>;
		using reference = ref<value_type>;

		constexpr rev_iter_t() = delete;

		constexpr rev_iter_t(pointer handle);

		constexpr rev_iter_t(cref<rev_iter_t> other);

		constexpr rev_iter_t(rval<rev_iter_t> other);

		constexpr rev_iter_t(cref<fwd_iter_t<T>> other);

		constexpr rev_iter_t(rval<fwd_iter_t<T>> other);

		constexpr ref<rev_iter_t> operator=(cref<rev_iter_t> other);

		constexpr ref<rev_iter_t> operator=(rval<rev_iter_t> other);

		constexpr ref<rev_iter_t> operator=(cref<fwd_iter_t<T>> other);

		constexpr ref<rev_iter_t> operator=(rval<fwd_iter_t<T>> other);

		constexpr ~rev_iter_t();

		constexpr reference operator*() const;

		constexpr pointer operator->() const;

		constexpr ref<rev_iter_t> operator++();

		constexpr ref<rev_iter_t> operator++(int);

		constexpr ref<rev_iter_t> operator--();

		constexpr ref<rev_iter_t> operator--(int);

		constexpr bool operator==(cref<rev_iter_t> other) const noexcept;

		constexpr bool operator!=(cref<rev_iter_t> other) const noexcept;

		constexpr bool operator==(cref<fwd_iter_t<T>> other) const noexcept;

		constexpr bool operator!=(cref<fwd_iter_t<T>> other) const noexcept;

	  private:
		pointer handle;
	};

	template<typename T, usize Width> class array_t<T, Width> {
	  private:
		ptr<T> data;

	  public:
		static constexpr usize width { Width };

		static constexpr usize size { width };

		static constexpr usize byte_size { size * sizeof(T) };

		constexpr ptr<T> data_ptr() noexcept {
			return data;
		}

		constexpr cptr<T> data_ptr() const noexcept {
			return data;
		}

		static_assert(size > 0, "array must have a size greater than zero!");
		static_assert(size * sizeof(T) <= Memory::Maximum, "array must not exceed the maximum size of an array!");

		static constexpr usize first { 0 };
		static constexpr usize last { size - 1 };

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

		constexpr ref<T> operator[](usize index) noexcept;

		constexpr cref<T> operator[](usize index) const noexcept;

		constexpr bool valid(usize index) const noexcept;

		constexpr ref<T> at(usize index);

		constexpr cref<T> at(usize index) const;

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

	template<typename T, usize Width, usize Height> class array_t<T, Width, Height> {
	  private:
		ptr<T> data;

	  public:
		static constexpr usize width { Width };
		static constexpr usize height { Height };

		static constexpr usize area { width * height };

		static constexpr usize size { area };

		static constexpr usize byte_size { size * sizeof(T) };

		constexpr ptr<T> data_ptr() noexcept {
			return data;
		}

		constexpr cptr<T> data_ptr() const noexcept {
			return data;
		}

		static_assert(size > 0, "array must have a size greater than zero!");
		static_assert(size * sizeof(T) <= Memory::Maximum, "array must not exceed the maximum size of an array!");

		static constexpr usize first { 0 };
		static constexpr usize last { size - 1 };

		static constexpr usize flatten(uhalf i, uhalf j) noexcept;

		static constexpr usize flatten(cref<point_t<uhalf>> position) noexcept;

		static constexpr point_t<uhalf> unflatten(usize index) noexcept;

		static constexpr void unflatten(usize index, ref<uhalf> i, ref<uhalf> j) noexcept;

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

		constexpr ref<T> operator[](usize index) noexcept;

		constexpr cref<T> operator[](usize index) const noexcept;

		constexpr ref<T> operator[](uhalf i, uhalf j) noexcept;

		constexpr cref<T> operator[](uhalf i, uhalf j) const noexcept;

		constexpr ref<T> operator[](cref<point_t<uhalf>> position) noexcept;

		constexpr cref<T> operator[](cref<point_t<uhalf>> position) const noexcept;

		constexpr bool valid(usize index) const noexcept;

		constexpr ref<T> at(usize index);

		constexpr cref<T> at(usize index) const;

		constexpr bool valid(uhalf i, uhalf j) const noexcept;

		constexpr ref<T> at(uhalf i, uhalf j);

		constexpr cref<T> at(uhalf i, uhalf j) const;

		constexpr bool valid(cref<point_t<uhalf>> position) const noexcept;

		constexpr ref<T> at(cref<point_t<uhalf>> position);

		constexpr cref<T> at(cref<point_t<uhalf>> position) const;

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

	template<typename T, usize Width, usize Height, usize Depth> class array_t<T, Width, Height, Depth> {
	  private:
		ptr<T> data;

	  public:
		static constexpr usize width { Width };
		static constexpr usize height { Height };
		static constexpr usize depth { Depth };

		static constexpr usize area { width * height };
		static constexpr usize volume { width * height * depth };

		static constexpr usize size { volume };

		static constexpr usize byte_size { size * sizeof(T) };

		constexpr ptr<T> data_ptr() noexcept {
			return data;
		}

		constexpr cptr<T> data_ptr() const noexcept {
			return data;
		}

		static_assert(size > 0, "array must have a size greater than zero!");
		static_assert(size * sizeof(T) <= Memory::Maximum, "array must not exceed the maximum size of an array!");

		static constexpr usize first { 0 };
		static constexpr usize last { size - 1 };

		static constexpr usize flatten(uhalf i, uhalf j, uhalf k) noexcept;

		static constexpr usize flatten(cref<coord_t<uhalf>> position) noexcept;

		static constexpr coord_t<uhalf> unflatten(usize index) noexcept;

		static constexpr void unflatten(usize index, ref<uhalf> i, ref<uhalf> j, ref<uhalf> k) noexcept;

		using iterator = fwd_iter_t<T>;
		using const_iterator = const fwd_iter_t<T>;

		using reverse_iterator = rev_iter_t<T>;
		using const_reverse_iterator = rev_iter_t<T>;

		constexpr array_t();

		constexpr array_t(std::initializer_list<T> elements);

		constexpr array_t(cref<array_t> other);

		constexpr array_t(rval<array_t> other) noexcept;

		constexpr ref<array_t> operator=(cref<array_t> other) noexcept;

		constexpr ref<array_t> operator=(rval<array_t> other) noexcept;

		constexpr ~array_t() noexcept;

		constexpr ref<T> operator[](usize index) noexcept;

		constexpr cref<T> operator[](usize index) const noexcept;

		constexpr ref<T> operator[](uhalf i, uhalf j, uhalf k) noexcept;

		constexpr cref<T> operator[](uhalf i, uhalf j, uhalf k) const noexcept;

		constexpr ref<T> operator[](cref<coord_t<uhalf>> position) noexcept;

		constexpr cref<T> operator[](cref<coord_t<uhalf>> position) const noexcept;

		constexpr bool valid(usize index) const noexcept;

		constexpr ref<T> at(usize index);

		constexpr cref<T> at(usize index) const;

		constexpr bool valid(uhalf i, uhalf j, uhalf k) const noexcept;

		constexpr ref<T> at(uhalf i, uhalf j, uhalf k);

		constexpr cref<T> at(uhalf i, uhalf j, uhalf k) const;

		constexpr bool valid(cref<coord_t<uhalf>> position) const noexcept;

		constexpr ref<T> at(cref<coord_t<uhalf>> position);

		constexpr cref<T> at(cref<coord_t<uhalf>> position) const;

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
} // namespace Bleakdepth
