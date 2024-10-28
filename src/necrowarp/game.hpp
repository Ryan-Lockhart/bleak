#pragma once

#include "necrowarp/ecs/systems.hpp"
#include "necrowarp/ecs/systems/base.hpp"
#include <bleak.hpp>

#include <cstdlib>
#include <thread>

#include <necrowarp/game_state.hpp>
#include <necrowarp/globals.hpp>

namespace necrowarp {
	using namespace bleak;

	class Game {
	  public:
		static inline int run() noexcept {
			startup();

			std::thread([]() -> void {
				do { process_turn(); } while (window.is_running());
			}).detach();

			do {
				render();
				input();
				update();
			} while (window.is_running());

			shutdown();

			return EXIT_SUCCESS;
		};

	  private:
		static inline void startup() noexcept {
			Mouse::initialize();
			Keyboard::initialize();

			GamepadManager::initialize();

			Mouse::hide_cursor();

			primary_gamepad = GamepadManager::lease(0,
				[]() {
					gamepad_enabled = false;
					primary_gamepad = nullptr;
				},
				[](cptr<gamepad_t> gamepad) {
					gamepad_enabled = true;
					primary_gamepad = gamepad;
				}
			);

			gamepad_enabled = primary_gamepad != nullptr;

			if (primary_gamepad == nullptr) {
				message_log.add("no gamepad detected\n");
			}

			region_t<cell_state_t, globals::RegionSize, globals::ZoneSize, globals::BorderSize> region{ "res\\maps\\test_region.map" };

			constexpr cell_state_t open_state{ cell_trait_t::Open, cell_trait_t::Transperant, cell_trait_t::Seen, cell_trait_t::Explored };
			constexpr cell_state_t closed_state{ cell_trait_t::Solid, cell_trait_t::Opaque, cell_trait_t::Seen, cell_trait_t::Explored };

			constexpr binary_applicator_t<cell_state_t> cell_applicator{ closed_state, open_state };

			for (extent_t::product_t i{ 0 }; i < region.region_area; ++i) {
				region[i].set<zone_region_t::Border>(closed_state);
				region[i].generate<zone_region_t::Interior>(random_engine, globals::FillPercent, globals::AutomotaIterations, globals::AutomotaThreshold, cell_applicator);

				region[i].collapse<zone_region_t::Interior>(cell_trait_t::Solid, 0x00, cell_trait_t::Open);

				region[i].randomize<zone_region_t::All>(random_engine, 0.25, cell_trait_t::Smooth, cell_trait_t::Rough);
				region[i].randomize<zone_region_t::All>(random_engine, 2.0 / 3.0, cell_trait_t::Protrudes, cell_trait_t::Recedes);

				region[i].randomize<zone_region_t::All, rock_type_t>(random_engine);
				region[i].randomize<zone_region_t::All, mineral_type_t>(random_engine);
			}

			region.compile(game_map);

			std::vector<area_t> areas{ area_t::partition(game_map, cell_trait_t::Open) };

			if (areas.size() > 1) {
				cref<area_t> largest_area{ *std::max_element(areas.begin(), areas.end(), [](cref<area_t> a, cref<area_t> b) { return a.size() < b.size(); }) };

				for (crauto area : areas) {
					if (area != largest_area) {
						area.apply(game_map, cell_trait_t::Solid);
					}
				}
			}

			Clock::tick();

			input_timer.reset();
			cursor_timer.reset();
			epoch_timer.reset();

			message_log.flush_to_console(std::cout);
			error_log.flush_to_console(std::cerr);

			systems::initialize<systems::status_systems>(ecs);
		}

		static inline void input() noexcept {
			if (window.is_closing()) {
				return;
			}

			if constexpr (globals::UseFrameLimit) {
				Clock::tick(globals::FrameTime);
			} else {
				Clock::tick();
			}

			window.poll_events();

			if (Keyboard::is_key_down(bindings::Quit)) {
				window.close();
				return;
			}

			if (Keyboard::is_key_down(bindings::RevealMap)) {
				game_map.apply<zone_region_t::All>(cell_trait_t::Explored);
			}

			if (processing_turn) {
				return;
			}
		}

		static inline void process_turn() noexcept {
			if (window.is_closing()) {
				return;
			}

			processing_turn = true;

			ecs.progress();

			processing_turn = false;

			sdl::delay(1000);
		}

		static inline void update() noexcept {
			sine_wave.update<wave_type_t::Sine>(Clock::elapsed());

			const offset_t mouse_pos{ Mouse::get_position() };

			draw_cursor = mouse_pos.x < globals::UniversalOrigin.x || mouse_pos.y < globals::UniversalOrigin.y || mouse_pos.x > globals::UniversalExtent.x || mouse_pos.y > globals::UniversalExtent.y;

			if (draw_cursor) {
				cursor.update();
			} else {
				if (gamepad_enabled && gamepad_active) {
					if (primary_gamepad->left_stick.current_state != cardinal_t::Central && cursor_timer.ready()) {
						grid_cursor.update(primary_gamepad->left_stick.current_state);

						cursor_timer.record();
					}
				} else {
					grid_cursor.update(camera.get_position());
				}

				grid_cursor.color.set_alpha(sine_wave.current_value());
			}

			if (draw_warp_cursor) {
				warp_cursor.color.set_alpha(sine_wave.current_value());
			}
		}

		static inline void render() noexcept {
			if (window.is_closing()) {
				return;
			}

			renderer.clear(colors::Black);

			game_map.draw(game_atlas, camera);

			renderer.draw_outline_rect(offset_t::Zero, globals::WindowSize + globals::WindowBorder * 2, globals::BorderSize, colors::Black);
			renderer.draw_outline_rect(offset_t::Zero, globals::WindowSize + globals::WindowBorder * 2, colors::White);
			renderer.draw_outline_rect(globals::UniversalOffset, globals::WindowSize, colors::White);

			if (draw_cursor) {
				cursor.draw();
			} else {
				grid_cursor.draw(camera, globals::CursorOffset);
			}

			if (draw_warp_cursor) {
				warp_cursor.draw(camera, globals::CursorOffset);
			}

			const runes_t fps_text{ std::format("FPS:{:4}", static_cast<u32>(Clock::frame_time())), colors::Green };

			ui_atlas.draw_label(renderer, fps_text, offset_t{ globals::UIGridSize.w - 1, 0 }, cardinal_t::Southwest, extent_t{ 1, 1 }, colors::Black, colors::White);

			runes_t title_text{ globals::GameTitle, colors::Marble };

			ui_atlas.draw_label(renderer, title_text, offset_t{ globals::UIGridSize.w / 2, 0 }, cardinal_t::South, extent_t{ 1, 1 }, colors::Black, colors::White);

			renderer.present();
		}

		static inline void shutdown() noexcept {
			if (primary_gamepad != nullptr) {
				GamepadManager::release(0);
			}

			GamepadManager::terminate();

			Keyboard::terminate();
			Mouse::terminate();

			message_log.flush_to_file();
			error_log.flush_to_file();
		}

		static inline void terminate_prematurely() noexcept {
			std::cout << "Message Log:\n";
			message_log.flush_to_console(std::cout);

			std::cerr << "\nError Log:\n";
			error_log.flush_to_console(std::cerr);

			shutdown();
			exit(EXIT_FAILURE);
		}
	};
} // namespace necrowarp
