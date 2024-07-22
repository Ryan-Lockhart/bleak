#include <bleak/typedef.hpp>

#include <algorithm>
#include <type_traits>
#include <utility>

#include <bleak/concepts.hpp>
#include <vector>

namespace bleak {
	template<typename T> struct rememberance_t {
		T current;
		T previous;

		constexpr rememberance_t() : current{}, previous{} {}

		explicit constexpr rememberance_t(cref<T> value) : current{ value }, previous{ value } {}

		constexpr rememberance_t(cref<T> current, cref<T> previous) : current{ current }, previous{ previous } {}

		constexpr rememberance_t(rval<T> current, rval<T> previous) : current{ std::move(current) }, previous{ std::move(previous) } {}

		constexpr bool operator==(cref<rememberance_t<T>> other) const { return current == other.current && previous == other.previous; }

		constexpr bool operator!=(cref<rememberance_t<T>> other) const { return current != other.current || previous != other.previous; }

		constexpr bool is_new(cref<T> value) const { return value != current; }

		constexpr ref<T> remember(cref<T> value) {
			if (!is_new(value)) {
				return *this;
			}

			std::swap(current, previous);
			current = value;

			return *this;
		}

		constexpr ref<T> remember(rval<T> value) {
			if (!is_new(value)) {
				return *this;
			}

			std::swap(current, previous);
			current = std::move(value);

			return *this;
		}

		constexpr rememberance_t<T> perpetuate(cref<T> value) const { return rememberance_t<T>{ *this }.remember(value); }

		constexpr rememberance_t<T> perpetuate(rval<T> value) const { return rememberance_t<T>{ *this }.remember(std::move(value)); }
	};

	template<typename T> struct cenotaph {
		constexpr cenotaph() : memories{} { memories.push_back(rememberance_t<T>{}); }

		constexpr cenotaph(cref<T> value) : memories{ rememberance_t<T>{ value } } {}

		constexpr ref<rememberance_t<T>> primary() { return memories.back(); }

		constexpr cref<rememberance_t<T>> primary() const { return memories.back(); }

		constexpr ref<T> memorialize(cref<T> value) {
			memories.push_back(primary().perpetuate(value));

			return *this;
		}

		constexpr ref<T> memorialize(rval<T> value) {
			memories.push_back(primary().perpetuate(std::move(value)));

			return *this;
		}

		template<typename... Memories> requires (std::is_same<T, Memories>::value, ...) && (sizeof...(Memories) > 1) constexpr ref<T> memorialize(cref<Memories>... values) {
            for (crauto value : { values... }) {
                memorialize(value);
            }

			return *this;
		}

		template<typename... Memories> requires (std::is_same<T, Memories>::value, ...) && (sizeof...(Memories) > 1) constexpr ref<T> memorialize(rval<Memories>... values) {
            for (rvauto value : { values... }) {
                memorialize(std::move(value));
            }

			return *this;
		}

	  private:
		std::vector<rememberance_t<T>> memories;
	};
} // namespace bleak
