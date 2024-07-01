#pragma once

#include "typedef.hpp"

#include <bitset>

#include <SDL.h>

#include "input.hpp"
#include "point.hpp"

namespace Bleakdepth {
	struct button_t {
	  private:
		i32 value;

	  public:
		static const button_t Unknown;

		static const button_t Left;
		static const button_t Middle;
		static const button_t Right;

		static const button_t SideOne;
		static const button_t SideTwo;

		static const button_t Start;
		static const button_t End;

		static const usize Count;

		constexpr inline button_t() noexcept = delete;

		constexpr inline button_t(i32 value) noexcept : value { value } {}

		constexpr inline operator u8() const noexcept { return value; }

		constexpr inline bool operator==(button_t other) const noexcept { return value == other.value; }

		constexpr inline bool operator!=(button_t other) const noexcept { return value != other.value; }

		constexpr inline operator std::string() const;
	};

	constexpr const button_t button_t::Unknown { 0 };

	constexpr const button_t button_t::Left { SDL_BUTTON_LEFT };
	constexpr const button_t button_t::Middle { SDL_BUTTON_MIDDLE };
	constexpr const button_t button_t::Right { SDL_BUTTON_RIGHT };

	constexpr const button_t button_t::SideOne { SDL_BUTTON_X1 };
	constexpr const button_t button_t::SideTwo { SDL_BUTTON_X2 };

	constexpr const button_t button_t::Start = button_t::Left;
	constexpr const button_t button_t::End = button_t::SideTwo;

	constexpr const usize button_t::Count = button_t::SideTwo + 1;

	constexpr button_t::operator std::string() const {
		switch (value) {
		case button_t::Left:
			return "Left";
		case button_t::Middle:
			return "Middle";
		case button_t::Right:
			return "Right";

		case button_t::SideOne:
			return "SideOne";
		case button_t::SideTwo:
			return "SideTwo";

		default:
			return "Unknown";
		}
	}

	struct Mouse {
	  private:
		static inline std::bitset<button_t::Count> current_state;
		static inline std::bitset<button_t::Count> previous_state;

		static inline point_t<i32> current_position;
		static inline point_t<i32> previous_position;

		static inline point_t<f32> current_scroll;
		static inline point_t<f32> previous_scroll;

		static inline bool initialized;

	  public:
		static inline bool is_initialized() { return Mouse::initialized; }

		static inline void initialize() {
			Mouse::current_state.reset();
			Mouse::previous_state.reset();

			initialized = true;
		}

		static inline void update() {
			u32 state = SDL_GetMouseState(&Mouse::current_position.x, &Mouse::current_position.y);

			for (usize i { button_t::Start }; i <= button_t::End; ++i) {
				Mouse::previous_state[i] = Mouse::current_state[i];
				Mouse::current_state[i] = state & SDL_BUTTON(i);
			}
		}

		static inline void update(cref<point_t<i32>> position) {
			Mouse::previous_position = Mouse::current_position;
			Mouse::current_position = position;
		}

		static inline void update(cref<point_t<f32>> scroll) {
			Mouse::previous_scroll = Mouse::current_scroll;
			Mouse::current_scroll = scroll;
		}

		static inline input_state_t at(button_t button) {
			if (previous_state[button]) {
				return current_state[button] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return current_state[button] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline input_state_t at(int button) {
			if (previous_state[button]) {
				return current_state[button] ? input_state_t::Pressed : input_state_t::Up;
			} else {
				return current_state[button] ? input_state_t::Down : input_state_t::Released;
			}
		}

		static inline bool is_button_pressed(button_t button);
		static inline bool is_button_pressed(int button);

		static inline bool is_button_released(button_t button);
		static inline bool is_button_released(int button);

		static inline bool is_button_down(button_t button);
		static inline bool is_button_down(int button);

		static inline bool is_button_up(button_t button);
		static inline bool is_button_up(int button);

		static inline bool any_button_pressed() {
			for (int i { button_t::Start }; i <= button_t::End; ++i) {
				if (Mouse::is_button_pressed(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_button_released() {
			for (int i { button_t::Start }; i <= button_t::End; ++i) {
				if (Mouse::is_button_released(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_button_down() {
			for (int i { button_t::Start }; i <= button_t::End; ++i) {
				if (Mouse::is_button_down(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_button_up() {
			for (int i { button_t::Start }; i <= button_t::End; ++i) {
				if (Mouse::is_button_up(i)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool are_buttons_pressed(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!Mouse::is_button_pressed(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool are_buttons_released(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!Mouse::is_button_released(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool are_buttons_down(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!Mouse::is_button_down(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool are_buttons_up(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!Mouse::is_button_up(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool any_buttons_pressed(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (Mouse::is_button_pressed(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool any_buttons_released(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (Mouse::is_button_released(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool any_buttons_down(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (Mouse::is_button_down(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool any_buttons_up(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (Mouse::is_button_up(button)) {
					return true;
				}
			}

			return false;
		}

		static inline point_t<i32> get_position() { return Mouse::current_position; }

		static inline point_t<f32> get_scroll() { return Mouse::current_scroll; }

		static inline void show_cursor() { SDL_ShowCursor(SDL_ENABLE); }

		static inline void hide_cursor() { SDL_ShowCursor(SDL_DISABLE); }
	};

	inline bool Mouse::is_button_pressed(button_t button) { return Mouse::at(button) == input_state_t::Pressed; }

	inline bool Mouse::is_button_pressed(int button) { return Mouse::at(button) == input_state_t::Pressed; }

	inline bool Mouse::is_button_released(button_t button) { return Mouse::at(button) == input_state_t::Released; }

	inline bool Mouse::is_button_released(int button) { return Mouse::at(button) == input_state_t::Released; }

	inline bool Mouse::is_button_down(button_t button) { return Mouse::at(button) == input_state_t::Down; }

	inline bool Mouse::is_button_down(int button) { return Mouse::at(button) == input_state_t::Down; }

	inline bool Mouse::is_button_up(button_t button) { return Mouse::at(button) == input_state_t::Up; }

	inline bool Mouse::is_button_up(int button) { return Mouse::at(button) == input_state_t::Up; }
} // namespace Bleakdepth
