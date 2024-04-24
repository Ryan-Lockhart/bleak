#pragma once

#include "typedef.hpp"

#include <bitset>

#include <SDL.h>

#include "point.hpp"

struct Mouse
{
public:
	enum Button : u16
	{
		MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT,
		MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
		MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT,
		MOUSE_BUTTON_X1 = SDL_BUTTON_X1,
		MOUSE_BUTTON_X2 = SDL_BUTTON_X2
	};

	static constexpr Button MOUSE_BUTTON_START = MOUSE_BUTTON_LEFT;
	static constexpr Button MOUSE_BUTTON_END = MOUSE_BUTTON_X2;

	enum class ButtonState
	{
		Pressed,
		Released,
		Down,
		Up
	};

private:
	static std::bitset<MOUSE_BUTTON_END> currentState;
	static std::bitset<MOUSE_BUTTON_END> previousState;

public:

	static inline void update(Button button, bool state)
	{
		previousState[button] = currentState[button];
		currentState[button] = state;
	}

	template<typename... Buttons, typename = std::pair<Button, bool>>
	static inline void update(Buttons... buttonStates)
	{
		for (const auto& [button, state] : { buttonStates... })
		{
			previousState[button] = currentState[button];
			currentState[button] = state;
		}
	}

	static inline void update(cptr<Button> buttons, cptr<bool> states, usize count)
	{
		for (usize i{ 0 }; i < count; ++i)
		{
			previousState[buttons[i]] = currentState[buttons[i]];
			currentState[buttons[i]] = states[i];
		}
	}

	static inline ButtonState at(Button button) { return previousState[button] ? currentState[button] ? ButtonState::Pressed : ButtonState::Up : currentState[button] ? ButtonState::Down : ButtonState::Released; }
	static inline ButtonState at(int button) { return previousState[button] ? currentState[button] ? ButtonState::Pressed : ButtonState::Up : currentState[button] ? ButtonState::Down : ButtonState::Released; }

	static inline bool IsButtonPressed(Button button) { return Mouse::at(button) == ButtonState::Pressed; }
	static inline bool IsButtonPressed(int button) { return Mouse::at(button) == ButtonState::Pressed; }

	static inline bool IsButtonReleased(Button button) { return Mouse::at(button) == ButtonState::Released; }
	static inline bool IsButtonReleased(int button) { return Mouse::at(button) == ButtonState::Released; }

	static inline bool IsButtonDown(Button button) { return Mouse::at(button) == ButtonState::Down; }
	static inline bool IsButtonDown(int button) { return Mouse::at(button) == ButtonState::Down; }

	static inline bool IsButtonUp(Button button) { return Mouse::at(button) == ButtonState::Up; }
	static inline bool IsButtonUp(int button) { return Mouse::at(button) == ButtonState::Up; }

	static inline bool AnyButtonPressed()
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonPressed(i))
				return true;
		}

		return false;
	}

	static inline bool AnyButtonReleased()
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonReleased(i))
				return true;
		}

		return false;
	}

	static inline bool AnyButtonDown()
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonDown(i))
				return true;
		}

		return false;
	}

	static inline bool AnyButtonUp()
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonUp(i))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = Button>
	static inline bool AreButtonsPressed(Buttons... buttons)
	{
		for (Button button : { buttons... })
		{
			if (!IsButtonPressed(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = Button>
	static inline bool AreButtonsReleased(Buttons... buttons)
	{
		for (Button button : { buttons... })
		{
			if (!IsButtonReleased(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = Button>
	static inline bool AreButtonsDown(Buttons... buttons)
	{
		for (Button button : { buttons... })
		{
			if (!IsButtonDown(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = Button>
	static inline bool AreButtonsUp(Buttons... buttons)
	{
		for (Button button : { buttons... })
		{
			if (!IsButtonUp(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = Button>
	static inline bool AnyButtonsPressed(Buttons... buttons)
	{
		for (Button button : { buttons... })
		{
			if (IsButtonPressed(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = Button>
	static inline bool AnyButtonsReleased(Buttons... buttons)
	{
		for (Button button : { buttons... })
		{
			if (IsButtonReleased(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = Button>
	static inline bool AnyButtonsDown(Buttons... buttons)
	{
		for (Button button : { buttons... })
		{
			if (IsButtonDown(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = Button>
	static inline bool AnyButtonsUp(Buttons... buttons)
	{
		for (Button button : { buttons... })
		{
			if (IsButtonUp(button))
				return true;
		}

		return false;
	}

	static inline Point<i32> GetPosition() { i32 x, y; SDL_GetMouseState(&x, &y); return { x, y }; }

	static inline void ShowCursor() { SDL_ShowCursor(SDL_ENABLE); }
	static inline void HideCursor() { SDL_ShowCursor(SDL_DISABLE); }
};
