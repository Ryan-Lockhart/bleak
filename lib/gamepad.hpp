#pragma once

#include "SDL_error.h"
#include "typedef.hpp"

#include <format>
#include <map>
#include <vector>

#include <SDL.h>

#include "cardinal.hpp"
#include "input.hpp"
#include "log.hpp"
#include "point.hpp"

namespace Bleakdepth {
	struct gamepad_t {
	  public:
		struct axis_t {
			inline axis_t(i32 id, cptrc<SDL_Joystick> joystick) noexcept : joystick { joystick }, id { id } {
				if (joystick == nullptr) {
					error_log.add("nullptr passed to axis {} constructor\n", id);
				}
			}

			cptrc<SDL_Joystick> joystick;
			i32 id;

			i16 current_state;
			i16 previous_state;

			inline constexpr void update() noexcept {
				previous_state = current_state;
				current_state = SDL_JoystickGetAxis(const_cast<ptr<SDL_Joystick>>(joystick), id);
			}

			inline constexpr i16 delta() const noexcept { return current_state - previous_state; }
		};

		struct hat_t {
		  private:
			static inline constexpr cardinal_t to_cardinal(i32 value) noexcept {
				switch (value) {
				case SDL_HAT_CENTERED:
					return cardinal_t::Central;
				case SDL_HAT_UP:
					return cardinal_t::North;
				case SDL_HAT_RIGHT:
					return cardinal_t::East;
				case SDL_HAT_DOWN:
					return cardinal_t::South;
				case SDL_HAT_LEFT:
					return cardinal_t::West;
				case SDL_HAT_RIGHTUP:
					return cardinal_t::Northeast;
				case SDL_HAT_RIGHTDOWN:
					return cardinal_t::Southeast;
				case SDL_HAT_LEFTUP:
					return cardinal_t::Northwest;
				case SDL_HAT_LEFTDOWN:
					return cardinal_t::Southwest;
				default:
					return cardinal_t::Central;
				}
			}

			inline cardinal_t get_state() const noexcept { return to_cardinal(SDL_JoystickGetHat(const_cast<ptr<SDL_Joystick>>(joystick), id)); }

		  public:
			inline hat_t(i32 id, cptrc<SDL_Joystick> joystick) noexcept : joystick { joystick }, id { id } {
				if (joystick == nullptr) {
					error_log.add("nullptr passed to hat {} constructor\n", id);
				}
			}

			cptrc<SDL_Joystick> joystick;
			i32 id;

			cardinal_t current_state;
			cardinal_t previous_state;

			inline constexpr void update() noexcept {
				previous_state = current_state;
				current_state = get_state();
			}
		};

		struct ball_t {
			inline ball_t(i32 id, cptrc<SDL_Joystick> joystick) noexcept : joystick { joystick }, id { id }, delta {} {
				if (joystick == nullptr) {
					error_log.add("nullptr passed to ball {} constructor\n", id);
				}
			}

			cptrc<SDL_Joystick> joystick;
			i32 id;

			point_t<i32> delta;

			inline constexpr void update() noexcept {
				if (!SDL_JoystickGetBall(const_cast<ptr<SDL_Joystick>>(joystick), id, &delta.x, &delta.y)) {
					error_log.add("failed to get ball {} state\n", id);
				}
			}
		};

		struct button_t {
			inline button_t(i32 id, cptrc<SDL_Joystick> joystick) noexcept : joystick { joystick }, id { id } {
				if (joystick == nullptr) {
					error_log.add("nullptr passed to button {} constructor\n", id);
				}
			}

			cptrc<SDL_Joystick> joystick;
			i32 id;

			bool current_state;
			bool previous_state;

			inline constexpr void update() noexcept {
				previous_state = current_state;
				current_state = SDL_JoystickGetButton(const_cast<ptr<SDL_Joystick>>(joystick), id);
			}

			inline constexpr input_state_t state() const noexcept {
				if (previous_state) {
					return current_state ? input_state_t::Pressed : input_state_t::Up;
				} else {
					return current_state ? input_state_t::Down : input_state_t::Released;
				}
			}
		};

	  private:
		std::vector<axis_t> axes;
		std::vector<hat_t> hats;
		std::vector<ball_t> balls;
		std::vector<button_t> buttons;

