#pragma once

#include <bleak/typedef.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

#include <bleak/log.hpp>
#include <bleak/steam.hpp>

namespace bleak {
	struct subsystem_s {
	  private:
		static inline bool steam_initialized{ false };
		static inline bool sdl_initialized{ false };
		static inline bool sdl_image_initialized{ false };
		static inline bool sdl_mixer_initialized{ false };
		static inline bool sdl_net_initialized{ false };
		static inline bool sdl_ttf_initialized{ false };

		static inline void initialize_steam() noexcept {
			if (steam_initialized) {
				return;
			}

			steam_initialized = steam::initialize() == steam::init_result::Ok;
			
			if (!steam_initialized) {
				error_log.add("[ERROR]: failed to initialize steam subsystem: {}", steam::get_error());
			}
		}

		static inline void initialize_steam(u32 app_id) noexcept {
			if (steam_initialized) {
				return;
			}

			steam::init_result result = steam::initialize(app_id);

			steam_initialized = result == steam::init_result::Ok;
			
			if (!steam_initialized) {
				error_log.add("[{}]: failed to initialize steam subsystem: {}",
					result == steam::init_result::InvalidLaunch ? "WARNING" : "ERROR",
					steam::get_error()
				);
			}
		}

		static inline void initialize_sdl() noexcept {
			if (sdl_initialized) {
				return;
			}

			sdl_initialized = SDL_Init(SDL_INIT_VIDEO) == 0;

			if (!sdl_initialized) {
				error_log.add("[ERROR]: failed to initialize sdl subsystem: {}", SDL_GetError());
			}
		}

		static inline void initialize_sdl_image() noexcept {
			if (sdl_image_initialized) {
				return;
			}

			sdl_image_initialized = IMG_Init(IMG_INIT_PNG) != 0;

			if (!sdl_image_initialized) {
				error_log.add("[ERROR]: failed to initialize sdl-image subsystem: {}", IMG_GetError());
			}
		}

		static inline void initialize_sdl_mixer() noexcept {
			if (sdl_mixer_initialized) {
				return;
			}

			sdl_mixer_initialized = Mix_Init(MIX_INIT_OGG) != 0;

			if (!sdl_mixer_initialized) {
				error_log.add("[ERROR]: failed to initialize sdl-mixer subsystem: {}", Mix_GetError());
			}
		}

		static inline void initialize_sdl_net() noexcept {
			if (sdl_net_initialized) {
				return;
			}

			sdl_net_initialized = SDLNet_Init() == 0;

			if (!sdl_net_initialized) {
				error_log.add("[ERROR]: failed to initialize sdl-net subsystem: {}", SDLNet_GetError());
			}
		}

		static inline void initialize_sdl_ttf() noexcept {
			if (sdl_ttf_initialized) {
				return;
			}

			sdl_ttf_initialized = TTF_Init() == 0;

			if (!sdl_ttf_initialized) {
				error_log.add("[ERROR]: failed to initialize sdl-ttf subsystem: {}", TTF_GetError());
			}
		}

		static inline void terminate_steam() noexcept {
			if (!steam_initialized) {
				return;
			}
			
			steam::shutdown();

			steam_initialized = false;
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

			initialize_steam();
			initialize_sdl();
			initialize_sdl_image();
			initialize_sdl_mixer();
			initialize_sdl_net();
			initialize_sdl_ttf();
		}
		inline subsystem_s(u32 app_id) noexcept {
			if (is_initialized()) {
				return;
			}

			initialize_steam(app_id);
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
			terminate_steam();
		}

		static inline bool is_initialized() noexcept { return steam_initialized && sdl_initialized && sdl_image_initialized && sdl_mixer_initialized && sdl_net_initialized && sdl_ttf_initialized; }
	};
} // namespace bleak
