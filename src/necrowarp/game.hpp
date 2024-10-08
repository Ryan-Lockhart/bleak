#pragma once

#include "bleak/constants/colors.hpp"
#include "entities/entity.hpp"
#include <bleak.hpp>

#include <cstdlib>
#include <thread>

#include <necrowarp/entities.hpp>
#include <necrowarp/entity_state.hpp>
#include <necrowarp/entity_state.tpp>
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
		static inline void primary_gamepad_disconnected() noexcept {
			gamepad_enabled = false;
			primary_gamepad = nullptr;
		}

		static inline void primary_gamepad_reconnected(cptr<gamepad_t> gamepad) noexcept {
			gamepad_enabled = true;
			primary_gamepad = gamepad;
		}

		static inline bool camera_input() noexcept {
			if constexpr (globals::MapSize <= globals::GameGridSize) {
				return camera.center_on<true>(globals::MapCenter);
			}

			if (Keyboard::is_key_down(bindings::CameraLock)) {
				camera_locked = !camera_locked;
			}

			if (camera_locked) {
				return camera.center_on(player.position);
			}

			const offset_t direction = []() -> offset_t {
				offset_t::scalar_t x{ 0 }, y{ 0 };

				if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_t::North])) {
					--y;
				} if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_t::South])) {
					++y;
				} if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_t::West])) {
					--x;
				} if (Keyboard::is_key_pressed(bindings::CameraMovement[cardinal_t::East])) {
					++x;
				}

				if (gamepad_enabled && x == 0 && y == 0) {
					return static_cast<offset_t>(primary_gamepad->right_stick.current_state);
				}

				return offset_t{ x, y };
			}();

			if (direction != offset_t::Zero) {
				input_timer.record();
				return camera.move(direction * globals::CameraSpeed);
			}

			return false;
		}

		static inline bool character_input() noexcept {
			player.command = entity_command_t{};

			if (Keyboard::any_keys_pressed(bindings::Wait)) {
				input_timer.record();
				epoch_timer.record();

				return true;
			} else if (Keyboard::is_key_down(bindings::RandomWarp)) {
				player.command = entity_command_t{ command_type_t::RandomWarp, player.position };

				input_timer.record();
				epoch_timer.record();

				return true;
			} else if (Keyboard::is_key_down(bindings::TargetWarp)) {
				if (!game_map.within<zone_region_t::Interior>(grid_cursor.get_position()) || game_map[grid_cursor.get_position()].solid) {
					return false;
				}

				player.command = entity_command_t{ entity_registry.contains(grid_cursor.get_position()) ? command_type_t::ConsumeWarp : command_type_t::TargetWarp, player.position, grid_cursor.get_position() };

				input_timer.record();
				epoch_timer.record();

				return true;
			} else if (Keyboard::is_key_down(bindings::SummonWraith)) {
				player.command = entity_command_t{ command_type_t::SummonWraith, player.position };

				input_timer.record();
				epoch_timer.record();

				return true;
			} else if (Keyboard::is_key_down(bindings::GrandSummoning)) {
				player.command = entity_command_t{ command_type_t::GrandSummoning, player.position };

				input_timer.record();
				epoch_timer.record();

				return true;
			}

			const offset_t direction = []() -> offset_t {
				offset_t::scalar_t x{ 0 }, y{ 0 };

				if (Keyboard::any_keys_pressed(bindings::CharacterMovement[cardinal_t::North])) {
					--y;
				} if (Keyboard::any_keys_pressed(bindings::CharacterMovement[cardinal_t::South])) {
					++y;
				} if (Keyboard::any_keys_pressed(bindings::CharacterMovement[cardinal_t::West])) {
					--x;
				} if (Keyboard::any_keys_pressed(bindings::CharacterMovement[cardinal_t::East])) {
					++x;
				}

				if (gamepad_enabled && x == 0 && y == 0) {
					return static_cast<offset_t>(primary_gamepad->dpad.current_state);
				}

				return offset_t{ x, y };
			}();

			if (direction != offset_t::Zero) {
				const offset_t target_position{ player.position + direction };

				if (!game_map.within<zone_region_t::Interior>(target_position) || game_map[target_position].solid) {
					return false;
				}

				const command_type_t command_type{ !entity_registry.contains(target_position) ? command_type_t::Move : player.clash_or_consume(target_position) };

				player.command = entity_command_t{ command_type, player.position, target_position };

				draw_warp_cursor = false;

				input_timer.record();
				epoch_timer.record();

				return true;
			}

			return false;
		}

		static inline void startup() noexcept {
			Mouse::initialize();
			Keyboard::initialize();

			GamepadManager::initialize();

			Mouse::hide_cursor();

			primary_gamepad = GamepadManager::lease(0, &primary_gamepad_disconnected, &primary_gamepad_reconnected);
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

			cauto player_pos{ game_map.find_random<zone_region_t::Interior>(random_engine, cell_trait_t::Open) };

			if (!player_pos.has_value()) {
				error_log.add("could not find open position for player!");
				terminate_prematurely();
			}

			player.position = player_pos.value();
			good_goal_map.add(player_pos.value());

			entity_registry.spawn<ladder_t>(globals::NumberOfLadders, globals::MinimumLadderDistance);
			entity_registry.spawn<skull_t>(globals::StartingSkulls, globals::MinimumSkullDistance);

			evil_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);
			good_goal_map.recalculate<zone_region_t::Interior>(game_map, cell_trait_t::Open, entity_registry);

			Clock::tick();

			input_timer.reset();
			cursor_timer.reset();
			epoch_timer.reset();

			message_log.flush_to_console(std::cout);
			error_log.flush_to_console(std::cerr);
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

			if (input_timer.ready()) {
				if (epoch_timer.ready() && !player_acted) {
					player_acted = character_input();
				}

				camera_input();
			}
		}

		static inline void process_turn() noexcept {
			if (window.is_closing() || !player_acted) {
				return;
			}

			processing_turn = true;

			wave_size = clamp(globals::StartingAdventurers + total_kills() / globals::KillsPerPopulation, globals::MinimumWaveSize, globals::MaximumWaveSize);

			if (entity_registry.empty<ALL_GOOD_NPCS>() && spawns_remaining <= 0) {
				spawns_remaining = wave_size;
			}

			while (spawns_remaining > 0) {
				cauto spawn_pos = []() -> std::optional<offset_t> {
					for (cref<ladder_t> ladder : entity_storage<ladder_t>) {
						if (entity_registry.contains<ALL_GOOD_NPCS>(ladder.position)) {
							continue;
						}

						return ladder.position;
					}

					return std::nullopt;
				}();

				if (!spawn_pos.has_value()) {
					break;
				}
				
				entity_registry.add<true>(priest_t{ spawn_pos.value() });

				--spawns_remaining;

				continue;

				static std::uniform_int_distribution<u16> spawn_distribution{ globals::SpawnDistributionLow, globals::SpawnDistributionHigh };

				const u8 spawn_chance{ static_cast<u8>(spawn_distribution(random_engine)) };

				if (wave_size >= globals::HugeWaveSize) {
					if (spawn_chance < 60) {
						entity_registry.add<true>(adventurer_t{ spawn_pos.value() });
					} else if (spawn_chance < 96) {
						entity_registry.add<true>(paladin_t{ spawn_pos.value() });
					} else {
						entity_registry.add<true>(priest_t{ spawn_pos.value() });
					}
				} else if (wave_size >= globals::LargeWaveSize) {
					if (spawn_chance < 70) {
						entity_registry.add<true>(adventurer_t{ spawn_pos.value() });
					} else if (spawn_chance < 97) {
						entity_registry.add<true>(paladin_t{ spawn_pos.value() });
					} else  {
						entity_registry.add<true>(priest_t{ spawn_pos.value() });
					}
				} else if (wave_size >= globals::MediumWaveSize) {
					if (spawn_chance < 80) {
						entity_registry.add<true>(adventurer_t{ spawn_pos.value() });
					} else if (spawn_chance < 98) {
						entity_registry.add<true>(paladin_t{ spawn_pos.value() });
					} else {
						entity_registry.add<true>(priest_t{ spawn_pos.value() });
					}
				} else if (wave_size >= globals::SmallWaveSize) {
					if (spawn_chance < 90) {
						entity_registry.add<true>(adventurer_t{ spawn_pos.value() });
					} else if (spawn_chance < 99) {
						entity_registry.add<true>(paladin_t{ spawn_pos.value() });
					} else {
						entity_registry.add<true>(priest_t{ spawn_pos.value() });
					}
				} else {
					entity_registry.add<true>(adventurer_t{ spawn_pos.value() });
				}
				
				--spawns_remaining;
			}
			
			entity_registry.update();

			player_acted = false;
			processing_turn = false;
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

			entity_registry.draw(camera);

			renderer.draw_composite_rect(offset_t{ globals::GlyphSize }, extent_t{ max(player.max_energy(), player.max_armor()) + 1, 3 } * globals::CellSize - globals::GlyphSize, colors::Black, colors::White, 1);

			for (u8 i{ 0 }; i < player.max_energy(); ++i) {
				game_atlas.draw(glyph_t{ EnergyGlyph.index, color_t{ 0xFF, static_cast<u8>(player.get_energy() > i ? 0xFF : 0x80) } }, offset_t{ i, 0 }, offset_t{ 4, 4 });
			}

			for (u8 i{ 0 }; i < player.max_armor(); ++i) {
				game_atlas.draw(glyph_t{ ArmorGlyph.index, color_t{ 0xFF, static_cast<u8>(player.get_armor() > i ? 0xFF : 0x80) } }, offset_t{ i, 1 }, offset_t{ 4, 4 });
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
