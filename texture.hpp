#pragma once

#include "typedef.hpp"

#include <fmt/core.h>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "point.hpp"

struct Texture
{
private:
	static inline ptr<SDL_Texture> LoadTexture(ptr<SDL_Renderer> renderer, cstr path)
	{
		ptr<SDL_Texture> texture{ IMG_LoadTexture(renderer, path) };

		if (!texture) throw std::runtime_error(fmt::format("could not load texture: {}", SDL_GetError()));

		return texture;
	}

	static inline Point GetTextureSize(ptr<SDL_Texture> texture)
	{
		Point size;
		
		if (SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y) < 0)
			throw std::runtime_error(fmt::format("could not get texture size: {}", SDL_GetError()));
		
		return size;
	}

	static inline u32 GetTextureAccess(ptr<SDL_Texture> texture)
	{
		u32 access;

		if (SDL_QueryTexture(texture, &access, nullptr, nullptr, nullptr) < 0)
			throw std::runtime_error(fmt::format("could not get texture access: {}", SDL_GetError()));

		return access;
	}
	static inline i32 GetTextureChannels(ptr<SDL_Texture> texture)
	{
		i32 channels;

		if (SDL_QueryTexture(texture, nullptr, &channels, nullptr, nullptr) < 0)
			throw std::runtime_error(fmt::format("could not get texture channels: {}", SDL_GetError()));

		return channels;
	}

	ptr<SDL_Renderer> renderer;
	ptr<SDL_Texture> texture;

public:
	struct Info
	{
		const Point size;
		const i32 access;
		const u32 channels;
	};

	const Info info;

	inline Texture() = delete;

	constexpr Texture(cref<Texture> other) noexcept = delete;
	constexpr Texture(rval<Texture> other) noexcept : renderer(std::move(other.renderer)), texture(std::move(other.texture)), info(other.info) {}

	constexpr ref<Texture> operator=(cref<Texture> other) noexcept = delete;
	constexpr ref<Texture> operator=(rval<Texture> other) noexcept = delete;

	inline Texture(SDL_Renderer* renderer, cstr path) : renderer(renderer), texture(LoadTexture(renderer, path)), info { GetTextureSize(texture), GetTextureAccess(texture), GetTextureChannels(texture) }
	{
		if (renderer == nullptr) throw std::runtime_error("renderer must not be null!");
		if (texture == nullptr) throw std::runtime_error(std::format("failed to load texture!"));

		if (info.size.x <= 0) throw std::runtime_error("texture width must be greater than zero!");
		if (info.size.y <= 0) throw std::runtime_error("texture width must be greater than zero!");

		if (info.channels == SDL_PIXELFORMAT_UNKNOWN) throw std::runtime_error("texture channels must be known!");
	}

	inline ~Texture() { if (texture != nullptr) SDL_DestroyTexture(texture); }

	constexpr ptr<SDL_Texture> handle() { return texture; }
	constexpr cptr<SDL_Texture> handle() const { return texture; }

	constexpr void draw(const Point& pos) const { const SDL_Rect dst{ pos.x, pos.y, info.size.x, info.size.y }; SDL_RenderCopy(renderer, texture, nullptr, &dst); }
	constexpr void draw(const Point& pos, cref<Color> color) const
	{
		const SDL_Rect dst{ pos.x, pos.y, info.size.x, info.size.y };
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); SDL_RenderCopy(renderer, texture, nullptr, &dst);
	}

	constexpr void draw(const SDL_Rect* dst) const { SDL_RenderCopy(renderer, texture, nullptr, dst); }
	constexpr void draw(const SDL_Rect* dst, cref<Color> color) const
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderCopy(renderer, texture, nullptr, dst);
	}

	constexpr void draw(const SDL_Rect* src, const SDL_Rect* dst) const { SDL_RenderCopy(renderer, texture, src, dst); }
	constexpr void draw(const SDL_Rect* src, const SDL_Rect* dst, cref<Color> color) const
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderCopy(renderer, texture, src, dst);
	}
};
