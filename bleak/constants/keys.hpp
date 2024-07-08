#pragma once

#include "bleak/typedef.hpp"

#include <SDL.h>

namespace bleak {
	namespace Keys {
		using key_t = SDL_Scancode;

		static constexpr key_t First = SDL_SCANCODE_UNKNOWN;
		static constexpr key_t Last = SDL_SCANCODE_ENDCALL;

		static constexpr usize Count = SDL_NUM_SCANCODES;

		struct Alpha {
			static constexpr key_t A = SDL_SCANCODE_A;
			static constexpr key_t B = SDL_SCANCODE_B;
			static constexpr key_t C = SDL_SCANCODE_C;
			static constexpr key_t D = SDL_SCANCODE_D;
			static constexpr key_t E = SDL_SCANCODE_E;
			static constexpr key_t F = SDL_SCANCODE_F;
			static constexpr key_t G = SDL_SCANCODE_G;
			static constexpr key_t H = SDL_SCANCODE_H;
			static constexpr key_t I = SDL_SCANCODE_I;
			static constexpr key_t J = SDL_SCANCODE_J;
			static constexpr key_t K = SDL_SCANCODE_K;
			static constexpr key_t L = SDL_SCANCODE_L;
			static constexpr key_t M = SDL_SCANCODE_M;
			static constexpr key_t N = SDL_SCANCODE_N;
			static constexpr key_t O = SDL_SCANCODE_O;
			static constexpr key_t P = SDL_SCANCODE_P;
			static constexpr key_t Q = SDL_SCANCODE_Q;
			static constexpr key_t R = SDL_SCANCODE_R;
			static constexpr key_t S = SDL_SCANCODE_S;
			static constexpr key_t T = SDL_SCANCODE_T;
			static constexpr key_t U = SDL_SCANCODE_U;
			static constexpr key_t V = SDL_SCANCODE_V;
			static constexpr key_t W = SDL_SCANCODE_W;
			static constexpr key_t X = SDL_SCANCODE_X;
			static constexpr key_t Y = SDL_SCANCODE_Y;
			static constexpr key_t Z = SDL_SCANCODE_Z;
		};

		struct Numeric {
			static constexpr key_t Zero = SDL_SCANCODE_0;
			static constexpr key_t One = SDL_SCANCODE_1;
			static constexpr key_t Two = SDL_SCANCODE_2;
			static constexpr key_t Three = SDL_SCANCODE_3;
			static constexpr key_t Four = SDL_SCANCODE_4;
			static constexpr key_t Five = SDL_SCANCODE_5;
			static constexpr key_t Six = SDL_SCANCODE_6;
			static constexpr key_t Seven = SDL_SCANCODE_7;
			static constexpr key_t Eight = SDL_SCANCODE_8;
			static constexpr key_t Nine = SDL_SCANCODE_9;
		};

		static constexpr key_t Space = SDL_SCANCODE_SPACE;
		static constexpr key_t Enter = SDL_SCANCODE_RETURN;
		static constexpr key_t Escape = SDL_SCANCODE_ESCAPE;
		static constexpr key_t Backspace = SDL_SCANCODE_BACKSPACE;
		static constexpr key_t Tab = SDL_SCANCODE_TAB;
		static constexpr key_t LeftBracket = SDL_SCANCODE_LEFTBRACKET;
		static constexpr key_t RightBracket = SDL_SCANCODE_RIGHTBRACKET;
		static constexpr key_t Semicolon = SDL_SCANCODE_SEMICOLON;
		static constexpr key_t Apostrophe = SDL_SCANCODE_APOSTROPHE;
		static constexpr key_t Comma = SDL_SCANCODE_COMMA;
		static constexpr key_t Period = SDL_SCANCODE_PERIOD;
		static constexpr key_t Slash = SDL_SCANCODE_SLASH;
		static constexpr key_t Backslash = SDL_SCANCODE_BACKSLASH;
		static constexpr key_t Grave = SDL_SCANCODE_GRAVE;
		static constexpr key_t Minus = SDL_SCANCODE_MINUS;
		static constexpr key_t Equals = SDL_SCANCODE_EQUALS;
		static constexpr key_t Insert = SDL_SCANCODE_INSERT;
		static constexpr key_t Delete = SDL_SCANCODE_DELETE;
		static constexpr key_t Home = SDL_SCANCODE_HOME;
		static constexpr key_t End = SDL_SCANCODE_END;
		static constexpr key_t PageUp = SDL_SCANCODE_PAGEUP;
		static constexpr key_t PageDown = SDL_SCANCODE_PAGEDOWN;
		static constexpr key_t PrintScreen = SDL_SCANCODE_PRINTSCREEN;
		static constexpr key_t Pause = SDL_SCANCODE_PAUSE;

