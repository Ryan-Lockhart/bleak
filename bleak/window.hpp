#pragma once

#include "bleak/typedef.hpp"

#include <format>

#include <SDL.h>

#include "bleak/extent.hpp"
#include "bleak/gamepad.hpp"
#include "bleak/keyboard.hpp"
#include "bleak/log.hpp"
#include "bleak/mouse.hpp"
#include "bleak/offset.hpp"
#include "bleak/rect.hpp"
#include "bleak/subsystem.hpp"

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
				error_log.add("cannot destroy window: window handle is null!");
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

		inline sdl::window create(cstr title, cref<offset_2d_t> position, cref<extent_2d_t> size, sdl::window_flags flags) noexcept {
			ptr<SDL_Window> new_window { SDL_CreateWindow(title, position.x, position.y, size.w, size.h, flags) };

			if (new_window == nullptr) {
				error_log.add(std::format("failed to create window: {}", SDL_GetError()));
			}

			return new_window;
		}

		static inline void destroy(ptr<SDL_Window> window) noexcept {
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

		inline window_t(cstr title, cref<extent_2d_t> size, sdl::window_flags flags) noexcept :
			window { create(title, sdl::WINDOW_POSITION_CENTERED, size, flags) },
			title { title },
			size { size },
			flags { flags } {}

		inline window_t(cstr title, cref<offset_2d_t> position, cref<extent_2d_t> size, sdl::window_flags flags) noexcept :
			window { create(title, position, size, flags) },
			title { title },
			size { size },
			flags { flags } {}

		inline window_t(cref<window_t> other) noexcept = delete;

		inline window_t(rval<window_t> other) noexcept :
			window(std::move(other.window)),
			title(std::move(other.title)),
			size(std::move(other.size)),
			flags(std::move(other.flags)) {
			other.window = nullptr;
		}

		inline ref<window_t> operator=(cref<window_t> other) noexcept = delete;

		inline ref<window_t> operator=(rval<window_t> other) noexcept = delete;

		inline ~window_t() { destroy(window); }

		inline void poll_events() {
			if (!is_valid()) {
				return;
			}

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

		constexpr bool is_valid() const noexcept { return window != nullptr; }

		constexpr bool is_closing() const noexcept { return closing; }

		constexpr bool is_running() const noexcept { return !closing; }

		constexpr void close() noexcept { closing = true; }

		constexpr ptr<SDL_Window> handle() noexcept { return window; }

		constexpr cptr<SDL_Window> handle() const noexcept { return window; }

		constexpr offset_2d_t origin() const noexcept { return offset_2d_t::zero; }

		constexpr offset_2d_t center() const noexcept { return origin() + size / 2; }

		constexpr offset_2d_t extents() const noexcept { return origin() + size - 1; }

		constexpr rect_t bounds() const noexcept { return { origin(), size }; }
	};
} // namespace bleak
