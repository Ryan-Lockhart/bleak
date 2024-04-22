#pragma once

#include "typedef.hpp"
#include "point.hpp"

struct Mouse
{
	static inline i32 GetX() { return ray::GetMouseX(); }
	static inline i32 GetY() { return ray::GetMouseY(); }

	template<typename T, typename = Vector2>
	static inline T GetPosition();

	template<> static inline Vector2 GetPosition() { return ray::GetMousePosition(); }
	template<> static inline Point GetPosition() { return { GetX(), GetY() }; }

	static inline void SetPosition(Point position) { ray::SetMousePosition(position.x, position.y); }
	static inline void SetPosition(i32 x, i32 y) { ray::SetMousePosition(x, y); }

	static constexpr MouseButton MOUSE_BUTTON_START = MOUSE_LEFT_BUTTON;
	static constexpr MouseButton MOUSE_BUTTON_END = MOUSE_BUTTON_BACK;

	inline static bool IsButtonPressed(MouseButton button) { return ray::IsMouseButtonPressed(button); }
	inline static bool IsButtonPressed(int button) { return ray::IsMouseButtonPressed(button); }

	inline static bool IsButtonReleased(MouseButton button) { return ray::IsMouseButtonReleased(button); }
	inline static bool IsButtonReleased(int button) { return ray::IsMouseButtonReleased(button); }

	inline static bool IsButtonDown(MouseButton button) { return ray::IsMouseButtonDown(button); }
	inline static bool IsButtonDown(int button) { return ray::IsMouseButtonDown(button); }

	inline static bool IsButtonUp(MouseButton button) { return ray::IsMouseButtonUp(button); }
	inline static bool IsButtonUp(int button) { return ray::IsMouseButtonUp(button); }

	static bool AnyButtonPressed()
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonPressed(i))
				return true;
		}
	}

	static bool AnyButtonReleased()
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonReleased(i))
				return true;
		}
	}

	static bool AnyButtonDown()
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonDown(i))
				return true;
		}
	}

	static bool AnyButtonUp()
	{
		for (int i{ MOUSE_BUTTON_START }; i < MOUSE_BUTTON_END; ++i)
		{
			if (IsButtonUp(i))
				return true;
		}
	}

	template<typename... Buttons, typename = MouseButton>
	static bool AreButtonsPressed(Buttons... buttons)
	{
		for (MouseButton button : { buttons... })
		{
			if (!IsButtonPressed(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = MouseButton>
	static bool AreButtonsReleased(Buttons... buttons)
	{
		for (MouseButton button : { buttons... })
		{
			if (!IsButtonReleased(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = MouseButton>
	static bool AreButtonsDown(Buttons... buttons)
	{
		for (MouseButton button : { buttons... })
		{
			if (!IsButtonDown(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = MouseButton>
	static bool AreButtonsUp(Buttons... buttons)
	{
		for (MouseButton button : { buttons... })
		{
			if (!IsButtonUp(button))
				return false;
		}

		return true;
	}

	template<typename... Buttons, typename = MouseButton>
	static bool AnyButtonsPressed(Buttons... buttons)
	{
		for (MouseButton button : { buttons... })
		{
			if (IsButtonPressed(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = MouseButton>
	static bool AnyButtonsReleased(Buttons... buttons)
	{
		for (MouseButton button : { buttons... })
		{
			if (IsButtonReleased(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = MouseButton>
	static bool AnyButtonsDown(Buttons... buttons)
	{
		for (MouseButton button : { buttons... })
		{
			if (IsButtonDown(button))
				return true;
		}

		return false;
	}

	template<typename... Buttons, typename = MouseButton>
	static bool AnyButtonsUp(Buttons... buttons)
	{
		for (MouseButton button : { buttons... })
		{
			if (IsButtonUp(button))
				return true;
		}

		return false;
	}
};
