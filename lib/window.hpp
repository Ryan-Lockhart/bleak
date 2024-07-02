#pragma once

#include "typedef.hpp"

#include <format>
#include <stdexcept>

#include <SDL.h>

#include "gamepad.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "point.hpp"
#include "rect.hpp"
#include "subsystem.hpp"

namespace Bleakdepth {
	class window_t {
	  private:
		ptr<SDL_Window> window;

	  public:
		const cstr title;
		const size_t<i32> size;
		const u32 flags;

	  private:
		bool closing = false;

		static inline ptr<SDL_Window> create(cstr title, i32 x, i32 y, i32 width, i32 height, u32 flags) {
			if (!Subsystem::is_initialized()) {
				Subsystem::initialize();
			}

			if (!Mouse::is_initialized()) {
				Mouse::initialize();
			}

			if (!Keyboard::is_initialized()) {
				Keyboard::initialize();
			}

			if (width <= 0) {
				throw std::invalid_argument("width must be greater than zero!");
			}
			if (height <= 0) {
				throw std::invalid_argument("height must be greater than zero!");
			}

			ptr<SDL_Window> new_window { SDL_CreateWindow(title, x, y, width, height, flags) };

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

			if (Subsystem::is_initialized()) {
				Subsystem::terminate();
			}
		}

	  public:
		inline window_t() = delete;

		inline window_t(cstr title, cref<size_t<i32>> size, u32 flags) :
			window { create(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.w, size.h, flags) },
			title { title },
			size { size },
			flags { flags } {}

		inline window_t(cstr title, cref<point_t<i32>> position, cref<size_t<i32>> size, u32 flags) :
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

		constexpr point_t<i32> origin() const noexcept { return size_t<i32>::Zero; }

		constexpr point_t<i32> center() const noexcept { return size / 2; }

		constexpr point_t<i32> extents() const noexcept { return size - 1; }

		constexpr rect_t<i32> bounds() const noexcept { return { origin(), extents() }; }
	};
} // namespace Bleakdepth
