#pragma once

#include <bleak/typedef.hpp>

#include <concepts>
#include <limits>
#include <type_traits>

#include <bleak/primitive.hpp>

// macro for explicit instantiation of value type traits
#define TYPE_TRAIT_VALUE(trait, T, val)		\
	template<> struct trait<T> {			\
		static constexpr decltype(val) value = val;	\
	}

// macro to enable type trait for type
#define ENABLE_TYPE_TRAIT(trait, T) \
	TYPE_TRAIT_VALUE(trait, T, true)

#define DISABLE_TYPE_TRAIT(trait, T) \
	TYPE_TRAIT_VALUE(trait, T, false)

// macro for explicit instantiation of using type traits
#define TYPE_TRAIT_USING(trait, T, val) \
	template<> struct trait<T> {		\
		using type = val;				\
	}

// macro for explicit instantiation of type trait comparators
#define TYPE_TRAIT_COMPARATOR(trait, T, U, val) \
	template<> struct trait<T, U> {		\
		static constexpr decltype(val) value = val;	\
	}

namespace std {
	TYPE_TRAIT_VALUE(is_floating_point, bleak::f16, true);
	TYPE_TRAIT_VALUE(is_floating_point, bleak::f128, true);
}

namespace bleak {
	template<typename T> concept Integer = std::numeric_limits<T>::is_integer;

	template<typename T> struct is_integer {
		static bool constexpr value = Integer<T>;
	};

	template<typename T> constexpr bool is_integer_v = is_integer<T>::value;

	template<typename T> concept SignedInteger = Integer<T> && std::numeric_limits<T>::is_signed;

	template<typename T> struct is_signed_integer {
		static constexpr bool value = SignedInteger<T>;
	};

	template<typename T> constexpr bool is_signed_integer_v = is_signed_integer<T>::value;

	template<typename T> concept UnsignedInteger = Integer<T> && !std::numeric_limits<T>::is_signed;

	template<typename T> struct is_unsigned_integer {
		static constexpr bool value = UnsignedInteger<T>;
	};

	template<typename T> constexpr bool is_unsigned_integer_v = is_unsigned_integer<T>::value;

	template<typename T> concept FloatingPoint = std::is_floating_point<T>::value;

	template<typename T> struct is_floating_point {
		static bool constexpr value = FloatingPoint<T>;
	};

	template<typename T> constexpr bool is_floating_point_v = is_floating_point<T>::value;

	template<typename T> concept Numeric = Integer<T> || FloatingPoint<T>;

	template<typename T> struct is_numeric {
		static bool constexpr value = Numeric<T>;
	};

	template<typename T> constexpr bool is_numeric_v = is_numeric<T>::value;

	enum struct operator_e { Addition, Subtraction, Multiplication, Division, Modulus };

	template<typename T, typename U>
	concept Addable = requires(T a, U b) {
		{ a + b } -> std::convertible_to<T>;
		{ a += b } -> std::convertible_to<ref<T>>;
	};

	template<typename T, typename U> concept Subtractable = requires(T a, U b) {
		{ a - b } -> std::convertible_to<T>;
		{ a -= b } -> std::convertible_to<ref<T>>;
	};

	template<typename T, typename U> concept Multipliable = requires(T a, U b) {
		{ a* b } -> std::convertible_to<T>;
		{ a *= b } -> std::convertible_to<ref<T>>;
	};

	template<typename T, typename U> concept Divisible = requires(T a, U b) {
		{ a / b } -> std::convertible_to<T>;
		{ a /= b } -> std::convertible_to<ref<T>>;
	};

	template<typename T, typename U> concept Modulable = requires(T a, U b) {
		{ a % b } -> std::convertible_to<T>;
		{ a %= b } -> std::convertible_to<ref<T>>;
	};

	template<typename T, typename U, operator_e Operator> struct is_operable;

	template<typename T, typename U> struct is_operable<T, U, operator_e::Addition> {
		static bool constexpr value = Addable<T, U>;
	};

	template<typename T, typename U> struct is_operable<T, U, operator_e::Subtraction> {
		static bool constexpr value = Subtractable<T, U>;
	};

	template<typename T, typename U> struct is_operable<T, U, operator_e::Multiplication> {
		static bool constexpr value = Multipliable<T, U>;
	};

	template<typename T, typename U> struct is_operable<T, U, operator_e::Division> {
		static bool constexpr value = Divisible<T, U>;
	};

	template<typename T, typename U> struct is_operable<T, U, operator_e::Modulus> {
		static bool constexpr value = Modulable<T, U>;
	};

	template<typename T, typename U, operator_e Operator> constexpr bool is_operable_v = is_operable<T, U, Operator>::value;

	template<typename T, operator_e Operator> using is_operable_unary = is_operable<T, T, Operator>;

	template<typename T, operator_e Operator> constexpr bool is_operable_unary_v = is_operable_unary<T, Operator>::value;

	enum struct comparator_e { Less, Greater, LessEq, GreaterEq, Equatable, Ordered, Comparable };

