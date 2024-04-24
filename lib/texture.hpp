#pragma once

#include "typedef.hpp"

#include <stdexcept>
#include <format>

#include <SDL.h>
#include <SDL_image.h>

#include "point.hpp"
#include "color.hpp"

#include <renderer.hpp>

struct Texture
{
private:
	struct Info
	{
		const Point<i32> size;
		const i32 access;
		const u32 channels;
	};

	static inline ptr<SDL_Texture> LoadTexture(ref<Renderer> renderer, cstr path)
	{
		ptr<SDL_Texture> texture{ IMG_LoadTexture(renderer.handle(), path) };

		if (!texture) throw std::runtime_error(std::format("could not load texture: {}", SDL_GetError()));

		return texture;
	}

	static inline Info GetTextureInfo(ptr<SDL_Texture> texture)
	{
		Point<i32> size;
		i32 access;
		u32 channels;

		if (SDL_QueryTexture(texture, &channels, &access, &size.x, &size.y) < 0)
			throw std::runtime_error(std::format("could not get texture info: {}", SDL_GetError()));

		return { size, access, channels };
	}

	ref<Renderer> renderer;
	ptr<SDL_Texture> texture;

public:
	const Info info;

	inline Texture() = delete;

	constexpr Texture(cref<Texture> other) noexcept = delete;
	constexpr Texture(rval<Texture> other) noexcept : renderer(other.renderer), texture(std::move(other.texture)), info(other.info) {}

	constexpr ref<Texture> operator=(cref<Texture> other) noexcept = delete;
	constexpr ref<Texture> operator=(rval<Texture> other) noexcept = delete;

	inline Texture(ref<Renderer> renderer, cstr path) :
		renderer{ renderer },
		texture{ LoadTexture(renderer, path) },
		info { GetTextureInfo(texture) }
	{
		if (texture == nullptr) throw std::runtime_error(std::format("failed to load texture!"));

		if (info.size.x <= 0) throw std::runtime_error("texture width must be greater than zero!");
		if (info.size.y <= 0) throw std::runtime_error("texture width must be greater than zero!");

		if (info.channels == SDL_PIXELFORMAT_UNKNOWN) throw std::runtime_error("texture channels must be known!");
	}

	inline ~Texture() { if (texture != nullptr) SDL_DestroyTexture(texture); }

	constexpr ptr<SDL_Texture> handle() { return texture; }
	constexpr cptr<SDL_Texture> handle() const { return texture; }

	constexpr void draw(cref<Point<i32>> pos) const
	{
		const SDL_Rect dst{ pos.x, pos.y, info.size.x, info.size.y };
		SDL_RenderCopy(renderer.handle(), texture, nullptr, &dst);
	}
	constexpr void draw(cref<Point<i32>> pos, cref<Color> color) const
	{
		const SDL_Rect dst{ pos.x, pos.y, info.size.x, info.size.y };
		SDL_SetRenderDrawColor(renderer.handle(), color.r, color.g, color.b, color.a);
		SDL_RenderCopy(renderer.handle(), texture, nullptr, &dst);
	}

	constexpr void draw(const SDL_Rect* dst) const { SDL_RenderCopy(renderer.handle(), texture, nullptr, dst); }
	constexpr void draw(const SDL_Rect* dst, cref<Color> color) const
	{
		SDL_SetRenderDrawColor(renderer.handle(), color.r, color.g, color.b, color.a);
		SDL_RenderCopy(renderer.handle(), texture, nullptr, dst);
	}

	constexpr void draw(const SDL_Rect* src, const SDL_Rect* dst) const { SDL_RenderCopy(renderer.handle(), texture, src, dst); }
	constexpr void draw(const SDL_Rect* src, const SDL_Rect* dst, cref<Color> color) const
	{
		SDL_SetRenderDrawColor(renderer.handle(), color.r, color.g, color.b, color.a);
		SDL_RenderCopy(renderer.handle(), texture, src, dst);
	}
};