		struct Keypad {
			static constexpr key_t Divide = SDL_SCANCODE_KP_DIVIDE;
			static constexpr key_t Multiply = SDL_SCANCODE_KP_MULTIPLY;
			static constexpr key_t Minus = SDL_SCANCODE_KP_MINUS;
			static constexpr key_t Plus = SDL_SCANCODE_KP_PLUS;
			static constexpr key_t Enter = SDL_SCANCODE_KP_ENTER;
			static constexpr key_t Period = SDL_SCANCODE_KP_PERIOD;
			static constexpr key_t Equals = SDL_SCANCODE_KP_EQUALS;

			struct Numeric {
				static constexpr key_t Zero = SDL_SCANCODE_KP_0;
				static constexpr key_t One = SDL_SCANCODE_KP_1;
				static constexpr key_t Two = SDL_SCANCODE_KP_2;
				static constexpr key_t Three = SDL_SCANCODE_KP_3;
				static constexpr key_t Four = SDL_SCANCODE_KP_4;
				static constexpr key_t Five = SDL_SCANCODE_KP_5;
				static constexpr key_t Six = SDL_SCANCODE_KP_6;
				static constexpr key_t Seven = SDL_SCANCODE_KP_7;
				static constexpr key_t Eight = SDL_SCANCODE_KP_8;
				static constexpr key_t Nine = SDL_SCANCODE_KP_9;
			};
		};

		struct Function {
			static constexpr key_t One = SDL_SCANCODE_F1;
			static constexpr key_t Two = SDL_SCANCODE_F2;
			static constexpr key_t Three = SDL_SCANCODE_F3;
			static constexpr key_t Four = SDL_SCANCODE_F4;
			static constexpr key_t Five = SDL_SCANCODE_F5;
			static constexpr key_t Six = SDL_SCANCODE_F6;
			static constexpr key_t Seven = SDL_SCANCODE_F7;
			static constexpr key_t Eight = SDL_SCANCODE_F8;
			static constexpr key_t Nine = SDL_SCANCODE_F9;
			static constexpr key_t Ten = SDL_SCANCODE_F10;
			static constexpr key_t Eleven = SDL_SCANCODE_F11;
			static constexpr key_t Twelve = SDL_SCANCODE_F12;
		};

		struct Arrow {
			static constexpr key_t Up = SDL_SCANCODE_UP;
			static constexpr key_t Down = SDL_SCANCODE_DOWN;
			static constexpr key_t Left = SDL_SCANCODE_LEFT;
			static constexpr key_t Right = SDL_SCANCODE_RIGHT;
		};

		struct Modifier {
			struct Left {
				static constexpr key_t Shift = SDL_SCANCODE_LSHIFT;
				static constexpr key_t Control = SDL_SCANCODE_LCTRL;
				static constexpr key_t Alt = SDL_SCANCODE_LALT;
				static constexpr key_t Meta = SDL_SCANCODE_LGUI;
			};

			struct Right {
				static constexpr key_t Shift = SDL_SCANCODE_RSHIFT;
				static constexpr key_t Control = SDL_SCANCODE_RCTRL;
				static constexpr key_t Alt = SDL_SCANCODE_RALT;
				static constexpr key_t Meta = SDL_SCANCODE_RGUI;
			};

			static constexpr key_t CapsLock = SDL_SCANCODE_CAPSLOCK;
			static constexpr key_t NumLock = SDL_SCANCODE_NUMLOCKCLEAR;
			static constexpr key_t ScrollLock = SDL_SCANCODE_SCROLLLOCK;
		};
	}; // namespace Keys
} // namespace bleak
