#pragma once

#include <bleak/typedef.hpp>

#include <bleak/sparse.hpp>

namespace bleak {
	template<HashableByPosition... Ts> struct multi_sparse_t : private sparse_t<Ts>... {
		template<typename T> using hash_fn = T::hasher::offset;
		template<typename T> using comp_fn = T::comparator::offset;

		constexpr multi_sparse_t() : sparse_t<Ts>{}... {}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr bool contains(offset_t position) const noexcept {
			if (sparse_t<T>::empty()) {
				return false;
			}
			
			return sparse_t<T>::contains(position);
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr bool empty() const noexcept { return sparse_t<T>::empty(); }

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr size_t size() const noexcept { return sparse_t<T>::size(); }

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr void clear() noexcept { sparse_t<T>::clear(); }

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr ptr<T> operator[](offset_t position) noexcept {
			auto iter{ sparse_t<T>::find(position) };

			if (iter == sparse_t<T>::end()) {
				return nullptr;
			}

			return &cast_away(*iter);
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr cptr<T> operator[](offset_t position) const noexcept {
			cauto iter{ sparse_t<T>::find(position) };

			if (iter == sparse_t<T>::end()) {
				return nullptr;
			}

			return &(*iter);
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr ref<T> at(offset_t position) noexcept {
			auto iter{ sparse_t<T>::find(position) };

			if (iter == sparse_t<T>::end()) {
				error_log.add(std::format("cannot get value at {}; it is not within the sparse set! (get ready for fireworks)", (std::string)position));
			}

			return cast_way(*iter);
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr cref<T> at(offset_t position) const noexcept {
			cauto iter{ sparse_t<T>::find(position) };

			if (iter == sparse_t<T>::end()) {
				error_log.add(std::format("cannot get value at {}; it is not within the sparse set! (get ready for fireworks)", (std::string)position));
			}

			return *iter;
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr bool move(offset_t from, offset_t to) noexcept {
			if (cauto to_iter{ sparse_t<T>::find(to) }; to_iter != sparse_t<T>::end()) {
				return false;
			}

			cauto iter{ sparse_t<T>::find(from) };

			if (iter == sparse_t<T>::end()) {
				return false;
			}

			rvauto node{ sparse_t<T>::extract(iter) };

			node.value().position = to;

			return sparse_t<T>::insert(std::move(node)).inserted;
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr bool add(cref<T> value) noexcept {
			if (cauto iter{ sparse_t<T>::find(value.position) }; iter != sparse_t<T>::end()) {
				return false;
			}

			cauto [_, inserted] = sparse_t<T>::insert(value);

			return inserted;
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr bool add(rval<T> value) noexcept {
			if (cauto iter{ sparse_t<T>::find(value.position) }; iter != sparse_t<T>::end()) {
				return false;
			}

			cauto [_, inserted] = sparse_t<T>::emplace(std::move(value));

			return inserted;
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr bool remove(offset_t position) noexcept {
			cauto iter{ sparse_t<T>::find(position) };

			if (iter == sparse_t<T>::end()) {
				return false;
			}

			sparse_t<T>::erase(iter);

			return true;
		}

		template<HashableByPosition T>
			requires is_one_of<T, Ts...>::value
		constexpr rval<T> extract(offset_t position) {
			cauto iter{ sparse_t<T>::find(position) };

			if (iter == sparse_t<T>::end()) {
				error_log.add("ERROR: cannot extract value at {}; it is not within the sparse set!", (std::string)position);
			}

			return std::move(sparse_t<T>::extract(iter));
		}
	};
} // namespace bleak
