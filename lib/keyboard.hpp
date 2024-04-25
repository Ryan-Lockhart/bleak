#pragma once

#include "input.hpp"
#include "typedef.hpp"

#include <bitset>

#include <SDL.h>

namespace Bleakdepth {
	struct keyboard {
	  public:
		using key_t = SDL_Scancode;

		static constexpr key_t KEY_START = SDL_SCANCODE_UNKNOWN;
		static constexpr key_t KEY_END = SDL_SCANCODE_ENDCALL;
		static constexpr usize KEY_COUNT = SDL_NUM_SCANCODES;

	  private:
		static inline std::bitset<KEY_COUNT> currentState;
		static inline std::bitset<KEY_COUNT> previousState;

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

			for (usize i { KEY_START }; i <= KEY_END; ++i) {
				previousState[i] = currentState[i];
				currentState[i] = state[i];
			}
		}

		static inline input_state_t operator[](key_t key) {
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

		static inline bool IsKeyPressed(key_t key);
		static inline bool IsKeyPressed(int key);

		static inline bool IsKeyReleased(key_t key);
		static inline bool IsKeyReleased(int key);

		static inline bool IsKeyDown(key_t key);
		static inline bool IsKeyDown(int key);

		static inline bool IsKeyUp(key_t key);
		static inline bool IsKeyUp(int key);

		static inline bool AnyKeyPressed() {
			for (int i { KEY_START }; i <= KEY_END; ++i) {
				if (IsKeyPressed(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyReleased() {
			for (int i { KEY_START }; i <= KEY_END; ++i) {
				if (IsKeyReleased(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyDown() {
			for (int i { KEY_START }; i <= KEY_END; ++i) {
				if (IsKeyDown(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyUp() {
			for (int i { KEY_START }; i <= KEY_END; ++i) {
				if (IsKeyUp(i)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Keys, typename = key_t> static inline bool AreKeysPressed(Keys... keys) {
			for (key_t key : { keys... }) {
				if (!IsKeyPressed(key)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Keys, typename = key_t> static inline bool AreKeysReleased(Keys... keys) {
			for (key_t key : { keys... }) {
				if (!IsKeyReleased(key)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Keys, typename = key_t> static inline bool AreKeysDown(Keys... keys) {
			for (key_t key : { keys... }) {
				if (!IsKeyDown(key)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Keys, typename = key_t> static inline bool AreKeysUp(Keys... keys) {
			for (key_t key : { keys... }) {
				if (!IsKeyUp(key)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Keys, typename = key_t> static inline bool AnyKeysPressed(Keys... keys) {
			for (key_t key : { keys... }) {
				if (IsKeyPressed(key)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Keys, typename = key_t> static inline bool AnyKeysReleased(Keys... keys) {
			for (key_t key : { keys... }) {
				if (IsKeyReleased(key)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Keys, typename = key_t> static inline bool AnyKeysDown(Keys... keys) {
			for (key_t key : { keys... }) {
				if (IsKeyDown(key)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Keys, typename = key_t> static inline bool AnyKeysUp(Keys... keys) {
			for (key_t key : { keys... }) {
				if (IsKeyUp(key)) {
					return true;
				}
			}

			return false;
		}
	};

	static inline keyboard Keyboard;

	inline bool keyboard::IsKeyPressed(key_t key) { return Keyboard[key] == input_state_t::Pressed; }

	inline bool keyboard::IsKeyPressed(int key) { return Keyboard[key] == input_state_t::Pressed; }

	inline bool keyboard::IsKeyReleased(key_t key) { return Keyboard[key] == input_state_t::Released; }

	inline bool keyboard::IsKeyReleased(int key) { return Keyboard[key] == input_state_t::Released; }

	inline bool keyboard::IsKeyDown(key_t key) { return Keyboard[key] == input_state_t::Down; }

	inline bool keyboard::IsKeyDown(int key) { return Keyboard[key] == input_state_t::Down; }

	inline bool keyboard::IsKeyUp(key_t key) { return Keyboard[key] == input_state_t::Up; }

	inline bool keyboard::IsKeyUp(int key) { return Keyboard[key] == input_state_t::Up; }
} // namespace Bleakdepth