		ptr<SDL_Joystick> joystick;
		i32 id;

		static inline constexpr usize get_num_axes(ptr<SDL_Joystick> joystick) noexcept { return SDL_JoystickNumAxes(joystick); }

		static inline constexpr usize get_num_hats(ptr<SDL_Joystick> joystick) noexcept { return SDL_JoystickNumHats(joystick); }

		static inline constexpr usize get_num_balls(ptr<SDL_Joystick> joystick) noexcept { return SDL_JoystickNumBalls(joystick); }

		static inline constexpr usize get_num_buttons(ptr<SDL_Joystick> joystick) noexcept { return SDL_JoystickNumButtons(joystick); }

	  public:
		enum class gamepad_input_t : u8 { Axis, Hat, Ball, Button };

		inline gamepad_t(int id, SDL_Joystick* joystick) noexcept : axes {}, hats {}, balls {}, buttons {}, joystick { joystick }, id { id } {
			if (joystick == nullptr) {
				error_log.add("nullptr passed to gamepad {} constructor\n", id);
			}

			usize num_axes { get_num_axes(joystick) };
			usize num_hats { get_num_hats(joystick) };
			usize num_balls { get_num_balls(joystick) };
			usize num_buttons { get_num_buttons(joystick) };

			for (usize i { 0 }; i < num_axes; ++i) {
				axes.emplace_back(i, joystick);
			}

			for (usize i { 0 }; i < num_hats; ++i) {
				hats.emplace_back(i, joystick);
			}

			for (usize i { 0 }; i < num_balls; ++i) {
				balls.emplace_back(i, joystick);
			}

			for (usize i { 0 }; i < num_buttons; ++i) {
				buttons.emplace_back(i, joystick);
			}
		}

		inline gamepad_t(cref<gamepad_t> other) noexcept :
			axes { other.axes },
			hats { other.hats },
			balls { other.balls },
			buttons { other.buttons },
			joystick { other.joystick },
			id { other.id } {}

		inline gamepad_t(rval<gamepad_t> other) noexcept :
			axes { std::move(other.axes) },
			hats { std::move(other.hats) },
			balls { std::move(other.balls) },
			buttons { std::move(other.buttons) },
			joystick { other.joystick },
			id { other.id } {
			other.joystick = nullptr;
		}

		inline ~gamepad_t() noexcept {
			if (joystick != nullptr) {
				if (!axes.empty()) {
					axes.clear();
				}

				if (!hats.empty()) {
					hats.clear();
				}

				if (!balls.empty()) {
					balls.clear();
				}

				if (!buttons.empty()) {
					buttons.clear();
				}

				SDL_JoystickClose(joystick);
				joystick = nullptr;
			}
		}

		inline void update() noexcept {
			for (auto& axis : axes) {
				axis.update();
			}

			for (auto& hat : hats) {
				hat.update();
			}

			for (auto& ball : balls) {
				ball.update();
			}

			for (auto& button : buttons) {
				button.update();
			}
		}

		template<gamepad_input_t Input, typename T> inline constexpr cref<T> at(int index) const noexcept;

		template<> inline constexpr cref<axis_t> at<gamepad_input_t::Axis, axis_t>(int index) const noexcept { return axes.at(index); }

		template<> inline constexpr cref<hat_t> at<gamepad_input_t::Hat, hat_t>(int index) const noexcept { return hats.at(index); }

		template<> inline constexpr cref<ball_t> at<gamepad_input_t::Ball, ball_t>(int index) const noexcept { return balls.at(index); }

		template<> inline constexpr cref<button_t> at<gamepad_input_t::Button, button_t>(int index) const noexcept { return buttons.at(index); }

		inline constexpr i32 get_id() const noexcept { return id; }

		inline constexpr std::string power_level() const noexcept {
			SDL_JoystickPowerLevel level { SDL_JoystickCurrentPowerLevel(joystick) };

			switch (level) {
			case SDL_JOYSTICK_POWER_UNKNOWN:
				return "unknown";
			case SDL_JOYSTICK_POWER_EMPTY:
				return "empty";
			case SDL_JOYSTICK_POWER_LOW:
				return "low";
			case SDL_JOYSTICK_POWER_MEDIUM:
				return "medium";
			case SDL_JOYSTICK_POWER_FULL:
				return "full";
			case SDL_JOYSTICK_POWER_WIRED:
				return "wired";
			default:
				return "unknown";
			}
		}

