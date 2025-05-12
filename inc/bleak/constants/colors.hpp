#pragma once

#include <bleak/typedef.hpp>

#include <bleak/color.hpp>

namespace bleak {
	namespace colors {
		static constexpr color_t White{ u8{ 0xFF }, u8{ 0xFF }, u8{ 0xFF } };
		static constexpr color_t Black{ u8{ 0x00 }, u8{ 0x00 }, u8{ 0x00 } };
		static constexpr color_t Grey{ u8{ 0x80 }, u8{ 0x80 }, u8{ 0x80 } };

		static constexpr color_t Transparent{ u8{ 0x00 }, u8{ 0x00 }, u8{ 0x00 }, u8{ 0x00 } };

		static constexpr color_t Red{ u8{ 0xFF }, u8{ 0x00 }, u8{ 0x00 } };
		static constexpr color_t Green{ u8{ 0x00 }, u8{ 0xFF }, u8{ 0x00 } };
		static constexpr color_t Blue{ u8{ 0x00 }, u8{ 0x00 }, u8{ 0xFF } };

		static constexpr color_t Marble{ u8{ 240 }, u8{ 232 }, u8{ 232 } };
		static constexpr color_t Intrite{ u8{ 112 }, u8{ 104 }, u8{ 104 } };
		static constexpr color_t Charcoal{ u8{ 40 }, u8{ 32 }, u8{ 32 } };

		static constexpr color_t Cyan{ u8{ 0x00 }, u8{ 0xFF }, u8{ 0xFF } };
		static constexpr color_t Magenta{ u8{ 0xFF }, u8{ 0x00 }, u8{ 0xFF } };
		static constexpr color_t Yellow{ u8{ 0xFF }, u8{ 0xFF }, u8{ 0x00 } };
		static constexpr color_t Orange{ u8{ 0xFF }, u8{ 0x80 }, u8{ 0x00 } };

		namespace light {
			static constexpr color_t Grey{ u8{ 0xC0 }, u8{ 0xC0 }, u8{ 0xC0 } };

			static constexpr color_t Red{ u8{ 0xFF }, u8{ 0x80 }, u8{ 0x80 } };
			static constexpr color_t Green{ u8{ 0x80 }, u8{ 0xFF }, u8{ 0x80 } };
			static constexpr color_t Blue{ u8{ 0x80 }, u8{ 0x80 }, u8{ 0xFF } };

			static constexpr color_t Intrite{ u8{ 132 }, u8{ 124 }, u8{ 124 } };
			static constexpr color_t Charcoal{ u8{ 60 }, u8{ 58 }, u8{ 58 } };

			static constexpr color_t Cyan{ u8{ 0x80 }, u8{ 0xFF }, u8{ 0xFF } };
			static constexpr color_t Magenta{ u8{ 0xFF }, u8{ 0x80 }, u8{ 0xFF } };
			static constexpr color_t Yellow{ u8{ 0xFF }, u8{ 0xFF }, u8{ 0x80 } };
			static constexpr color_t Orange{ u8{ 0xC0 }, u8{ 0x80 }, u8{ 0x00 } };
		} // namespace light

		namespace dark {
			static constexpr color_t Grey{ u8{ 0x40 }, u8{ 0x40 }, u8{ 0x40 } };

			static constexpr color_t Red{ u8{ 0x80 }, u8{ 0x00 }, u8{ 0x00 } };
			static constexpr color_t Green{ u8{ 0x00 }, u8{ 0x80 }, u8{ 0x00 } };
			static constexpr color_t Blue{ u8{ 0x00 }, u8{ 0x00 }, u8{ 0x80 } };

			static constexpr color_t Marble{ u8{ 200 }, u8{ 192 }, u8{ 192 } };
			static constexpr color_t Intrite{ u8{ 92 }, u8{ 84 }, u8{ 84 } };
			static constexpr color_t Charcoal{ u8{ 20 }, u8{ 16 }, u8{ 16 } };

			static constexpr color_t Cyan{ u8{ 0x00 }, u8{ 0x80 }, u8{ 0x80 } };
			static constexpr color_t Magenta{ u8{ 0x80 }, u8{ 0x00 }, u8{ 0x80 } };
			static constexpr color_t Yellow{ u8{ 0x80 }, u8{ 0x80 }, u8{ 0x00 } };
			static constexpr color_t Orange{ u8{ 0x80 }, u8{ 0x40 }, u8{ 0x00 } };
		} // namespace dark

		namespace materials {
			static constexpr color_t Blood{ u8{ 157 }, u8{ 34 }, u8{ 53 } };
			static constexpr color_t LightBlood{ u8{ 177 }, u8{ 34 }, u8{ 53 } };
			static constexpr color_t DarkBlood{ u8{ 137 }, u8{ 14 }, u8{ 33 } };

			static constexpr color_t Ebony{ u8{ 40 }, u8{ 44 }, u8{ 52 } };
			static constexpr color_t Ivory{ u8{ 255 }, u8{ 255 }, u8{ 240 } };

			static constexpr color_t Oak{ u8{ 120 }, u8{ 81 }, u8{ 45 } };
			static constexpr color_t Willow{ u8{ 168 }, u8{ 172 }, u8{ 155 } };
			static constexpr color_t Birch{ u8{ 234 }, u8{ 225 }, u8{ 216 } };
		} // namespace materials

		namespace metals {
			static constexpr color_t Iron{ u8{ 58 }, u8{ 60 }, u8{ 64 } };
			static constexpr color_t Steel{ u8{ 113 }, u8{ 121 }, u8{ 126 } };

			static constexpr color_t Gold{ u8{ 255 }, u8{ 215 }, u8{ 0 } };
			static constexpr color_t Silver{ u8{ 192 }, u8{ 192 }, u8{ 192 } };
			static constexpr color_t Copper{ u8{ 184 }, u8{ 115 }, u8{ 51 } };

			static constexpr color_t Tin{ u8{ 145 }, u8{ 145 }, u8{ 145 } };
			static constexpr color_t Bronze{ u8{ 205 }, u8{ 127 }, u8{ 50 } };

			static constexpr color_t Zinc{ u8{ 186 }, u8{ 196 }, u8{ 200 } };
			static constexpr color_t Brass{ u8{ 181 }, u8{ 166 }, u8{ 66 } };

			namespace shackles {
				static constexpr color_t Calcitic{ u8{ 255 }, u8{ 255 }, u8{ 240 } };
				static constexpr color_t Spectral{ u8{ 72 }, u8{ 100 }, u8{ 83 } };
				static constexpr color_t Sanguine{ u8{ 157 }, u8{ 34 }, u8{ 53 } };
			} // namespace shackles
		} // namespace metals
	} // namespace colors
} // namespace bleak
