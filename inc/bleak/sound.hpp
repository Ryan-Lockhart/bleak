#pragma once

#include <bleak/typedef.hpp>

#include <cassert>
#include <chrono>
#include <future>
#include <thread>

#include <SDL_mixer.h>

#include <bleak/mixer.hpp>

namespace bleak {
	namespace sdl {
		using chunk_t = Mix_Chunk;

		namespace chunk {
            static inline ptr<chunk_t> load(cstr path) noexcept {
				if (!mixer_s::is_initialized()) {
					mixer_s::initialize();
				}

				return Mix_LoadWAV(path);
			}

            static inline void free(ref<ptr<chunk_t>> sound) noexcept {
				if (!mixer_s::is_initialized()) {
					mixer_s::initialize();
				}

                if (sound == nullptr) {
                    return;
                }

                Mix_FreeChunk(sound);
                sound = nullptr;
            }

			static inline void play(ptr<chunk_t> sound) noexcept { Mix_PlayChannel(-1, sound, 0); }

			static inline void play(ptr<chunk_t> sound, i32 channel) noexcept { Mix_PlayChannel(channel, sound, 0); }

			static inline void loop(ptr<chunk_t> sound) noexcept { Mix_PlayChannel(-1, sound, -1); }

			static inline void loop(ptr<chunk_t> sound, i32 loops, i32 channel) noexcept { Mix_PlayChannel(channel, sound, loops); }

			static inline void delay(ptr<chunk_t> sound, usize interval) noexcept {
				auto fut = std::async([&]() { std::this_thread::sleep_for(std::chrono::milliseconds{ interval }); play(sound); });
			}

			static inline void delay(ptr<chunk_t> sound, usize interval, i32 channel) noexcept {
				auto fut = std::async([&]() { std::this_thread::sleep_for(std::chrono::milliseconds{ interval }); play(sound, channel); });
			}
		} // namespace chunk
	} // namespace sdl

    struct sound_t {
      private:
        ptr<sdl::chunk_t> sound;
		i32 reserved_channel;

      public:
        inline sound_t(ptr<sdl::chunk_t> sound) noexcept : sound{ sound }, reserved_channel{ mixer_s::reserve_any() } {}
        inline sound_t(ptr<sdl::chunk_t> sound, i32 channel) noexcept : sound{ sound }, reserved_channel{ channel } {}

        inline sound_t(cstr path) noexcept : sound{ sdl::chunk::load(path) }, reserved_channel{ mixer_s::reserve_any() } { assert(sound); }
        inline sound_t(cstr path, i32 channel) noexcept : sound{ sdl::chunk::load(path) }, reserved_channel{ channel } { assert(sound); }

		inline sound_t(cref<sound_t> other) noexcept = delete;
		inline sound_t(rval<sound_t> other) noexcept : sound{ std::move(other.sound) }, reserved_channel{ other.reserved_channel } {
			other.reserved_channel = -1;
			other.sound = nullptr;
		}

		ref<sound_t> inline operator=(cref<sound_t> other) noexcept = delete;
		ref<sound_t> inline operator=(rval<sound_t> other) noexcept {
			reserved_channel = std::move(other.reserved_channel);
			other.reserved_channel = -1;
			
			sound = std::move(other.sound);
			other.sound = nullptr;

			return *this;
		}

        inline ~sound_t() noexcept {
			mixer_s::free_channel(reserved_channel);
			sdl::chunk::free(sound);
		}

		inline i32 get_channel() noexcept { return reserved_channel; }

		inline bool register_channel(i32 channel) noexcept {
			if (!mixer_s::reserve_channel(channel)) {
				return false;
			}

			this->reserved_channel = channel;

			return true;
		}

		inline void unregister_channel() noexcept {
			mixer_s::free_channel(reserved_channel);
			reserved_channel = -1;
		}

        inline void play() noexcept { sdl::chunk::play(sound, reserved_channel); }

        inline void play(i32 channel) noexcept { sdl::chunk::play(sound, channel); }

        inline void loop() noexcept { sdl::chunk::loop(sound, -1, reserved_channel); }

        inline void loop(i32 loops) noexcept { sdl::chunk::loop(sound, loops, reserved_channel); }

        inline void loop(i32 loops, i32 channel) noexcept { sdl::chunk::loop(sound, loops, channel); }

        inline void delay(usize interval) noexcept { sdl::chunk::delay(sound, reserved_channel, interval); }

        inline void delay(usize interval, i32 channel) noexcept { sdl::chunk::delay(sound, channel, interval); }
    };
} // namespace bleak
