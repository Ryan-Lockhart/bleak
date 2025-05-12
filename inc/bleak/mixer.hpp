#pragma once

#include <bleak/typedef.hpp>

#include <cassert>
#include <bitset>

#include <SDL_mixer.h>

#include <bleak/log.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	namespace sdl {
		namespace mixer {
			using channel_t = i32;
			using volume_t = i32;

			static inline bool init(i32 frequency, u16 format, u8 channels, i32 chunk_size) noexcept { return Mix_OpenAudio(frequency, format, channels, chunk_size) == 0; }

			static inline bool init_mono(i32 frequency, u16 format, i32 chunk_size) noexcept { return Mix_OpenAudio(frequency, format, 1, chunk_size) == 0; }

			static inline bool init_stereo(i32 frequency, u16 format, i32 chunk_size) noexcept { return Mix_OpenAudio(frequency, format, 2, chunk_size) == 0; }

			static inline bool allocate_channels(i32 channels) noexcept { return Mix_AllocateChannels(channels) == channels; }

			static inline i32 num_channels() noexcept { return Mix_AllocateChannels(-1); }

			static inline bool is_playing(i32 channel) noexcept {
				assert(channel > -1);
				return Mix_Playing(channel);
			}

			static inline i32 num_playing() noexcept { return Mix_Playing(-1); }

			static inline bool is_paused(i32 channel) noexcept {
				assert(channel > -1);
				return Mix_Paused(channel);
			}

			static inline i32 num_paused() noexcept { return Mix_Paused(-1); }

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

	enum class channel_e : u8 { Mono, Stereo };

	struct mixer_s {
		public:
		static constexpr i32 num_channels{ 256 };

		static constexpr i32 min_channel{ 0 };
		static constexpr i32 max_channel{ num_channels - 1 };
	private:
		static inline std::bitset<num_channels> channels{};
		static inline bool initialized;

	  public:
		static constexpr i32 default_frequency{ 44100 };
		static constexpr u16 default_format{ MIX_DEFAULT_FORMAT };
		static constexpr i32 default_chunk_size{ 2048 };

		static inline void initialize() noexcept {
			if (initialized) {
				return;
			}

			mixer_s::initialized = sdl::mixer::init_stereo(default_frequency, default_format, default_chunk_size);
			
			if (!mixer_s::initialized) {
				error_log.add("failed to initialize SDL mixer: {}", Mix_GetError());
				error_log.flush_to_console();
			}

			assert(mixer_s::is_initialized());

			if (!sdl::mixer::allocate_channels(num_channels)) {
				error_log.add("failed to allocate channels: {}", Mix_GetError());
			}
		} 

		inline mixer_s() noexcept { initialize(); }

		inline mixer_s(i32 frequency, u16 format, channel_e channels, i32 chunk_size) noexcept {
			mixer_s::initialized = sdl::mixer::init(frequency, format, (u8)channels, chunk_size);			
			
			if (!mixer_s::initialized) {
				error_log.add("failed to initialize SDL mixer: {}", Mix_GetError());
				error_log.flush_to_console();
			}

			assert(mixer_s::is_initialized());

			if (!sdl::mixer::allocate_channels(num_channels)) {
				error_log.add("failed to allocate channels: {}", Mix_GetError());
			}
		}

		inline ~mixer_s() noexcept {
			sdl::mixer::close();
		}

		static inline bool is_initialized() noexcept { return initialized; }

		static inline bool is_valid(i32 channel) noexcept { return between<i32>(channel, min_channel, max_channel); }

		static inline bool is_reserved(i32 channel) noexcept {
			if (!is_valid(channel)) {
				return true;
			}

			return channels[channel];
		}

		static inline i32 reserve_any() noexcept {
			if (!is_initialized()) {
				initialize();
			}

			if (channels.all()) {
				return -1;
			}

			for (i32 i{ min_channel }; i < num_channels; ++i) {
				if (!channels[i]) {
					return i;
				}
			}

			return -1;
		}

		static inline bool reserve_channel(i32 channel) noexcept {
			if (is_reserved(channel)) {
				return false;
			}

			channels[channel] = true;

			return true;
		}

		static inline void free_channel(i32 channel) noexcept {
			if (!is_valid(channel)) {
				return;
			}

			channels[channel] = false;
		}

		static inline void pause() noexcept { sdl::mixer::pause(); }

		static inline void pause(i32 channel) noexcept { sdl::mixer::pause(channel); }

		static inline void resume() noexcept { sdl::mixer::resume(); }

		static inline void resume(i32 channel) noexcept { sdl::mixer::resume(channel); }

		static inline void halt() noexcept { sdl::mixer::halt(); }

		static inline void halt(i32 channel) noexcept { sdl::mixer::halt(channel); }
	};
} // namespace bleak
