#pragma once

#include <bleak/primitive.hpp>

#include <utility>

#include <SDL.h>

#include <bleak/extent.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	struct rect_t {
		offset_t position;
		extent_t size;

		constexpr rect_t() noexcept : position{}, size{} {}

		constexpr rect_t(cref<offset_t> position, cref<extent_t> size) noexcept : position{ position }, size{ size } {}

		constexpr rect_t(cref<rect_t> other) noexcept : position{ other.position }, size{ other.size } {}

		constexpr rect_t(rval<rect_t> other) noexcept : position{ std::move(other.position) }, size{ std::move(other.size) } {}

		constexpr rect_t(cref<sdl::rect> other) noexcept :
			position{ std::move(other.x), std::move(other.y) },
			size{ extent_t::scalar_cast(std::move(other.w)), extent_t::scalar_cast(std::move(other.h)) } {}

		constexpr rect_t(rval<sdl::rect> other) noexcept :
			position{ std::move(other.x), std::move(other.y) },
			size{ extent_t::scalar_cast(std::move(other.w)), extent_t::scalar_cast(std::move(other.h)) } {}

		constexpr explicit rect_t(cref<sdl::frect> other) noexcept :
			position{ offset_t::scalar_cast(other.x), offset_t::scalar_cast(other.y) },
			size{ extent_t::scalar_cast(other.w), extent_t::scalar_cast(other.h) } {}

		constexpr explicit rect_t(rval<sdl::frect> other) noexcept :
			position{ offset_t::scalar_cast(std::move(other.x)), offset_t::scalar_cast(std::move(other.y)) },
			size{ extent_t::scalar_cast(std::move(other.w)), extent_t::scalar_cast(std::move(other.h)) } {}

		constexpr ref<rect_t> operator=(cref<rect_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			position = other.position;
			size = other.size;

			return *this;
		}

		constexpr ref<rect_t> operator=(rval<rect_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			position = std::move(other.position);
			size = std::move(other.size);

			return *this;
		}

		constexpr ref<rect_t> operator=(cref<sdl::rect> other) noexcept {
			position.x = other.x;
			position.y = other.y;

			size.w = other.w;
			size.h = other.h;

			return *this;
		}

		constexpr ref<rect_t> operator=(rval<sdl::rect> other) noexcept {
			position.x = std::move(other.x);
			position.y = std::move(other.y);

			size.w = std::move(other.w);
			size.h = std::move(other.h);

			return *this;
		}

		constexpr ref<rect_t> operator=(cref<sdl::frect> other) noexcept {
			position.x = offset_t::scalar_cast(other.x);
			position.y = offset_t::scalar_cast(other.y);

			size.w = extent_t::scalar_cast(other.w);
			size.h = extent_t::scalar_cast(other.h);

			return *this;
		}

		constexpr ref<rect_t> operator=(rval<sdl::frect> other) noexcept {
			position.x = offset_t::scalar_cast(std::move(other.x));
			position.y = offset_t::scalar_cast(std::move(other.y));

			size.w = extent_t::scalar_cast(std::move(other.w));
			size.h = extent_t::scalar_cast(std::move(other.h));

			return *this;
		}

		constexpr rect_t operator+(cref<offset_t> offset) const noexcept { return rect_t{ position + offset, size }; }

		constexpr rect_t operator-(cref<offset_t> offset) const noexcept { return rect_t{ position - offset, size }; }

		constexpr rect_t operator+(cref<extent_t> extent) const noexcept { return rect_t{ position, size + extent }; }

		constexpr rect_t operator-(cref<extent_t> extent) const noexcept { return rect_t{ position, size - extent }; }

		constexpr rect_t operator*(cref<extent_t> extent) const noexcept { return rect_t{ position, size * extent }; }

		constexpr rect_t operator/(cref<extent_t> extent) const noexcept { return rect_t{ position, size / extent }; }

		constexpr ref<rect_t> operator+=(cref<offset_t> offset) noexcept {
			position += offset;

			return *this;
		}

		constexpr ref<rect_t> operator-=(cref<offset_t> offset) noexcept {
			position -= offset;

			return *this;
		}

		constexpr ref<rect_t> operator+=(cref<extent_t> extent) noexcept {
			size += extent;

			return *this;
		}

		constexpr ref<rect_t> operator-=(cref<extent_t> extent) noexcept {
			size -= extent;

			return *this;
		}

		constexpr ref<rect_t> operator*=(cref<extent_t> extent) noexcept {
			size *= extent;

			return *this;
		}

		constexpr ref<rect_t> operator/=(cref<extent_t> extent) noexcept {
			size /= extent;

			return *this;
		}

		constexpr bool operator==(cref<rect_t> other) const noexcept { return position == other.position && size == other.size; }

		constexpr bool operator!=(cref<rect_t> other) const noexcept { return position != other.position || size != other.size; }

		constexpr bool operator<(cref<rect_t> other) = delete;
		constexpr bool operator<=(cref<rect_t> other) = delete;

		constexpr bool operator>(cref<rect_t> other) = delete;
		constexpr bool operator>=(cref<rect_t> other) = delete;

		constexpr offset_t center() const noexcept { return position + size / 2; }

		constexpr extent_t::product_t area() const noexcept { return size.area(); }

		constexpr bool within(offset_t point) const noexcept { return point.x >= position.x && point.x <= position.x + size.w && point.y >= position.y && point.y <= position.y + size.h; }

		constexpr operator sdl::rect() const noexcept {
			return sdl::rect{ static_cast<i32>(position.x), static_cast<i32>(position.y), static_cast<i32>(size.w), static_cast<i32>(size.h) };
		}

		constexpr explicit operator sdl::frect() const noexcept {
			return sdl::frect{ static_cast<f32>(position.x), static_cast<f32>(position.y), static_cast<f32>(size.w), static_cast<f32>(size.h) };
		}

		constexpr operator std::string() const noexcept { return std::format("{} {}", (std::string)position, (std::string)size); }
	};
} // namespace bleak
