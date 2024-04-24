#pragma once

#include "typedef.hpp"

#include <bitset>

#include <SDL.h>

namespace Bleakdepth {
	struct keyboard {
	  public:
		using key_t = SDL_Scancode;

		static constexpr key_t KEY_START = SDL_SCANCODE_UNKNOWN;
		static constexpr key_t KEY_END = SDL_NUM_SCANCODES;

		enum class key_state_t { Pressed, Released, Down, Up };

	  private:
		static inline std::bitset<KEY_END> currentState;
		static inline std::bitset<KEY_END> previousState;

	  public:
		static constexpr void initialize() {
			currentState.reset();
			previousState.reset();
		}

		static constexpr void update(key_t key, bool state) {
			previousState[key] = currentState[key];
			currentState[key] = state;
		}

		template<typename... Keys, typename = std::pair<key_t, bool>> static constexpr void update(Keys... keyStates) {
			for (const auto& [key, state] : { keyStates... }) {
				previousState[key] = currentState[key];
				currentState[key] = state;
			}
		}

		static constexpr void update(cptr<key_t> keys, cptr<bool> states, usize count) {
			for (usize i { 0 }; i < count; ++i) {
				previousState[keys[i]] = currentState[keys[i]];
				currentState[keys[i]] = states[i];
			}
		}

		static constexpr key_state_t operator[](key_t key) {
			return previousState[key] ? currentState[key] ? key_state_t::Pressed : key_state_t::Up :
				   currentState[key]  ? key_state_t::Down :
										key_state_t::Released;
		}

		static constexpr key_state_t operator[](int key) {
			return previousState[key] ? currentState[key] ? key_state_t::Pressed : key_state_t::Up :
				   currentState[key]  ? key_state_t::Down :
										key_state_t::Released;
		}

		static constexpr bool IsKeyPressed(key_t key);
		static constexpr bool IsKeyPressed(int key);

		static constexpr bool IsKeyReleased(key_t key);
		static constexpr bool IsKeyReleased(int key);

		static constexpr bool IsKeyDown(key_t key);
		static constexpr bool IsKeyDown(int key);

		static constexpr bool IsKeyUp(key_t key);
		static constexpr bool IsKeyUp(int key);

		static inline bool AnyKeyPressed() {
			for (int i { KEY_START }; i < KEY_END; ++i) {
				if (IsKeyPressed(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyReleased() {
			for (int i { KEY_START }; i < KEY_END; ++i) {
				if (IsKeyReleased(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyDown() {
			for (int i { KEY_START }; i < KEY_END; ++i) {
				if (IsKeyDown(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyKeyUp() {
			for (int i { KEY_START }; i < KEY_END; ++i) {
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

	constexpr inline bool keyboard::IsKeyPressed(key_t key) { return Keyboard[key] == key_state_t::Pressed; }

	constexpr inline bool keyboard::IsKeyPressed(int key) { return Keyboard[key] == key_state_t::Pressed; }

	constexpr inline bool keyboard::IsKeyReleased(key_t key) { return Keyboard[key] == key_state_t::Released; }

	constexpr inline bool keyboard::IsKeyReleased(int key) { return Keyboard[key] == key_state_t::Released; }

	constexpr inline bool keyboard::IsKeyDown(key_t key) { return Keyboard[key] == key_state_t::Down; }

	constexpr inline bool keyboard::IsKeyDown(int key) { return Keyboard[key] == key_state_t::Down; }

	constexpr inline bool keyboard::IsKeyUp(key_t key) { return Keyboard[key] == key_state_t::Up; }

	constexpr inline bool keyboard::IsKeyUp(int key) { return Keyboard[key] == key_state_t::Up; }
} // namespace Bleakdepth
