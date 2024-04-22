#pragma once

#include "typedef.hpp"

#include <format>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "point.hpp"

struct Texture
{
private:
	static inline Point GetTextureSize(SDL_Texture* texture) { Point size; SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y); return size; }
	static inline u32 GetTextureAccess(SDL_Texture* texture) { u32 access; SDL_QueryTexture(texture, &access, nullptr, nullptr, nullptr); return access; }
	static inline i32 GetTextureChannels(SDL_Texture* texture) { i32 channels; SDL_QueryTexture(texture, nullptr, &channels, nullptr, nullptr); return channels; }

	ptr<SDL_Renderer> renderer;
	ptr<SDL_Texture> texture;

public:
	const Point size;
	const i32 access;
	const u32 channels;

	inline Texture() = delete;

	constexpr Texture(cref<Texture> other) noexcept = delete;
	constexpr Texture(rval<Texture> other) noexcept : renderer(std::move(other.renderer)), texture(std::move(other.texture)), size(other.size), access(other.access), channels(other.channels) {}

	constexpr ref<Texture> operator=(cref<Texture> other) noexcept = delete;
	constexpr ref<Texture> operator=(rval<Texture> other) noexcept = delete;

	inline Texture(SDL_Renderer* renderer, cstr path) : renderer(renderer), texture(IMG_LoadTexture(renderer, path)), size(GetTextureSize(texture)), access(GetTextureAccess(texture)), channels(GetTextureChannels(texture))
	{
		if (renderer == nullptr) throw std::runtime_error("renderer must not be null!");
		if (texture == nullptr) throw std::runtime_error(std::format("failed to load texture!"));

		if (size.x <= 0) throw std::runtime_error("texture width must be greater than zero!");
		if (size.y <= 0) throw std::runtime_error("texture width must be greater than zero!");

		if (channels == SDL_PIXELFORMAT_UNKNOWN) throw std::runtime_error("texture channels must be known!");
	}

	inline ~Texture() { if (texture != nullptr) SDL_DestroyTexture(texture); }

	constexpr ptr<SDL_Texture> handle() { return texture; }
	constexpr cptr<SDL_Texture> handle() const { return texture; }

	constexpr void draw(const SDL_Rect* src, const SDL_Rect* dst) const { SDL_RenderCopy(renderer, texture, src, dst); }
	constexpr void draw(const SDL_Rect* src, const SDL_Rect* dst, cref<Color> color) const
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderCopy(renderer, texture, src, dst);
	}
};
