#pragma once

#include <bleak/typedef.hpp>

#include <concepts>
#include <limits>
#include <type_traits>

namespace bleak {
	template<typename T> struct is_numeric {
		static bool constexpr value = std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value;
	};

	template<typename T> constexpr bool is_numeric_v = is_numeric<T>::value;

	template<typename T, typename U>
	concept Equatable = requires(T a, U b) {
		{ a == b } -> std::convertible_to<bool>;
		{ a != b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> struct is_equatable {
		static bool constexpr value = Equatable<T, U>;
	};

	template<typename T, typename U> constexpr bool is_equatable_v = is_equatable<T, U>::value;

	template<typename T, typename U>
	concept Comparable = requires(T a, U b) {
		{ a < b } -> std::convertible_to<bool>;
		{ a > b } -> std::convertible_to<bool>;
		{ a <= b } -> std::convertible_to<bool>;
		{ a >= b } -> std::convertible_to<bool>;
		{ a == b } -> std::convertible_to<bool>;
		{ a != b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> struct is_comparable {
		static bool constexpr value = Comparable<T, U>;
	};

	template<typename T, typename U> constexpr bool is_comparable_v = is_comparable<T, U>::value;

	template<typename T> struct is_drawable {
		static bool constexpr value = false;
	};

	template<typename T> constexpr bool is_drawable_v = is_drawable<T>::value;
} // namespace bleak
