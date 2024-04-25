#pragma once

#include "typedef.hpp"

#include <iterator>
#include <stdexcept>
#include <utility>

#include <format>

#include "point.hpp"

namespace Bleakdepth {
	struct Memory {
		// size in bytes of one byte
		static constexpr usize Byte { usize { 1 } << 0 };
		// size in bytes of one kilobyte
		static constexpr usize Kilobyte { usize { 1 } << 10 };
		// size in bytes of one megabyte
		static constexpr usize Megabyte { usize { 1 } << 20 };
		// size in bytes of one gigabyte
		static constexpr usize Gigabyte { usize { 1 } << 30 };
		// size in bytes of one terabyte
		static constexpr usize Terabyte { usize { 1 } << 40 };
		// size in bytes of one petabyte
		static constexpr usize Petabyte { usize { 1 } << 50 };
		// size in bytes of one exabyte
		static constexpr usize Exabyte { usize { 1 } << 60 };

		// size in bytes of one zettabyte minus one byte
		static constexpr usize Limit { usize { 0 } - 1 };
		// size in bytes of the maximum size of an array
		static constexpr usize Maximum { Gigabyte * 4 };
	};

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

		constexpr fwd_iter_t(pointer handle) : handle { handle } {}

		constexpr fwd_iter_t(cref<fwd_iter_t> other) : handle { other.handle } {}

