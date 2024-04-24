#pragma once

#include "typedef.hpp"

#include <iterator>
#include <stdexcept>
#include <utility>

#include <format>

#include "point.hpp"

namespace Bleakdepth {
	template<typename T> struct ForwardIterator {
		using iterator_category = std::contiguous_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = ptr<value_type>;
		using reference = ref<value_type>;

		constexpr ForwardIterator() = delete;

		constexpr ForwardIterator(pointer handle) : handle { handle } {}

		constexpr ~ForwardIterator() {};

		constexpr reference operator*() const { return *handle; }

		constexpr pointer operator->() const { return handle; }

		constexpr ref<ForwardIterator> operator++() {
			++handle;
			return *this;
		}

		constexpr ref<ForwardIterator> operator++(int) {
			auto temp { *this };
			++handle;
			return temp;
		}

		constexpr ref<ForwardIterator> operator--() {
			--handle;
			return *this;
		}

		constexpr ref<ForwardIterator> operator--(int) {
			auto temp { *this };
			--handle;
			return temp;
		}

		constexpr friend bool operator==(cref<ForwardIterator> lhs, cref<ForwardIterator> rhs) {
			return lhs.handle == rhs.handle;
		}

		constexpr friend bool operator!=(cref<ForwardIterator> lhs, cref<ForwardIterator> rhs) {
			return lhs.handle != rhs.handle;
		}

	  private:
		pointer handle;
	};

	template<typename T> struct ReverseIterator {
		using iterator_category = std::contiguous_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = ptr<value_type>;
		using reference = ref<value_type>;

		constexpr ReverseIterator() = delete;

		constexpr ReverseIterator(pointer handle) : handle { handle } {}

		constexpr ~ReverseIterator() {};

		constexpr reference operator*() const { return *handle; }

		constexpr pointer operator->() const { return handle; }

		constexpr ref<ReverseIterator> operator++() {
			--handle;
			return *this;
		}

		constexpr ref<ReverseIterator> operator++(int) {
			auto temp { *this };
			--handle;
			return temp;
		}

		constexpr ref<ReverseIterator> operator--() {
			++handle;
			return *this;
		}

		constexpr ref<ReverseIterator> operator--(int) {
			auto temp { *this };
			++handle;
			return temp;
		}

		constexpr friend bool operator==(cref<ReverseIterator> lhs, cref<ReverseIterator> rhs) {
			return lhs.handle == rhs.handle;
		}

		constexpr friend bool operator!=(cref<ReverseIterator> lhs, cref<ReverseIterator> rhs) {
			return lhs.handle != rhs.handle;
		}

	  private:
		pointer handle;
	};

	template<typename T, usize... Dimensions> class Array;

	template<typename T, usize Width> class Array<T, Width> {
	  private:
		ptr<T> data;

	  public:
		using iterator = ForwardIterator<T>;
		using const_iterator = const ForwardIterator<T>;

		using reverse_iterator = ReverseIterator<T>;
		using const_reverse_iterator = ReverseIterator<T>;

		constexpr Array() : data(new T[Width]) {}

		constexpr Array(cref<Array> other) : data(new T[Width]) {
			for (usize i { 0 }; i < Width; ++i) {
				data[i] = other.data[i];
			}
		}

		constexpr Array(rval<Array> other) : data(std::move(other.data)) { other.data = nullptr; }

		constexpr ~Array() { delete[] data; }

		constexpr ref<T> operator[](usize index) { return data[index]; }

		constexpr cref<T> operator[](usize index) const { return data[index]; }

		constexpr ref<T> at(usize index) {
			if (index >= Width) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr cref<T> at(usize index) const {
			if (index >= Width) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr ref<T> front() { return data[0]; }

		constexpr cref<T> front() const { return data[0]; }

		constexpr ref<T> back() { return data[Width - 1]; }

		constexpr cref<T> back() const { return data[Width - 1]; }

		constexpr iterator begin() { return iterator(data); }

		constexpr iterator end() { return iterator(data + Width); }

		constexpr const_iterator begin() const { return const_iterator(data); }

		constexpr const_iterator end() const { return const_iterator(data + Width); }

		constexpr const_iterator cbegin() const { return const_iterator(data); }

		constexpr const_iterator cend() const { return const_iterator(data + Width); }

		constexpr reverse_iterator rbegin() { return reverse_iterator(data + Width - 1); }

		constexpr reverse_iterator rend() { return reverse_iterator(data - 1); }

		constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator(data + Width - 1); }

		constexpr const_reverse_iterator rend() const { return const_reverse_iterator(data - 1); }

		constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator(data + Width - 1); }

		constexpr const_reverse_iterator crend() const { return const_reverse_iterator(data - 1); }
	};

