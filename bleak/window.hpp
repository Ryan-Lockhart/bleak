#pragma once

#include "typedef.hpp"

#include <format>
#include <stdexcept>

#include <SDL.h>

#include "bleak/extent.hpp"
#include "bleak/offset.hpp"
#include "gamepad.hpp"
#include "keyboard.hpp"
#include "log.hpp"
#include "mouse.hpp"
#include "point.hpp"
#include "rect.hpp"
#include "subsystem.hpp"

namespace bleak {
	namespace sdl {
		using window = ptr<SDL_Window>;
		using window_flags = SDL_WindowFlags;

		constexpr window_flags WINDOW_FLAGS_NONE { SDL_WINDOW_SHOWN };
		constexpr offset_2d_t WINDOW_POSITION_CENTERED = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };

		static inline window
		create_window(cstr title, extent_2d_t size, offset_2d_t position = WINDOW_POSITION_CENTERED, window_flags flags = WINDOW_FLAGS_NONE) noexcept {
			window handle = SDL_CreateWindow(title, position.x, position.y, size.w, size.h, flags);

			if (handle == nullptr) {
				error_log.add("failed to create window: {}", get_error());
			}

			return handle;
		}

		static inline void destroy_window(ref<window> handle) noexcept {
			if (handle != nullptr) {
				SDL_DestroyWindow(handle);
				handle = nullptr;
			} else {
				error_log.add("cannot destroy window: window handle is nullptr!");
			}
		}
	} // namespace sdl

	class window_t {
	  private:
		sdl::window window;

	  public:
		const cstr title;
		const extent_2d_t size;
		const u32 flags;

	  private:
		bool closing = false;

		inline sdl::window create(cstr title, extent_2d_t size, offset_2d_t position, u32 flags) {

			if (size.w <= 0) {
				throw std::invalid_argument("width must be greater than zero!");
			}
			if (size.h <= 0) {
				throw std::invalid_argument("height must be greater than zero!");
			}

			ptr<SDL_Window> new_window { SDL_CreateWindow(title, position.x, position.y, size.w, size.h, flags) };

			if (new_window == nullptr) {
				throw std::runtime_error(std::format("failed to create window: {}", SDL_GetError()));
			}

			return new_window;
		}

		static inline void destroy(ptr<SDL_Window> window) {
			if (window != nullptr) {
				SDL_DestroyWindow(window);
				window = nullptr;
			}

			GamepadManager::terminate();

			Keyboard::terminate();

			Mouse::terminate();

			Subsystem::terminate();
		}

	  public:
		inline window_t() = delete;

		inline window_t(cstr title, cref<size_t<i32>> size, u32 flags) :
			window { create(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.w, size.h, flags) },
			title { title },
			size { size },
			flags { flags } {}

		inline window_t(cstr title, cref<offset_2d_t> position, cref<size_t<i32>> size, u32 flags) :
			window { create(title, position.x, position.y, size.w, size.h, flags) },
			title { title },
			size { size },
			flags { flags } {}

		inline window_t(cstr title, cref<rect_t<i32>> transform, u32 flags) :
			window { create(title, transform.position.x, transform.position.y, transform.size.w, transform.size.h, flags) },
			title { title },
			size { transform.size },
			flags { flags } {}

		inline window_t(cstr title, i32 width, i32 height, u32 flags) :
			window { create(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags) },
			title { title },
			size { width, height },
			flags { flags } {}

		inline window_t(cstr title, i32 x, i32 y, i32 width, i32 height, u32 flags) :
			window { create(title, x, y, width, height, flags) },
			title { title },
			size { width, height },
			flags { flags } {}

		inline ~window_t() { destroy(window); }

		inline void poll_events() {
			SDL_Event event;

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					closing = true;
					break;

				case SDL_MOUSEMOTION:
				case SDL_MOUSEWHEEL:
					Mouse::process_event(event);
					break;

				case SDL_JOYDEVICEADDED:
				case SDL_JOYDEVICEREMOVED:
					GamepadManager::process_event(event);
					break;

				default:
					break;
				}
			}

			GamepadManager::update();
			Keyboard::update();
			Mouse::update();
		}

		constexpr bool is_closing() const noexcept { return closing; }

		constexpr bool is_running() const noexcept { return !closing; }

		constexpr void close() noexcept { closing = true; }

		constexpr ptr<SDL_Window> handle() noexcept { return window; }

		constexpr cptr<SDL_Window> handle() const noexcept { return window; }

		constexpr offset_2d_t origin() const noexcept { return size_t<i32>::Zero; }

		constexpr offset_2d_t center() const noexcept { return size / 2; }

		constexpr offset_2d_t extents() const noexcept { return size - 1; }

		constexpr rect_t<i32> bounds() const noexcept { return { origin(), extents() }; }
	};
} // namespace bleak
