#pragma once

#include <bleak/typedef.hpp>

#include <algorithm>
#include <utility>
#include <vector>

#include <bleak/concepts.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<typename T>
		requires is_equatable<T, T>::value
	struct rememberance_t {
		T current;
		T previous;

		constexpr rememberance_t() : current{}, previous{} {}

		explicit constexpr rememberance_t(cref<T> value) : current{ value }, previous{ value } {}

		constexpr rememberance_t(cref<T> current, cref<T> previous) : current{ current }, previous{ previous } {}

		constexpr rememberance_t(rval<T> current, rval<T> previous) : current{ std::move(current) }, previous{ std::move(previous) } {}

		constexpr bool operator==(cref<rememberance_t<T>> other) const { return current == other.current && previous == other.previous; }

		constexpr bool operator!=(cref<rememberance_t<T>> other) const { return current != other.current || previous != other.previous; }

		constexpr bool is_new(cref<T> value) const { return value != current; }

		constexpr bool is_old(cref<T> value) const { return value == previous; }

		constexpr bool has_swapped(cref<rememberance_t<T>> other) const { return current == other.previous && previous == other.current; }

		constexpr ref<rememberance_t<T>> remember(cref<T> value) {
			if (!is_new(value)) {
				return *this;
			}

			std::swap(current, previous);
			current = value;

			return *this;
		}

		constexpr ref<rememberance_t<T>> remember(rval<T> value) {
			if (!is_new(value)) {
				return *this;
			}

			std::swap(current, previous);
			current = std::move(value);

			return *this;
		}

		constexpr rememberance_t<T> perpetuate(cref<T> value) const { return rememberance_t<T>{ *this }.remember(value); }

		constexpr rememberance_t<T> perpetuate(rval<T> value) const { return rememberance_t<T>{ *this }.remember(std::move(value)); }

		struct hasher {
			static constexpr usize operator()(cref<rememberance_t<T>> rememberance) noexcept { return hash_combine(rememberance.current, rememberance.previous); }
		};
	};

	template<typename T> struct cenotaph {
		constexpr cenotaph() : memories{} { memories.push_back(rememberance_t<T>{}); }

		constexpr cenotaph(cref<T> value) : memories{ rememberance_t<T>{ value } } {}

		constexpr ref<rememberance_t<T>> primary() { return memories.back(); }

		constexpr cref<rememberance_t<T>> primary() const { return memories.back(); }

		constexpr ref<cenotaph<T>> memorialize(cref<T> value) {
			memories.push_back(primary().perpetuate(value));

			return *this;
		}

		constexpr ref<cenotaph<T>> memorialize(rval<T> value) {
			memories.push_back(primary().perpetuate(std::move(value)));

			return *this;
		}

		template<typename... Memories>
			requires is_homogeneous<T, Memories...>::value && is_plurary<Memories...>::value
		constexpr ref<cenotaph<T>> memorialize(cref<Memories>... values) {
			for (crauto value : { values... }) {
				memorialize(value);
			}

			return *this;
		}

		template<typename... Memories>
			requires is_homogeneous<T, Memories...>::value && is_plurary<Memories...>::value
		constexpr ref<cenotaph<T>> memorialize(rval<Memories>... values) {
			for (rvauto value : { values... }) {
				memorialize(std::move(value));
			}

			return *this;
		}

		DEFINE_ITERATORS(constexpr, std::vector<rememberance_t<T>>, memories);

	  private:
		std::vector<rememberance_t<T>> memories;
	};
} // namespace bleak
