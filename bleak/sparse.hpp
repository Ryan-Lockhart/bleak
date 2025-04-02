#pragma once

#include <bleak/typedef.hpp>

#include <stdexcept>
#include <unordered_set>

#include <bleak/concepts.hpp>
#include <bleak/offset.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<typename T> struct sparseling_t {
		T value;
		offset_t position;

		constexpr sparseling_t(cref<offset_t> position) : value{}, position{ position } {}

		constexpr sparseling_t(cref<T> value, cref<offset_t> position) : value{ value }, position{ position } {}

		constexpr sparseling_t(rval<T> value, cref<offset_t> position) : value{ std::move(value) }, position{ position } {}

		constexpr sparseling_t(cref<sparseling_t> other) : value{ other.value }, position{ other.position } {}

		constexpr sparseling_t(rval<sparseling_t> other) : value{ std::move(other.value) }, position{ std::move(other.position) } {}
		
		constexpr ref<sparseling_t<T>> operator=(cref<sparseling_t> other) {
			value = other.value;
			position = other.position;

			return *this;
		}

		constexpr ref<sparseling_t<T>> operator=(rval<sparseling_t> other) {
			value = std::move(other.value);
			position = std::move(other.position);

			return *this;
		}

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr size_t operator()(cref<sparseling_t> sparseling) noexcept { return offset_t::hasher::operator()(sparseling.position); }

				static constexpr size_t operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;

				static constexpr bool operator()(cref<sparseling_t> lhs, cref<sparseling_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<sparseling_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(cref<offset_t> lhs, cref<sparseling_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};

	template<typename T>
		requires is_hashable_by_position<T>::value
	struct sparse_t {
		using hash_fn = T::hasher::offset;
		using comp_fn = T::comparator::offset;

		using backend_t = std::unordered_set<T, hash_fn, comp_fn>;

		constexpr sparse_t() : values{} {}

		constexpr bool contains(cref<offset_t> position) const noexcept {
			if (values.empty()) {
				return false;
			}
			
			return values.contains(position);
		}

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
			auto iter{ values.find(position) };

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

			return values.insert(std::move(node)).inserted;
		}

		constexpr bool add(cref<T> value) noexcept {
			if (cauto iter{ values.find(value.position) }; iter != values.end()) {
				return false;
			}

			values.insert(value);

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
