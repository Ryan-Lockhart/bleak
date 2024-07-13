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

	constexpr ref<offset_1d_t> offset_1d_t::clamp(cref<extent_1d_t> min, cref<extent_1d_t> max) {
		x = x < min.w ? min.w : x > max.w ? max.w : x;

		return *this;
	}

	constexpr offset_1d_t offset_1d_t::clamp(offset_1d_t value, cref<extent_1d_t> min, cref<extent_1d_t> max) { return value.clamp(min, max); }

	constexpr offset_1d_t offset_1d_t::operator+(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x + extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator-(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x - extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator*(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x * extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator/(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x / extent.w) }; }

	constexpr offset_1d_t offset_1d_t::operator%(cref<extent_1d_t> extent) const noexcept { return offset_1d_t{ scalar_cast(x % extent.w) }; }

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

	constexpr offset_1d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(x) }; }

	constexpr offset_1d_t::operator offset_3d_t() const noexcept {
		return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(x), offset_3d_t::scalar_t{ 0 }, offset_3d_t::scalar_t{ 0 } };
	}

	constexpr const offset_2d_t offset_2d_t::zero{ 0 };

	constexpr const offset_2d_t offset_2d_t::central{ offset_2d_t::zero };

	constexpr const offset_2d_t offset_2d_t::north{ 0, -1 };
	constexpr const offset_2d_t offset_2d_t::south{ 0, 1 };

	constexpr const offset_2d_t offset_2d_t::east{ 1, 0 };
	constexpr const offset_2d_t offset_2d_t::west{ -1, 0 };

	constexpr const offset_2d_t offset_2d_t::northwest{ offset_2d_t::north + offset_2d_t::west };
	constexpr const offset_2d_t offset_2d_t::northeast{ offset_2d_t::north + offset_2d_t::east };

	constexpr const offset_2d_t offset_2d_t::southwest{ offset_2d_t::south + offset_2d_t::west };
	constexpr const offset_2d_t offset_2d_t::southeast{ offset_2d_t::south + offset_2d_t::east };

	constexpr offset_2d_t offset_2d_t::clamp(offset_2d_t value, cref<extent_2d_t> min, cref<extent_2d_t> max) { return value.clamp(min, max); }

	constexpr ref<offset_2d_t> offset_2d_t::clamp(cref<extent_2d_t> min, cref<extent_2d_t> max) {
		x = x < min.w ? min.w : x > max.w ? max.w : x;
		y = y < min.h ? min.h : y > max.h ? max.h : y;

		return *this;
	}

	constexpr offset_2d_t offset_2d_t::operator+(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x + extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator-(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x - extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator*(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x * extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator/(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x / extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator%(cref<extent_1d_t> extent) const noexcept { return offset_2d_t{ scalar_cast(x % extent.w), y }; }

	constexpr offset_2d_t offset_2d_t::operator+(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h) };
	}

	constexpr offset_2d_t offset_2d_t::operator-(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x - extent.w), scalar_cast(y - extent.h) };
	}

	constexpr offset_2d_t offset_2d_t::operator*(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x * extent.w), scalar_cast(y * extent.h) };
	}

	constexpr offset_2d_t offset_2d_t::operator/(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x / extent.w), scalar_cast(y / extent.h) };
	}

	constexpr offset_2d_t offset_2d_t::operator%(cref<extent_2d_t> extent) const noexcept {
		return offset_2d_t{ scalar_cast(x % extent.w), scalar_cast(y % extent.h) };
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator+=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x + extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator-=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x - extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator*=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x * extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator/=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x / extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator%=(cref<extent_1d_t> extent) noexcept {
		x = scalar_cast(x % extent.w);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator+=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x + extent.w);
		y = scalar_cast(y + extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator-=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x - extent.w);
		y = scalar_cast(y - extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator*=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x * extent.w);
		y = scalar_cast(y * extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator/=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x / extent.w);
		y = scalar_cast(y / extent.h);

		return *this;
	}

	constexpr ref<offset_2d_t> offset_2d_t::operator%=(cref<extent_2d_t> extent) noexcept {
		x = scalar_cast(x % extent.w);
		y = scalar_cast(y % extent.h);

		return *this;
	}

	constexpr offset_2d_t::operator offset_1d_t() const noexcept { return offset_1d_t{ static_cast<offset_1d_t::scalar_t>(x) }; }

	constexpr offset_2d_t::operator offset_3d_t() const noexcept {
		return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(x), static_cast<offset_3d_t::scalar_t>(y), offset_3d_t::scalar_t{ 0 } };
	}

	constexpr offset_2d_t::operator extent_2d_t() const noexcept { return extent_2d_t{ static_cast<extent_2d_t::scalar_t>(x), static_cast<extent_2d_t::scalar_t>(y) }; }

	constexpr offset_2d_t::operator extent_3d_t() const noexcept {
		return extent_3d_t{ static_cast<extent_3d_t::scalar_t>(x), static_cast<extent_3d_t::scalar_t>(y), extent_3d_t::scalar_t{ 0 } };
	}

	constexpr offset_3d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(x), static_cast<offset_2d_t::scalar_t>(y) }; }

	constexpr offset_3d_t::operator extent_2d_t() const noexcept { return extent_2d_t{ static_cast<extent_2d_t::scalar_t>(x), static_cast<extent_2d_t::scalar_t>(y) }; }

	constexpr offset_3d_t::operator extent_3d_t() const noexcept {
		return extent_3d_t{ static_cast<extent_3d_t::scalar_t>(x), static_cast<extent_3d_t::scalar_t>(y), static_cast<extent_3d_t::scalar_t>(z) };
	}

	constexpr const offset_3d_t offset_3d_t::zero{ 0 };

	constexpr const offset_3d_t offset_3d_t::central{ offset_3d_t::zero };

	constexpr const offset_3d_t offset_3d_t::north{ 0, -1, 0 };
	constexpr const offset_3d_t offset_3d_t::south{ 0, 1, 0 };

	constexpr const offset_3d_t offset_3d_t::east{ 1, 0, 0 };
	constexpr const offset_3d_t offset_3d_t::west{ -1, 0, 0 };

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

	constexpr ref<offset_3d_t> offset_3d_t::clamp(cref<extent_3d_t> min, cref<extent_3d_t> max) {
		x = x < min.w ? min.w : x > max.w ? max.w : x;
		y = y < min.h ? min.h : y > max.h ? max.h : y;
		z = z < min.d ? min.d : z > max.d ? max.d : z;

		return *this;
	}

	constexpr offset_3d_t offset_3d_t::clamp(offset_3d_t value, cref<extent_3d_t> min, cref<extent_3d_t> max) { return value.clamp(min, max); }

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_1d_t> extent) const noexcept { return offset_3d_t{ scalar_cast(x + extent.w), y, z }; }

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_2d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), z };
	}

	constexpr offset_3d_t offset_3d_t::operator+(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator-(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator*(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator/(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
	}

	constexpr offset_3d_t offset_3d_t::operator%(cref<extent_3d_t> extent) const noexcept {
		return offset_3d_t{ scalar_cast(x + extent.w), scalar_cast(y + extent.h), scalar_cast(z + extent.d) };
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
} // namespace bleak
