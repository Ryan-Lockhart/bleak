#pragma once

#include "bleak/primitive.hpp"

#include <cmath>

#include <SDL.h>

#include "bleak/extent/extent_1d.hpp"
#include "bleak/extent/extent_2d.hpp"
#include "bleak/extent/extent_3d.hpp"

#include "bleak/offset.hpp" // IWYU pragma: export

constexpr const bleak::extent_1d_t bleak::extent_1d_t::zero{ extent_1d_t::scalar_t{ 0 } };

constexpr const bleak::extent_2d_t bleak::extent_2d_t::zero{ extent_2d_t::scalar_t{ 0 }, extent_2d_t::scalar_t{ 0 } };

constexpr const bleak::extent_3d_t bleak::extent_3d_t::zero{ extent_3d_t::scalar_t{ 0 }, extent_3d_t::scalar_t{ 0 }, extent_3d_t::scalar_t{ 0 } };

constexpr bleak::offset_2d_t bleak::extent_2d_t::operator-() const noexcept { return offset_2d_t{ -offset_2d_t::scalar_cast(w), -offset_2d_t::scalar_cast(h) }; }

constexpr bleak::extent_1d_t::operator extent_2d_t() const noexcept { return extent_2d_t{ static_cast<extent_2d_t::scalar_t>(w), extent_2d_t::scalar_t{ 0 } }; }

constexpr bleak::extent_1d_t::operator extent_3d_t() const noexcept { return extent_3d_t{ static_cast<extent_3d_t::scalar_t>(w), extent_3d_t::scalar_t{ 0 }, extent_3d_t::scalar_t{ 0 } }; }

constexpr bleak::extent_1d_t::operator offset_1d_t() const noexcept { return offset_1d_t{ static_cast<offset_1d_t::scalar_t>(w) }; }

constexpr bleak::extent_1d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(w), offset_2d_t::scalar_t{ 0 } }; }

constexpr bleak::extent_1d_t::operator offset_3d_t() const noexcept { return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(w), offset_3d_t::scalar_t{ 0 }, offset_3d_t::scalar_t{ 0 } }; }

constexpr bleak::extent_2d_t::operator offset_1d_t() const noexcept { return offset_1d_t{ static_cast<offset_1d_t::scalar_t>(w) }; }

constexpr bleak::extent_2d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(w), static_cast<offset_2d_t::scalar_t>(h) }; }

constexpr bleak::extent_2d_t::operator offset_3d_t() const noexcept { return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(w), static_cast<offset_3d_t::scalar_t>(h), offset_3d_t::scalar_t{ 0 } }; }

constexpr bleak::extent_2d_t::operator extent_1d_t() const noexcept { return extent_1d_t{ static_cast<extent_1d_t::scalar_t>(w) }; }

constexpr bleak::extent_2d_t::operator extent_3d_t() const noexcept { return extent_3d_t{ static_cast<extent_3d_t::scalar_t>(w), static_cast<extent_3d_t::scalar_t>(h), extent_3d_t::scalar_t{ 0 } }; }

constexpr bleak::extent_3d_t::operator extent_2d_t() const noexcept { return extent_2d_t{ static_cast<extent_2d_t::scalar_t>(w), static_cast<extent_2d_t::scalar_t>(h) }; }

constexpr bleak::extent_3d_t::operator extent_1d_t() const noexcept { return extent_1d_t{ static_cast<extent_1d_t::scalar_t>(w) }; }

constexpr bleak::extent_3d_t::operator offset_1d_t() const noexcept { return offset_1d_t{ static_cast<offset_1d_t::scalar_t>(w) }; }

constexpr bleak::extent_3d_t::operator offset_2d_t() const noexcept { return offset_2d_t{ static_cast<offset_2d_t::scalar_t>(w), static_cast<offset_2d_t::scalar_t>(h) }; }

constexpr bleak::extent_3d_t::operator offset_3d_t() const noexcept { return offset_3d_t{ static_cast<offset_3d_t::scalar_t>(w), static_cast<offset_3d_t::scalar_t>(h), static_cast<offset_3d_t::scalar_t>(d) }; }
