#pragma once

#include "typedef.hpp"

#include <bitset>

#include <SDL.h>

struct Keyboard
{
public:
	using Key = SDL_Scancode;

	static constexpr Key KEY_START = SDL_SCANCODE_UNKNOWN;
	static constexpr Key KEY_END = SDL_NUM_SCANCODES;

	enum class KeyState
	{
		Pressed,
		Released,
		Down,
		Up
	};

private:
	std::bitset<KEY_END> currentState;
	std::bitset<KEY_END> previousState;

public:
	inline Keyboard() : currentState(false), previousState(false) {}
	inline ~Keyboard() = default;

	inline Keyboard(cref<Keyboard> other) = delete;
	inline Keyboard(rval<Keyboard> other) = delete;

	inline ref<Keyboard> operator=(cref<Keyboard> other) = delete;
	inline ref<Keyboard> operator=(rval<Keyboard> other) = delete;

	inline void update(Key key, bool state)
	{
		previousState[key] = currentState[key];
		currentState[key] = state;
	}

	template<typename... Keys, typename = std::pair<Key, bool>>
	inline void update(Keys... keyStates)
	{
		for (const auto& [key, state] : { keyStates... })
		{
			previousState[key] = currentState[key];
			currentState[key] = state;
		}
	}

	inline void update(cptr<Key> keys, cptr<bool> states, usize count)
	{
		for (usize i{ 0 }; i < count; ++i)
		{
			previousState[keys[i]] = currentState[keys[i]];
			currentState[keys[i]] = states[i];
		}
	}

	inline KeyState operator[](Key key) const { return previousState[key] ? currentState[key] ? KeyState::Pressed : KeyState::Up : currentState[key] ? KeyState::Down : KeyState::Released; }
	inline KeyState operator[](int key) const { return previousState[key] ? currentState[key] ? KeyState::Pressed : KeyState::Up : currentState[key] ? KeyState::Down : KeyState::Released; }

	inline bool IsKeyPressed(Key key) const { return (*this)[key] == KeyState::Pressed; }
	inline bool IsKeyPressed(int key) const { return (*this)[key] == KeyState::Pressed; }

	inline bool IsKeyReleased(Key key) const { return (*this)[key] == KeyState::Released; }
	inline bool IsKeyReleased(int key) const { return (*this)[key] == KeyState::Released; }

	inline bool IsKeyDown(Key key) const { return (*this)[key] == KeyState::Down; }
	inline bool IsKeyDown(int key) const { return (*this)[key] == KeyState::Down; }

	inline bool IsKeyUp(Key key) const { return (*this)[key] == KeyState::Up; }
	inline bool IsKeyUp(int key) const { return (*this)[key] == KeyState::Up; }

	inline bool AnyKeyPressed() const
	{
		for (int i{ KEY_START }; i < KEY_END; ++i)
		{
			if (IsKeyPressed(i))
				return true;
		}

		return false;
	}

	inline bool AnyKeyReleased() const
	{
		for (int i{ KEY_START }; i < KEY_END; ++i)
		{
			if (IsKeyReleased(i))
				return true;
		}

		return false;
	}

	inline bool AnyKeyDown() const
	{
		for (int i{ KEY_START }; i < KEY_END; ++i)
		{
			if (IsKeyDown(i))
				return true;
		}

		return false;
	}

	inline bool AnyKeyUp() const
	{
		for (int i{ KEY_START }; i < KEY_END; ++i)
		{
			if (IsKeyUp(i))
				return true;
		}

		return false;
	}

	template<typename... Keys, typename = Key>
	inline bool AreKeysPressed(Keys... keys) const
	{
		for (Key key : { keys... })
		{
			if (!IsKeyPressed(key))
				return false;
		}

		return true;
	}

	template<typename... Keys, typename = Key>
	inline bool AreKeysReleased(Keys... keys) const
	{
		for (Key key : { keys... })
		{
			if (!IsKeyReleased(key))
				return false;
		}

		return true;
	}

	template<typename... Keys, typename = Key>
	inline bool AreKeysDown(Keys... keys) const
	{
		for (Key key : { keys... })
		{
			if (!IsKeyDown(key))
				return false;
		}

		return true;
	}

	template<typename... Keys, typename = Key>
	inline bool AreKeysUp(Keys... keys) const
	{
		for (Key key : { keys... })
		{
			if (!IsKeyUp(key))
				return false;
		}

		return true;
	}

	template<typename... Keys, typename = Key>
	inline bool AnyKeysPressed(Keys... keys) const
	{
		for (Key key : { keys... })
		{
			if (IsKeyPressed(key))
				return true;
		}

		return false;
	}

	template<typename... Keys, typename = Key>
	inline bool AnyKeysReleased(Keys... keys) const
	{
		for (Key key : { keys... })
		{
			if (IsKeyReleased(key))
				return true;
		}

		return false;
	}

	template<typename... Keys, typename = Key>
	inline bool AnyKeysDown(Keys... keys) const
	{
		for (Key key : { keys... })
		{
			if (IsKeyDown(key))
				return true;
		}

		return false;
	}

	template<typename... Keys, typename = Key>
	inline bool AnyKeysUp(Keys... keys) const
	{
		for (Key key : { keys... })
		{
			if (IsKeyUp(key))
				return true;
		}

		return false;
	}
};
