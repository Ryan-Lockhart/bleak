#pragma once

#include <bleak/typedef.hpp>

#include <SDL.h>

namespace bleak {
	namespace sdl {
		using key = SDL_Scancode;
	}

	namespace keys {
		static constexpr sdl::key First = SDL_SCANCODE_UNKNOWN;
		static constexpr sdl::key Last = SDL_SCANCODE_ENDCALL;

		static constexpr usize Count = SDL_NUM_SCANCODES;

		namespace Alpha {
			static constexpr sdl::key A = SDL_SCANCODE_A;
			static constexpr sdl::key B = SDL_SCANCODE_B;
			static constexpr sdl::key C = SDL_SCANCODE_C;
			static constexpr sdl::key D = SDL_SCANCODE_D;
			static constexpr sdl::key E = SDL_SCANCODE_E;
			static constexpr sdl::key F = SDL_SCANCODE_F;
			static constexpr sdl::key G = SDL_SCANCODE_G;
			static constexpr sdl::key H = SDL_SCANCODE_H;
			static constexpr sdl::key I = SDL_SCANCODE_I;
			static constexpr sdl::key J = SDL_SCANCODE_J;
			static constexpr sdl::key K = SDL_SCANCODE_K;
			static constexpr sdl::key L = SDL_SCANCODE_L;
			static constexpr sdl::key M = SDL_SCANCODE_M;
			static constexpr sdl::key N = SDL_SCANCODE_N;
			static constexpr sdl::key O = SDL_SCANCODE_O;
			static constexpr sdl::key P = SDL_SCANCODE_P;
			static constexpr sdl::key Q = SDL_SCANCODE_Q;
			static constexpr sdl::key R = SDL_SCANCODE_R;
			static constexpr sdl::key S = SDL_SCANCODE_S;
			static constexpr sdl::key T = SDL_SCANCODE_T;
			static constexpr sdl::key U = SDL_SCANCODE_U;
			static constexpr sdl::key V = SDL_SCANCODE_V;
			static constexpr sdl::key W = SDL_SCANCODE_W;
			static constexpr sdl::key X = SDL_SCANCODE_X;
			static constexpr sdl::key Y = SDL_SCANCODE_Y;
			static constexpr sdl::key Z = SDL_SCANCODE_Z;
		};

		namespace Numeric {
			static constexpr sdl::key Zero = SDL_SCANCODE_0;
			static constexpr sdl::key One = SDL_SCANCODE_1;
			static constexpr sdl::key Two = SDL_SCANCODE_2;
			static constexpr sdl::key Three = SDL_SCANCODE_3;
			static constexpr sdl::key Four = SDL_SCANCODE_4;
			static constexpr sdl::key Five = SDL_SCANCODE_5;
			static constexpr sdl::key Six = SDL_SCANCODE_6;
			static constexpr sdl::key Seven = SDL_SCANCODE_7;
			static constexpr sdl::key Eight = SDL_SCANCODE_8;
			static constexpr sdl::key Nine = SDL_SCANCODE_9;
		};

		static constexpr sdl::key Space = SDL_SCANCODE_SPACE;
		static constexpr sdl::key Enter = SDL_SCANCODE_RETURN;
		static constexpr sdl::key Escape = SDL_SCANCODE_ESCAPE;
		static constexpr sdl::key Backspace = SDL_SCANCODE_BACKSPACE;
		static constexpr sdl::key Tab = SDL_SCANCODE_TAB;
		static constexpr sdl::key LeftBracket = SDL_SCANCODE_LEFTBRACKET;
		static constexpr sdl::key RightBracket = SDL_SCANCODE_RIGHTBRACKET;
		static constexpr sdl::key Semicolon = SDL_SCANCODE_SEMICOLON;
		static constexpr sdl::key Apostrophe = SDL_SCANCODE_APOSTROPHE;
		static constexpr sdl::key Comma = SDL_SCANCODE_COMMA;
		static constexpr sdl::key Period = SDL_SCANCODE_PERIOD;
		static constexpr sdl::key Slash = SDL_SCANCODE_SLASH;
		static constexpr sdl::key Backslash = SDL_SCANCODE_BACKSLASH;
		static constexpr sdl::key Grave = SDL_SCANCODE_GRAVE;
		static constexpr sdl::key Minus = SDL_SCANCODE_MINUS;
		static constexpr sdl::key Equals = SDL_SCANCODE_EQUALS;
		static constexpr sdl::key Insert = SDL_SCANCODE_INSERT;
		static constexpr sdl::key Delete = SDL_SCANCODE_DELETE;
		static constexpr sdl::key Home = SDL_SCANCODE_HOME;
		static constexpr sdl::key End = SDL_SCANCODE_END;
		static constexpr sdl::key PageUp = SDL_SCANCODE_PAGEUP;
		static constexpr sdl::key PageDown = SDL_SCANCODE_PAGEDOWN;
		static constexpr sdl::key PrintScreen = SDL_SCANCODE_PRINTSCREEN;
		static constexpr sdl::key Pause = SDL_SCANCODE_PAUSE;

