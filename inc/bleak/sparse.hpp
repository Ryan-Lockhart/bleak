#pragma once

#include <bleak/typedef.hpp>

#include <unordered_set>

#include <bleak/concepts.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	template<typename T>
		requires is_hashable_by_position<T>::value
	struct sparse_t {
		using hash_fn = T::hasher::offset;

		constexpr sparse_t() : values{} {}

		constexpr ptr<T> operator[](cref<offset_t> position) noexcept {
			auto iter{ values.find(position) };
			
			if (iter == values.end()) {
				return nullptr;
			}

			return iter;
		}

		constexpr cptr<T> operator[](cref<offset_t> position) const noexcept {
			cauto iter{ values.find(position) };
			
			if (iter == values.end()) {
				return nullptr;
			}

			return iter;
		}

	  private:
		std::unordered_set<T, hash_fn> values;
	};
} // namespace bleak
