#pragma once

#include <bleak/typedef.hpp>

#include <bitset>

#include <bleak/input.hpp>

#include <bleak/constants/keys.hpp>
#include <bleak/constants/bindings.hpp>

namespace bleak {
	struct Keyboard {
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

		static inline input_state_t at(sdl::key key) noexcept {
			if (previous_state[key]) {
				return current_state[key] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return current_state[key] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline input_state_t at(int key) noexcept {
			if (previous_state[key]) {
				return current_state[key] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return current_state[key] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline bool is_key_pressed(sdl::key key) noexcept;
		static inline bool is_key_pressed(int key) noexcept;

		static inline bool is_key_released(sdl::key key) noexcept;
		static inline bool is_key_released(int key) noexcept;

		static inline bool is_key_down(sdl::key key) noexcept;
		static inline bool is_key_down(int key) noexcept;

		static inline bool is_key_up(sdl::key key) noexcept;
		static inline bool is_key_up(int key) noexcept;

		static inline bool any_key_pressed() noexcept {
			for (int i { keys::First }; i <= keys::Last; ++i) {
				if (is_key_pressed(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_key_released() noexcept {
			for (int i { keys::First }; i <= keys::Last; ++i) {
				if (is_key_released(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_key_down() noexcept {
			for (int i { keys::First }; i <= keys::Last; ++i) {
				if (is_key_down(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_key_up() noexcept {
			for (int i { keys::First }; i <= keys::Last; ++i) {
				if (is_key_up(i)) {
					return true;
				}
			}

			return false;
		}

		// returns true if all keys in the collection are pressed
		template<typename... K, typename = sdl::key> static inline bool are_keys_pressed(K... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (!is_key_pressed(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> static inline bool are_keys_pressed(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (!is_key_pressed(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are released
		template<typename... K, typename = sdl::key> static inline bool are_keys_released(K... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (!is_key_released(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> static inline bool are_keys_released(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (!is_key_released(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are down
		template<typename... K, typename = sdl::key> static inline bool are_keys_down(K... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (!is_key_down(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> static inline bool are_keys_down(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (!is_key_down(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are up
		template<typename... K, typename = sdl::key> static inline bool are_keys_up(K... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (!is_key_up(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> static inline bool are_keys_up(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (!is_key_up(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if any key in the collection is pressed
		template<typename... K, typename = sdl::key> requires is_plurary<K...>::value static inline bool any_keys_pressed(K... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (is_key_pressed(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> static inline bool any_keys_pressed(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (is_key_pressed(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is released
		template<typename... K, typename = sdl::key> static inline bool any_keys_released(K... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (is_key_released(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> static inline bool any_keys_released(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (is_key_released(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is down
		template<typename... K, typename = sdl::key> static inline bool any_keys_down(K... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (is_key_down(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> static inline bool any_keys_down(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (is_key_down(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is up
		template<typename... K, typename = sdl::key> static inline bool any_keys_up(K... keys) noexcept {
			for (sdl::key key : { keys... }) {
				if (is_key_up(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> static inline bool any_keys_up(cref<bindings::key_collection_t<Size>> keys) noexcept {
			for (sdl::key key : keys) {
				if (is_key_up(key)) {
					return true;
				}
			}

			return false;
		}
	};

	inline bool Keyboard::is_key_pressed(sdl::key key) noexcept { return Keyboard::at(key) == input_state_t::Pressed; }

	inline bool Keyboard::is_key_pressed(int key) noexcept { return Keyboard::at(key) == input_state_t::Pressed; }

	inline bool Keyboard::is_key_released(sdl::key key) noexcept { return Keyboard::at(key) == input_state_t::Released; }

	inline bool Keyboard::is_key_released(int key) noexcept { return Keyboard::at(key) == input_state_t::Released; }

	inline bool Keyboard::is_key_down(sdl::key key) noexcept { return Keyboard::at(key) == input_state_t::Down; }

	inline bool Keyboard::is_key_down(int key) noexcept { return Keyboard::at(key) == input_state_t::Down; }

	inline bool Keyboard::is_key_up(sdl::key key) noexcept { return Keyboard::at(key) == input_state_t::Up; }

	inline bool Keyboard::is_key_up(int key) noexcept { return Keyboard::at(key) == input_state_t::Up; }
} // namespace Bleakdepth
