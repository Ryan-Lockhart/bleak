#pragma once

#include "color.hpp"

namespace Colors
{
	static constexpr Color White{ u8{ 0xFF }, u8{ 0xFF }, u8{ 0xFF } };
	static constexpr Color Black{ u8{ 0x00 }, u8{ 0x00 }, u8{ 0x00 } };

	static constexpr Color Transparent{ u8{ 0x00 }, u8{ 0x00 }, u8{ 0x00 }, u8{ 0x00 } };

	static constexpr Color Red{ u8{ 0xFF }, u8{ 0x00 }, u8{ 0x00 } };
	static constexpr Color Green{ u8{ 0x00 }, u8{ 0xFF }, u8{ 0x00 } };
	static constexpr Color Blue{ u8{ 0x00 }, u8{ 0x00 }, u8{ 0xFF } };

	static constexpr Color Marble{ u8{ 240 }, u8{ 232 }, u8{ 232 } };
	static constexpr Color Intrite{ u8{ 112 }, u8{ 104 }, u8{ 104 } };
	static constexpr Color Charcoal{ u8{ 40 }, u8{ 32 }, u8{ 32 } };

	static constexpr Color Cyan{ u8{ 0x00 }, u8{ 0xFF }, u8{ 0xFF } };
	static constexpr Color Magenta{ u8{ 0xFF }, u8{ 0x00 }, u8{ 0xFF } };
	static constexpr Color Yellow{ u8{ 0xFF }, u8{ 0xFF }, u8{ 0x00 } };

	static constexpr Color Grey{ u8{ 0x80 }, u8{ 0x80 }, u8{ 0x80 } };
	static constexpr Color LightGrey{ u8{ 0xC0 }, u8{ 0xC0 }, u8{ 0xC0 } };
	static constexpr Color DarkGrey{ u8{ 0x40 }, u8{ 0x40 }, u8{ 0x40 } };

	static constexpr Color LightRed{ u8{ 0xFF }, u8{ 0x80 }, u8{ 0x80 } };
	static constexpr Color LightGreen{ u8{ 0x80 }, u8{ 0xFF }, u8{ 0x80 } };
	static constexpr Color LightBlue{ u8{ 0x80 }, u8{ 0x80 }, u8{ 0xFF } };

	static constexpr Color LightIntrite{ u8{ 132 }, u8{ 124 }, u8{ 124 } };
	static constexpr Color LightCharcoal{ u8{ 60 }, u8{ 58 }, u8{ 58 } };

	static constexpr Color LightCyan{ u8{ 0x80 }, u8{ 0xFF }, u8{ 0xFF } };
	static constexpr Color LightMagenta{ u8{ 0xFF }, u8{ 0x80 }, u8{ 0xFF } };
	static constexpr Color LightYellow{ u8{ 0xFF }, u8{ 0xFF }, u8{ 0x80 } };

	static constexpr Color DarkRed{ u8{ 0x80 }, u8{ 0x00 }, u8{ 0x00 } };
	static constexpr Color DarkGreen{ u8{ 0x00 }, u8{ 0x80 }, u8{ 0x00 } };
	static constexpr Color DarkBlue{ u8{ 0x00 }, u8{ 0x00 }, u8{ 0x80 } };

	static constexpr Color DarkMarble{ u8{ 200 }, u8{ 192 }, u8{ 192 } };
	static constexpr Color DarkIntrite{ u8{ 92 }, u8{ 84 }, u8{ 84 } };
	static constexpr Color DarkCharcoal{ u8{ 20 }, u8{ 16 }, u8{ 16 } };

	static constexpr Color DarkCyan{ u8{ 0x00 }, u8{ 0x80 }, u8{ 0x80 } };
	static constexpr Color DarkMagenta{ u8{ 0x80 }, u8{ 0x00 }, u8{ 0x80 } };
	static constexpr Color DarkYellow{ u8{ 0x80 }, u8{ 0x80 }, u8{ 0x00 } };

	static constexpr Color BrightOrange{ u8{ 255 }, u8{ 94 }, u8{ 5 } };
	static constexpr Color LightOrange{ u8{ 255 }, u8{ 165 }, u8{ 115 } };
	static constexpr Color DarkOrange{ u8{ 200 }, u8{ 71 }, u8{ 0 } };
	
	namespace Materials
	{
		static constexpr Color LightBlood{ u8{ 177 }, u8{ 34 }, u8{ 53 } };
		static constexpr Color Blood{ u8{ 157 }, u8{ 34 }, u8{ 53 } };
		static constexpr Color DarkBlood{ u8{ 137 }, u8{ 14 }, u8{ 33 } };

		static constexpr Color Ebony{ u8{ 40 }, u8{ 44 }, u8{ 52 } };
		static constexpr Color Ivory{ u8{ 255 }, u8{ 255 }, u8{ 240 } };

		static constexpr Color Oak{ u8{ 120 }, u8{ 81 }, u8{ 45 } };
		static constexpr Color Willow{ u8{ 168 }, u8{ 172 }, u8{ 155 } };
		static constexpr Color Birch{ u8{ 234 }, u8{ 225 }, u8{ 216 } };
	}

	namespace Metals
	{
		static constexpr Color Iron{ u8{ 58 }, u8{ 60 }, u8{ 64 } };
		static constexpr Color Steel{ u8{ 113 }, u8{ 121 }, u8{ 126 } };

		static constexpr Color Gold{ u8{ 255 }, u8{ 215 }, u8{ 0 } };
		static constexpr Color Silver{ u8{ 192 }, u8{ 192 }, u8{ 192 } };
		static constexpr Color Copper{ u8{ 184 }, u8{ 115 }, u8{ 51 } };

		static constexpr Color Tin{ u8{ 145 }, u8{ 145 }, u8{ 145 } };
		static constexpr Color Bronze{ u8{ 205 }, u8{ 127 }, u8{ 50 } };

		static constexpr Color Zinc{ u8{ 186 }, u8{ 196 }, u8{ 200 } };
		static constexpr Color Brass{ u8{ 181 }, u8{ 166 }, u8{ 66 } };
	}
}
