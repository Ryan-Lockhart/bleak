#pragma once

#include "bleak/primitive.hpp"

#include <cmath>

#include <SDL.h>

#include "bleak/offset/offset_1d.hpp"
#include "bleak/offset/offset_2d.hpp"
#include "bleak/offset/offset_3d.hpp"

#include "bleak/extent.hpp" // IWYU pragma: export

constexpr bleak::offset_1d_t::operator offset_2d_t() const noexcept { return { static_cast<offset_2d_t::scalar_t>(x) }; }

constexpr bleak::offset_1d_t::operator offset_3d_t() const noexcept {
	return { static_cast<offset_3d_t::scalar_t>(x), offset_3d_t::scalar_t { 0 }, offset_3d_t::scalar_t { 0 } };
}

constexpr const bleak::offset_1d_t bleak::offset_1d_t::zero { 0 };

constexpr const bleak::offset_1d_t bleak::offset_1d_t::central { offset_1d_t::zero };

constexpr const bleak::offset_1d_t bleak::offset_1d_t::east { -1 };
constexpr const bleak::offset_1d_t bleak::offset_1d_t::west { 1 };

constexpr bleak::offset_2d_t::operator offset_1d_t() const noexcept { return { static_cast<offset_1d_t::scalar_t>(x) }; }

constexpr bleak::offset_2d_t::operator offset_3d_t() const noexcept {
	return { static_cast<offset_3d_t::scalar_t>(x), static_cast<offset_3d_t::scalar_t>(y), offset_3d_t::scalar_t { 0 } };
}

constexpr bleak::offset_2d_t::operator extent_2d_t() const noexcept {
	return { static_cast<extent_2d_t::scalar_t>(x), static_cast<extent_2d_t::scalar_t>(y) };
}

constexpr bleak::offset_2d_t::operator extent_3d_t() const noexcept {
	return { static_cast<extent_3d_t::scalar_t>(x), static_cast<extent_3d_t::scalar_t>(y), extent_3d_t::scalar_t { 0 } };
}

constexpr const bleak::offset_2d_t bleak::offset_2d_t::zero { 0 };

constexpr const bleak::offset_2d_t bleak::offset_2d_t::central { offset_2d_t::zero };

constexpr const bleak::offset_2d_t bleak::offset_2d_t::north = { 0, -1 };
constexpr const bleak::offset_2d_t bleak::offset_2d_t::south = { 0, 1 };

constexpr const bleak::offset_2d_t bleak::offset_2d_t::east = { 1, 0 };
constexpr const bleak::offset_2d_t bleak::offset_2d_t::west = { -1, 0 };

constexpr const bleak::offset_2d_t bleak::offset_2d_t::northwest { offset_2d_t::north + offset_2d_t::west };
constexpr const bleak::offset_2d_t bleak::offset_2d_t::northeast { offset_2d_t::north + offset_2d_t::east };

constexpr const bleak::offset_2d_t bleak::offset_2d_t::southwest { offset_2d_t::south + offset_2d_t::west };
constexpr const bleak::offset_2d_t bleak::offset_2d_t::southeast { offset_2d_t::south + offset_2d_t::east };

constexpr bleak::offset_3d_t::operator offset_2d_t() const noexcept {
	return { static_cast<offset_2d_t::scalar_t>(x), static_cast<offset_2d_t::scalar_t>(y) };
}

constexpr bleak::offset_3d_t::operator extent_2d_t() const noexcept {
	return { static_cast<extent_2d_t::scalar_t>(x), static_cast<extent_2d_t::scalar_t>(y) };
}

constexpr bleak::offset_3d_t::operator extent_3d_t() const noexcept {
	return { static_cast<extent_3d_t::scalar_t>(x), static_cast<extent_3d_t::scalar_t>(y), static_cast<extent_3d_t::scalar_t>(z) };
}

constexpr const bleak::offset_3d_t bleak::offset_3d_t::zero { 0 };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::central { offset_3d_t::zero };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::north = { 0, -1, 0 };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::south = { 0, 1, 0 };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::east = { 1, 0, 0 };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::west = { -1, 0, 0 };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::up { 0, 0, 1 };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::down { 0, 0, -1 };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::northwest { offset_3d_t::north + offset_3d_t::west };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::northeast { offset_3d_t::north + offset_3d_t::east };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::southwest { offset_3d_t::south + offset_3d_t::west };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::southeast { offset_3d_t::south + offset_3d_t::east };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::central { offset_3d_t::central + offset_3d_t::up };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::north { offset_3d_t::north + offset_3d_t::up };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::south { offset_3d_t::south + offset_3d_t::up };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::west { offset_3d_t::west + offset_3d_t::up };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::east { offset_3d_t::east + offset_3d_t::up };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::northwest { offset_3d_t::northwest + offset_3d_t::up };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::Northeast { offset_3d_t::northeast + offset_3d_t::up };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::southwest { offset_3d_t::southwest + offset_3d_t::up };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::up::southeast { offset_3d_t::southeast + offset_3d_t::up };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::central { offset_3d_t::central + offset_3d_t::down };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::north { offset_3d_t::north + offset_3d_t::down };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::south { offset_3d_t::south + offset_3d_t::down };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::west { offset_3d_t::west + offset_3d_t::down };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::east { offset_3d_t::east + offset_3d_t::down };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::northwest { offset_3d_t::northwest + offset_3d_t::down };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::northeast { offset_3d_t::northeast + offset_3d_t::down };

constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::southwest { offset_3d_t::southwest + offset_3d_t::down };
constexpr const bleak::offset_3d_t bleak::offset_3d_t::vertical::down::southeast { offset_3d_t::southeast + offset_3d_t::down };
