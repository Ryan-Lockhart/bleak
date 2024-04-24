#pragma once

#include "typedef.hpp"

#include <stdexcept>
#include <utility>

namespace Bleakdepth {
	template<typename Key, typename Value> struct pair_t {
		Key key;
		Value value;

		constexpr pair_t() : key {}, value {} {}

		constexpr pair_t(cref<Key> key, cref<Value> value) : key { key }, value { value } {}

		constexpr pair_t(rval<Key> key, rval<Value> value) : key { std::move(key) }, value { std::move(value) } {}

		constexpr pair_t(cref<pair_t> other) : key { other.key }, value { other.value } {}

		constexpr pair_t(rval<pair_t> other) : key { std::move(other.key) }, value { std::move(other.value) } {}

		constexpr ref<pair_t> operator=(cref<pair_t> other) noexcept {
			key = other.key;
			value = other.value;

			return *this;
		}

		constexpr ref<pair_t> operator=(rval<pair_t> other) noexcept {
			key = std::move(other.key);
			value = std::move(other.value);

			return *this;
		}

		constexpr ~pair_t() = default;

		constexpr bool operator==(cref<pair_t> other) const noexcept {
			return key == other.key && value == other.value;
		}

		constexpr bool operator!=(cref<pair_t> other) const noexcept {
			return key != other.key || value != other.value;
		}

		constexpr bool operator<(cref<pair_t> other) const noexcept { return value < other.value; }

		constexpr bool operator>(cref<pair_t> other) const noexcept { return value > other.value; }

		constexpr bool operator<=(cref<pair_t> other) const noexcept { return value <= other.value; }

		constexpr bool operator>=(cref<pair_t> other) const noexcept { return value >= other.value; }
	};

	template<typename Key, typename Value, usize Size, typename Index = usize> class lut_t {
	  public:
		using KeyIndex = pair_t<Key, Index>;
		using KeyValue = pair_t<Key, Value>;

	  private:
		KeyIndex lookup[Size];
		Value values[Size];

	  public:
		constexpr lut_t() noexcept = default;

		template<typename... Elements, typename = KeyValue>
		constexpr lut_t(Elements... elements) noexcept : lookup {}, values {} {
			if (sizeof...(Elements) != Size) {
				throw std::invalid_argument("lookup table size mismatch!");
			}

			usize i { 0 };
			for (const auto& element : { elements... }) {
				lookup[i] = { element.key, i };
				values[i] = element.value;
				++i;
			}
		}

		constexpr lut_t(cref<lut_t> other) noexcept : lookup { other.lookup }, values { other.values } {}

		constexpr lut_t(rval<lut_t> other) noexcept :
			lookup { std::move(other.lookup) },
			values { std::move(other.values) } {}

		constexpr ref<Value> operator[](cref<Key> key) {
			for (const auto& [k, i] : lookup) {
				if (k == key) {
					return values[i];
				}
			}

			throw std::invalid_argument("key not found!");
		}

		constexpr cref<Value> operator[](cref<Key> key) const {
			for (const auto& [k, i] : lookup) {
				if (k == key) {
					return values[i];
				}
			}

			throw std::invalid_argument("key not found!");
		}
	};
} // namespace Bleakdepth
