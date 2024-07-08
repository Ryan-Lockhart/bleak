#pragma once

#include "bleak/primitive.hpp" // IWYU pragma: export

#include <cmath>

#include <SDL.h>

#include "bleak/offset/offset_1d.hpp" // IWYU pragma: export
#include "bleak/offset/offset_2d.hpp" // IWYU pragma: export
#include "bleak/offset/offset_3d.hpp" // IWYU pragma: export

#include "bleak/extent.hpp" // IWYU pragma: export

namespace bleak {
	constexpr const offset_1d_t offset_1d_t::zero{ 0 };

	constexpr const offset_1d_t offset_1d_t::central{ offset_1d_t::zero };

	constexpr const offset_1d_t offset_1d_t::east{ -1 };
	constexpr const offset_1d_t offset_1d_t::west{ 1 };

	constexpr offset_1d_t offset_1d_t::operator+(cref<extent_1d_t> extent) const noexcept { return { x + scalar_cast(extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator-(cref<extent_1d_t> extent) const noexcept { return { x - scalar_cast(extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator*(cref<extent_1d_t> extent) const noexcept { return { x * scalar_cast(extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator/(cref<extent_1d_t> extent) const noexcept { return { x / scalar_cast(extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator%(cref<extent_1d_t> extent) const noexcept { return { x % scalar_cast(extent.w) }; }

	constexpr ref<offset_1d_t> offset_1d_t::operator+=(cref<extent_1d_t> extent) noexcept {
		x += extent.w;

		return *this;
	}

	constexpr ref<offset_1d_t> offset_1d_t::operator-=(cref<extent_1d_t> extent) noexcept {
		x -= extent.w;

		return *this;
	}

	constexpr ref<offset_1d_t> offset_1d_t::operator*=(cref<extent_1d_t> extent) noexcept {
		x *= extent.w;

		return *this;
	}

	constexpr ref<offset_1d_t> offset_1d_t::operator/=(cref<extent_1d_t> extent) noexcept {
		x /= extent.w;

		return *this;
	}

	constexpr ref<offset_1d_t> offset_1d_t::operator%=(cref<extent_1d_t> extent) noexcept {
		x %= extent.w;

		return *this;
	}

	constexpr offset_1d_t::operator offset_2d_t() const noexcept { return { static_cast<offset_2d_t::scalar_t>(x) }; }

	constexpr offset_1d_t::operator offset_3d_t() const noexcept {
		return { static_cast<offset_3d_t::scalar_t>(x), offset_3d_t::scalar_t{ 0 }, offset_3d_t::scalar_t{ 0 } };
	}

	constexpr const offset_2d_t offset_2d_t::zero{ 0 };

	constexpr const offset_2d_t offset_2d_t::central{ offset_2d_t::zero };

	constexpr const offset_2d_t offset_2d_t::north = { 0, -1 };
	constexpr const offset_2d_t offset_2d_t::south = { 0, 1 };

	constexpr const offset_2d_t offset_2d_t::east = { 1, 0 };
	constexpr const offset_2d_t offset_2d_t::west = { -1, 0 };

	constexpr const offset_2d_t offset_2d_t::northwest{ offset_2d_t::north + offset_2d_t::west };
	constexpr const offset_2d_t offset_2d_t::northeast{ offset_2d_t::north + offset_2d_t::east };

	constexpr const offset_2d_t offset_2d_t::southwest{ offset_2d_t::south + offset_2d_t::west };
	constexpr const offset_2d_t offset_2d_t::southeast{ offset_2d_t::south + offset_2d_t::east };

	constexpr offset_2d_t offset_2d_t::operator+(cref<extent_1d_t> extent) const noexcept { return { x + scalar_cast(extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator-(cref<extent_1d_t> extent) const noexcept { return { x - scalar_cast(extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator*(cref<extent_1d_t> extent) const noexcept { return { x * scalar_cast(extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator/(cref<extent_1d_t> extent) const noexcept { return { x / scalar_cast(extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator%(cref<extent_1d_t> extent) const noexcept { return { x % scalar_cast(extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator+(cref<extent_2d_t> extent) const noexcept { return { x + scalar_cast(extent.w), scalar_cast(y + extent.h) }; }

	constexpr offset_2d_t offset_2d_t::operator-(cref<extent_2d_t> extent) const noexcept { return { x - scalar_cast(extent.w), scalar_cast(y - extent.h) }; }

	constexpr offset_2d_t offset_2d_t::operator*(cref<extent_2d_t> extent) const noexcept { return { x * scalar_cast(extent.w), scalar_cast(y * extent.h) }; }

	constexpr offset_2d_t offset_2d_t::operator/(cref<extent_2d_t> extent) const noexcept { return { x / scalar_cast(extent.w), scalar_cast(y / extent.h) }; }

	constexpr offset_2d_t offset_2d_t::operator%(cref<extent_2d_t> extent) const noexcept { return { x % scalar_cast(extent.w), scalar_cast(y % extent.h) }; }

	constexpr ref<offset_2d_t> offset_2d_t::operator+=(cref<extent_1d_t> extent) noexcept {
		x += scalar_cast(extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator-=(cref<extent_1d_t> extent) noexcept {
		x -= scalar_cast(extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator*=(cref<extent_1d_t> extent) noexcept {
		x *= scalar_cast(extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator/=(cref<extent_1d_t> extent) noexcept {
		x /= scalar_cast(extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator%=(cref<extent_1d_t> extent) noexcept {
		x %= scalar_cast(extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator+=(cref<extent_2d_t> extent) noexcept {
		x += scalar_cast(extent.w);
		y += scalar_cast(extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator-=(cref<extent_2d_t> extent) noexcept {
		x -= scalar_cast(extent.w);
		y -= scalar_cast(extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator*=(cref<extent_2d_t> extent) noexcept {
		x *= scalar_cast(extent.w);
		y *= scalar_cast(extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator/=(cref<extent_2d_t> extent) noexcept {
		x /= scalar_cast(extent.w);
		y /= scalar_cast(extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator%=(cref<extent_2d_t> extent) noexcept {
		x %= scalar_cast(extent.w);
		y %= scalar_cast(extent.h);

		return *this;
	}

	constexpr offset_2d_t::operator offset_1d_t() const noexcept { return { static_cast<offset_1d_t::scalar_t>(x) }; }

	constexpr offset_2d_t::operator offset_3d_t() const noexcept {
		return { static_cast<offset_3d_t::scalar_t>(x), static_cast<offset_3d_t::scalar_t>(y), offset_3d_t::scalar_t{ 0 } };
	}

	constexpr offset_2d_t::operator extent_2d_t() const noexcept { return { static_cast<extent_2d_t::scalar_t>(x), static_cast<extent_2d_t::scalar_t>(y) }; }

	constexpr offset_2d_t::operator extent_3d_t() const noexcept {
		return { static_cast<extent_3d_t::scalar_t>(x), static_cast<extent_3d_t::scalar_t>(y), extent_3d_t::scalar_t{ 0 } };
	}

	constexpr offset_3d_t::operator offset_2d_t() const noexcept { return { static_cast<offset_2d_t::scalar_t>(x), static_cast<offset_2d_t::scalar_t>(y) }; }

	constexpr offset_3d_t::operator extent_2d_t() const noexcept { return { static_cast<extent_2d_t::scalar_t>(x), static_cast<extent_2d_t::scalar_t>(y) }; }

	constexpr offset_3d_t::operator extent_3d_t() const noexcept {
		return { static_cast<extent_3d_t::scalar_t>(x), static_cast<extent_3d_t::scalar_t>(y), static_cast<extent_3d_t::scalar_t>(z) };
	}

	constexpr const offset_3d_t offset_3d_t::zero{ 0 };

	constexpr const offset_3d_t offset_3d_t::central{ offset_3d_t::zero };

	constexpr const offset_3d_t offset_3d_t::north = { 0, -1, 0 };
	constexpr const offset_3d_t offset_3d_t::south = { 0, 1, 0 };

	constexpr const offset_3d_t offset_3d_t::east = { 1, 0, 0 };
	constexpr const offset_3d_t offset_3d_t::west = { -1, 0, 0 };

	constexpr const offset_3d_t offset_3d_t::up{ 0, 0, 1 };
	constexpr const offset_3d_t offset_3d_t::down{ 0, 0, -1 };

	constexpr const offset_3d_t offset_3d_t::northwest{ offset_3d_t::north + offset_3d_t::west };
	constexpr const offset_3d_t offset_3d_t::northeast{ offset_3d_t::north + offset_3d_t::east };

	constexpr const offset_3d_t offset_3d_t::southwest{ offset_3d_t::south + offset_3d_t::west };
	constexpr const offset_3d_t offset_3d_t::southeast{ offset_3d_t::south + offset_3d_t::east };

	constexpr const offset_3d_t offset_3d_t::vertical::up::central{ offset_3d_t::central + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::up::north{ offset_3d_t::north + offset_3d_t::up };
	constexpr const offset_3d_t offset_3d_t::vertical::up::south{ offset_3d_t::south + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::up::west{ offset_3d_t::west + offset_3d_t::up };
	constexpr const offset_3d_t offset_3d_t::vertical::up::east{ offset_3d_t::east + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::up::northwest{ offset_3d_t::northwest + offset_3d_t::up };
	constexpr const offset_3d_t offset_3d_t::vertical::up::Northeast{ offset_3d_t::northeast + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::up::southwest{ offset_3d_t::southwest + offset_3d_t::up };
	constexpr const offset_3d_t offset_3d_t::vertical::up::southeast{ offset_3d_t::southeast + offset_3d_t::up };

	constexpr const offset_3d_t offset_3d_t::vertical::down::central{ offset_3d_t::central + offset_3d_t::down };

	constexpr const offset_3d_t offset_3d_t::vertical::down::north{ offset_3d_t::north + offset_3d_t::down };
	constexpr const offset_3d_t offset_3d_t::vertical::down::south{ offset_3d_t::south + offset_3d_t::down };

	constexpr const offset_3d_t offset_3d_t::vertical::down::west{ offset_3d_t::west + offset_3d_t::down };
	constexpr const offset_3d_t offset_3d_t::vertical::down::east{ offset_3d_t::east + offset_3d_t::down };

	constexpr const offset_3d_t offset_3d_t::vertical::down::northwest{ offset_3d_t::northwest + offset_3d_t::down };
	constexpr const offset_3d_t offset_3d_t::vertical::down::northeast{ offset_3d_t::northeast + offset_3d_t::down };

	constexpr const offset_3d_t offset_3d_t::vertical::down::southwest{ offset_3d_t::southwest + offset_3d_t::down };
	constexpr const offset_3d_t offset_3d_t::vertical::down::southeast{ offset_3d_t::southeast + offset_3d_t::down };

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_1d_t> extent) const noexcept { return { scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_1d_t> extent) const noexcept { return { scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_1d_t> extent) const noexcept { return { scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_1d_t> extent) const noexcept { return { scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_1d_t> extent) const noexcept { return { scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_2d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_2d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_2d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_2d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_2d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_3d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_3d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_3d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_3d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_3d_t> extent) const noexcept {
		return { scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator+=(cref<extent_1d_t> extent) noexcept {
		x += extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator-=(cref<extent_1d_t> extent) noexcept {
		x -= extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator*=(cref<extent_1d_t> extent) noexcept {
		x *= extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator/=(cref<extent_1d_t> extent) noexcept {
		x /= extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator%=(cref<extent_1d_t> extent) noexcept {
		x %= extent.w;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator+=(cref<extent_2d_t> extent) noexcept {
		x += extent.w;
		y += extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator-=(cref<extent_2d_t> extent) noexcept {
		x -= extent.w;
		y -= extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator*=(cref<extent_2d_t> extent) noexcept {
		x *= extent.w;
		y *= extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator/=(cref<extent_2d_t> extent) noexcept {
		x /= extent.w;
		y /= extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator%=(cref<extent_2d_t> extent) noexcept {
		x %= extent.w;
		y %= extent.h;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator+=(cref<extent_3d_t> extent) noexcept {
		x += extent.w;
		y += extent.h;
		z += extent.d;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator-=(cref<extent_3d_t> extent) noexcept {
		x -= extent.w;
		y -= extent.h;
		z -= extent.d;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator*=(cref<extent_3d_t> extent) noexcept {
		x *= extent.w;
		y *= extent.h;
		z *= extent.d;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator/=(cref<extent_3d_t> extent) noexcept {
		x /= extent.w;
		y /= extent.h;
		z /= extent.d;

		return *this;
	}

	constexpr ref<offset_3d_t> offset_3d_t::operator%=(cref<extent_3d_t> extent) noexcept {
		x %= extent.w;
		y %= extent.h;
		z %= extent.d;

		return *this;
	}

	template<extent_1d_t Size> static constexpr extent_1d_t::product_t flatten(cref<offset_1d_t> offset) noexcept { return offset.x; }

	template<extent_2d_t Size> static constexpr extent_2d_t::product_t flatten(cref<offset_2d_t> offset) noexcept { return offset.y * Size.w + offset.x; }

	template<extent_3d_t Size> static constexpr extent_3d_t::product_t flatten(cref<offset_3d_t> offset) noexcept {
		return offset.z * Size.area() + offset.y * Size.w + offset.x;
	}

	template<extent_1d_t Size> static constexpr offset_1d_t unflatten(extent_1d_t::product_t index) noexcept {
		return offset_1d_t{ offset_1d_t::scalar_cast(index) };
	}

	template<extent_2d_t Size> static constexpr offset_2d_t unflatten(extent_2d_t::product_t index) noexcept {
		lldiv_t result{ std::lldiv(index, Size.w) };
		return offset_2d_t{ result.rem, result.quot };
	}

	template<extent_3d_t Size> static constexpr offset_3d_t unflatten(extent_3d_t::product_t index) noexcept {
		lldiv_t outer{ std::lldiv(index, Size.w) };
		lldiv_t inner{ std::lldiv(outer.quot, Size.area()) };
		return offset_3d_t{ outer.rem, inner.rem, inner.quot };
	}
} // namespace bleak
