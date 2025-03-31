#pragma once

#include <bleak/typedef.hpp>

#include <format>
#include <map>

#include <SDL.h>

#include <bleak/cardinal.hpp>
#include <bleak/input.hpp>
#include <bleak/log.hpp>
#include <bleak/steam.hpp>

extern "C" {
	typedef enum {
		SDL_JOYSTICK_BUTTON_CROSS = 0,
		SDL_JOYSTICK_BUTTON_CIRCLE = 1,
		SDL_JOYSTICK_BUTTON_SQUARE = 2,
		SDL_JOYSTICK_BUTTON_TRIANGLE = 3,
		SDL_JOYSTICK_BUTTON_LEFTSTART = 4,
		SDL_JOYSTICK_BUTTON_PLAYSTATION = 5,
		SDL_JOYSTICK_BUTTON_RIGHTSTART = 6,
		SDL_JOYSTICK_BUTTON_LEFTSTICK = 7,
		SDL_JOYSTICK_BUTTON_RIGHTSTICK = 8,
		SDL_JOYSTICK_BUTTON_LEFTSHOULDER = 9,
		SDL_JOYSTICK_BUTTON_RIGHTSHOULDER = 10,
		SDL_JOYSTICK_BUTTON_UP = 11,
		SDL_JOYSTICK_BUTTON_DOWN = 12,
		SDL_JOYSTICK_BUTTON_LEFT = 13,
		SDL_JOYSTICK_BUTTON_RIGHT = 14,
		SDL_JOYSTICK_BUTTON_TOUCHPAD = 15,
		SDL_JOYSTICK_BUTTON_MICROPHONE = 16,
		SDL_NUM_JOYSTICK_BUTTONS = 17
	} SDL_JoystickButton;

	typedef enum {
		SDL_JOYSTICK_AXIS_LEFTX = 0,
		SDL_JOYSTICK_AXIS_LEFTY = 1,
		SDL_JOYSTICK_AXIS_RIGHTX = 2,
		SDL_JOYSTICK_AXIS_RIGHTY = 3,
		SDL_JOYSTICK_AXIS_TRIGGERLEFT = 4,
		SDL_JOYSTICK_AXIS_TRIGGERRIGHT = 5,
		SDL_NUM_JOYSTICK_AXES = 6
	} SDL_JoystickAxis;
}

// steam input translation layer
namespace steam {
	namespace input {
		using namespace bleak;

		static constexpr usize max_devices{ STEAM_INPUT_MAX_COUNT };

		static constexpr usize max_analog_actions{ STEAM_INPUT_MAX_ANALOG_ACTIONS };

		static constexpr usize max_digital_actions{ STEAM_INPUT_MAX_DIGITAL_ACTIONS };

		static constexpr usize max_origins{ STEAM_INPUT_MAX_ORIGINS };

		static constexpr usize max_active_layers{ STEAM_INPUT_MAX_ACTIVE_LAYERS };

		static constexpr usize handle_all_controllers{ STEAM_INPUT_HANDLE_ALL_CONTROLLERS };

		static constexpr f32 min_analog_action_data{ STEAM_INPUT_MIN_ANALOG_ACTION_DATA };
		
		static constexpr f32 max_analog_action_data{ STEAM_INPUT_MAX_ANALOG_ACTION_DATA };

		enum class source_mode : u8 {
			None = EInputSourceMode::k_EInputSourceMode_None,
			Dpad = EInputSourceMode::k_EInputSourceMode_Dpad,
			Buttons = EInputSourceMode::k_EInputSourceMode_Buttons,
			FourButtons = EInputSourceMode::k_EInputSourceMode_FourButtons,
			AbsoluteMouse = EInputSourceMode::k_EInputSourceMode_AbsoluteMouse,
			RelativeMouse = EInputSourceMode::k_EInputSourceMode_RelativeMouse,
			JoystickMove = EInputSourceMode::k_EInputSourceMode_JoystickMove,
			JoystickMouse = EInputSourceMode::k_EInputSourceMode_JoystickMouse,
			JoystickCamera = EInputSourceMode::k_EInputSourceMode_JoystickCamera,
			ScrollWheel = EInputSourceMode::k_EInputSourceMode_ScrollWheel,
			Trigger = EInputSourceMode::k_EInputSourceMode_Trigger,
			TouchMenu = EInputSourceMode::k_EInputSourceMode_TouchMenu,
			MouseJoystick = EInputSourceMode::k_EInputSourceMode_MouseJoystick,
			MouseRegion = EInputSourceMode::k_EInputSourceMode_MouseRegion,
			RadialMenu = EInputSourceMode::k_EInputSourceMode_RadialMenu,
			SingleButton = EInputSourceMode::k_EInputSourceMode_SingleButton,
			Switches = EInputSourceMode::k_EInputSourceMode_Switches
		};