	template<typename T, typename U> concept Less = requires(T a, U b) {
		{ a < b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> concept Greater = requires(T a, U b) {
		{ a > b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> concept LessEq = requires(T a, U b) {
		{ a <= b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> concept GreaterEq = requires(T a, U b) {
		{ a >= b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> concept Equatable = requires(T a, U b) {
		{ a == b } -> std::convertible_to<bool>;
		{ a != b } -> std::convertible_to<bool>;
	};

	template<typename T, typename U> concept Ordered = Less<T, U> && Greater<T, U>;

	template<typename T, typename U> concept Comparable = Less<T, U> && Greater<T, U> && LessEq<T, U> && GreaterEq<T, U> && Equatable<T, U>;

	template<typename T, typename U> struct is_equatable {
		static bool constexpr value = Equatable<T, U>;
	};

	template<typename T, typename U> constexpr bool is_equatable_v = is_equatable<T, U>::value;

	template<typename T, typename U, comparator_e Comparator> struct is_comparable;

	template<typename T, typename U> struct is_comparable<T, U, comparator_e::Less> {
		static bool constexpr value = Less<T, U>;
	};

	template<typename T, typename U> struct is_comparable<T, U, comparator_e::Greater> {
		static bool constexpr value = Greater<T, U>;
	};

	template<typename T, typename U> struct is_comparable<T, U, comparator_e::LessEq> {
		static bool constexpr value = LessEq<T, U>;
	};

	template<typename T, typename U> struct is_comparable<T, U, comparator_e::GreaterEq> {
		static bool constexpr value = GreaterEq<T, U>;
	};

	template<typename T, typename U> struct is_comparable<T, U, comparator_e::Equatable> {
		static bool constexpr value = Equatable<T, U>;
	};

	template<typename T, typename U> struct is_comparable<T, U, comparator_e::Ordered> {
		static bool constexpr value = Ordered<T, U>;
	};

	template<typename T, typename U> struct is_comparable<T, U, comparator_e::Comparable> {
		static bool constexpr value = Comparable<T, U>;
	};

	template<typename T, typename U, comparator_e Comparator> constexpr bool is_comparable_v = is_comparable<T, U, Comparator>::value;

	template<typename T, typename... Params> struct is_homogeneous {
		static bool constexpr value = (std::is_same<T, Params>::value && ...);
	};

	template<typename T, typename... Params> constexpr bool is_homogeneous_v = is_homogeneous<T, Params...>::value;

	template<typename T, typename... Params> struct is_heterogeneous {
		static bool constexpr value = (!std::is_same<T, Params>::value || ...);
	};

	template<typename T, typename... Params> constexpr bool is_heterogeneous_v = is_heterogeneous<T, Params...>::value;

	template<typename... Params> struct is_empty {
		static bool constexpr value = sizeof...(Params) == 0;
	};

	template<typename... Params> constexpr bool is_empty_v = is_empty<Params...>::value;

	template<typename... Params> struct is_unary {
		static bool constexpr value = sizeof...(Params) == 1;
	};

	template<typename... Params> constexpr bool is_unary_v = is_unary<Params...>::value;

	template<typename... Params> struct is_plurary {
		static bool constexpr value = sizeof...(Params) > 1;
	};

	template<typename... Params> constexpr bool is_plurary_v = is_plurary<Params...>::value;

	template<typename T, typename... Params> struct is_none_of {
		static bool constexpr value = (!std::is_same<T, Params>::value && ...);
	};

	template<typename... Params> constexpr bool is_none_of_v = is_none_of<Params...>::value;

	template<typename T, typename... Params> struct is_one_of {
		static bool constexpr value = (std::is_same<T, Params>::value || ...);
	};

	template<typename... Params> constexpr bool is_one_of_v = is_one_of<Params...>::value;

	template<typename T> concept Hashable = requires(T t) {
		{ T::hasher::operator()(t) } -> std::convertible_to<usize>;
	};

	template<typename T> struct is_hashable {
		static bool constexpr value = Hashable<T>;
	};

	template<typename T> constexpr bool is_hashable_v = is_hashable<T>::value;

	template<typename T> struct is_drawable { static bool constexpr value = false; };

	template<typename T> constexpr bool is_drawable_v = is_drawable<T>::value;

	template<typename T> concept Drawable = is_drawable<T>::value;

	template<typename T> concept HasPosition = requires(T t) {
		requires(requires{ { t.position } -> std::convertible_to<offset_t>; } || requires{ { t.get_position() } -> std::convertible_to<offset_t>; });
	};

	template<typename T> struct has_position {
		static bool constexpr value = HasPosition<T>;
	};

	template<typename T> constexpr bool has_position_v = has_position<T>::value;

	template<typename T> concept HashableByPosition = requires(T t) {
		requires(HasPosition<T> && requires{ { T::hasher::offset::operator()(t) } -> std::convertible_to<usize>; });
	};

	template<typename T> struct is_hashable_by_position {
		static constexpr bool value = HashableByPosition<T>;
	};

	template<typename T> constexpr bool is_hashable_by_position_v = is_hashable_by_position<T>::value;

	template<typename T> concept HasSize = requires(T t) {
		requires(requires{ { t.size } -> std::convertible_to<extent_t>; } || requires{ { t.get_size() } -> std::convertible_to<extent_t>; });
	};

	template<typename T> struct has_size {
		static bool constexpr value = HasSize<T>;
	};

	template<typename T> constexpr bool has_size_v = has_size<T>::value;

	template<typename T> concept HashableBySize = requires(T t) {
		requires(HasSize<T> && requires{ { T::hasher::size::operator()(t) } -> std::convertible_to<usize>; });		
	};

	template<typename T> struct is_hashable_by_size {
		static constexpr bool value = HashableBySize<T>;
	};

	template<typename T> constexpr bool is_hashable_by_size_v = is_hashable_by_size<T>::value;

	template<typename T> concept SparseBlockage = requires(T t, offset_t position) {
		{ t.contains(position) } -> std::convertible_to<bool>;
	};
} // namespace bleak
