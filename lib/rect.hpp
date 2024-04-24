#pragma once

#include "typedef.hpp"

#include <utility>

#include <SDL.h>

#include "point.hpp"

namespace Bleakdepth {
	template<typename T = i32> struct rect_t {
		point_t<T> position;
		size_t<T> size;

		constexpr rect_t() noexcept : position {}, size {} {}

		constexpr rect_t(T w, T h) noexcept : position {}, size { w, h } {}

		constexpr rect_t(T x, T y, T w, T h) noexcept : position { x, y }, size { w, h } {}

		constexpr rect_t(cref<size_t<T>> size) noexcept : position {}, size { size } {}

		constexpr rect_t(cref<point_t<T>> position, cref<size_t<T>> size) noexcept :
			position { position },
			size { size } {}

		constexpr rect_t(cref<rect_t> other) noexcept : position { other.position }, size { other.size } {}

		constexpr rect_t(rval<rect_t> other) noexcept :
			position { std::move(other.position) },
			size { std::move(other.size) } {}

		constexpr rect_t(cref<SDL_Rect> other) noexcept :
			position { std::move(other.x), std::move(other.y) },
			size { std::move(other.w), std::move(other.h) } {}

		constexpr rect_t(rval<SDL_Rect> other) noexcept :
			position { std::move(other.x), std::move(other.y) },
			size { std::move(other.w), std::move(other.h) } {}

		constexpr explicit rect_t(cref<SDL_FRect> other) noexcept :
			position { static_cast<T>(other.x), static_cast<T>(other.y) },
			size { static_cast<T>(other.w), static_cast<T>(other.h) } {}

		constexpr explicit rect_t(rval<SDL_FRect> other) noexcept :
			position { static_cast<T>(std::move(other.x)), static_cast<T>(std::move(other.y)) },
			size { static_cast<T>(std::move(other.w)), static_cast<T>(std::move(other.h)) } {}

		constexpr ref<rect_t> operator=(cref<rect_t> other) noexcept {
			position = other.position;
			size = other.size;

			return *this;
		}

		constexpr ref<rect_t> operator=(rval<rect_t> other) noexcept {
			position = std::move(other.position);
			size = std::move(other.size);

			return *this;
		}

		constexpr ref<rect_t> operator=(cref<SDL_Rect> other) noexcept {
			position.x = other.x;
			position.y = other.y;

			size.w = other.w;
			size.h = other.h;

			return *this;
		}

		constexpr ref<rect_t> operator=(rval<SDL_Rect> other) noexcept {
			position.x = std::move(other.x);
			position.y = std::move(other.y);

			size.w = std::move(other.w);
			size.h = std::move(other.h);

			return *this;
		}

		constexpr ref<rect_t> operator=(cref<SDL_FRect> other) noexcept {
			position.x = static_cast<T>(other.x);
			position.y = static_cast<T>(other.y);

			size.w = static_cast<T>(other.w);
			size.h = static_cast<T>(other.h);

			return *this;
		}

		constexpr ref<rect_t> operator=(rval<SDL_FRect> other) noexcept {
			position.x = static_cast<T>(std::move(other.x));
			position.y = static_cast<T>(std::move(other.y));

			size.w = static_cast<T>(std::move(other.w));
			size.h = static_cast<T>(std::move(other.h));

			return *this;
		}

		constexpr bool operator==(cref<rect_t> other) const noexcept {
			return position == other.position && size == other.size;
		}

		constexpr bool operator!=(cref<rect_t> other) const noexcept {
			return position != other.position || size != other.size;
		}

		constexpr bool operator<(cref<rect_t> other) = delete;
		constexpr bool operator<=(cref<rect_t> other) = delete;

		constexpr bool operator>(cref<rect_t> other) = delete;
		constexpr bool operator>=(cref<rect_t> other) = delete;

		constexpr point_t<T> center() const noexcept { return position + size / 2; }

		constexpr T area() const noexcept { return size.size(); }

		constexpr operator SDL_Rect() const noexcept { return SDL_Rect { position.x, position.y, size.w, size.h }; }

		constexpr explicit operator SDL_FRect() const noexcept {
			return SDL_FRect { (f32)position.x, (f32)position.y, (f32)size.w, (f32)size.h };
		}
	};
} // namespace Bleakdepth
