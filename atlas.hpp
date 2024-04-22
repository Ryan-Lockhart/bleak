#pragma once

#include <stdexcept>
#include <utility>

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture.hpp"

#include "point.hpp"
#include "rect.hpp"

#include "glyph.hpp"

class Atlas
{
private:
	const std::string path;

	std::vector<SDL_Rect> rects;

	Texture texture;

	const Point atlasSize;
	const Point imageSize;
	const Point glyphSize;

public:
	inline Atlas() = delete;
	inline Atlas(rval<Texture> texture, cref<Point> size) :
		path(path),

		rects(),
		texture(std::move(texture)),

		atlasSize(size),
		imageSize(this->texture.size),
		glyphSize(imageSize / size)
	{
		if (atlasSize.x <= 0 || atlasSize.y <= 0)
			throw std::runtime_error("atlas size must be positive!");

		if (imageSize.x <= 0 || imageSize.y <= 0)
			throw std::runtime_error("image size must be positive!");

		if (glyphSize.x <= 0 || glyphSize.y <= 0)
			throw std::runtime_error("glyph size must be positive!");

		if (imageSize.x % size.x != 0 || imageSize.y % size.y != 0)
			throw std::runtime_error("image size must be divisible by the atlas size!");

		rects.reserve(atlasSize.size());

		for (int y = 0; y < size.y; y++)
		{
			for (int x = 0; x < size.x; x++)
			{
				rects.emplace_back(
					x * glyphSize.x,
					y * glyphSize.y,
					glyphSize.x,
					glyphSize.y
				);
			}
		}
	}

	inline Atlas(cref<Atlas> other) = delete;
	inline Atlas(rval<Atlas> other) = delete;

	inline ref<Atlas> operator=(cref<Atlas> other) = delete;
	inline ref<Atlas> operator=(rval<Atlas> other) = delete;

	inline ~Atlas() = default;

	inline void drawGlyph(cref<Glyph> glyph, cref<Point> position, bool onGrid = true) const
	{
		if (glyph.index < 0 || glyph.index >= rects.size()) throw std::out_of_range("glyph index out of range!");

		const Point pos = onGrid ? position * glyphSize : position;
		const SDL_Rect dst{ pos.x, pos.y, glyphSize.x, glyphSize.y };

		texture.draw(&rects[glyph.index], &dst, glyph.color);
	}

	inline void drawGlyphs(cref<std::vector<Glyph>> glyphs, cref<std::vector<Point>> positions, bool onGrid = true) const
	{
		if (glyphs.size() != positions.size()) throw std::invalid_argument("glyphs and positions must have the same size!");

		for (usize i = 0; i < glyphs.size(); i++)
			drawGlyph(glyphs[i], positions[i], onGrid);
	}

	inline void drawGlyphs(cptr<Glyph> glyphs, cptr<Point> positions, usize count, bool onGrid = true) const
	{
		for (usize i = 0; i < count; i++)
			drawGlyph(glyphs[i], positions[i], onGrid);
	}
};
