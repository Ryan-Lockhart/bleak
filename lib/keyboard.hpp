#pragma once

#include "typedef.hpp"

#include "constants/keys.hpp"
#include "input.hpp"

#include <bitset>

namespace Bleakdepth {
	struct keyboard {
	  private:
		static inline std::bitset<Keys::Count> currentState;
		static inline std::bitset<Keys::Count> previousState;

		static inline bool init;

	  public:
		static inline bool initialized() { return init; }

		static inline void initialize() {
			currentState.reset();
			previousState.reset();

			init = true;
		}

		static inline void update() {
			cptr<u8> state = SDL_GetKeyboardState(nullptr);

			for (usize i { Keys::First }; i <= Keys::Last; ++i) {
				previousState[i] = currentState[i];
				currentState[i] = state[i];
			}
		}

		static inline input_state_t operator[](Keys::key_t key) {
			if (previousState[key]) {
				return currentState[key] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return currentState[key] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline input_state_t operator[](int key) {
			if (previousState[key]) {
				return currentState[key] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return currentState[key] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline bool IsKeyPressed(Keys::key_t key);
		static inline bool IsKeyPressed(int key);

		static inline bool IsKeyReleased(Keys::key_t key);
		static inline bool IsKeyReleased(int key);

		static inline bool IsKeyDown(Keys::key_t key);
		static inline bool IsKeyDown(int key);

		static inline bool IsKeyUp(Keys::key_t key);
		static inline bool IsKeyUp(int key);

		static inline bool AnyKeyPressed() {
			for (int i { Keys::First }; i <= Keys::Last; ++i) {
				if (IsKeyPressed(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyReleased() {
			for (int i { Keys::First }; i <= Keys::Last; ++i) {
				if (IsKeyReleased(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyDown() {
			for (int i { Keys::First }; i <= Keys::Last; ++i) {
				if (IsKeyDown(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyUp() {
			for (int i { Keys::First }; i <= Keys::Last; ++i) {
				if (IsKeyUp(i)) {
					return true;
				}
			}

			return false;
		}

		// returns true if all keys in the collection are pressed
		template<typename... K, typename = Keys::key_t> static inline bool AreKeysPressed(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (!IsKeyPressed(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> inline bool AreKeysPressed(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (!IsKeyPressed(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are released
		template<typename... K, typename = Keys::key_t> static inline bool AreKeysReleased(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (!IsKeyReleased(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> inline bool AreKeysReleased(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (!IsKeyReleased(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are down
		template<typename... K, typename = Keys::key_t> static inline bool AreKeysDown(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (!IsKeyDown(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> inline bool AreKeysDown(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (!IsKeyDown(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if all keys in the collection are up
		template<typename... K, typename = Keys::key_t> static inline bool AreKeysUp(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (!IsKeyUp(key)) {
					return false;
				}
			}

			return true;
		}

		template<usize Size> inline bool AreKeysUp(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (!IsKeyUp(key)) {
					return false;
				}
			}

			return true;
		}

		// returns true if any key in the collection is pressed
		template<typename... K, typename = Keys::key_t> static inline bool AnyKeysPressed(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (IsKeyPressed(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> inline bool AnyKeysPressed(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (IsKeyPressed(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is released
		template<typename... K, typename = Keys::key_t> static inline bool AnyKeysReleased(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (IsKeyReleased(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> inline bool AnyKeysReleased(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (IsKeyReleased(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is down
		template<typename... K, typename = Keys::key_t> static inline bool AnyKeysDown(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (IsKeyDown(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> inline bool AnyKeysDown(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (IsKeyDown(key)) {
					return true;
				}
			}

			return false;
		}

		// returns true if any key in the collection is up
		template<typename... K, typename = Keys::key_t> static inline bool AnyKeysUp(K... keys) {
			for (Keys::key_t key : { keys... }) {
				if (IsKeyUp(key)) {
					return true;
				}
			}

			return false;
		}

		template<usize Size> inline bool AnyKeysUp(cref<std::array<Keys::key_t, Size>> keys) {
			for (Keys::key_t key : keys) {
				if (IsKeyUp(key)) {
					return true;
				}
			}

			return false;
		}
	};

	static inline keyboard Keyboard;

	inline bool keyboard::IsKeyPressed(Keys::key_t key) { return Keyboard[key] == input_state_t::Pressed; }

	inline bool keyboard::IsKeyPressed(int key) { return Keyboard[key] == input_state_t::Pressed; }

	inline bool keyboard::IsKeyReleased(Keys::key_t key) { return Keyboard[key] == input_state_t::Released; }

	inline bool keyboard::IsKeyReleased(int key) { return Keyboard[key] == input_state_t::Released; }

	inline bool keyboard::IsKeyDown(Keys::key_t key) { return Keyboard[key] == input_state_t::Down; }

	inline bool keyboard::IsKeyDown(int key) { return Keyboard[key] == input_state_t::Down; }

	inline bool keyboard::IsKeyUp(Keys::key_t key) { return Keyboard[key] == input_state_t::Up; }

	inline bool keyboard::IsKeyUp(int key) { return Keyboard[key] == input_state_t::Up; }
} // namespace Bleakdepth
