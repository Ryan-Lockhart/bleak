#pragma once

#include "typedef.hpp"

#include <utility>

#include <SDL.h>

#include "point.hpp"

template<typename T = i32> struct Rect
{
	Point<T> position;
	Size<T> size;

	constexpr Rect() noexcept :
		position{ },
		size{ }
	{ }

	constexpr Rect(T w, T h) noexcept :
		position{ },
		size{ w, h }
	{ }

	constexpr Rect(T x, T y, T w, T h) noexcept :
		position{ x, y },
		size{ w, h }
	{ }

	constexpr Rect(cref<Size<T>> size) noexcept :
		position{ },
		size{ size }
	{ }

	constexpr Rect(cref<Point<T>> position, cref<Size<T>> size) noexcept :
		position{ position },
		size{ size }
	{ }

	constexpr Rect(cref<Rect> other) noexcept :
		position{ other.position },
		size{ other.size }
	{ }

	constexpr Rect(rval<Rect> other) noexcept :
		position{ std::move(other.position) },
		size{ std::move(other.size) }
	{ }

	constexpr Rect(cref<SDL_Rect> other) noexcept :
		position {
			std::move(other.x),
			std::move(other.y)
		},
		size {
			std::move(other.w),
			std::move(other.h)
		}
	{ }

	constexpr Rect(rval<SDL_Rect> other) noexcept :
		position {
			std::move(other.x),
			std::move(other.y)
		},
		size {
			std::move(other.w),
			std::move(other.h)
		}
	{ }

	constexpr explicit Rect(cref<SDL_FRect> other) noexcept :
		position {
			static_cast<T>(other.x),
			static_cast<T>(other.y)
		},
		size {
			static_cast<T>(other.w),
			static_cast<T>(other.h)
		}
	{ }

	constexpr explicit Rect(rval<SDL_FRect> other) noexcept :
		position {
			static_cast<T>(std::move(other.x)),
			static_cast<T>(std::move(other.y))
		},
		size {
			static_cast<T>(std::move(other.w)),
			static_cast<T>(std::move(other.h))
		}
	{ }

	constexpr ref<Rect> operator=(cref<Rect> other) noexcept
	{
		position = other.position;
		size = other.size;

		return *this;
	}

	constexpr ref<Rect> operator=(rval<Rect> other) noexcept
	{
		position = std::move(other.position);
		size = std::move(other.size);

		return *this;
	}

	constexpr ref<Rect> operator=(cref<SDL_Rect> other) noexcept
	{
		position.x = other.x;
		position.y = other.y;

		size.w = other.w;
		size.h = other.h;

		return *this;
	}

	constexpr ref<Rect> operator=(rval<SDL_Rect> other) noexcept
	{
		position.x = std::move(other.x);
		position.y = std::move(other.y);

		size.w = std::move(other.w);
		size.h = std::move(other.h);

		return *this;
	}

	constexpr ref<Rect> operator=(cref<SDL_FRect> other) noexcept
	{
		position.x = static_cast<T>(other.x);
		position.y = static_cast<T>(other.y);

		size.w = static_cast<T>(other.w);
		size.h = static_cast<T>(other.h);

		return *this;
	}

	constexpr ref<Rect> operator=(rval<SDL_FRect> other) noexcept
	{
		position.x = static_cast<T>(std::move(other.x));
		position.y = static_cast<T>(std::move(other.y));

		size.w = static_cast<T>(std::move(other.w));
		size.h = static_cast<T>(std::move(other.h));

		return *this;
	}

	constexpr bool operator==(cref<Rect> other) const noexcept { return position == other.position && size == other.size; }
	constexpr bool operator!=(cref<Rect> other) const noexcept { return position != other.position || size != other.size; }

	constexpr bool operator<(cref<Rect> other) = delete;
	constexpr bool operator<=(cref<Rect> other) = delete;

	constexpr bool operator>(cref<Rect> other) = delete;
	constexpr bool operator>=(cref<Rect> other) = delete;

	constexpr Point<T> center() const noexcept { return position + size / 2; }

	constexpr T area() const noexcept { return size.size(); }

	constexpr operator SDL_Rect() const noexcept { return SDL_Rect{ position.x, position.y, size.w, size.h }; }
	constexpr explicit operator SDL_FRect() const noexcept { return SDL_FRect{ (f32)position.x, (f32)position.y, (f32)size.w, (f32)size.h }; }
};