		enum class origin : u8 {
			None,
			SteamController,
			SteamDeck,
			DualShock,
			DualSense,
			Xbox360,
			XboxOne,
			Switch,
			Horipad
		};

		template<origin Origin> struct action;

		template<> struct action<origin::None>{ static constexpr u16 none{ EInputActionOrigin::k_EInputActionOrigin_None }; };

		template<> struct action<origin::SteamController>{
			static constexpr u16 a{ EInputActionOrigin::k_EInputActionOrigin_SteamController_A };
			static constexpr u16 b{ EInputActionOrigin::k_EInputActionOrigin_SteamController_B };
			static constexpr u16 x{ EInputActionOrigin::k_EInputActionOrigin_SteamController_X };
			static constexpr u16 y{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Y };
			static constexpr u16 left_bumper{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftBumper };
			static constexpr u16 right_bumper{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightBumper };
			static constexpr u16 left_grip{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftGrip };
			static constexpr u16 right_grip{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightGrip };
			static constexpr u16 start{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Start };
			static constexpr u16 back{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Back };
			static constexpr u16 left_pad_touch{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftPad_Touch };
			static constexpr u16 left_pad_swipe{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftPad_Swipe };
			static constexpr u16 left_pad_click{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftPad_Click };
			static constexpr u16 left_pad_d_pad_north{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftPad_DPadNorth };
			static constexpr u16 left_pad_d_pad_south{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftPad_DPadSouth };
			static constexpr u16 left_pad_d_pad_west{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftPad_DPadWest };
			static constexpr u16 left_pad_d_pad_east{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftPad_DPadEast };
			static constexpr u16 right_pad_touch{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightPad_Touch };
			static constexpr u16 right_pad_swipe{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightPad_Swipe };
			static constexpr u16 right_pad_click{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightPad_Click };
			static constexpr u16 right_pad_d_pad_north{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightPad_DPadNorth };
			static constexpr u16 right_pad_d_pad_south{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightPad_DPadSouth };
			static constexpr u16 right_pad_d_pad_west{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightPad_DPadWest };
			static constexpr u16 right_pad_d_pad_east{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightPad_DPadEast };
			static constexpr u16 left_trigger_pull{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftTrigger_Pull };
			static constexpr u16 left_trigger_click{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftTrigger_Click };
			static constexpr u16 right_trigger_pull{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightTrigger_Pull };
			static constexpr u16 right_trigger_click{ EInputActionOrigin::k_EInputActionOrigin_SteamController_RightTrigger_Click };
			static constexpr u16 left_stick_move{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftStick_Move };
			static constexpr u16 left_stick_click{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftStick_Click };
			static constexpr u16 left_stick_d_pad_north{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftStick_DPadNorth };
			static constexpr u16 left_stick_d_pad_south{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftStick_DPadSouth };
			static constexpr u16 left_stick_d_pad_west{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftStick_DPadWest };
			static constexpr u16 left_stick_d_pad_east{ EInputActionOrigin::k_EInputActionOrigin_SteamController_LeftStick_DPadEast };
			static constexpr u16 gyro_move{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Gyro_Move };
			static constexpr u16 gyro_pitch{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Gyro_Pitch };
			static constexpr u16 gyro_yaw{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Gyro_Yaw };
			static constexpr u16 gyro_roll{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Gyro_Roll };
			static constexpr u16 reserved0{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved0 };
			static constexpr u16 reserved1{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved1 };
			static constexpr u16 reserved2{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved2 };
			static constexpr u16 reserved3{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved3 };
			static constexpr u16 reserved4{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved4 };
			static constexpr u16 reserved5{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved5 };
			static constexpr u16 reserved6{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved6 };
			static constexpr u16 reserved7{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved7 };
			static constexpr u16 reserved8{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved8 };
			static constexpr u16 reserved9{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved9 };
			static constexpr u16 reserved10{ EInputActionOrigin::k_EInputActionOrigin_SteamController_Reserved10 };
		};
	} // namespace input
} // namespace steam

