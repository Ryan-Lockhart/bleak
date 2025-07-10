#pragma once

#include <bleak/typedef.hpp>

#include <bitset>

#include <SDL.h>

#include <bleak/input.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	enum struct button_e : usize {
		Left = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right = SDL_BUTTON_RIGHT,

		SideOne = SDL_BUTTON_X1,
		SideTwo = SDL_BUTTON_X2
	};

	static constexpr usize ButtonStart{ static_cast<usize>(button_e::Left) };
	static constexpr usize ButtonEnd{ static_cast<usize>(button_e::SideTwo) };

	static constexpr usize ButtonCount{ ButtonEnd + 1 };

	constexpr cstr to_string(button_e button) noexcept {
		switch (button) {
			case button_e::Left: {
				return "left";
			} case button_e::Middle: {
				return "middle";
			} case button_e::Right: {
				return "right";
			} 	case button_e::SideOne: {
				return "side one";
			} case button_e::SideTwo: {
				return "side two";
			} default: {
				return "unknown";
			}
		}
	}

	template<> struct is_button<button_e> {
		static constexpr bool value = true;
	};

	struct mouse_s {
	  private:
		static inline std::bitset<ButtonCount> current_state;
		static inline std::bitset<ButtonCount> previous_state;

		static inline offset_t current_position{};
		static inline offset_t previous_position{};

		static inline offset_t current_scroll{};
		static inline offset_t previous_scroll{};

		static inline bool initialized;

	  public:
		static inline bool is_initialized() { return initialized; }

		static inline void initialize() {
			if (initialized) {
				return;
			}

			SDL_SetRelativeMouseMode(SDL_TRUE);

			current_state.reset();
			previous_state.reset();

			initialized = true;
		}

		static inline void terminate() {
			if (!initialized) {
				return;
			}

			// no cleanup required

			initialized = false;
		}

		static inline void update() {
			int x, y;
			u32 state = SDL_GetMouseState(&x, &y);

			previous_position = current_position;
			current_position = offset_t{ offset_t::scalar_cast(x), offset_t::scalar_cast(y) };

			for (usize i{ ButtonStart }; i <= ButtonEnd; ++i) {
				previous_state[i] = current_state[i];
				current_state[i] = state & SDL_BUTTON(i);
			}
		}

		static inline void process_event(ref<SDL_Event> event) noexcept {
			switch (event.type) {
				case SDL_MOUSEMOTION: {
					previous_position = current_position;
					current_position = offset_t{ event.motion.x, event.motion.y };

					break;
				} case SDL_MOUSEWHEEL: {
					previous_scroll = current_scroll;
					current_scroll = offset_t{ event.wheel.x, event.wheel.y };

					break;
				} default: {
					break;
				}
			}
		}

		static inline input_e at(button_e button) {
			const usize idx{ static_cast<usize>(button) };

			if (previous_state[idx]) {
				return current_state[idx] ? input_e::Pressed : input_e::Up;
			} else {
				return current_state[idx] ? input_e::Down : input_e::Released;
			}
		}

		static inline input_e at(usize button) {
			if (previous_state[button]) {
				return current_state[button] ? input_e::Pressed : input_e::Up;
			} else {
				return current_state[button] ? input_e::Down : input_e::Released;
			}
		}

		template<input_e Input> static inline bool is_button(button_e button) noexcept { return at(button) == Input; }

		template<input_e Input> static inline bool is_button(usize button) noexcept { return at(button) == Input; }

		template<input_e Input> static inline bool any_button() noexcept {
			for (usize i{ ButtonStart }; i <= ButtonEnd; ++i) {
				if (is_button<Input>(i)) {
					return true;
				}
			}

			return false;
		}

		template<input_e Input, Button... Buttons> static inline bool are_buttons(Buttons... buttons) {
			for (button_e button : { buttons... }) {
				if (!is_button<Input>(button)) {
					return false;
				}
			}

			return true;
		}

		template<input_e Input, Button... Buttons> static inline bool any_buttons(Buttons... buttons) {
			for (button_e button : { buttons... }) {
				if (is_button<Input>(button)) {
					return true;
				}
			}

			return false;
		}

		static inline bool is_inside(offset_t position, extent_t size) {
			return current_position.x >= position.x && current_position.y >= position.y && current_position.x <= position.x + size.w && current_position.y <= position.y + size.h;
		}

		static inline offset_t get_position() { return current_position; }

		static inline offset_t get_scroll() { return current_scroll; }

		static inline void show_cursor() { SDL_ShowCursor(SDL_ENABLE); }

		static inline void hide_cursor() { SDL_ShowCursor(SDL_DISABLE); }
	};
} // namespace bleak
