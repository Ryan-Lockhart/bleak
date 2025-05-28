#pragma once

#include <bleak/typedef.hpp>

#include <immintrin.h>

#include <bleak/concepts.hpp>

namespace bleak {
	template<Integer I> constexpr inline I sat_add(I lhs, I rhs) noexcept {
		I result{};

		if (__builtin_add_overflow(lhs, rhs, &result)) {
			result = std::numeric_limits<I>::max();
		}

		return result;
	}

	template<Integer I, Numeric N> constexpr inline I sat_add(I lhs, N rhs) noexcept {
		I result{};

		if (__builtin_add_overflow(lhs, static_cast<I>(rhs), &result)) {
			result = std::numeric_limits<I>::max();
		}

		return result;
	}

	template u8 sat_add<u8>(u8 lhs, u8 rhs) noexcept;
	template u16 sat_add<u16>(u16 lhs, u16 rhs) noexcept;
	template u32 sat_add<u32>(u32 lhs, u32 rhs) noexcept;
	template u64 sat_add<u64>(u64 lhs, u64 rhs) noexcept;

	template i8 sat_add<i8>(i8 lhs, i8 rhs) noexcept;
	template i16 sat_add<i16>(i16 lhs, i16 rhs) noexcept;
	template i32 sat_add<i32>(i32 lhs, i32 rhs) noexcept;
	template i64 sat_add<i64>(i64 lhs, i64 rhs) noexcept;

#if defined (BLEAK_64)
	template u128 sat_add<u128>(u128 lhs, u128 rhs) noexcept;
	template i128 sat_add<i128>(i128 lhs, i128 rhs) noexcept;
#endif

	template<Integer I> constexpr inline I sat_sub(I lhs, I rhs) noexcept {
		I result{};

		if (__builtin_sub_overflow(lhs, rhs, &result)) {
			result = std::numeric_limits<I>::lowest();
		}

		return result;
	}

	template<Integer I, Numeric N> constexpr inline I sat_sub(I lhs, N rhs) noexcept {
		I result{};

		if (__builtin_sub_overflow(lhs, static_cast<I>(rhs), &result)) {
			result = std::numeric_limits<I>::lowest();
		}

		return result;
	}

	template u8 sat_sub<u8>(u8 lhs, u8 rhs) noexcept;
	template u16 sat_sub<u16>(u16 lhs, u16 rhs) noexcept;
	template u32 sat_sub<u32>(u32 lhs, u32 rhs) noexcept;
	template u64 sat_sub<u64>(u64 lhs, u64 rhs) noexcept;

	template i8 sat_sub<i8>(i8 lhs, i8 rhs) noexcept;
	template i16 sat_sub<i16>(i16 lhs, i16 rhs) noexcept;
	template i32 sat_sub<i32>(i32 lhs, i32 rhs) noexcept;
	template i64 sat_sub<i64>(i64 lhs, i64 rhs) noexcept;

#if defined (BLEAK_64)
	template u128 sat_sub<u128>(u128 lhs, u128 rhs) noexcept;
	template i128 sat_sub<i128>(i128 lhs, i128 rhs) noexcept;
#endif

	template<Integer I> constexpr inline I sat_mul(I lhs, I rhs) noexcept {
		I result{};

		if (__builtin_mul_overflow(lhs, rhs, &result)) {
			result = std::numeric_limits<I>::max();
		}

		return result;
	}

	template<Integer I, Numeric N> constexpr inline I sat_mul(I lhs, N rhs) noexcept {
		I result{};

		if (__builtin_mul_overflow(lhs, static_cast<I>(rhs), &result)) {
			result = std::numeric_limits<I>::max();
		}

		return result;
	}

	template u8 sat_mul<u8>(u8 lhs, u8 rhs) noexcept;
	template u16 sat_mul<u16>(u16 lhs, u16 rhs) noexcept;
	template u32 sat_mul<u32>(u32 lhs, u32 rhs) noexcept;
	template u64 sat_mul<u64>(u64 lhs, u64 rhs) noexcept;

	template i8 sat_mul<i8>(i8 lhs, i8 rhs) noexcept;
	template i16 sat_mul<i16>(i16 lhs, i16 rhs) noexcept;
	template i32 sat_mul<i32>(i32 lhs, i32 rhs) noexcept;
	template i64 sat_mul<i64>(i64 lhs, i64 rhs) noexcept;

#if defined (BLEAK_64)
	template u128 sat_mul<u128>(u128 lhs, u128 rhs) noexcept;
	template i128 sat_mul<i128>(i128 lhs, i128 rhs) noexcept;
#endif
} //namespace bleak
