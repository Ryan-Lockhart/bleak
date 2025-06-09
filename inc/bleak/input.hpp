#pragma once

#include <bleak/typedef.hpp>

namespace bleak {
	enum struct input_e : u8 {
		Pressed = 0 << 1,
		Released = 1 << Pressed,
		Down = 1 << Released,
		Up = 1 << Down
	};
	
	constexpr cstr to_string(input_e input) noexcept {
		switch (input) {
			case input_e::Pressed: {
				return "pressed";
			} case input_e::Released: {
				return "released";
			} case input_e::Down: {
				return "down";
			} case input_e::Up: {
				return "up";
			}
		}
	}
	
	template<typename T> struct is_button {
		static constexpr bool value = false;
	};

	template<typename T> concept Button = is_button<T>::value;
} // namespace Bleakdepth
