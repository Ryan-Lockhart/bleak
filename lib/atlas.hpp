#pragma once

#include <stdexcept>
#include <utility>

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "texture.hpp"
#include "array.hpp"

#include "point.hpp"
#include "rect.hpp"

#include "glyph.hpp"

template<usize Width, usize Height> class Atlas
{
private:
	static_assert(Width > 0, "atlas width must be greater than zero!");
	static_assert(Height > 0, "atlas height must be greater than zero!");

	Array<Rect<i32>, Width, Height> rects;

	Texture texture;
	
	const Size<i32> imageSize;
	const Size<i32> glyphSize;

public:
	static constexpr Size<i32> size{ Width, Height };

	inline Atlas() = delete;
	inline Atlas(rval<Texture> texture) :
		rects{ }, texture{ std::move(texture) },

		imageSize{ this->texture.info.size },
		glyphSize{ imageSize / size }
	{
		if (imageSize.x <= 0 || imageSize.y <= 0) throw std::runtime_error("image size must be greater than zero!");
		if (glyphSize.x <= 0 || glyphSize.y <= 0) throw std::runtime_error("glyph size must be greater than zero!");

		if (imageSize.x % size.w != 0 || imageSize.y % size.h != 0)
			throw std::runtime_error("image size must be divisible by the atlas size!");

		for (int y = 0; y < size.y; ++y)
		{
			for (int x = 0; x < size.x; ++x)
			{
				rects[x, y] = Rect{
					x * glyphSize.x,
					y * glyphSize.y,
					glyphSize.w,
					glyphSize.h
				};
			}
		}
	}

	inline Atlas(cref<Atlas> other) = delete;
	inline Atlas(rval<Atlas> other) = delete;

	inline ref<Atlas> operator=(cref<Atlas> other) = delete;
	inline ref<Atlas> operator=(rval<Atlas> other) = delete;

	inline ~Atlas() = default;

	inline void draw(cref<Glyph> glyph, cref<Point<i32>> position, bool onGrid = true) const
	{
		if (glyph.index < 0 || glyph.index >= rects.size())
			throw std::out_of_range("glyph index out of range!");

		const Point pos = onGrid ? position * glyphSize : position;
		const SDL_Rect dst{ pos.x, pos.y, glyphSize.x, glyphSize.y };

		texture.draw(&rects[glyph.index], &dst, glyph.color);
	}
};
