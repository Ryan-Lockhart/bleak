#pragma once

#include <stdexcept>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

namespace Bleakdepth {
	struct Subsystem {
	  private:
		static inline bool sdl_initialized;
		static inline bool sdl_image_initialized;
		static inline bool sdl_mixer_initialized;
		static inline bool sdl_net_initialized;
		static inline bool sdl_ttf_initialized;

		static inline void initialize_sdl() {
			if (sdl_initialized) {
				return;
			}

			sdl_initialized = SDL_Init(SDL_INIT_VIDEO) == 0;
			std::string sdl_error { SDL_GetError() };

			if (!sdl_initialized) {
				throw std::runtime_error("failed to initialize sdl: " + sdl_error);
			}
		}

		static inline void initialize_sdl_image() {
			if (sdl_image_initialized) {
				return;
			}

			sdl_image_initialized = IMG_Init(IMG_INIT_PNG) != 0;
			std::string sdl_image_error { IMG_GetError() };

			if (!sdl_image_initialized) {
				throw std::runtime_error("failed to initialize sdl-image: " + sdl_image_error);
			}
		}

		static inline void initialize_sdl_mixer() {
			if (sdl_mixer_initialized) {
				return;
			}

			sdl_mixer_initialized = Mix_Init(MIX_INIT_OGG) != 0;
			std::string sdl_mixer_error { Mix_GetError() };

			if (!sdl_mixer_initialized) {
				throw std::runtime_error("failed to initialize sdl-mixer: " + sdl_mixer_error);
			}
		}

		static inline void initialize_sdl_net() {
			if (sdl_net_initialized) {
				return;
			}

			sdl_net_initialized = SDLNet_Init() == 0;
			std::string sdl_net_error { SDLNet_GetError() };

			if (!sdl_net_initialized) {
				throw std::runtime_error("failed to initialize sdl-net: " + sdl_net_error);
			}
		}

		static inline void initialize_sdl_ttf() {
			if (sdl_ttf_initialized) {
				return;
			}

			sdl_ttf_initialized = TTF_Init() == 0;
			std::string sdl_ttf_error { TTF_GetError() };

			if (!sdl_ttf_initialized) {
				throw std::runtime_error("failed to initialize sdl-ttf: " + sdl_ttf_error);
			}
		}

		static inline void terminate_sdl() {
			if (!sdl_initialized) {
				return;
			}

			SDL_Quit();
			sdl_initialized = false;
		}

		static inline void terminate_sdl_image() {
			if (!sdl_image_initialized) {
				return;
			}

			IMG_Quit();
			sdl_image_initialized = false;
		}

		static inline void terminate_sdl_mixer() {
			if (!sdl_mixer_initialized) {
				return;
			}

			Mix_Quit();
			sdl_mixer_initialized = false;
		}

		static inline void terminate_sdl_net() {
			if (!sdl_net_initialized) {
				return;
			}

			SDLNet_Quit();
			sdl_net_initialized = false;
		}

		static inline void terminate_sdl_ttf() {
			if (!sdl_ttf_initialized) {
				return;
			}

			TTF_Quit();
			sdl_ttf_initialized = false;
		}

	  public:
		static inline void initialize() {
			if (is_initialized()) {
				return;
			}

			initialize_sdl();
			initialize_sdl_image();
			initialize_sdl_mixer();
			initialize_sdl_net();
			initialize_sdl_ttf();
		}

		static inline void terminate() {
			if (!is_initialized()) {
				return;
			}

			terminate_sdl_ttf();
			terminate_sdl_net();
			terminate_sdl_mixer();
			terminate_sdl_image();
			terminate_sdl();
		}

		static inline bool is_initialized() {
			return sdl_initialized && sdl_image_initialized && sdl_mixer_initialized && sdl_net_initialized
				   && sdl_ttf_initialized;
		}
	};
} // namespace Bleakdepth
