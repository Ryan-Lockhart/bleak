#pragma once

#include <raylib.h>

static struct Keyboard
{
	inline static bool IsKeyDown(KeyboardKey key) { return IsKeyDown(key); }
	inline static bool IsKeyPressed(KeyboardKey key) { return IsKeyPressed(key); }
	inline static bool IsKeyReleased(KeyboardKey key) { return IsKeyReleased(key); }

	static bool AnyKeyPressed()
	{
		for (int i{ 0 }; i < KEY_KB_MENU; ++i)
		{
			if (IsKeyPressed((KeyboardKey)i))
				return true;
		}
	}

	static bool AnyKeyReleased()
	{
		for (int i{ 0 }; i < KEY_KB_MENU; ++i)
		{
			if (IsKeyReleased((KeyboardKey)i))
				return true;
		}
	}

	static bool AnyKeyDown()
	{
		for (int i{ 0 }; i < KEY_KB_MENU; ++i)
		{
			if (IsKeyDown((KeyboardKey)i))
				return true;
		}
	}

	template<typename... Keys, typename = KeyboardKey>
	static bool AreKeysDown(Keys... keys)
	{
		for (KeyboardKey key : { keys... })
		{
			if (!IsKeyDown(key))
				return false;
		}

		return true;
	}

	template<typename... Keys, typename = KeyboardKey>
	static bool AreKeysPressed(Keys... keys)
	{
		for (KeyboardKey key : { keys... })
		{
			if (!IsKeyPressed(key))
				return false;
		}

		return true;
	}

	template<typename... Keys, typename = KeyboardKey>
	static bool AreKeysReleased(Keys... keys)
	{
		for (KeyboardKey key : { keys... })
		{
			if (!IsKeyReleased(key))
				return false;
		}

		return true;
	}

	template<typename... Keys, typename = KeyboardKey>
	static bool AnyKeysDown(Keys... keys)
	{
		for (KeyboardKey key : { keys... })
		{
			if (IsKeyDown(key))
				return true;
		}

		return false;
	}

	template<typename... Keys, typename = KeyboardKey>
	static bool AnyKeysPressed(Keys... keys)
	{
		for (KeyboardKey key : { keys... })
		{
			if (IsKeyPressed(key))
				return true;
		}

		return false;
	}

	template<typename... Keys, typename = KeyboardKey>
	static bool AnyKeysReleased(Keys... keys)
	{
		for (KeyboardKey key : { keys... })
		{
			if (IsKeyReleased(key))
				return true;
		}

		return false;
	}
};
