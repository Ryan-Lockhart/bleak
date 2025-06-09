#pragma once

#include <bleak/typedef.hpp>

#include <bitset>

#include <bleak/input.hpp>

#include <bleak/constants/keys.hpp>
#include <bleak/constants/bindings.hpp>

namespace bleak {
	struct keyboard_s {
	  private:
		static inline std::bitset<keys::Count> current_state;
		static inline std::bitset<keys::Count> previous_state;

		static inline bool initialized;

	  public:
		static inline bool is_initialized() noexcept { return initialized; }

		static inline void initialize() {
			if (initialized) {
				return;
			}

			current_state.reset();
			previous_state.reset();

			initialized = true;
		}

		static inline void terminate() noexcept {
			if (!initialized) {
				return;
			}

			// no cleanup required

			initialized = false;
		}

		static inline void update() noexcept {
			cptr<u8> state = SDL_GetKeyboardState(nullptr);

			for (usize i { keys::First }; i <= keys::Last; ++i) {
				previous_state[i] = current_state[i];
				current_state[i] = state[i];
			}
		}

		static inline input_e at(sdl::key key) noexcept {
			if (previous_state[key]) {
				return current_state[key] ? input_e::Pressed : input_e::Up;
			} else {
				return current_state[key] ? input_e::Down : input_e::Released;
			}
		}

		static inline input_e at(int key) noexcept {
			if (previous_state[key]) {
				return current_state[key] ? input_e::Pressed : input_e::Up;
			} else {
				return current_state[key] ? input_e::Down : input_e::Released;
			}
		}

		template<input_e Input> static inline bool is_key(sdl::key key) noexcept { return at(key) == Input; }

		template<input_e Input> static inline bool is_key(int key) noexcept { return at(key) == Input; }

		template<input_e Input> static inline bool any_key() noexcept {
			for (int i { keys::First }; i <= keys::Last; ++i) {
				if (is_key<Input>(i)) {
					return true;
				}
			}

			return false;
		}

		template<input_e Input, Key... Keys> static inline bool are_keys(Keys... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (!if_key<Input>(key)) {
					return false;
				}
			}

			return true;
		}

		template<input_e Input, usize Size> static inline bool are_keys(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (!is_key<Input>(key)) {
					return false;
				}
			}

			return true;
		}

		template<input_e Input, Key... Keys> requires is_plurary<Keys...>::value static inline bool any_keys(Keys... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (is_key<Input>(key)) {
					return true;
				}
			}

			return false;
		}

		template<input_e Input, usize Size> static inline bool any_keys(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (is_key<Input>(key)) {
					return true;
				}
			}

			return false;
		}
	};
} // namespace bleak
