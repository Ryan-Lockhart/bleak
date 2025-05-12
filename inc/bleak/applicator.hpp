#pragma once

#include <bleak/typedef.hpp>

#include <type_traits>

#include <bleak/concepts.hpp>
#include <bleak/random.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<typename T> struct binary_applicator_t {
		DEFINE_GENERIC_USING(binary_applicator_t, T);

		const value_t true_value;
		const value_t false_value;

		constexpr binary_applicator_t() noexcept = delete;

		constexpr binary_applicator_t(cref_t true_value, cref_t false_value) noexcept : true_value{ true_value }, false_value{ false_value } {}

		constexpr binary_applicator_t(rval_t true_value, rval_t false_value) noexcept : true_value{ std::move(true_value) }, false_value{ std::move(false_value) } {}

		constexpr binary_applicator_t(cref<self_t> other) noexcept = default;

		constexpr binary_applicator_t(rval<self_t> other) noexcept = delete;

		constexpr ref<self_t> operator=(cref<self_t> other) noexcept = delete;

		constexpr ref<self_t> operator=(rval<self_t> other) noexcept = delete;

		constexpr ~binary_applicator_t() noexcept = default;

		constexpr value_t operator()(bool condition) const noexcept { return condition ? true_value : false_value; }

		template<typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr value_t operator()(ref<Randomizer> generator, f64 probability) const noexcept {
			return std::bernoulli_distribution{ probability }(generator) ? true_value : false_value;
		}

		template<typename Randomizer, typename Distribution>
			requires is_random_engine<Randomizer>::value && is_random_distribution<Distribution>::value && is_numeric<typename Distribution::result_type>::value
		constexpr value_t operator()(ref<Randomizer> generator, ref<Distribution> distribution) const noexcept {
			return distribution(generator) ? true_value : false_value;
		}
	};

	template<typename T>
		requires is_comparable<T, T, comparator_t::Ordered>::value
	struct ternary_applicator_t {
		DEFINE_GENERIC_USING(ternary_applicator_t, T);

		const value_t greater_value;
		const value_t equal_value;
		const value_t less_value;

		constexpr ternary_applicator_t() noexcept = delete;

		constexpr ternary_applicator_t(cref_t greater_value, cref_t equal_value, cref_t less_value) noexcept : greater_value{ greater_value }, equal_value{ equal_value }, less_value{ less_value } {}

		constexpr ternary_applicator_t(rval_t greater_value, rval_t equal_value, rval_t less_value) noexcept : greater_value{ std::move(greater_value) }, equal_value{ std::move(equal_value) }, less_value{ std::move(less_value) } {}

		constexpr ternary_applicator_t(cref<self_t> other) noexcept = default;

		constexpr ternary_applicator_t(rval<self_t> other) noexcept = delete;

		constexpr ref<self_t> operator=(cref<self_t> other) noexcept = delete;

		constexpr ref<self_t> operator=(rval<self_t> other) noexcept = delete;

		constexpr ~ternary_applicator_t() noexcept = default;

		constexpr value_t operator()(i32 target) const noexcept {
			if (target > 0) {
				return greater_value;
			} else if (target < 0) {
				return less_value;
			} else {
				return equal_value;
			}
		}

		template<typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr value_t operator()(ref<Randomizer> generator) const noexcept {
			const i32 value{ std::uniform_int_distribution<i32>{ -1, 1 }(generator) };

			if (value > 0) {
				return greater_value;
			} else if (value < 0) {
				return less_value;
			} else {
				return equal_value;
			}
		}

		template<typename Randomizer, typename Distribution>
			requires is_random_engine<Randomizer>::value && is_random_distribution<Distribution>::value && is_numeric<typename Distribution::result_type>::value
		constexpr value_t operator()(ref<Randomizer> generator, ref<Distribution> distribution, typename Distribution::result_type target) const noexcept {
			const auto value{ distribution(generator) };

			if (value > target) {
				return greater_value;
			} else if (value < target) {
				return less_value;
			} else {
				return equal_value;
			}
		}
	};

	template<typename T, Numeric N>
		requires std::is_convertible<T, N>::value
	struct numeric_applicator_t {
		DEFINE_GENERIC_USING_VAARGS(numeric_applicator_t, T, N);

		const value_t minimum_value;
		const value_t maximum_value;

		constexpr numeric_applicator_t() noexcept = delete;

		constexpr numeric_applicator_t(cref_t minimum_value, cref_t maximum_value) noexcept : minimum_value{ minimum_value }, maximum_value{ maximum_value } {}

		constexpr numeric_applicator_t(rval_t minimum_value, rval_t maximum_value) noexcept : minimum_value{ std::move(minimum_value) }, maximum_value{ std::move(maximum_value) } {}

		constexpr numeric_applicator_t(cref<numeric_applicator_t<T, N>> other) noexcept = default;

		constexpr numeric_applicator_t(rval<numeric_applicator_t<T, N>> other) noexcept = delete;

		constexpr ref<numeric_applicator_t<T, N>> operator=(cref<numeric_applicator_t<T, N>> other) noexcept = delete;

		constexpr ref<numeric_applicator_t<T, N>> operator=(rval<numeric_applicator_t<T, N>> other) noexcept = delete;

		constexpr ~numeric_applicator_t() noexcept = default;

		template<typename Randomizer>
			requires is_random_engine<Randomizer>::value
		constexpr value_t operator()(ref<Randomizer> generator) const noexcept {
			const N value{ std::uniform_int_distribution<N>{ static_cast<N>(minimum_value), static_cast<N>(maximum_value) }(generator) };

			return static_cast<value_t>(value);
		}
	};
} // namespace bleak
