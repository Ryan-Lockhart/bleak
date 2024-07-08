#pragma once

#include "bleak/typedef.hpp"

#include <iterator>

namespace bleak {
	template<typename T> struct fwd_iter_t;
	template<typename T> struct rev_iter_t;

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
} // namespace bleak

#include "bleak/array/iter.tpp" // IWYU pragma: export