	template<typename T, usize Width, usize Height> class Array<T, Width, Height> {
	  private:
		ptr<T> data;

	  public:
		static constexpr usize Area = Width * Height;

		static inline constexpr usize flatten(uhalf i, uhalf j) { return j * Width + i; }

		static inline constexpr usize flatten(cref<point_t<uhalf>> position) { return position.y * Width + position.x; }

		static inline constexpr point_t<uhalf> unflatten(usize index) { return { index % Width, index / Width }; }

		static inline constexpr void unflatten(usize index, ref<uhalf> i, ref<uhalf> j) {
			i = index % Width;
			j = index / Width;
		}

		using iterator = ForwardIterator<T>;
		using const_iterator = const ForwardIterator<T>;

		using reverse_iterator = ReverseIterator<T>;
		using const_reverse_iterator = ReverseIterator<T>;

		constexpr Array() : data(new T[Area]) {}

		constexpr Array(cref<Array> other) : data(new T[Area]) {
			for (usize i { 0 }; i < Area; ++i) {
				data[i] = other.data[i];
			}
		}

		constexpr Array(rval<Array> other) : data(std::move(other.data)) { other.data = nullptr; }

		constexpr ~Array() { delete[] data; }

		constexpr ref<T> operator[](usize index) { return data[index]; }

		constexpr cref<T> operator[](usize index) const { return data[index]; }

		constexpr ref<T> operator[](uhalf i, uhalf j) { return data[flatten(i, j)]; }

		constexpr cref<T> operator[](uhalf i, uhalf j) const { return data[flatten(i, j)]; }

		constexpr ref<T> at(usize index) {
			if (index >= Area) {
				throw std::out_of_range(std::format("index of {} was out of range!", index));
			}

			return data[index];
		}

		constexpr cref<T> at(usize index) const {
			if (index >= Area) {
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
			if (i >= Width || j >= Height) {
				throw std::out_of_range(std::format("index of [{}, {}] was out of range!", i, j));
			}

			return data[flatten(i, j)];
		}

		constexpr ref<T> front() { return data[0]; }

		constexpr cref<T> front() const { return data[0]; }

		constexpr ref<T> back() { return data[Area - 1]; }

		constexpr cref<T> back() const { return data[Area - 1]; }

		constexpr ref<T> front(uhalf j) { return data[j * Width]; }

		constexpr cref<T> front(uhalf j) const { return data[j * Width]; }

		constexpr ref<T> back(uhalf j) { return data[j * Width + Width - 1]; }

		constexpr cref<T> back(uhalf j) const { return data[j * Width + Width - 1]; }

		constexpr iterator begin() { return iterator(data); }

		constexpr iterator end() { return iterator(data + Area); }

		constexpr const_iterator begin() const { return const_iterator(data); }

		constexpr const_iterator end() const { return const_iterator(data + Area); }

		constexpr const_iterator cbegin() const { return const_iterator(data); }

		constexpr const_iterator cend() const { return const_iterator(data + Area); }

		constexpr reverse_iterator rbegin() { return reverse_iterator(data + Area - 1); }

		constexpr reverse_iterator rend() { return reverse_iterator(data - 1); }

		constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator(data + Area - 1); }

		constexpr const_reverse_iterator rend() const { return const_reverse_iterator(data - 1); }

		constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator(data + Area - 1); }

		constexpr const_reverse_iterator crend() const { return const_reverse_iterator(data - 1); }

		constexpr iterator begin(uhalf j) { return iterator(data + j * Width); }

		constexpr iterator end(uhalf j) { return iterator(data + (j + 1) * Width); }

		constexpr const_iterator begin(uhalf j) const { return const_iterator(data + j * Width); }

		constexpr const_iterator end(uhalf j) const { return const_iterator(data + (j + 1) * Width); }

		constexpr const_iterator cbegin(uhalf j) const { return const_iterator(data + j * Width); }

		constexpr const_iterator cend(uhalf j) const { return const_iterator(data + (j + 1) * Width); }

		constexpr reverse_iterator rbegin(uhalf j) { return reverse_iterator(data + (j + 1) * Width - 1); }

		constexpr reverse_iterator rend(uhalf j) { return reverse_iterator(data + j * Width - 1); }

		constexpr const_reverse_iterator rbegin(uhalf j) const {
			return const_reverse_iterator(data + (j + 1) * Width - 1);
		}

		constexpr const_reverse_iterator rend(uhalf j) const { return const_reverse_iterator(data + j * Width - 1); }

		constexpr const_reverse_iterator crbegin(uhalf j) const {
			return const_reverse_iterator(data + (j + 1) * Width - 1);
		}

		constexpr const_reverse_iterator crend(uhalf j) const { return const_reverse_iterator(data + j * Width - 1); }
	};
} // namespace Bleakdepth
