#pragma once

#include <bleak/typedef.hpp>

namespace bleak {
	// following assumes IEEE 754 floating point standard for all floating point types
	// any platform that does not conform to this standard will not work with this library (until I add support for it :P)

	namespace bitdef {
		template<typename T> static constexpr auto exponent();
		template<typename T> static constexpr auto mantissa();
		template<typename T> static constexpr auto bits();
		template<typename T> static constexpr auto bytes();

		constexpr auto sign_bit = 1;

#if defined(HAS_MINIFLOAT)
		namespace f8 {
			constexpr auto exponent = 4;
			constexpr auto mantissa = 3;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(::f8) == bytes, "f8 is not 8 bits!");
		} // namespace f8
#endif

		namespace f16 {
			constexpr auto exponent = 5;
			constexpr auto mantissa = 10;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(bleak::f16) == bytes, "f16 is not 16 bits!");
		} // namespace f16

		namespace f32 {
			constexpr auto exponent = 8;
			constexpr auto mantissa = 23;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(bleak::f32) == bytes, "f32 is not 32 bits!");
		} // namespace f32

		namespace f64 {
			constexpr auto exponent = 11;
			constexpr auto mantissa = 52;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(bleak::f64) == bytes, "f64 is not 64 bits!");
		} // namespace f64

#if defined(BLEAKDEPTH_64)
		namespace f128 {
			constexpr auto exponent = 15;
			constexpr auto mantissa = 112;
			constexpr auto bits = exponent + mantissa + sign_bit;
			constexpr auto bytes = bits / 8;

			static_assert(sizeof(bleak::f128) == bytes, "f128 is not 128 bits!");
		} // namespace f128
#endif

#if defined(HAS_MINIFLOAT)
		template<> constexpr auto exponent<bleak::f8>() { return f8::exponent; }

		template<> constexpr auto mantissa<bleak::f8>() { return f8::mantissa; }

		template<> constexpr auto bits<bleak::f8>() { return f8::bits; }

		template<> constexpr auto bytes<bleak::f8>() { return f8::bytes; }
#endif

		template<> constexpr auto exponent<bleak::f16>() { return f16::exponent; }

		template<> constexpr auto mantissa<bleak::f16>() { return f16::mantissa; }

		template<> constexpr auto bits<bleak::f16>() { return f16::bits; }

		template<> constexpr auto bytes<bleak::f16>() { return f16::bytes; }

		template<> constexpr auto exponent<bleak::f32>() { return f32::exponent; }

		template<> constexpr auto mantissa<bleak::f32>() { return f32::mantissa; }

		template<> constexpr auto bits<bleak::f32>() { return f32::bits; }

		template<> constexpr auto bytes<bleak::f32>() { return f32::bytes; }

		template<> constexpr auto exponent<bleak::f64>() { return f64::exponent; }

		template<> constexpr auto mantissa<bleak::f64>() { return f64::mantissa; }

		template<> constexpr auto bits<bleak::f64>() { return f64::bits; }

		template<> constexpr auto bytes<bleak::f64>() { return f64::bytes; }

#if defined(BLEAKDEPTH_64)
		template<> constexpr auto exponent<bleak::f128>() { return f128::exponent; }

		template<> constexpr auto mantissa<bleak::f128>() { return f128::mantissa; }

		template<> constexpr auto bits<bleak::f128>() { return f128::bits; }

		template<> constexpr auto bytes<bleak::f128>() { return f128::bytes; }
#endif

	} // namespace bitdef
} // namespace bleak
