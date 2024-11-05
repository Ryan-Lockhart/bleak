#pragma once

#include <bleak/typedef.hpp>

#include <cmath>
#include <limits>
#include <type_traits>

#include <bleak/cardinal.hpp>
#include <bleak/concepts.hpp>
#include <bleak/lut.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	template<typename T>
		requires is_numeric<T>::value
	constexpr T PlanarDistance;

	template<> inline constexpr i8 PlanarDistance<i8>{ 1 };
	template<> inline constexpr i16 PlanarDistance<i16>{ 1 };
	template<> inline constexpr i32 PlanarDistance<i32>{ 1 };
	template<> inline constexpr i64 PlanarDistance<i64>{ 1 };

	template<> inline constexpr u8 PlanarDistance<u8>{ 1 };
	template<> inline constexpr u16 PlanarDistance<u16>{ 1 };
	template<> inline constexpr u32 PlanarDistance<u32>{ 1 };
	template<> inline constexpr u64 PlanarDistance<u64>{ 1 };

	template<> inline constexpr f32 PlanarDistance<f32>{ 1.0f };
	template<> inline constexpr f64 PlanarDistance<f64>{ 1.0 };

	template<typename T>
		requires is_numeric<T>::value
	constexpr T PlanarDiagonalDistance;

	template<> inline constexpr i8 PlanarDiagonalDistance<i8>{ 1 };
	template<> inline constexpr i16 PlanarDiagonalDistance<i16>{ 1 };
	template<> inline constexpr i32 PlanarDiagonalDistance<i32>{ 1 };
	template<> inline constexpr i64 PlanarDiagonalDistance<i64>{ 1 };

	template<> inline constexpr u8 PlanarDiagonalDistance<u8>{ 1 };
	template<> inline constexpr u16 PlanarDiagonalDistance<u16>{ 1 };
	template<> inline constexpr u32 PlanarDiagonalDistance<u32>{ 1 };
	template<> inline constexpr u64 PlanarDiagonalDistance<u64>{ 1 };

	template<> inline constexpr f32 PlanarDiagonalDistance<f32>{ 1.4142135623730950488016887242097f };
	template<> inline constexpr f64 PlanarDiagonalDistance<f64>{ 1.4142135623730950488016887242097 };

	template<typename T>
		requires is_numeric<T>::value
	constexpr T CubicDistance;

	template<> inline constexpr i8 CubicDistance<i8>{ 1 };
	template<> inline constexpr i16 CubicDistance<i16>{ 1 };
	template<> inline constexpr i32 CubicDistance<i32>{ 1 };
	template<> inline constexpr i64 CubicDistance<i64>{ 1 };

	template<> inline constexpr u8 CubicDistance<u8>{ 1 };
	template<> inline constexpr u16 CubicDistance<u16>{ 1 };
	template<> inline constexpr u32 CubicDistance<u32>{ 1 };
	template<> inline constexpr u64 CubicDistance<u64>{ 1 };

	template<> inline constexpr f32 CubicDistance<f32>{ 1.0f };
	template<> inline constexpr f64 CubicDistance<f64>{ 1.0 };

	template<typename T>
		requires is_numeric<T>::value
	constexpr T CubicDiagonalDistance;

	template<> inline constexpr i8 CubicDiagonalDistance<i8>{ 2 };
	template<> inline constexpr i16 CubicDiagonalDistance<i16>{ 2 };
	template<> inline constexpr i32 CubicDiagonalDistance<i32>{ 2 };
	template<> inline constexpr i64 CubicDiagonalDistance<i64>{ 2 };

	template<> inline constexpr f32 CubicDiagonalDistance<f32>{ 1.7320508075688772935274463415059f };
	template<> inline constexpr f64 CubicDiagonalDistance<f64>{ 1.7320508075688772935274463415059 };

	inline constexpr f64 Pi{ 3.1415926535897932384626433832795 };

	inline constexpr f64 DegreesToRadians{ Pi / 180.0 };

	inline constexpr f64 RadiansToDegrees{ 180.0 / Pi };

	inline constexpr f64 degrees_to_radians(f64 degrees) { return degrees * DegreesToRadians; }

	inline constexpr f64 radians_to_degrees(f64 radians) { return radians * RadiansToDegrees; }

	inline constexpr lut_t<cardinal_t, f64, 9> DirectionToAngle {
		pair_t<cardinal_t, f64>{ cardinal_e::Central, std::numeric_limits<f64>::quiet_NaN() },
		pair_t<cardinal_t, f64>{ cardinal_e::North, 0.0 },
		pair_t<cardinal_t, f64>{ cardinal_e::Northwest, 315.0 },
		pair_t<cardinal_t, f64>{ cardinal_e::West, 270.0 },
		pair_t<cardinal_t, f64>{ cardinal_e::Southwest, 225.0 },
		pair_t<cardinal_t, f64>{ cardinal_e::South, 180.0 },
		pair_t<cardinal_t, f64>{ cardinal_e::Southeast, 135.0 },
		pair_t<cardinal_t, f64>{ cardinal_e::East, 90.0 },
		pair_t<cardinal_t, f64>{ cardinal_e::Northeast, 45.0 }
	};

	inline constexpr f64 direction_to_angle(cardinal_t direction) { return DirectionToAngle[direction]; }

	inline constexpr f64 PercentOfCircle{ 0.002777777777777778 };

	template<typename T>
		requires std::is_floating_point<T>::value
	inline constexpr T atan2(T y, T x);

	template<> inline constexpr f32 atan2(f32 y, f32 x)
	{
		if (std::abs(y) < 0.0000000001f && x >= 0.0f) {
			return 0.0f;
		}

		f32 ax{ std::abs(x) };
		f32 ay{ std::abs(y) };

		if (ax < ay)
		{
			f32 a{ ax / ay };
			f32 s{ a * a };
			f32 r{ 0.25f - (((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a) * 0.15915494309189535f };
			return x < 0.0f ? y < 0.0f ? 0.5f + r : 0.5f - r : y < 0.0f ? 1.0f - r : r;
		}
		else
		{
			f32 a{ ay / ax };
			f32 s{ a * a };
			f32 r{ (((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a) * 0.15915494309189535f };
			return x < 0.0f ? y < 0.0f ? 0.5f + r : 0.5f - r : y < 0.0 ? 1.0f - r : r;
		}
	}

	template<> inline constexpr f64 atan2(f64 y, f64 x)
	{
		if (std::abs(y) < 0.0000000001 && x >= 0.0) {
			return 0.0;
		}

		f64 ax{ std::abs(x) };
		f64 ay{ std::abs(y) };

		if (ax < ay)
		{
			f64 a{ ax / ay };
			f64 s{ a * a };
			f64 r{ 0.25 - (((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a) * 0.15915494309189535 };
			return x < 0.0 ? y < 0.0 ? 0.5 + r : 0.5 - r : y < 0.0 ? 1.0 - r : r;
		}
		else
		{
			f64 a{ ay / ax };
			f64 s{ a * a };
			f64 r{ (((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a) * 0.15915494309189535 };
			return x < 0.0 ? y < 0.0 ? 0.5 + r : 0.5 - r : y < 0.0 ? 1.0 - r : r;
		}
	}
} // namespace bleak
