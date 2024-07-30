#pragma once

#include <bleak/typedef.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

#include <bleak/log.hpp>

namespace bleak {
	struct subsystem_s {
	  private:
		static inline bool sdl_initialized;
		static inline bool sdl_image_initialized;
		static inline bool sdl_mixer_initialized;
		static inline bool sdl_net_initialized;
		static inline bool sdl_ttf_initialized;

		static inline void initialize_sdl() noexcept {
			if (sdl_initialized) {
				return;
			}

			sdl_initialized = SDL_Init(SDL_INIT_VIDEO) == 0;

			if (!sdl_initialized) {
				error_log.add("failed to initialize sdl: {}", SDL_GetError());
			}
		}

		static inline void initialize_sdl_image() noexcept {
			if (sdl_image_initialized) {
				return;
			}

			sdl_image_initialized = IMG_Init(IMG_INIT_PNG) != 0;

			if (!sdl_image_initialized) {
				error_log.add("failed to initialize sdl-image: {}", IMG_GetError());
			}
		}

		static inline void initialize_sdl_mixer() noexcept {
			if (sdl_mixer_initialized) {
				return;
			}

			sdl_mixer_initialized = Mix_Init(MIX_INIT_OGG) != 0;

			if (!sdl_mixer_initialized) {
				error_log.add("failed to initialize sdl-mixer: {}", Mix_GetError());
			}
		}

		static inline void initialize_sdl_net() noexcept {
			if (sdl_net_initialized) {
				return;
			}

			sdl_net_initialized = SDLNet_Init() == 0;

			if (!sdl_net_initialized) {
				error_log.add("failed to initialize sdl-net: {}", SDLNet_GetError());
			}
		}

		static inline void initialize_sdl_ttf() noexcept {
			if (sdl_ttf_initialized) {
				return;
			}

			sdl_ttf_initialized = TTF_Init() == 0;

			if (!sdl_ttf_initialized) {
				error_log.add("failed to initialize sdl-ttf: {}", TTF_GetError());
			}
		}

		static inline void terminate_sdl() noexcept {
			if (!sdl_initialized) {
				return;
			}

			SDL_Quit();
			sdl_initialized = false;
		}

		static inline void terminate_sdl_image() noexcept {
			if (!sdl_image_initialized) {
				return;
			}

			IMG_Quit();
			sdl_image_initialized = false;
		}

		static inline void terminate_sdl_mixer() noexcept {
			if (!sdl_mixer_initialized) {
				return;
			}

			Mix_Quit();
			sdl_mixer_initialized = false;
		}

		static inline void terminate_sdl_net() noexcept {
			if (!sdl_net_initialized) {
				return;
			}

			SDLNet_Quit();
			sdl_net_initialized = false;
		}

		static inline void terminate_sdl_ttf() noexcept {
			if (!sdl_ttf_initialized) {
				return;
			}

			TTF_Quit();
			sdl_ttf_initialized = false;
		}

	  public:
		inline subsystem_s() noexcept {
			if (is_initialized()) {
				return;
			}

			initialize_sdl();
			initialize_sdl_image();
			initialize_sdl_mixer();
			initialize_sdl_net();
			initialize_sdl_ttf();
		}

		inline ~subsystem_s() noexcept {
			if (!is_initialized()) {
				return;
			}

			terminate_sdl_ttf();
			terminate_sdl_net();
			terminate_sdl_mixer();
			terminate_sdl_image();
			terminate_sdl();
		}

		static inline bool is_initialized() noexcept { return sdl_initialized && sdl_image_initialized && sdl_mixer_initialized && sdl_net_initialized && sdl_ttf_initialized; }
	};
} // namespace bleak
