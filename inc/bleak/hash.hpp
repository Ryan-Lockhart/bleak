#pragma once

#include <bleak/typedef.hpp>

#include <functional>
#include <limits>

#include <bleak/concepts.hpp>

namespace bleak {
	template<typename T, typename... Values> constexpr void hash_combine(ref<usize> seed, cref<T> v, cref<Values>... values) {
		static constexpr auto digits = std::numeric_limits<std::size_t>::digits;
		static_assert(digits == 64 || digits == 32);

		if constexpr (digits == 64) {
			if constexpr (is_hashable<T>::value) {
				seed += 0x9e3779b9 + T::hasher::operator()(v);
			} else {
				seed += 0x9e3779b9 + std::hash<T>()(v);
			}

			static constexpr std::size_t m = 0xe9846af9b1a615d;

			seed ^= seed >> 32;
			seed *= m;
			seed ^= seed >> 32;
			seed *= m;
			seed ^= seed >> 28;
		} else {
			if constexpr (is_hashable<T>::value) {
				seed += 0x9e3779b9 + T::hasher::operator()(v);
			} else {
				seed += 0x9e3779b9 + std::hash<T>()(v);
			}

			static constexpr std::size_t m1 = 0x21f0aaad;
			static constexpr std::size_t m2 = 0x735a2d97;

			seed ^= seed >> 16;
			seed *= m1;
			seed ^= seed >> 15;
			seed *= m2;
			seed ^= seed >> 15;
		}

		(hash_combine(seed, values), ...);
	}

	template<typename T, typename... Values> static constexpr usize hash_combine(cref<T> v, cref<Values>... values) noexcept {
		usize seed{ 0 };

		(hash_combine(seed, v, values), ...);

		return seed;
	}

	template<typename T> static constexpr void hash_array(ref<usize> seed, cptr<T> begin_iter, cptr<T> end_iter) noexcept {
		for (cauto iter{ begin_iter }; iter != end_iter; ++iter) {
			hash_combine(seed, *iter);
		}
	}

	template<typename T> static constexpr usize hash_array(cptr<T> begin_iter, cptr<T> end_iter) noexcept {
		usize seed{ 0 };

		hash_array(seed, begin_iter, end_iter);

		return seed;
	}
} // namespace bleak