// native sdl gamepad input implementation
namespace bleak {
	using joystick_t = SDL_Joystick;

	struct gamepad_t {
		using button_t = SDL_JoystickButton;
		using axis_t = SDL_JoystickAxis;

		static constexpr i16 JOYSTICK_DEAD_ZONE{ 8000 };

		struct stick_t {
		  private:
			static inline constexpr cardinal_t to_cardinal(i16 x, i16 y) noexcept {
				cardinal_t result{ cardinal_e::Central };

				if (x < -JOYSTICK_DEAD_ZONE) {
					result += cardinal_e::West;
				}
				if (x > JOYSTICK_DEAD_ZONE) {
					result += cardinal_e::East;
				}

				if (y < -JOYSTICK_DEAD_ZONE) {
					result += cardinal_e::North;
				}
				if (y > JOYSTICK_DEAD_ZONE) {
					result += cardinal_e::South;
				}

				return result;
			}

			static inline constexpr cardinal_t to_cardinal(i16 x, i16 y, i16 deadzone) noexcept {
				cardinal_t result{ cardinal_e::Central };

				if (x < -deadzone) {
					result += cardinal_e::West;
				}
				if (x > deadzone) {
					result += cardinal_e::East;
				}

				if (y < -deadzone) {
					result += cardinal_e::North;
				}
				if (y > deadzone) {
					result += cardinal_e::South;
				}

				return result;
			}

			inline cardinal_t get_state() const noexcept {
				i16 x{ SDL_JoystickGetAxis(joystick, x_axis_id) };
				i16 y{ SDL_JoystickGetAxis(joystick, y_axis_id) };

				return to_cardinal(x, y);
			}

		  public:
			inline stick_t(ptr<joystick_t> joystick, axis_t x_axis_id, axis_t y_axis_id) noexcept : joystick{ joystick }, x_axis_id{ x_axis_id }, y_axis_id{ y_axis_id } {
				if (joystick == nullptr) {
					error_log.add("nullptr passed to stick [{}, {}] constructor\n", (i32)x_axis_id, (i32)y_axis_id);
				}
			}

			ptr<joystick_t> joystick;
			axis_t x_axis_id, y_axis_id;

			cardinal_t current_state;
			cardinal_t previous_state;

			inline constexpr void update() noexcept {
				previous_state = current_state;
				current_state = get_state();
			}
		};

		struct buttons_t {
			inline buttons_t(ptr<joystick_t> joystick) noexcept : joystick{ joystick } {
				if (joystick == nullptr) {
					error_log.add("nullptr passed to buttons constructor\n");
				}
			}

			ptr<joystick_t> joystick;

			bool current_state[SDL_NUM_JOYSTICK_BUTTONS];
			bool previous_state[SDL_NUM_JOYSTICK_BUTTONS];

			inline constexpr void update() noexcept {
				for (usize i{ 0 }; i < SDL_NUM_JOYSTICK_BUTTONS; ++i) {
					previous_state[i] = current_state[i];
					current_state[i] = SDL_JoystickGetButton(joystick, i);
				}
			}

			inline input_state_t at(button_t button) const noexcept {
				if (previous_state[button]) {
					return current_state[button] ? input_state_t::Pressed : input_state_t::Up;
				} else {
					return current_state[button] ? input_state_t::Down : input_state_t::Released;
				}
			}

			inline input_state_t at(int button) const noexcept {
				if (previous_state[button]) {
					return current_state[button] ? input_state_t::Pressed : input_state_t::Up;
				} else {
					return current_state[button] ? input_state_t::Down : input_state_t::Released;
				}
			}
		};

