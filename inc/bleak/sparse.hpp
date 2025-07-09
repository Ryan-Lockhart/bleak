#pragma once

#include <bleak/typedef.hpp>

#include <optional>

#include <bleak/concepts.hpp>
#include <bleak/log.hpp>
#include <bleak/offset.hpp>
#include <bleak/utility.hpp>

#include <gtl/phmap.hpp>

namespace bleak {
	template<typename T> struct sparse_t {
		using backend_t = gtl::flat_hash_map<offset_t, T, offset_t::std_hasher>;

		using node_t = typename backend_t::node_type;

		constexpr sparse_t() : data{} {}

		constexpr bool contains(offset_t position) const noexcept {
			if (data.empty()) {
				return false;
			}
			
			return data.contains(position);
		}

		constexpr bool empty() const noexcept { return data.empty(); }

		constexpr size_t size() const noexcept { return data.size(); }

		constexpr void clear() noexcept { data.clear(); }

		constexpr ptr<T> operator[](offset_t position) noexcept {
			auto iter{ data.find(position) };

			if (iter == data.end()) {
				return nullptr;
			}

			return &iter->second;
		}

		constexpr cptr<T> operator[](offset_t position) const noexcept {
			cauto iter{ data.find(position) };

			if (iter == data.end()) {
				return nullptr;
			}

			return &iter->second;
		}

		constexpr bool update(offset_t from, offset_t to) noexcept {
			if (cauto to_iter{ data.find(to) }; to_iter != data.end()) {
				return false;
			}

			cauto iter{ data.find(from) };

			if (iter == data.end()) {
				return false;
			}

			rvauto node{ data.extract(iter) };

			const_cast<ref<offset_t>>(node.key()) = to;

			return data.insert(std::move(node)).inserted;
		}

		constexpr bool add(offset_t position) noexcept {
			if (cauto iter{ data.find(position) }; iter != data.end()) {
				return false;
			}

			cauto [_, inserted] = data.emplace(position, T{});

			return inserted;
		}

		constexpr bool add(offset_t position, cref<T> value) noexcept {
			if (cauto iter{ data.find(position) }; iter != data.end()) {
				return false;
			}

			cauto [_, inserted] = data.emplace(position, value);

			return inserted;
		}

		constexpr bool add(offset_t position, rval<T> value) noexcept {
			if (cauto iter{ data.find(position) }; iter != data.end()) {
				return false;
			}

			cauto [_, inserted] = data.emplace(position, std::move(value));

			return inserted;
		}

		constexpr bool remove(offset_t position) noexcept { return data.erase(position); }

		constexpr std::optional<node_t> extract(offset_t position) {
			cauto iter{ data.find(position) };

			if (iter == data.end()) {
				return std::nullopt;
			}

			return data.extract(iter);
		}

		DEFINE_FWD_ITER(constexpr, backend_t, data);

	  private:
		backend_t data;
	};
} // namespace bleak
