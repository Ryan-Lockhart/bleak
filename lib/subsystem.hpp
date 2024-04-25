#pragma once

#include <iostream>

#include <stdexcept>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

namespace Bleakdepth {
	struct subsystem {
	  private:
		static inline bool sdl_initialized;
		static inline bool sdl_image_initialized;
		static inline bool sdl_mixer_initialized;
		static inline bool sdl_net_initialized;
		static inline bool sdl_ttf_initialized;

	  public:
		static inline void initialize() {
			if (initialized()) {
				return;
			}

			sdl_initialized = SDL_Init(SDL_INIT_VIDEO) == 0;
			std::string sdl_error { SDL_GetError() };

			sdl_image_initialized = IMG_Init(IMG_INIT_PNG) != 0;
			std::string sdl_image_error { IMG_GetError() };

			sdl_mixer_initialized = Mix_Init(MIX_INIT_OGG) != 0;
			std::string sdl_mixer_error { Mix_GetError() };

			sdl_net_initialized = SDLNet_Init() == 0;
			std::string sdl_net_error { SDLNet_GetError() };

			sdl_ttf_initialized = TTF_Init() == 0;
			std::string sdl_ttf_error { TTF_GetError() };

			if (!initialized()) {
				terminate();

				if (!sdl_initialized) {
					std::cerr << "failed to initialize sdl: " << sdl_error << "\n";
				}
				if (!sdl_image_initialized) {
					std::cerr << "failed to initialize sdl-image: " << sdl_image_error << "\n";
				}
				if (!sdl_mixer_initialized) {
					std::cerr << "failed to initialize sdl-mixer: " << sdl_mixer_error << "\n";
				}
				if (!sdl_net_initialized) {
					std::cerr << "failed to initialize sdl-net: " << sdl_net_error << "\n";
				}
				if (!sdl_ttf_initialized) {
					std::cerr << "failed to initialize sdl-ttf: " << sdl_ttf_error << "\n";
				}

				throw std::runtime_error("failed to initialize subsystems!");
			}
		}

		static inline void terminate() {
			if (!initialized()) {
				return;
			}

			if (sdl_ttf_initialized) {
				TTF_Quit();
				sdl_ttf_initialized = false;
				std::cout << "sdl-ttf terminated\n";
			}

			if (sdl_net_initialized) {
				SDLNet_Quit();
				sdl_net_initialized = false;
				std::cout << "sdl-net terminated\n";
			}

			if (sdl_mixer_initialized) {
				Mix_Quit();
				sdl_mixer_initialized = false;
				std::cout << "sdl-mixer terminated\n";
			}

			if (sdl_image_initialized) {
				IMG_Quit();
				sdl_image_initialized = false;
				std::cout << "sdl-image terminated\n";
			}

			if (sdl_initialized) {
				SDL_Quit();
				sdl_initialized = false;
				std::cout << "sdl terminated\n";
			}
		}

		static inline bool initialized() {
			return sdl_initialized && sdl_image_initialized && sdl_mixer_initialized && sdl_net_initialized
				   && sdl_ttf_initialized;
		}
	};

	static inline subsystem Subsystem;
} // namespace Bleakdepth
