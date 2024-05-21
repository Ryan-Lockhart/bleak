#pragma once

#include "typedef.hpp"

#include "input.hpp"
#include <bitset>

#include <SDL.h>

#include "point.hpp"

namespace Bleakdepth {
	struct button_t {
	  private:
		i32 value;

	  public:
	  	static const button_t Unknown;

		static const button_t Left;
		static const button_t Middle;
		static const button_t Right;

		static const button_t SideOne;
		static const button_t SideTwo;

		static const button_t Start;
		static const button_t End;

		static const usize Count;

		constexpr inline button_t() noexcept = delete;

		constexpr inline button_t(i32 value) noexcept : value { value } {}

		constexpr inline operator u8() const noexcept { return value; }

		constexpr inline bool operator==(button_t other) const noexcept { return value == other.value; }

		constexpr inline bool operator!=(button_t other) const noexcept { return value != other.value; }

		constexpr inline operator std::string() const;
	};

	constexpr const button_t button_t::Unknown { 0 };

	constexpr const button_t button_t::Left { SDL_BUTTON_LEFT };
	constexpr const button_t button_t::Middle { SDL_BUTTON_MIDDLE };
	constexpr const button_t button_t::Right { SDL_BUTTON_RIGHT };

	constexpr const button_t button_t::SideOne { SDL_BUTTON_X1 };
	constexpr const button_t button_t::SideTwo { SDL_BUTTON_X2 };

	constexpr const button_t button_t::Start = button_t::Left;
	constexpr const button_t button_t::End = button_t::SideTwo;

	constexpr const usize button_t::Count = button_t::SideTwo + 1;

	constexpr button_t::operator std::string() const {
		switch (value) {
		case button_t::Left:
			return "Left";
		case button_t::Middle:
			return "Middle";
		case button_t::Right:
			return "Right";

		case button_t::SideOne:
			return "SideOne";
		case button_t::SideTwo:
			return "SideTwo";

		default:
			return "Unknown";
		}
	}

	struct mouse {
	  private:
		static inline std::bitset<button_t::Count> currentState;
		static inline std::bitset<button_t::Count> previousState;

		static inline point_t<i32> currentPosition;
		static inline point_t<i32> previousPosition;

		static inline point_t<f32> currentScroll;
		static inline point_t<f32> previousScroll;

		static inline bool init;

	  public:
		static inline bool initialized() { return mouse::init; }

		static inline void initialize() {
			mouse::currentState.reset();
			mouse::previousState.reset();

			init = true;
		}

		static inline void update() {
			u32 state = SDL_GetMouseState(&mouse::currentPosition.x, &mouse::currentPosition.y);

			for (usize i { button_t::Start }; i <= button_t::End; ++i) {
				mouse::previousState[i] = mouse::currentState[i];
				mouse::currentState[i] = state & SDL_BUTTON(i);
			}
		}

		static inline void update(cref<point_t<i32>> position) {
			mouse::previousPosition = mouse::currentPosition;
			mouse::currentPosition = position;
		}

		static inline void update(cref<point_t<f32>> scroll) {
			mouse::previousScroll = mouse::currentScroll;
			mouse::currentScroll = scroll;
		}

		static inline input_state_t operator[](button_t button) {
			if (previousState[button]) {
				return currentState[button] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return currentState[button] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline input_state_t operator[](int button) {
			if (previousState[button]) {
				return currentState[button] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return currentState[button] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline bool IsButtonPressed(button_t button);
		static inline bool IsButtonPressed(int button);

		static inline bool IsButtonReleased(button_t button);
		static inline bool IsButtonReleased(int button);

		static inline bool IsButtonDown(button_t button);
		static inline bool IsButtonDown(int button);

		static inline bool IsButtonUp(button_t button);
		static inline bool IsButtonUp(int button);

		static inline bool AnyButtonPressed() {
			for (int i { button_t::Start }; i <= button_t::End; ++i) {
				if (mouse::IsButtonPressed(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyButtonReleased() {
			for (int i { button_t::Start }; i <= button_t::End; ++i) {
				if (mouse::IsButtonReleased(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyButtonDown() {
			for (int i { button_t::Start }; i <= button_t::End; ++i) {
				if (mouse::IsButtonDown(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool AnyButtonUp() {
			for (int i { button_t::Start }; i <= button_t::End; ++i) {
				if (mouse::IsButtonUp(i)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool AreButtonsPressed(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!mouse::IsButtonPressed(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool AreButtonsReleased(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!mouse::IsButtonReleased(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool AreButtonsDown(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!mouse::IsButtonDown(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool AreButtonsUp(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!mouse::IsButtonUp(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool AnyButtonsPressed(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (mouse::IsButtonPressed(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool AnyButtonsReleased(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (mouse::IsButtonReleased(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool AnyButtonsDown(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (mouse::IsButtonDown(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool AnyButtonsUp(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (mouse::IsButtonUp(button)) {
					return true;
				}
			}

			return false;
		}

		static inline point_t<i32> GetPosition() { return mouse::currentPosition; }

		static inline point_t<f32> GetScroll() { return mouse::currentScroll; }

		static inline void ShowCursor() { SDL_ShowCursor(SDL_ENABLE); }

		static inline void HideCursor() { SDL_ShowCursor(SDL_DISABLE); }
	};

	static inline mouse Mouse;

	inline bool mouse::IsButtonPressed(button_t button) { return Mouse[button] == input_state_t::Pressed; }

	inline bool mouse::IsButtonPressed(int button) { return Mouse[button] == input_state_t::Pressed; }

	inline bool mouse::IsButtonReleased(button_t button) { return Mouse[button] == input_state_t::Released; }

	inline bool mouse::IsButtonReleased(int button) { return Mouse[button] == input_state_t::Released; }

	inline bool mouse::IsButtonDown(button_t button) { return Mouse[button] == input_state_t::Down; }

	inline bool mouse::IsButtonDown(int button) { return Mouse[button] == input_state_t::Down; }

	inline bool mouse::IsButtonUp(button_t button) { return Mouse[button] == input_state_t::Up; }

	inline bool mouse::IsButtonUp(int button) { return Mouse[button] == input_state_t::Up; }
} // namespace Bleakdepth
