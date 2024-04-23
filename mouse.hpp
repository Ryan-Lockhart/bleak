#pragma once

#include "typedef.hpp"
#include "point.hpp"

#include <bitset>

#include <SDL2/SDL.h>

struct Mouse
{
public:
	enum Button : int
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
	std::bitset<MOUSE_BUTTON_END> currentState;
	std::bitset<MOUSE_BUTTON_END> previousState;

public:
	inline Mouse() : currentState(false), previousState(false) {}
	inline ~Mouse() = default;

	inline Mouse(cref<Mouse> other) = delete;
	inline Mouse(rval<Mouse> other) = delete;

	inline ref<Mouse> operator=(cref<Mouse> other) = delete;
	inline ref<Mouse> operator=(rval<Mouse> other) = delete;

	inline void update(Button button, bool state)
	{
		previousState[button] = currentState[button];
		currentState[button] = state;
	}

	inline void update(cref<std::vector<Button>> buttons, cref<std::vector<bool>> states)
	{
		if (buttons.size() != states.size()) throw std::runtime_error("keys and states must have the same!");

		for (usize i{ 0 }; i < buttons.size(); ++i)
		{
			previousState[buttons[i]] = currentState[buttons[i]];
			currentState[buttons[i]] = states[i];
		}
	}

	inline void update(cptr<Button> buttons, cptr<bool> states, usize count)
	{
		for (usize i{ 0 }; i < count; ++i)
		{
			previousState[buttons[i]] = currentState[buttons[i]];
			currentState[buttons[i]] = states[i];
		}
	}

	inline ButtonState operator[](Button button) const { return previousState[button] ? currentState[button] ? ButtonState::Pressed : ButtonState::Up : currentState[button] ? ButtonState::Down : ButtonState::Released; }
	inline ButtonState operator[](int button) const { return previousState[button] ? currentState[button] ? ButtonState::Pressed : ButtonState::Up : currentState[button] ? ButtonState::Down : ButtonState::Released; }

	inline bool IsButtonPressed(Button button) const { return (*this)[button] == ButtonState::Pressed; }
	inline bool IsButtonPressed(int button) const { return (*this)[button] == ButtonState::Pressed; }

	inline bool IsButtonReleased(Button button) const { return (*this)[button] == ButtonState::Released; }
	inline bool IsButtonReleased(int button) const { return (*this)[button] == ButtonState::Released; }

	inline bool IsButtonDown(Button button) const { return (*this)[button] == ButtonState::Down; }
	inline bool IsButtonDown(int button) const { return (*this)[button] == ButtonState::Down; }

	inline bool IsButtonUp(Button button) const { return (*this)[button] == ButtonState::Up; }
	inline bool IsButtonUp(int button) const { return (*this)[button] == ButtonState::Up; }

	inline bool AnyButtonPressed() const
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonPressed(i))
				return true;
		}
	}

	inline bool AnyButtonReleased() const
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonReleased(i))
				return true;
		}
	}

	inline bool AnyButtonDown() const
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonDown(i))
				return true;
		}
	}

	inline bool AnyButtonUp() const
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonUp(i))
				return true;
		}
	}

	template<typename... Buttons, typename = Button>
	inline bool AreButtonsPressed(Buttons... buttons) const
	{
		for (Button button : { buttons... })
		{
			if (!IsButtonPressed(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = Button>
	inline bool AreButtonsReleased(Buttons... buttons) const
	{
		for (Button button : { buttons... })
		{
			if (!IsButtonReleased(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = Button>
	inline bool AreButtonsDown(Buttons... buttons) const
	{
		for (Button button : { buttons... })
		{
			if (!IsButtonDown(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = Button>
	inline bool AreButtonsUp(Buttons... buttons) const
	{
		for (Button button : { buttons... })
		{
			if (!IsButtonUp(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = Button>
	inline bool AnyButtonsPressed(Buttons... buttons) const
	{
		for (Button button : { buttons... })
		{
			if (IsButtonPressed(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = Button>
	inline bool AnyButtonsReleased(Buttons... buttons) const
	{
		for (Button button : { buttons... })
		{
			if (IsButtonReleased(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = Button>
	inline bool AnyButtonsDown(Buttons... buttons) const
	{
		for (Button button : { buttons... })
		{
			if (IsButtonDown(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = Button>
	inline bool AnyButtonsUp(Buttons... buttons) const
	{
		for (Button button : { buttons... })
		{
			if (IsButtonUp(button))
				return true;
		}

		return false;
	}

	inline Point GetPosition() const { i32 x, y; SDL_GetMouseState(&x, &y); return { x, y }; }

	inline void ShowCursor() const { SDL_ShowCursor(SDL_ENABLE); }
	inline void HideCursor() const { SDL_ShowCursor(SDL_DISABLE); }
};
