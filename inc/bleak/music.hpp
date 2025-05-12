#pragma once

#include <bleak/typedef.hpp>

#include <cassert>

#include <SDL_mixer.h>

#include <bleak/mixer.hpp>

namespace bleak {
	namespace sdl {
		using music_t = Mix_Music;

		namespace music {
			static inline ptr<sdl::music_t> load(cstr path) noexcept { return Mix_LoadMUS(path); }

			static inline void free(ref<ptr<sdl::music_t>> music) noexcept {
				if (music == nullptr) {
					return;
				}

				Mix_FreeMusic(music);
				music = nullptr;
			}

			static inline bool is_playing() noexcept { return Mix_PlayingMusic(); }

			static inline bool is_paused() noexcept { return Mix_PausedMusic(); }

			static inline bool is_fading() noexcept { return Mix_FadingMusic(); }

			static inline i32 get_volume() noexcept { return Mix_VolumeMusic(-1); }

			static inline void set_volume(i32 volume) noexcept {
				assert(volume > -1);
				Mix_VolumeMusic(volume);
			}

			static inline void pause() noexcept { Mix_PauseMusic(); }

			static inline void resume() noexcept { Mix_ResumeMusic(); }

			static inline void halt() noexcept { Mix_HaltMusic(); }

			static inline void play(ptr<music_t> music) noexcept {
				assert(music);
				Mix_PlayMusic(music, 0);
			}

			static inline void loop(ptr<music_t> music) noexcept {
				assert(music);
				Mix_PlayMusic(music, -1);
			}

			static inline void loop(ptr<music_t> music, i32 loops) noexcept {
				assert(music);
				assert(loops > 0);
				Mix_PlayMusic(music, loops);
			}

			static inline void fade_in(ptr<music_t> music, i32 ms) noexcept {
				assert(music);
				Mix_FadeInMusic(music, 0, ms);
			}

			static inline void fade_out(i32 ms) noexcept { Mix_FadeOutMusic(ms); }

			static inline void loop_in(ptr<music_t> music, i32 ms) noexcept {
				assert(music);
				Mix_FadeInMusic(music, -1, ms);
			}

			static inline void loop_in(ptr<music_t> music, i32 loops, i32 ms) noexcept {
				assert(music);
				assert(loops > 0);
				Mix_FadeInMusic(music, loops, ms);
			}
		} // namespace music
	} // namespace sdl

	struct music_t {
	  private:
		ptr<sdl::music_t> music;

	  public:
		inline music_t(ptr<sdl::music_t> music) noexcept : music{ music } { assert(music); }

		inline music_t(cstr path) noexcept : music{ sdl::music::load(path) } { assert(music); }

		inline ~music_t() noexcept { sdl::music::free(music); }

		inline void pause() noexcept { sdl::music::pause(); }

		inline void resume() noexcept { sdl::music::resume(); }

		inline void halt() noexcept { sdl::music::halt(); }

		inline void play() noexcept { sdl::music::play(music); }

		inline void play(i32 volume) noexcept {
			sdl::music::set_volume(volume);
			sdl::music::play(music);
		}

		inline void loop() noexcept { sdl::music::loop(music, -1); }

		inline void loop(i32 loops) noexcept { sdl::music::loop(music, loops); }

		inline void loop(i32 loops, i32 volume) noexcept {
			sdl::music::set_volume(volume);
			sdl::music::loop(music, loops);
		}

		inline void fade_in(i32 ms) noexcept { sdl::music::fade_in(music, ms); }

		inline void fade_out(i32 ms) noexcept { sdl::music::fade_out(ms); }

		inline void loop_in(i32 ms) noexcept { sdl::music::loop_in(music, ms); }

		inline void loop_in(i32 loops, i32 ms) noexcept { sdl::music::loop_in(music, loops, ms); }
	};
} // namespace bleak