		constexpr fwd_iter_t(rval<fwd_iter_t> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

		constexpr explicit fwd_iter_t(cref<rev_iter_t<T>> other) : handle { other.handle } {}

		constexpr explicit fwd_iter_t(rval<rev_iter_t<T>> other) : handle { std::move(other.handle) } {
			other.handle = nullptr;
		}

		constexpr ref<fwd_iter_t> operator=(cref<rev_iter_t<T>> other) {
			if (this == &other) {
				return *this;
			}

			handle = other.handle;
			return *this;
		}

		constexpr ref<fwd_iter_t> operator=(rval<rev_iter_t<T>> other) {
			if (this == &other) {
				return *this;
			}

			handle = std::move(other.handle);
			other.handle = nullptr;
			return *this;
		}

		constexpr ref<fwd_iter_t> operator=(cref<fwd_iter_t> other) {
			if (this == &other) {
				return *this;
			}

			handle = other.handle;
			return *this;
		}

		constexpr ref<fwd_iter_t> operator=(rval<fwd_iter_t> other) {
			if (this == &other) {
				return *this;
			}

			handle = std::move(other.handle);
			other.handle = nullptr;
			return *this;
		}

		constexpr ~fwd_iter_t() {};

		constexpr reference operator*() const noexcept { return *handle; }

		constexpr pointer operator->() const noexcept { return handle; }

		constexpr ref<fwd_iter_t> operator++() noexcept {
			++handle;
			return *this;
		}

		constexpr ref<fwd_iter_t> operator++(int) noexcept {
			auto temp { *this };
			++handle;
			return temp;
		}

		constexpr ref<fwd_iter_t> operator--() noexcept {
			--handle;
			return *this;
		}

		constexpr ref<fwd_iter_t> operator--(int) noexcept {
			auto temp { *this };
			--handle;
			return temp;
		}

		constexpr bool operator==(cref<fwd_iter_t> other) const noexcept { return handle == other.handle; }

		constexpr bool operator!=(cref<fwd_iter_t> other) noexcept { return handle != other.handle; }

		constexpr bool operator==(cref<rev_iter_t<T>> other) noexcept { return handle == other.handle; }

		constexpr bool operator!=(cref<rev_iter_t<T>> other) noexcept { return handle != other.handle; }

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

		constexpr rev_iter_t(pointer handle) : handle { handle } {}

		constexpr rev_iter_t(cref<rev_iter_t> other) : handle { other.handle } {}

		constexpr rev_iter_t(rval<rev_iter_t> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

		constexpr rev_iter_t(cref<fwd_iter_t<T>> other) : handle { other.handle } {}

		constexpr rev_iter_t(rval<fwd_iter_t<T>> other) : handle { std::move(other.handle) } { other.handle = nullptr; }

		constexpr ref<rev_iter_t> operator=(cref<rev_iter_t> other) {
			if (this == &other) {
				return *this;
			}

			handle = other.handle;
			return *this;
		}

		constexpr ref<rev_iter_t> operator=(rval<rev_iter_t> other) {
			if (this == &other) {
				return *this;
			}

			handle = std::move(other.handle);
			other.handle = nullptr;
			return *this;
		}

		constexpr ref<rev_iter_t> operator=(cref<fwd_iter_t<T>> other) {
			if (this == &other) {
				return *this;
			}

			handle = other.handle;
			return *this;
		}

		constexpr ref<rev_iter_t> operator=(rval<fwd_iter_t<T>> other) {
			if (this == &other) {
				return *this;
			}

			handle = std::move(other.handle);
			other.handle = nullptr;
			return *this;
		}

		constexpr ~rev_iter_t() {};

		constexpr reference operator*() const { return *handle; }

		constexpr pointer operator->() const { return handle; }

		constexpr ref<rev_iter_t> operator++() {
			--handle;
			return *this;
		}

		constexpr ref<rev_iter_t> operator++(int) {
			auto temp { *this };
			--handle;
			return temp;
		}

		constexpr ref<rev_iter_t> operator--() {
			++handle;
			return *this;
		}

		constexpr ref<rev_iter_t> operator--(int) {
			auto temp { *this };
			++handle;
			return temp;
		}

		constexpr friend bool operator==(cref<rev_iter_t> lhs, cref<rev_iter_t> rhs) {
			return lhs.handle == rhs.handle;
		}

		constexpr friend bool operator!=(cref<rev_iter_t> lhs, cref<rev_iter_t> rhs) {
			return lhs.handle != rhs.handle;
		}

		constexpr bool operator==(cref<rev_iter_t> other) noexcept { return handle == other.handle; }

		constexpr bool operator!=(cref<rev_iter_t> other) noexcept { return handle != other.handle; }

		constexpr bool operator==(cref<fwd_iter_t<T>> other) const noexcept { return handle == other.handle; }

		constexpr bool operator!=(cref<fwd_iter_t<T>> other) noexcept { return handle != other.handle; }

	  private:
		pointer handle;
	};

	template<typename T, usize Width> class array_t<T, Width> {
	  private:
		ptr<T> data;

	  public:
		static constexpr usize size { Width };

		static_assert(size > 0, "array_t must have a size greater than zero!");
		static_assert(size * sizeof(T) <= Memory::Maximum, "array must not exceed the maximum size of an array!");

		static constexpr usize first { 0 };
		static constexpr usize last { size - 1 };

		using iterator = fwd_iter_t<T>;
		using const_iterator = fwd_iter_t<const T>;

		using reverse_iterator = rev_iter_t<T>;
		using const_reverse_iterator = rev_iter_t<const T>;

		constexpr array_t() : data(new T[size]) {}

		constexpr array_t(cref<array_t> other) : data(new T[size]) {
			for (usize i { 0 }; i < size; ++i) {
				data[i] = other.data[i];
			}
		}

		constexpr array_t(rval<array_t> other) : data(std::move(other.data)) { other.data = nullptr; }

		constexpr ~array_t() { delete[] data; }

		constexpr ref<T> operator[](usize index) noexcept { return data[index]; }

		constexpr cref<T> operator[](usize index) const noexcept { return data[index]; }

		constexpr bool valid(usize index) const noexcept { return index < size; }

		constexpr ref<T> at(usize index) {
			if (index >= size) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr cref<T> at(usize index) const {
			if (index >= size) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr ref<T> front() { return data[first]; }

		constexpr cref<T> front() const { return data[first]; }

		constexpr ref<T> back() { return data[last]; }

		constexpr cref<T> back() const { return data[last]; }

		constexpr iterator begin() { return iterator { data }; }

		constexpr iterator end() { return iterator { data + size }; }

		constexpr const_iterator begin() const { return const_iterator { data }; }

		constexpr const_iterator end() const { return const_iterator { data + size }; }

		constexpr const_iterator cbegin() const { return const_iterator { data }; }

		constexpr const_iterator cend() const { return const_iterator { data + size }; }

		constexpr reverse_iterator rbegin() { return reverse_iterator { data + size - 1 }; }

		constexpr reverse_iterator rend() { return reverse_iterator { data - 1 }; }

		constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator { data + size - 1 }; }

		constexpr const_reverse_iterator rend() const { return const_reverse_iterator { data - 1 }; }

		constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator { data + size - 1 }; }

		constexpr const_reverse_iterator crend() const { return const_reverse_iterator { data - 1 }; }
	};

	template<typename T, usize Width> using row_t = array_t<T, Width>;

	template<typename T, usize Width, usize Height> class array_t<T, Width, Height> {
	  private:
		ptr<T> data;

	  public:
		static constexpr usize area { Width * Height };

		static constexpr usize size { area };

		static_assert(size > 0, "array_t must have a size greater than zero!");
		static_assert(size * sizeof(T) <= Memory::Maximum, "array must not exceed the maximum size of an array!");

		static constexpr usize first { 0 };
		static constexpr usize last { size - 1 };

		static inline constexpr usize flatten(uhalf i, uhalf j) { return j * Width + i; }

		static inline constexpr usize flatten(cref<point_t<uhalf>> position) { return position.y * Width + position.x; }

		static inline constexpr point_t<uhalf> unflatten(usize index) { return { index % Width, index / Width }; }

		static inline constexpr void unflatten(usize index, ref<uhalf> i, ref<uhalf> j) {
			i = index % Width;
			j = index / Width;
		}

		using iterator = fwd_iter_t<T>;
		using const_iterator = fwd_iter_t<const T>;

		using reverse_iterator = rev_iter_t<T>;
		using const_reverse_iterator = rev_iter_t<const T>;

		constexpr array_t() : data(new T[size]) {}

		constexpr array_t(cref<array_t> other) : data(new T[size]) {
			for (usize i { 0 }; i < size; ++i) {
				data[i] = other.data[i];
			}
		}

		constexpr array_t(rval<array_t> other) : data(std::move(other.data)) { other.data = nullptr; }

		constexpr ~array_t() { delete[] data; }

		constexpr ref<T> operator[](usize index) { return data[index]; }

		constexpr cref<T> operator[](usize index) const { return data[index]; }

		constexpr ref<T> operator[](uhalf i, uhalf j) { return data[flatten(i, j)]; }

		constexpr cref<T> operator[](uhalf i, uhalf j) const { return data[flatten(i, j)]; }

		constexpr ref<T> at(usize index) {
			if (index >= size) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr cref<T> at(usize index) const {
			if (index >= size) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr ref<T> at(uhalf i, uhalf j) {
			if (i >= Width || j >= Height) {
				throw std::out_of_range(std::format("index of [{}, {}] was out of range!", i, j));
			}

			return data[flatten(i, j)];
		}

		constexpr cref<T> at(uhalf i, uhalf j) const {
			usize index { flatten(i, j) };
			if (index >= size) {
				throw std::out_of_range(std::format("index of [{}, {}] was out of range!", i, j));
			}

			return data[index];
		}

		constexpr ref<T> at(cref<point_t<uhalf>> position) {
			usize index { flatten(position) };
			if (index >= size) {
				throw std::out_of_range(std::format("index of [{}, {}] was out of range!", position.x, position.y));
			}

			return data[index];
		}

		constexpr cref<T> at(cref<point_t<uhalf>> position) const {
			usize index { flatten(position) };
			if (index >= size) {
				throw std::out_of_range(std::format("index of [{}, {}] was out of range!", position.x, position.y));
			}

			return data[index];
		}

		constexpr ref<T> front() { return data[first]; }

		constexpr cref<T> front() const { return data[first]; }

		constexpr ref<T> back() { return data[last]; }

		constexpr cref<T> back() const { return data[last]; }

		constexpr iterator begin() { return iterator { data }; }

		constexpr iterator end() { return iterator { data + size }; }

		constexpr const_iterator begin() const { return const_iterator { data }; }

		constexpr const_iterator end() const { return const_iterator { data + size }; }

		constexpr const_iterator cbegin() const { return const_iterator { data }; }

		constexpr const_iterator cend() const { return const_iterator { data + size }; }

		constexpr reverse_iterator rbegin() { return reverse_iterator { data + size - 1 }; }

		constexpr reverse_iterator rend() { return reverse_iterator { data - 1 }; }

		constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator { data + size - 1 }; }

		constexpr const_reverse_iterator rend() const { return const_reverse_iterator { data - 1 }; }

		constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator { data + size - 1 }; }

		constexpr const_reverse_iterator crend() const { return const_reverse_iterator { data - 1 }; }
	};

	template<typename T, usize Width, usize Height> using layer_t = array_t<T, Width, Height>;

	template<typename T, usize Width, usize Height, usize Depth> class array_t<T, Width, Height, Depth> {
	  private:
		ptr<T> data;

	  public:
		static constexpr usize area = Width * Height;
		static constexpr usize volume = Width * Height * Depth;

		static constexpr usize size = volume;

		static_assert(size > 0, "array_t must have a size greater than zero!");
		static_assert(size * sizeof(T) <= Memory::Maximum, "array must not exceed the maximum size of an array!");

		static constexpr usize first { 0 };
		static constexpr usize last { size - 1 };

		static inline constexpr usize flatten(uhalf i, uhalf j, uhalf k) {
			return (usize)k * Width * Height + j * Width + i;
		}

		static inline constexpr usize flatten(cref<coord_t<uhalf>> position) {
			return (usize)position.z * Width * Height + position.y * Width + position.x;
		}

		static inline constexpr coord_t<uhalf> unflatten(usize index) {
			usize div { index / area };

			return coord_t<uhalf> { (uhalf)(index % Width), (uhalf)(div % Height), (uhalf)(div / Height) };
		}

		static inline constexpr void unflatten(usize index, ref<uhalf> i, ref<uhalf> j, ref<uhalf> k) {
			usize div { index / area };

			i = index % Width;
			j = div % Height;
			k = div / Height;
		}

		using iterator = fwd_iter_t<T>;
		using const_iterator = const fwd_iter_t<T>;

		using reverse_iterator = rev_iter_t<T>;
		using const_reverse_iterator = rev_iter_t<T>;

		constexpr array_t() : data(new T[size]) {}

		constexpr array_t(cref<array_t> other) : data(new T[size]) {
			for (usize i { 0 }; i < size; ++i) {
				data[i] = other.data[i];
			}
		}

		constexpr array_t(rval<array_t> other) : data(std::move(other.data)) { other.data = nullptr; }

		constexpr ~array_t() { delete[] data; }

		constexpr ref<T> operator[](usize index) { return data[index]; }

		constexpr cref<T> operator[](usize index) const { return data[index]; }

		constexpr ref<T> operator[](uhalf i, uhalf j, uhalf k) { return data[flatten(i, j, k)]; }

		constexpr cref<T> operator[](uhalf i, uhalf j, uhalf k) const { return data[flatten(i, j, k)]; }

		constexpr ref<T> at(usize index) {
			if (index >= size) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr cref<T> at(usize index) const {
			if (index >= size) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr ref<T> at(uhalf i, uhalf j, uhalf k) {
			usize index { flatten(i, j, k) };
			if (index >= size) {
				throw std::out_of_range(std::format("index of [{}, {}, {}] was out of range!", i, j, k));
			}

			return data[index];
		}

		constexpr cref<T> at(uhalf i, uhalf j, uhalf k) const {
			usize index { flatten(i, j, k) };
			if (index >= size) {
				throw std::out_of_range(std::format("index of [{}, {}, {}] was out of range!", i, j, k));
			}

			return data[index];
		}

		constexpr ref<T> front() { return data[first]; }

		constexpr cref<T> front() const { return data[first]; }

		constexpr ref<T> back() { return data[last]; }

		constexpr cref<T> back() const { return data[last]; }

		constexpr iterator begin() { return iterator { data }; }

		constexpr iterator end() { return iterator { data + size }; }

		constexpr const_iterator begin() const { return const_iterator { data }; }

		constexpr const_iterator end() const { return const_iterator { data + size }; }

		constexpr const_iterator cbegin() const { return const_iterator { data }; }

		constexpr const_iterator cend() const { return const_iterator { data + size }; }

		constexpr reverse_iterator rbegin() { return reverse_iterator { data + size - 1 }; }

		constexpr reverse_iterator rend() { return reverse_iterator { data - 1 }; }

		constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator { data + size - 1 }; }

		constexpr const_reverse_iterator rend() const { return const_reverse_iterator { data - 1 }; }

		constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator { data + size - 1 }; }

		constexpr const_reverse_iterator crend() const { return const_reverse_iterator { data - 1 }; }
	};

	template<typename T, usize Width, usize Height, usize Depth> using volume_t = array_t<T, Width, Height, Depth>;
} // namespace Bleakdepth
