#pragma once

#include <bleak/typedef.hpp>

#include <cassert>

#include <SDL_mixer.h>

#include <bleak/mixer.hpp>

namespace bleak {
	namespace sdl {
		using chunk_t = Mix_Chunk;

		namespace chunk {
            static inline ptr<chunk_t> load(cstr path) noexcept { return Mix_LoadWAV(path); }

            static inline void free(ref<ptr<chunk_t>> sound) noexcept {
                if (sound == nullptr) {
                    return;
                }

                Mix_FreeChunk(sound);
                sound = nullptr;
            }

			static inline void play(ptr<chunk_t> sound) noexcept { Mix_PlayChannel(-1, sound, 0); }

			static inline void play(ptr<chunk_t> sound, i32 channel) noexcept { Mix_PlayChannel(channel, sound, 0); }

			static inline void loop(ptr<chunk_t> sound) noexcept { Mix_PlayChannel(-1, sound, -1); }

			static inline void loop(ptr<chunk_t> sound, i32 channel, i32 loops) noexcept { Mix_PlayChannel(channel, sound, loops); }
		} // namespace chunk
	} // namespace sdl

    struct sound_t {
      private:
        ptr<sdl::chunk_t> sound;

      public:
        inline sound_t(ptr<sdl::chunk_t> sound) noexcept : sound{ sound } {}

        inline sound_t(cstr path) noexcept : sound{ sdl::chunk::load(path) } { assert(sound); }

        inline ~sound_t() noexcept { sdl::chunk::free(sound); }

        inline void play() noexcept { sdl::chunk::play(sound); }

        inline void play(i32 channel) noexcept { sdl::chunk::play(sound, channel); }

        inline void loop() noexcept { sdl::chunk::loop(sound); }

        inline void loop(i32 channel, i32 loops) noexcept { sdl::chunk::loop(sound, channel, loops); }
    };
} // namespace bleak
