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
			static constexpr color_t Ebony{ u8{ 40 }, u8{ 44 }, u8{ 52 } };
			static constexpr color_t Ivory{ u8{ 255 }, u8{ 255 }, u8{ 240 } };

			static constexpr color_t Oak{ u8{ 120 }, u8{ 81 }, u8{ 45 } };
			static constexpr color_t Willow{ u8{ 168 }, u8{ 172 }, u8{ 155 } };
			static constexpr color_t Birch{ u8{ 234 }, u8{ 225 }, u8{ 216 } };
		} // namespace materials
	} // namespace colors
} // namespace bleak