		inline constexpr usize axis_count() const noexcept { return axes.size(); }

		inline constexpr usize hat_count() const noexcept { return hats.size(); }

		inline constexpr usize ball_count() const noexcept { return balls.size(); }

		inline constexpr usize button_count() const noexcept { return buttons.size(); }
	};

	struct GamepadManager {
	  private:
		static inline std::map<i32, gamepad_t> gamepads;
		static inline bool initialized;

	  public:
		static inline bool is_initialized() noexcept { return initialized; }

		static inline void initialize() noexcept {
			if (initialized) {
				return;
			}

			if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0 && SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
				error_log.add("failed to initialize game controller subsystem: {}\n", SDL_GetError());
				return;
			}

			int num_controllers { SDL_NumJoysticks() };

			for (i32 i { 0 }; i < num_controllers; ++i) {
				if (!SDL_IsGameController(i)) {
					error_log.add("joystick {} is not a game controller\n", i);
					continue;
				}

				SDL_Joystick* gamepad { SDL_JoystickOpen(i) };

				if (gamepad == nullptr) {
					error_log.add("failed to open joystick {}: {}\n", i, SDL_GetError());
					continue;
				}

				gamepads.emplace(i, gamepad_t { i, gamepad });
			}

			initialized = true;
		}

		static inline void terminate() noexcept {
			if (!initialized) {
				return;
			}

			gamepads.clear();

			SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);

			initialized = false;
		}

		static inline cptrc<gamepad_t> at(i32 id) noexcept {
			auto it { gamepads.find(id) };

			if (it == gamepads.end()) {
				error_log.add("joystick {} not found\n", id);
				return nullptr;
			}

			return &it->second;
		}

		static inline bool add_joystick(int id) noexcept;
		static inline bool remove_joystick(int id) noexcept;

		static inline void update() noexcept {
			for (auto& [id, gamepad] : gamepads) {
				gamepad.update();
			}
		}

		static inline void process_event(ref<SDL_Event> event) {
			switch (event.type) {
			case SDL_CONTROLLERDEVICEADDED: {
				if (!add_joystick(event.cdevice.which)) {
					error_log.add("failed to register joystick {}\n", event.cdevice.which);
				}
			} break;

			case SDL_CONTROLLERDEVICEREMOVED: {
				if (!remove_joystick(event.cdevice.which)) {
					error_log.add("failed to unregister joystick {}\n", event.cdevice.which);
				}
			} break;

			default:
				break;
			}
		}

		static inline void announce_gamepads() noexcept {
			if (gamepads.empty()) {
				message_log.add("no gamepads connected\n");
				return;
			}

			message_log.add("gamepad count: {}\n\n", gamepads.size());

			for (auto& [id, gamepad] : gamepads) {
				message_log.add(
					"gamepad {}:\n\tpower level: {},\n\tnumber of axes: {},\n\tnumber of hats: {},\n\tnumber of balls: {},\n\tnumber of buttons: {}\n\n",
					id,
					gamepad.power_level(),
					gamepad.axis_count(),
					gamepad.hat_count(),
					gamepad.ball_count(),
					gamepad.button_count()
				);
			}
		}
	};

	inline bool GamepadManager::add_joystick(int id) noexcept {
		auto it { gamepads.find(id) };

		if (it != gamepads.end()) {
			error_log.add("joystick {} already connected\n", id);
			return false;
		}

		if (!SDL_IsGameController(id)) {
			error_log.add("joystick {} is not supported\n", id);
			return false;
		}

		SDL_Joystick* gamepad { SDL_JoystickOpen(id) };

		if (gamepad == nullptr) {
			error_log.add("failed to open joystick {}: {}\n", id, SDL_GetError());
			return false;
		}

		gamepads.emplace(id, gamepad_t { id, gamepad });

		return true;
	}

	inline bool GamepadManager::remove_joystick(int id) noexcept {
		auto it { gamepads.find(id) };

		if (it == gamepads.end()) {
			error_log.add("joystick {} not found\n", id);
			return false;
		}

		gamepads.erase(it);

		return true;
	}
} // namespace Bleakdepth
