#pragma once

#include <bleak/typedef.hpp>

#include <stdexcept>
#include <unordered_set>

#include <bleak/concepts.hpp>
#include <bleak/offset.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<typename T>
		requires is_hashable_by_position<T>::value
	struct sparse_t {
		using hash_fn = T::hasher::offset;
		using comp_fn = T::comparator::offset;

		using backend_t = std::unordered_set<T, hash_fn, comp_fn>;

		constexpr sparse_t() : values{} {}

		constexpr bool contains(cref<offset_t> position) const noexcept { return values.contains(position); }

		constexpr bool empty() const noexcept { return values.empty(); }

		constexpr size_t size() const noexcept { return values.size(); }

		constexpr void clear() noexcept { values.clear(); }

		constexpr ptr<T> operator[](cref<offset_t> position) noexcept {
			auto iter{ values.find(position) };

			if (iter == values.end()) {
				return nullptr;
			}

			return &cast_away(*iter);
		}

		constexpr cptr<T> operator[](cref<offset_t> position) const noexcept {
			cauto iter{ values.find(position) };

			if (iter == values.end()) {
				return nullptr;
			}

			return &(*iter);
		}

		constexpr ref<T> at(cref<offset_t> position) {
			cauto iter{ values.find(position) };

			if (iter == values.end()) {
				throw std::out_of_range{ std::format("cannot get value at {}; it is not within the sparse set!", (std::string)position) };
			}

			return *iter;
		}

		constexpr cref<T> at(cref<offset_t> position) const {
			cauto iter{ values.find(position) };

			if (iter == values.end()) {
				throw std::out_of_range{ std::format("cannot get value at {}; it is not within the sparse set!", (std::string)position) };
			}

			return *iter;
		}

		constexpr bool move(cref<offset_t> from, cref<offset_t> to) noexcept {
			if (cauto to_iter{ values.find(to) }; to_iter != values.end()) {
				return false;
			}

			cauto iter{ values.find(from) };

			if (iter == values.end()) {
				return false;
			}

			rvauto node{ values.extract(iter) };

			node.value().position = to;

			values.insert(std::move(node));

			return true;
		}

		constexpr bool add(cref<T> value) noexcept {
			if (cauto iter{ values.find(value.position) }; iter != values.end()) {
				return false;
			}

			values.emplace(value);

			return true;
		}

		constexpr bool add(rval<T> value) noexcept {
			if (cauto iter{ values.find(value.position) }; iter != values.end()) {
				return false;
			}

			values.emplace(std::move(value));

			return true;
		}

		constexpr bool remove(cref<offset_t> position) noexcept {
			cauto iter{ values.find(position) };

			if (iter == values.end()) {
				return false;
			}

			values.erase(iter);

			return true;
		}

		constexpr rval<T> extract(cref<offset_t> position) {
			cauto iter{ values.find(position) };

			if (iter == values.end()) {
				throw std::out_of_range{ std::format("cannot extract value at {}; it is not within the sparse set!", (std::string)position) };
			}

			return std::move(values.extract(iter));
		}

		DEFINE_FWD_ITER(constexpr, backend_t, values);

	  private:
		backend_t values;
	};
} // namespace bleak
