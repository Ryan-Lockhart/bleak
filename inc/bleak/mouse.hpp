#pragma once

#include <bleak/typedef.hpp>

#include <bitset>

#include <SDL.h>

#include <bleak/input.hpp>
#include <bleak/offset.hpp>

namespace bleak {
	struct Mouse {
	  public:
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

			static constexpr usize Count{ SDL_BUTTON_X2 + 1 };

			constexpr inline button_t() noexcept = delete;

			constexpr inline button_t(i32 value) noexcept : value{ value } {}

			constexpr inline operator u8() const noexcept { return value; }

			constexpr inline bool operator==(button_t other) const noexcept { return value == other.value; }

			constexpr inline bool operator!=(button_t other) const noexcept { return value != other.value; }

			constexpr inline operator std::string() const;
		};

	  private:
		static inline std::bitset<button_t::Count> current_state;
		static inline std::bitset<button_t::Count> previous_state;

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

			for (usize i{ button_t::Start }; i <= button_t::End; ++i) {
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
			for (int i{ button_t::Start }; i <= button_t::End; ++i) {
				if (is_button_pressed(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_button_released() {
			for (int i{ button_t::Start }; i <= button_t::End; ++i) {
				if (is_button_released(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_button_down() {
			for (int i{ button_t::Start }; i <= button_t::End; ++i) {
				if (is_button_down(i)) {
					return true;
				}
			}

			return false;
		}

		static inline bool any_button_up() {
			for (int i{ button_t::Start }; i <= button_t::End; ++i) {
				if (is_button_up(i)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool are_buttons_pressed(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!is_button_pressed(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool are_buttons_released(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!is_button_released(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool are_buttons_down(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!is_button_down(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool are_buttons_up(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (!is_button_up(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> static inline bool any_buttons_pressed(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (is_button_pressed(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool any_buttons_released(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (is_button_released(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool any_buttons_down(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (is_button_down(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> static inline bool any_buttons_up(Buttons... buttons) {
			for (button_t button : { buttons... }) {
				if (is_button_up(button)) {
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

	constexpr const Mouse::button_t Mouse::button_t::Unknown{ 0 };

	constexpr const Mouse::button_t Mouse::button_t::Left{ SDL_BUTTON_LEFT };
	constexpr const Mouse::button_t Mouse::button_t::Middle{ SDL_BUTTON_MIDDLE };
	constexpr const Mouse::button_t Mouse::button_t::Right{ SDL_BUTTON_RIGHT };

	constexpr const Mouse::button_t Mouse::button_t::SideOne{ SDL_BUTTON_X1 };
	constexpr const Mouse::button_t Mouse::button_t::SideTwo{ SDL_BUTTON_X2 };

	constexpr const Mouse::button_t Mouse::button_t::Start = button_t::Left;
	constexpr const Mouse::button_t Mouse::button_t::End = button_t::SideTwo;

	constexpr Mouse::button_t::operator std::string() const {
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

	inline bool Mouse::is_button_pressed(button_t button) { return Mouse::at(button) == input_state_t::Pressed; }

	inline bool Mouse::is_button_pressed(int button) { return Mouse::at(button) == input_state_t::Pressed; }

	inline bool Mouse::is_button_released(button_t button) { return Mouse::at(button) == input_state_t::Released; }

	inline bool Mouse::is_button_released(int button) { return Mouse::at(button) == input_state_t::Released; }

	inline bool Mouse::is_button_down(button_t button) { return Mouse::at(button) == input_state_t::Down; }

	inline bool Mouse::is_button_down(int button) { return Mouse::at(button) == input_state_t::Down; }

	inline bool Mouse::is_button_up(button_t button) { return Mouse::at(button) == input_state_t::Up; }

	inline bool Mouse::is_button_up(int button) { return Mouse::at(button) == input_state_t::Up; }
} // namespace bleak