		namespace Keypad {
			static constexpr sdl::key Divide = SDL_SCANCODE_KP_DIVIDE;
			static constexpr sdl::key Multiply = SDL_SCANCODE_KP_MULTIPLY;
			static constexpr sdl::key Minus = SDL_SCANCODE_KP_MINUS;
			static constexpr sdl::key Plus = SDL_SCANCODE_KP_PLUS;
			static constexpr sdl::key Enter = SDL_SCANCODE_KP_ENTER;
			static constexpr sdl::key Period = SDL_SCANCODE_KP_PERIOD;
			static constexpr sdl::key Equals = SDL_SCANCODE_KP_EQUALS;

			namespace Numeric {
				static constexpr sdl::key Zero = SDL_SCANCODE_KP_0;
				static constexpr sdl::key One = SDL_SCANCODE_KP_1;
				static constexpr sdl::key Two = SDL_SCANCODE_KP_2;
				static constexpr sdl::key Three = SDL_SCANCODE_KP_3;
				static constexpr sdl::key Four = SDL_SCANCODE_KP_4;
				static constexpr sdl::key Five = SDL_SCANCODE_KP_5;
				static constexpr sdl::key Six = SDL_SCANCODE_KP_6;
				static constexpr sdl::key Seven = SDL_SCANCODE_KP_7;
				static constexpr sdl::key Eight = SDL_SCANCODE_KP_8;
				static constexpr sdl::key Nine = SDL_SCANCODE_KP_9;
			};
		};

		namespace Function {
			static constexpr sdl::key One = SDL_SCANCODE_F1;
			static constexpr sdl::key Two = SDL_SCANCODE_F2;
			static constexpr sdl::key Three = SDL_SCANCODE_F3;
			static constexpr sdl::key Four = SDL_SCANCODE_F4;
			static constexpr sdl::key Five = SDL_SCANCODE_F5;
			static constexpr sdl::key Six = SDL_SCANCODE_F6;
			static constexpr sdl::key Seven = SDL_SCANCODE_F7;
			static constexpr sdl::key Eight = SDL_SCANCODE_F8;
			static constexpr sdl::key Nine = SDL_SCANCODE_F9;
			static constexpr sdl::key Ten = SDL_SCANCODE_F10;
			static constexpr sdl::key Eleven = SDL_SCANCODE_F11;
			static constexpr sdl::key Twelve = SDL_SCANCODE_F12;
		};

		namespace Arrow {
			static constexpr sdl::key Up = SDL_SCANCODE_UP;
			static constexpr sdl::key Down = SDL_SCANCODE_DOWN;
			static constexpr sdl::key Left = SDL_SCANCODE_LEFT;
			static constexpr sdl::key Right = SDL_SCANCODE_RIGHT;
		};

		namespace Modifier {
			namespace Left {
				static constexpr sdl::key Shift = SDL_SCANCODE_LSHIFT;
				static constexpr sdl::key Control = SDL_SCANCODE_LCTRL;
				static constexpr sdl::key Alt = SDL_SCANCODE_LALT;
				static constexpr sdl::key Meta = SDL_SCANCODE_LGUI;
			};

			namespace Right {
				static constexpr sdl::key Shift = SDL_SCANCODE_RSHIFT;
				static constexpr sdl::key Control = SDL_SCANCODE_RCTRL;
				static constexpr sdl::key Alt = SDL_SCANCODE_RALT;
				static constexpr sdl::key Meta = SDL_SCANCODE_RGUI;
			};

			static constexpr sdl::key CapsLock = SDL_SCANCODE_CAPSLOCK;
			static constexpr sdl::key NumLock = SDL_SCANCODE_NUMLOCKCLEAR;
			static constexpr sdl::key ScrollLock = SDL_SCANCODE_SCROLLLOCK;
		};
	}; // namespace Keys
} // namespace bleak
