#pragma once

#include <bleak/typedef.hpp>

#include <immintrin.h>

#include <bleak/concepts.hpp>
#include <bleak/vector.hpp>

namespace bleak {
	template<UnsignedInteger N, UnsignedInteger AB> requires (sizeof(N) == sizeof(AB) * 2) constexpr N interleave(AB a, AB b) noexcept;

	template<> constexpr inline u16 interleave(u8 a, u8 b) noexcept { return _pdep_u32(a, 0x5555) | _pdep_u32(b, 0xAAAA); }

	template<> constexpr inline u32 interleave(u16 a, u16 b) noexcept { return _pdep_u32(a, 0x5555'5555) | _pdep_u32(b, 0xAAAA'AAAA); }

	template<> constexpr inline u64 interleave(u32 a, u32 b) noexcept { return _pdep_u64(a, 0x5555'5555'5555'5555) | _pdep_u64(b, 0xAAAA'AAAA'AAAA'AAAA); }

	template<UnsignedInteger N, UnsignedInteger AB> requires (sizeof(N) == sizeof(AB) * 2) constexpr N interleave(vec2<AB> vec) noexcept;

	template<> constexpr inline u16 interleave(vec2<u8> vec) noexcept { return _pdep_u32(vec[0], 0x5555) | _pdep_u32(vec[1], 0xAAAA); }

	template<> constexpr inline u32 interleave(vec2<u16> vec) noexcept { return _pdep_u32(vec[0], 0x5555'5555) | _pdep_u32(vec[1], 0xAAAA'AAAA); }

	template<> constexpr inline u64 interleave(vec2<u32> vec) noexcept { return _pdep_u64(vec[0], 0x5555'5555'5555'5555) | _pdep_u64(vec[1], 0xAAAA'AAAA'AAAA'AAAA); }

	template<SignedInteger N, SignedInteger AB> requires (sizeof(N) == sizeof(AB) * 2) constexpr N interleave(AB a, AB b) noexcept;

	template<> constexpr inline i16 interleave(i8 a, i8 b) noexcept { return std::bit_cast<i16>(interleave<u16>(std::bit_cast<u8>(a), std::bit_cast<u8>(b))); }

	template<> constexpr inline i32 interleave(i16 a, i16 b) noexcept { return std::bit_cast<i32>(interleave<u32>(std::bit_cast<u16>(a), std::bit_cast<u16>(b))); }

	template<> constexpr inline i64 interleave(i32 a, i32 b) noexcept { return std::bit_cast<i64>(interleave<u64>(std::bit_cast<u32>(a), std::bit_cast<u32>(b))); }

	template<SignedInteger N, SignedInteger AB> requires (sizeof(N) == sizeof(AB) * 2) constexpr N interleave(vec2<AB> vec) noexcept;

	template<> constexpr inline i16 interleave(vec2<i8> vec) noexcept { return std::bit_cast<i16>(interleave<u16>(std::bit_cast<u8>(vec[0]), std::bit_cast<u8>(vec[1]))); }

	template<> constexpr inline i32 interleave(vec2<i16> vec) noexcept { return std::bit_cast<i32>(interleave<u32>(std::bit_cast<u16>(vec[0]), std::bit_cast<u16>(vec[1]))); }

	template<> constexpr inline i64 interleave(vec2<i32> vec) noexcept { return std::bit_cast<i64>(interleave<u64>(std::bit_cast<u32>(vec[0]), std::bit_cast<u32>(vec[1]))); }

	template<UnsignedInteger N, UnsignedInteger AB> requires (sizeof(N) == sizeof(AB) * 2) constexpr vec2<AB> deinterleave(N ab) noexcept;

	template<> constexpr inline vec2<u8> deinterleave(u16 ab) noexcept {
		return vec2<u8>{
			static_cast<u8>(_pext_u32(ab, 0x5555)),
			static_cast<u8>(_pext_u32(ab, 0xAAAA))
		};
	}

	template<> constexpr inline vec2<u16> deinterleave(u32 ab) noexcept {
		return vec2<u16>{
			static_cast<u16>(_pext_u32(ab, 0x5555'5555)),
			static_cast<u16>(_pext_u32(ab, 0xAAAA'AAAA))
		};
	}

	template<> constexpr inline vec2<u32> deinterleave(u64 ab) noexcept {
		return vec2<u32>{
			static_cast<u32>(_pext_u64(ab, 0x5555'5555'5555'5555)),
			static_cast<u32>(_pext_u64(ab, 0xAAAA'AAAA'AAAA'AAAA))
		};
	}

	template<SignedInteger N, SignedInteger AB> requires (sizeof(N) == sizeof(AB) * 2) constexpr vec2<AB> deinterleave(N ab) noexcept;

	template<> constexpr inline vec2<i8> deinterleave(i16 ab) noexcept {
		const u16 u_ab{ std::bit_cast<u16>(ab) };

		return vec2<i8>{
			std::bit_cast<i8>(static_cast<u8>(_pext_u32(u_ab, 0x5555))),
			std::bit_cast<i8>(static_cast<u8>(_pext_u32(u_ab, 0xAAAA)))
		};
	}

	template<> constexpr inline vec2<i16> deinterleave(i32 ab) noexcept {
		const u32 u_ab{ std::bit_cast<u32>(ab) };

		return vec2<i16>{
			std::bit_cast<i16>(static_cast<u16>(_pext_u32(u_ab, 0x5555'5555))),
			std::bit_cast<i16>(static_cast<u16>(_pext_u32(u_ab, 0xAAAA'AAAA)))
		};
	}

	template<> constexpr inline vec2<i32> deinterleave(i64 ab) noexcept {
		const u64 u_ab{ std::bit_cast<u64>(ab) };

		return vec2<i32>{
			std::bit_cast<i32>(static_cast<u32>(_pext_u64(u_ab, 0x5555'5555'5555'5555))),
			std::bit_cast<i32>(static_cast<u32>(_pext_u64(u_ab, 0xAAAA'AAAA'AAAA'AAAA)))
		};
	}
} // namespace bleak