		struct dpad_t {
		  private:
			static inline constexpr cardinal_t to_cardinal(cref<buttons_t> buttons) noexcept {
				cardinal_t result{ cardinal_e::Central };

				if (buttons.current_state[SDL_JOYSTICK_BUTTON_UP]) {
					result += cardinal_e::North;
				} else if (buttons.current_state[SDL_JOYSTICK_BUTTON_DOWN]) {
					result += cardinal_e::South;
				}

				if (buttons.current_state[SDL_JOYSTICK_BUTTON_LEFT]) {
					result += cardinal_e::West;
				} else if (buttons.current_state[SDL_JOYSTICK_BUTTON_RIGHT]) {
					result += cardinal_e::East;
				}

				return result;
			}

			inline cardinal_t get_state(cref<buttons_t> buttons) const noexcept { return to_cardinal(buttons); }

		  public:
			inline dpad_t(ptr<joystick_t> joystick) noexcept : joystick{ joystick } {
				if (joystick == nullptr) {
					error_log.add("nullptr passed to dpad constructor\n");
				}
			}

			ptr<joystick_t> joystick;

			cardinal_t current_state;
			cardinal_t previous_state;

			inline constexpr void update(cref<buttons_t> buttons) noexcept {
				previous_state = current_state;
				current_state = get_state(buttons);
			}
		};

		buttons_t buttons;

		stick_t left_stick;
		stick_t right_stick;

		dpad_t dpad;

		ptr<joystick_t> joystick;

		inline gamepad_t(joystick_t* joystick) noexcept :
			buttons{ joystick },
			left_stick{ joystick, SDL_JOYSTICK_AXIS_LEFTX, SDL_JOYSTICK_AXIS_LEFTY },
			right_stick{ joystick, SDL_JOYSTICK_AXIS_RIGHTX, SDL_JOYSTICK_AXIS_RIGHTY },
			dpad{ joystick },
			joystick{ joystick } {
			if (joystick == nullptr) {
				error_log.add("nullptr passed to gamepad constructor\n");
			}
		}

		inline gamepad_t(cref<gamepad_t> other) noexcept : buttons{ other.buttons }, left_stick{ other.left_stick }, right_stick{ other.right_stick }, dpad{ other.dpad }, joystick{ other.joystick } {}

		inline gamepad_t(rval<gamepad_t> other) noexcept :
			buttons{ std::move(other.buttons) },
			left_stick{ std::move(other.left_stick) },
			right_stick{ std::move(other.right_stick) },
			dpad{ std::move(other.dpad) },
			joystick{ std::move(other.joystick) } {
			other.joystick = nullptr;
		}

		inline ~gamepad_t() noexcept {
			if (joystick != nullptr) {
				SDL_JoystickClose(joystick);
				joystick = nullptr;
			}
		}

		inline void update() noexcept {
			buttons.update();
			dpad.update(buttons);

			left_stick.update();
			right_stick.update();
		}

