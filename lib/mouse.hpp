#pragma once

#include "typedef.hpp"

#include <bitset>

#include <SDL.h>

#include "point.hpp"

namespace Bleakdepth {
	struct mouse_t {
	  public:
		enum button_t : u16 {
			MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT,
			MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
			MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT,
			MOUSE_BUTTON_X1 = SDL_BUTTON_X1,
			MOUSE_BUTTON_X2 = SDL_BUTTON_X2
		};

		static constexpr button_t MOUSE_BUTTON_START = MOUSE_BUTTON_LEFT;
		static constexpr button_t MOUSE_BUTTON_END = MOUSE_BUTTON_X2;

		enum class button_state_t { Pressed, Released, Down, Up };

	  private:
		static inline std::bitset<MOUSE_BUTTON_END> currentState;
		static inline std::bitset<MOUSE_BUTTON_END> previousState;

	  public:
		static constexpr void initialize() {
			currentState.reset();
			previousState.reset();
		}

		static constexpr void update(button_t button, bool state) {
			previousState[button] = currentState[button];
			currentState[button] = state;
		}

		template<typename... Buttons, typename = std::pair<button_t, bool>>
		static constexpr void update(Buttons... buttonStates) {
			for (const auto& [button, state] : { buttonStates... }) {
				previousState[button] = currentState[button];
				currentState[button] = state;
			}
		}

		static constexpr void update(cptr<button_t> buttons, cptr<bool> states, usize count) {
			for (usize i { 0 }; i < count; ++i) {
				previousState[buttons[i]] = currentState[buttons[i]];
				currentState[buttons[i]] = states[i];
			}
		}

		static constexpr button_state_t operator[](button_t button) {
			return previousState[button] ? currentState[button] ? button_state_t::Pressed : button_state_t::Up :
				   currentState[button]	 ? button_state_t::Down :
										   button_state_t::Released;
		}

		static constexpr button_state_t operator[](int button) {
			return previousState[button] ? currentState[button] ? button_state_t::Pressed : button_state_t::Up :
				   currentState[button]	 ? button_state_t::Down :
										   button_state_t::Released;
		}

		static constexpr bool IsButtonPressed(button_t button);
		static constexpr bool IsButtonPressed(int button);

		static constexpr bool IsButtonReleased(button_t button);
		static constexpr bool IsButtonReleased(int button);

		static constexpr bool IsButtonDown(button_t button);
		static constexpr bool IsButtonDown(int button);

		static constexpr bool IsButtonUp(button_t button);
		static constexpr bool IsButtonUp(int button);

		static constexpr bool AnyButtonPressed() {
			for (int i { MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i) {
				if (IsButtonPressed(i)) {
					return true;
				}
			}

			return false;
		}

		static constexpr bool AnyButtonReleased() {
			for (int i { MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i) {
				if (IsButtonReleased(i)) {
					return true;
				}
			}

			return false;
		}

		static constexpr bool AnyButtonDown() {
			for (int i { MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i) {
				if (IsButtonDown(i)) {
					return true;
				}
			}

			return false;
		}

		static constexpr bool AnyButtonUp() {
			for (int i { MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i) {
				if (IsButtonUp(i)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static constexpr bool AreButtonsPressed(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!IsButtonPressed(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t>
		static constexpr bool AreButtonsReleased(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!IsButtonReleased(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static constexpr bool AreButtonsDown(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!IsButtonDown(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static constexpr bool AreButtonsUp(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!IsButtonUp(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static constexpr bool AnyButtonsPressed(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (IsButtonPressed(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t>
		static constexpr bool AnyButtonsReleased(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (IsButtonReleased(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static constexpr bool AnyButtonsDown(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (IsButtonDown(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static constexpr bool AnyButtonsUp(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (IsButtonUp(button)) {
					return true;
				}
			}

			return false;
		}

		static inline point_t<i32> GetPosition() {
			i32 x, y;
			SDL_GetMouseState(&x, &y);
			return { x, y };
		}

		static inline void ShowCursor() { SDL_ShowCursor(SDL_ENABLE); }

		static inline void HideCursor() { SDL_ShowCursor(SDL_DISABLE); }
	};

	static inline mouse_t Mouse;

	constexpr inline bool mouse_t::IsButtonPressed(button_t button) { return Mouse[button] == button_state_t::Pressed; }

	constexpr inline bool mouse_t::IsButtonPressed(int button) { return Mouse[button] == button_state_t::Pressed; }

	constexpr inline bool mouse_t::IsButtonReleased(button_t button) {
		return Mouse[button] == button_state_t::Released;
	}

	constexpr inline bool mouse_t::IsButtonReleased(int button) { return Mouse[button] == button_state_t::Released; }

	constexpr inline bool mouse_t::IsButtonDown(button_t button) { return Mouse[button] == button_state_t::Down; }

	constexpr inline bool mouse_t::IsButtonDown(int button) { return Mouse[button] == button_state_t::Down; }

	constexpr inline bool mouse_t::IsButtonUp(button_t button) { return Mouse[button] == button_state_t::Up; }

	constexpr inline bool mouse_t::IsButtonUp(int button) { return Mouse[button] == button_state_t::Up; }
} // namespace Bleakdepth
