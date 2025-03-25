#pragma once

#include <bleak/typedef.hpp>

#include <cassert>

#include <SDL_mixer.h>

#include <bleak/log.hpp>

namespace bleak {
	namespace sdl {
		namespace mixer {
			using channel_t = i32;
			using volume_t = i32;

			static inline bool init_mono(i32 frequency, u16 format, i32 chunk_size) noexcept { return Mix_OpenAudio(frequency, format, 1, chunk_size) == 0; }

			static inline bool init_stereo(i32 frequency, u16 format, i32 chunk_size) noexcept { return Mix_OpenAudio(frequency, format, 2, chunk_size) == 0; }

			static inline bool is_playing(i32 channel) noexcept {
				assert(channel > -1);
				return Mix_Playing(channel);
			}

			static inline i32 playing_count() noexcept { return Mix_Playing(-1); }

			static inline bool is_paused(i32 channel) noexcept {
				assert(channel > -1);
				return Mix_Paused(channel);
			}

			static inline i32 paused_count() noexcept { return Mix_Paused(-1); }

			// sets the volume of all channels
			static inline void set_volume(i32 volume) noexcept { Mix_Volume(-1, volume); }

			// sets the volume of a specific channel
			static inline void set_volume(i32 channel, i32 volume) noexcept {
				assert(channel > -1);
				Mix_Volume(channel, volume);
			}

			// returns the average volume of all channels
			static inline i32 get_volume() noexcept { return Mix_Volume(-1, -1); }

			// returns the volume of a specific channel
			static inline i32 get_volume(i32 channel) noexcept {
				assert(channel > -1);
				return Mix_Volume(channel, -1);
			}

			static inline void close() noexcept { Mix_CloseAudio(); }

			// pauses all channels
			static inline void pause() noexcept { Mix_Pause(-1); }

			// pauses a specific channel
			static inline void pause(i32 channel) noexcept {
				assert(channel > -1);
				Mix_Pause(channel);
			}

			// resumes all channels
			static inline void resume() noexcept { Mix_Resume(-1); }

			// resumes a specific channel
			static inline void resume(i32 channel) noexcept {
				assert(channel >= 0);
				Mix_Resume(channel);
			}

			// halts all channels
			static inline void halt() noexcept { Mix_HaltChannel(-1); }

			// halts a specific channel
			static inline void halt(i32 channel) noexcept {
				assert(channel >= 0);
				Mix_HaltChannel(channel);
			}
		} // namespace mixer
	} // namespace sdl

	enum class channel_t : u8 { Mono, Stereo };

	struct mixer_s {
	private:
		static inline bool initialized;
	  public:
		static constexpr i32 default_frequency{ 44100 };
		static constexpr u16 default_format{ MIX_DEFAULT_FORMAT };
		static constexpr i32 default_chunk_size{ 2048 };

		inline mixer_s() noexcept {
			mixer_s::initialized = sdl::mixer::init_stereo(default_frequency, default_format, default_chunk_size);
			
			if (!mixer_s::initialized) {
				error_log.add("failed to initialize sdl-mixer: {}", Mix_GetError());
				error_log.flush_to_console();
			}

			assert(mixer_s::is_initialized());
		}

		inline mixer_s(i32 frequency, u16 format, channel_t channels, i32 chunk_size) noexcept {
			if (channels == channel_t::Mono) {
				mixer_s::initialized = sdl::mixer::init_mono(frequency, format, chunk_size);
			} else if (channels == channel_t::Stereo) {
				mixer_s::initialized = sdl::mixer::init_stereo(frequency, format, chunk_size);
			}
			
			if (!mixer_s::initialized) {
				error_log.add("failed to initialize sdl-mixer: {}", Mix_GetError());
				error_log.flush_to_console();
			}

			assert(mixer_s::is_initialized());
		}

		inline ~mixer_s() noexcept {
			sdl::mixer::close();
		}

		static inline bool is_initialized() noexcept { return initialized; }

		static inline void pause() noexcept { sdl::mixer::pause(); }

		static inline void pause(i32 channel) noexcept { sdl::mixer::pause(channel); }

		static inline void resume() noexcept { sdl::mixer::resume(); }

		static inline void resume(i32 channel) noexcept { sdl::mixer::resume(channel); }

		static inline void halt() noexcept { sdl::mixer::halt(); }

		static inline void halt(i32 channel) noexcept { sdl::mixer::halt(channel); }
	};
} // namespace bleak
