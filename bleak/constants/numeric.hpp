#pragma once

#include <bleak/typedef.hpp>

#include <bleak/concepts.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	template<typename T>
		requires is_numeric<T>::value
	constexpr T PlanarDistance;

	template<> inline constexpr i8 PlanarDistance<i8> = 1;
	template<> inline constexpr i16 PlanarDistance<i16> = 1;
	template<> inline constexpr i32 PlanarDistance<i32> = 1;
	template<> inline constexpr i64 PlanarDistance<i64> = 1;

	template<> inline constexpr f32 PlanarDistance<f32> = 1.0f;
	template<> inline constexpr f64 PlanarDistance<f64> = 1.0;

	template<typename T>
		requires is_numeric<T>::value
	constexpr T PlanarDiagonalDistance;

	template<> inline constexpr i8 PlanarDiagonalDistance<i8> = 1;
	template<> inline constexpr i16 PlanarDiagonalDistance<i16> = 1;
	template<> inline constexpr i32 PlanarDiagonalDistance<i32> = 1;
	template<> inline constexpr i64 PlanarDiagonalDistance<i64> = 1;

	template<> inline constexpr f32 PlanarDiagonalDistance<f32> = 1.4142135623730950488016887242097f;
	template<> inline constexpr f64 PlanarDiagonalDistance<f64> = 1.4142135623730950488016887242097;

	template<typename T>
		requires is_numeric<T>::value
	constexpr T CubicDistance;

	template<> inline constexpr i8 CubicDistance<i8> = 1;
	template<> inline constexpr i16 CubicDistance<i16> = 1;
	template<> inline constexpr i32 CubicDistance<i32> = 1;
	template<> inline constexpr i64 CubicDistance<i64> = 1;

	template<> inline constexpr f32 CubicDistance<f32> = 1.0f;
	template<> inline constexpr f64 CubicDistance<f64> = 1.0;

	template<typename T>
		requires is_numeric<T>::value
	constexpr T CubicDiagonalDistance;

	template<> inline constexpr i8 CubicDiagonalDistance<i8> = 2;
	template<> inline constexpr i16 CubicDiagonalDistance<i16> = 2;
	template<> inline constexpr i32 CubicDiagonalDistance<i32> = 2;
	template<> inline constexpr i64 CubicDiagonalDistance<i64> = 2;

	template<> inline constexpr f32 CubicDiagonalDistance<f32> = 1.7320508075688772935274463415059f;
	template<> inline constexpr f64 CubicDiagonalDistance<f64> = 1.7320508075688772935274463415059;
} // namespace bleak
