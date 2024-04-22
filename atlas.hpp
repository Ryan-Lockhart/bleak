#pragma once

#include <stdexcept>
#include <utility>
#include <memory>

#include <string>
#include <vector>

#include <raylib.h>

#include "point.hpp"
#include "rect.hpp"

class Atlas
{
private:
	const std::string path;

	std::vector<Rect> rects;
	std::unique_ptr<Texture2D> texture;

	const Point atlasSize;
	const Point imageSize;
	const Point glyphSize;

public:
	Atlas() = delete;
	Atlas(cref<std::string> path, cref<Point> size) :
		path(path),

		rects(),
		texture(std::make_unique<Texture2D>(LoadTexture(path.c_str()))),

		atlasSize(size),
		imageSize(texture->width, texture->height),
		glyphSize(imageSize.x / size.x, imageSize.y / size.y)
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

	Atlas(cref<Atlas> other) = delete;
	Atlas(rval<Atlas> other) = delete;

	~Atlas() { UnloadTexture(*texture); }

	void drawGlyph(cref<Glyph> glyph, cref<Point> position, bool onGrid = true) const
	{

	}

};
