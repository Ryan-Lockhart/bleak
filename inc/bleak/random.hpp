#pragma once

#include <bleak/typedef.hpp> // IWYU pragma: keep

#include <random> // IWYU pragma: export

namespace bleak {
	template<typename T> struct is_random_engine {
		static const bool value = false;
	};

	template<> struct is_random_engine<std::minstd_rand> {
		static const bool value = true;
	};

	template<> struct is_random_engine<std::mt19937> {
		static const bool value = true;
	};

	template<> struct is_random_engine<std::mt19937_64> {
		static const bool value = true;
	};

	template<> struct is_random_engine<std::ranlux24_base> {
		static const bool value = true;
	};

	template<> struct is_random_engine<std::ranlux48_base> {
		static const bool value = true;
	};

	template<> struct is_random_engine<std::ranlux24> {
		static const bool value = true;
	};

	template<> struct is_random_engine<std::ranlux48> {
		static const bool value = true;
	};

	template<> struct is_random_engine<std::knuth_b> {
		static const bool value = true;
	};

	template<typename T> constexpr bool is_random_engine_v = is_random_engine<T>::value;

	template<typename T> struct is_random_distribution {
		static const bool value = false;
	};

	template<> struct is_random_distribution<std::uniform_int_distribution<>> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::uniform_real_distribution<>> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::bernoulli_distribution> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::binomial_distribution<>> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::geometric_distribution<>> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::negative_binomial_distribution<>> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::poisson_distribution<>> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::exponential_distribution<>> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::gamma_distribution<>> {
		static const bool value = true;
	};

	template<> struct is_random_distribution<std::weibull_distribution<>> {
		static const bool value = true;
	};

	template<typename T> constexpr bool is_random_distribution_v = is_random_distribution<T>::value;
} // namespace bleak
