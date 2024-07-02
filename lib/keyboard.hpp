#pragma once

#include "typedef.hpp"

#include "constants/keys.hpp"
#include "input.hpp"

#include <bitset>

namespace Bleakdepth {
	struct Keyboard {
	  private:
		static inline std::bitset<Keys::Count> current_state;
		static inline std::bitset<Keys::Count> previous_state;

		static inline bool initialized;

	  public:
		static inline bool is_initialized() { return initialized; }

		static inline void initialize() {
			if (initialized) {
				return;
			}

			current_state.reset();
			previous_state.reset();

			initialized = true;
		}

		static inline void terminate() {
			if (!initialized) {
				return;
			}

			// no cleanup required

			initialized = false;
		}

		static inline void update() {
			cptr<u8> state = SDL_GetKeyboardState(nullptr);

			for (usize i { Keys::First }; i <= Keys::Last; ++i) {
				previous_state[i] = current_state[i];
				current_state[i] = state[i];
			}
		}

		static inline input_state_t at(Keys::key_t key) {
			if (previous_state[key]) {
				return current_state[key] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return current_state[key] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline input_state_t at(int key) {
			if (previous_state[key]) {
				return current_state[key] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return current_state[key] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline bool is_key_pressed(Keys::key_t key);
		static inline bool is_key_pressed(int key);

		static inline bool is_key_released(Keys::key_t key);
		static inline bool is_key_released(int key);

		static inline bool is_key_down(Keys::key_t key);
		static inline bool is_key_down(int key);

		static inline bool is_key_up(Keys::key_t key);
		static inline bool is_key_up(int key);

		static inline bool any_key_pressed() {
			for (int i { Keys::First }; i <= Keys::Last; ++i) {
				if (is_key_pressed(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_key_released() {
			for (int i { Keys::First }; i <= Keys::Last; ++i) {
				if (is_key_released(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_key_down() {
			for (int i { Keys::First }; i <= Keys::Last; ++i) {
				if (is_key_down(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_key_up() {
			for (int i { Keys::First }; i <= Keys::Last; ++i) {
				if (is_key_up(i)) {
					return true;
				}
			}

			return false;
		}

		// returns true if all keys in the collection are pressed
		template<typename... K, typename = Keys::key_t> static inline bool are_keys_pressed(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (!is_key_pressed(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> static inline bool are_keys_pressed(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (!is_key_pressed(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are released
		template<typename... K, typename = Keys::key_t> static inline bool are_keys_released(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (!is_key_released(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> static inline bool are_keys_released(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (!is_key_released(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are down
		template<typename... K, typename = Keys::key_t> static inline bool are_keys_down(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (!is_key_down(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> static inline bool are_keys_down(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (!is_key_down(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are up
		template<typename... K, typename = Keys::key_t> static inline bool are_keys_up(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (!is_key_up(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> static inline bool are_keys_up(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (!is_key_up(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if any key in the collection is pressed
		template<typename... K, typename = Keys::key_t> static inline bool any_keys_pressed(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (is_key_pressed(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> static inline bool any_keys_pressed(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (is_key_pressed(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is released
		template<typename... K, typename = Keys::key_t> static inline bool any_keys_released(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (is_key_released(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> static inline bool any_keys_released(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (is_key_released(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is down
		template<typename... K, typename = Keys::key_t> static inline bool any_keys_down(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (is_key_down(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> inline bool any_keys_down(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (is_key_down(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is up
		template<typename... K, typename = Keys::key_t> static inline bool AnyKeysUp(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (is_key_up(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> static inline bool any_keys_up(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (is_key_up(key)) {
					return true;
				}
			}

			return false;
		}
	};

	inline bool Keyboard::is_key_pressed(Keys::key_t key) { return Keyboard::at(key) == input_state_t::Pressed; }

	inline bool Keyboard::is_key_pressed(int key) { return Keyboard::at(key) == input_state_t::Pressed; }

	inline bool Keyboard::is_key_released(Keys::key_t key) { return Keyboard::at(key) == input_state_t::Released; }

	inline bool Keyboard::is_key_released(int key) { return Keyboard::at(key) == input_state_t::Released; }

	inline bool Keyboard::is_key_down(Keys::key_t key) { return Keyboard::at(key) == input_state_t::Down; }

	inline bool Keyboard::is_key_down(int key) { return Keyboard::at(key) == input_state_t::Down; }

	inline bool Keyboard::is_key_up(Keys::key_t key) { return Keyboard::at(key) == input_state_t::Up; }

	inline bool Keyboard::is_key_up(int key) { return Keyboard::at(key) == input_state_t::Up; }
} // namespace Bleakdepth