		inline constexpr std::string power_level() const noexcept {
			SDL_JoystickPowerLevel level{ SDL_JoystickCurrentPowerLevel(joystick) };

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

		inline bool is_button_pressed(button_t button) const noexcept { return buttons.at(button) == input_state_t::Pressed; }

		inline bool is_button_pressed(int button) const noexcept { return buttons.at(button) == input_state_t::Pressed; }

		inline bool is_button_released(button_t button) const noexcept { return buttons.at(button) == input_state_t::Released; }

		inline bool is_button_released(int button) const noexcept { return buttons.at(button) == input_state_t::Released; }

		inline bool is_button_down(button_t button) const noexcept { return buttons.at(button) == input_state_t::Down; }

		inline bool is_button_down(int button) const noexcept { return buttons.at(button) == input_state_t::Down; }

		inline bool is_button_up(button_t button) const noexcept { return buttons.at(button) == input_state_t::Up; }

		inline bool is_button_up(int button) const noexcept { return buttons.at(button) == input_state_t::Up; }

		inline bool any_button_pressed() const noexcept {
			for (int i{ 0 }; i < SDL_NUM_JOYSTICK_BUTTONS; ++i) {
				if (is_button_pressed(i)) {
					return true;
				}
			}

			return false;
		}

		inline bool any_button_released() const noexcept {
			for (int i{ 0 }; i < SDL_NUM_JOYSTICK_BUTTONS; ++i) {
				if (is_button_released(i)) {
					return true;
				}
			}

			return false;
		}

		inline bool any_button_down() const noexcept {
			for (int i{ 0 }; i < SDL_NUM_JOYSTICK_BUTTONS; ++i) {
				if (is_button_down(i)) {
					return true;
				}
			}

			return false;
		}

		inline bool any_button_up() const noexcept {
			for (int i{ 0 }; i < SDL_NUM_JOYSTICK_BUTTONS; ++i) {
				if (is_button_up(i)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> inline bool are_buttons_pressed(Buttons... buttons) const noexcept {
			for (button_t button : { buttons... }) {
				if (!is_button_pressed(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> inline bool are_buttons_released(Buttons... buttons) const noexcept {
			for (button_t button : { buttons... }) {
				if (!is_button_released(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> inline bool are_buttons_down(Buttons... buttons) const noexcept {
			for (button_t button : { buttons... }) {
				if (!is_button_down(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> inline bool are_buttons_up(Buttons... buttons) const noexcept {
			for (button_t button : { buttons... }) {
				if (!is_button_up(button)) {
					return false;
				}
			}

			return true;
		}

		template<typename... Buttons, typename = button_t> inline bool any_buttons_pressed(Buttons... buttons) const noexcept {
			for (button_t button : { buttons... }) {
				if (is_button_pressed(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> inline bool any_buttons_released(Buttons... buttons) const noexcept {
			for (button_t button : { buttons... }) {
				if (is_button_released(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> inline bool any_buttons_down(Buttons... buttons) const noexcept {
			for (button_t button : { buttons... }) {
				if (is_button_down(button)) {
					return true;
				}
			}

			return false;
		}

		template<typename... Buttons, typename = button_t> inline bool any_buttons_up(Buttons... buttons) const noexcept {
			for (button_t button : { buttons... }) {
				if (is_button_up(button)) {
					return true;
				}
			}

			return false;
		}
	};

	struct gamepad_slot_t {
		ptr<gamepad_t> gamepad;

		fn_ptr<void> disconnect_callback{ nullptr };
		fn_ptr<void, cptr<gamepad_t>> reconnect_callback{ nullptr };

		inline bool has_callbacks() const noexcept { return disconnect_callback != nullptr || reconnect_callback != nullptr; }

		inline void invoke_disconnect() const noexcept {
			if (disconnect_callback != nullptr) {
				disconnect_callback();
			}
		}

		inline void invoke_reconnect() const noexcept {
			if (reconnect_callback != nullptr) {
				reconnect_callback(gamepad);
			}
		}

		inline gamepad_slot_t(ptr<joystick_t> joystick) : gamepad{ new gamepad_t{ joystick } } {}

		inline gamepad_slot_t(fn_ptr<void> disconnect_callback, fn_ptr<void, cptr<gamepad_t>> reconnect_callback) :
			gamepad{ nullptr },
			disconnect_callback{ disconnect_callback },
			reconnect_callback{ reconnect_callback } {}

		inline gamepad_slot_t(ptr<joystick_t> joystick, fn_ptr<void> disconnect_callback, fn_ptr<void, cptr<gamepad_t>> reconnect_callback) :
			gamepad{ new gamepad_t{ joystick } },
			disconnect_callback{ disconnect_callback },
			reconnect_callback{ reconnect_callback } {}

		inline gamepad_slot_t(cref<gamepad_slot_t> other) noexcept : gamepad{ other.gamepad }, disconnect_callback{ other.disconnect_callback }, reconnect_callback{ other.reconnect_callback } {}

		inline gamepad_slot_t(rval<gamepad_slot_t> other) noexcept : gamepad{ std::move(other.gamepad) }, disconnect_callback{ std::move(other.disconnect_callback) }, reconnect_callback{ std::move(other.reconnect_callback) } {}

		inline ~gamepad_slot_t() noexcept {
			if (gamepad != nullptr) {
				delete gamepad;
				gamepad = nullptr;

				invoke_disconnect();
			}
		}
	};

	struct GamepadManager {
	  private:
		static inline std::map<i32, gamepad_slot_t> slots;

		static inline bool initialized;

	  public:
		static inline bool is_initialized() noexcept { return initialized; }

		static inline void initialize() noexcept {
			if (initialized) {
				return;
			}

			if (SDL_WasInit(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) == 0 && SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) {
				error_log.add("failed to initialize game controller subsystem: {}\n", SDL_GetError());
				return;
			}

			int num_controllers{ SDL_NumJoysticks() };

			for (i32 i{ 0 }; i < num_controllers; ++i) {
				if (!SDL_IsGameController(i)) {
					error_log.add("joystick {} is not a game controller\n", i);
					continue;
				}

				ptr<joystick_t> gamepad{ SDL_JoystickOpen(i) };

				if (gamepad == nullptr) {
					error_log.add("failed to open joystick {}: {}\n", i, SDL_GetError());
					continue;
				}

				slots.emplace(i, gamepad);
			}

			initialized = true;
		}

		static inline void terminate() noexcept {
			if (!initialized) {
				return;
			}

			slots.clear();

			SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

			initialized = false;
		}

		static inline cptr<gamepad_t> lease(i32 id, fn_ptr<void> disconnected_callback, fn_ptr<void, cptr<gamepad_t>> reconnected_callback) noexcept {
			if (disconnected_callback == nullptr || reconnected_callback == nullptr) {
				error_log.add("a callback was not provided for gamepad lease!\n");
				return nullptr;
			}

			auto iter{ slots.find(id) };

			// should allow for preemptive leasing
			if (iter == slots.end()) {
				slots.emplace(id, gamepad_slot_t{ disconnected_callback, reconnected_callback });
				return nullptr;
			}

			ref<gamepad_slot_t> target{ iter->second };

			if (target.has_callbacks()) {
				error_log.add("attempting to lease gamepad slot that is already leased!\n");
				return nullptr;
			}

			target.disconnect_callback = disconnected_callback;
			target.reconnect_callback = reconnected_callback;

			return target.gamepad;
		}

		static inline void release(i32 id) noexcept {
			auto iter{ slots.find(id) };

			if (iter == slots.end()) {
				error_log.add("attempting to release gamepad slot that does not exist!\n");
				return;
			}

			if (!iter->second.has_callbacks()) {
				error_log.add("attempting to release gamepad slot that is not leased!\n");
				return;
			}

			iter->second.disconnect_callback = nullptr;
			iter->second.reconnect_callback = nullptr;
		}

		static inline bool add_joystick(int id) noexcept;
		static inline bool remove_joystick(int id) noexcept;

		static inline void update() noexcept {
			for (auto& [id, slot] : slots) {
				if (slot.gamepad != nullptr) {
					slot.gamepad->update();
				}
			}
		}

		static inline void process_event(ref<SDL_Event> event) noexcept {
			switch (event.type) {
			case SDL_JOYDEVICEADDED: {
				if (!add_joystick(event.jdevice.which)) {
					error_log.add("failed to register joystick {}\n", event.jdevice.which);
				}
			} break;

			case SDL_JOYDEVICEREMOVED: {
				if (!remove_joystick(event.jdevice.which)) {
					error_log.add("failed to unregister joystick {}\n", event.jdevice.which);
				}
			} break;

			default:
				break;
			}
		}
	};

	inline bool GamepadManager::add_joystick(int id) noexcept {
		auto it{ slots.find(id) };

		if (it != slots.end() && it->second.gamepad != nullptr) {
			error_log.add("joystick {} already connected\n", id);
			return false;
		}

		ptr<joystick_t> gamepad{ SDL_JoystickOpen(id) };

		if (gamepad == nullptr) {
			error_log.add("failed to open joystick {}: {}\n", id, SDL_GetError());
			return false;
		}

		if (it != slots.end()) {
			it->second.gamepad = new gamepad_t{ gamepad };
			it->second.invoke_reconnect();
			return true;
		} else {
			slots.emplace(id, gamepad);
		}

		return true;
	}

	inline bool GamepadManager::remove_joystick(int id) noexcept {
		auto it{ slots.find(id) };

		if (it == slots.end()) {
			error_log.add("joystick {} not found\n", id);
			return false;
		}

		if (it->second.gamepad == nullptr) {
			error_log.add("joystick {} already disconnected\n", id);
			return false;
		}

		delete it->second.gamepad;
		it->second.gamepad = nullptr;
		it->second.invoke_disconnect();

		return true;
	}
} // namespace bleak
