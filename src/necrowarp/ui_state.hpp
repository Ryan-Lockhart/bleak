#pragma once

#include <bleak.hpp>

#include <necrowarp/ui.hpp>
#include <necrowarp/game_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	using namespace bleak;

	static inline label_t title_label{
		anchor_t{ { globals::UIGridSize.w / 2, 1 }, cardinal_e::North },
		embedded_label_t{
			runes_t{ globals::GameTitle, colors::Marble },
			embedded_box_t{ colors::Black,
			border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline labeled_button_t play_button{
		anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h / 2 }, cardinal_e::Central },
		embedded_label_t{
			runes_t{ "Play", colors::Green },
			embedded_box_t{ colors::Grey, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline labeled_button_t config_button{
		anchor_t{ { play_button.position.x, play_button.position.y + play_button.calculate_size().h + 3 }, cardinal_e::Central },
		embedded_label_t{
			runes_t{ "Config", colors::Orange },
			embedded_box_t{ colors::Grey, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline labeled_button_t quit_button{
		anchor_t{ { config_button.position.x, config_button.position.y + config_button.calculate_size().h + 3 }, cardinal_e::Central },
		embedded_label_t{
			runes_t{ "Quit", colors::White },
			embedded_box_t{ colors::Grey, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline wave_t confirm_quit_wave{ 1.0, 0.5, 1.0 };

	static inline label_t confirm_quit_label{
		anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h / 2 }, cardinal_e::Central },
		embedded_label_t{
			runes_t{ "Are you sure you want to quit?", colors::White },
			embedded_box_t{ colors::Red, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline labeled_button_t confirm_quit_button{
		anchor_t{ { confirm_quit_label.position.x - 1, confirm_quit_label.position.y + 4 }, cardinal_e::East },
		embedded_label_t{
			runes_t{ "Yes", colors::Green },
			embedded_box_t{ colors::Grey, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline labeled_button_t cancel_quit_button{
		anchor_t{ { confirm_quit_label.position.x + 1, confirm_quit_label.position.y + 4 }, cardinal_e::West },
		embedded_label_t{
			runes_t{ "No", colors::Red },
			embedded_box_t{ colors::Grey, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline label_t fps_label{
		anchor_t{ { globals::UIGridSize.w, 1 }, cardinal_e::Northeast },
		embedded_label_t{
			runes_t{ "FPS: 0000", colors::Green },
			embedded_box_t{ colors::Black, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	constexpr glyph_t ActiveEnergyGlyph{ EnergyGlyph.index, color_t{ 0xFF, static_cast<u8>(0xFF) } };
	constexpr glyph_t InactiveEnergyGlyph{ EnergyGlyph.index, color_t{ 0xFF, static_cast<u8>(0x80) } };

	constexpr glyph_t ActiveArmorGlyph{ ArmorGlyph.index, color_t{ 0xFF, static_cast<u8>(0xFF) } };
	constexpr glyph_t InactiveArmorGlyph{ ArmorGlyph.index, color_t{ 0xFF, static_cast<u8>(0x80) } };

	static inline status_bar_t<2> player_statuses{
		anchor_t{ offset_t{ 1, 1 }, cardinal_e::Northwest},
		std::array<status_t, 2>{
			status_t{ player_t::MaximumEnergy, ActiveEnergyGlyph, InactiveEnergyGlyph },
			status_t{ player_t::MaximumArmor, ActiveArmorGlyph, InactiveArmorGlyph }
		},
		embedded_box_t{ colors::Black, { colors::White, 1 } },
		extent_t{ 1, 1 }
	};

	static inline bool any_hovered() noexcept {
		return
			title_label.is_hovered() ||
			fps_label.is_hovered() ||
			player_statuses.is_hovered();
	}

	static inline bool on_border() noexcept {
		const offset_t mouse_pos{ Mouse::get_position() };

		return
			mouse_pos.x <= globals::UniversalOrigin.x ||
			mouse_pos.y <= globals::UniversalOrigin.y ||
			mouse_pos.x >= globals::UniversalExtent.x ||
			mouse_pos.y >= globals::UniversalExtent.y;
	}

	static inline void draw_window_border() noexcept {
		renderer.draw_outline_rect(offset_t::Zero, globals::WindowSize + globals::WindowBorder * 2, globals::BorderSize, colors::Black);
		renderer.draw_outline_rect(offset_t::Zero, globals::WindowSize + globals::WindowBorder * 2, colors::White);
		renderer.draw_outline_rect(globals::UniversalOffset, globals::WindowSize, colors::White);
	};

	struct ui_registry_t {
		inline void update() noexcept {
			if (window.is_closing()) {
				return;
			}

			if (phase.current_phase == game_phase_t::MainMenu) {
				play_button.update(Mouse::button_t::Left);
				config_button.update(Mouse::button_t::Left);
				quit_button.update(Mouse::button_t::Left);

				if (play_button.is_active()) {
					phase.current_phase = game_phase_t::Playing;
				} else if (config_button.is_active()) {
					phase.transition(game_phase_t::ConfigMenu);
				} else if (quit_button.is_active()) {
					phase.transition(game_phase_t::Exiting);
				}
			} else if (phase.current_phase == game_phase_t::Exiting) {
				confirm_quit_button.update(Mouse::button_t::Left);
				cancel_quit_button.update(Mouse::button_t::Left);

				if (confirm_quit_button.is_active()) {
					window.close();
				} else if (cancel_quit_button.is_active()) {
					phase.transition(game_phase_t::MainMenu);
				}
			}

			fps_label.text = {
				std::format("FPS: {:2}", static_cast<u32>(Clock::frame_time())),
				colors::Green
			};

			draw_cursor = any_hovered() || on_border();

			cursor.update();

			if (gamepad_enabled && gamepad_active) {
				if (primary_gamepad->left_stick.current_state != cardinal_e::Central && cursor_timer.ready()) {
					grid_cursor.update(primary_gamepad->left_stick.current_state);

					cursor_timer.record();
				}
			} else {
				grid_cursor.update(camera.get_position());
			}

			grid_cursor.color.set_alpha(sine_wave.current_value());
			warp_cursor.color.set_alpha(sine_wave.current_value());

			player_statuses[0].current_value = player.get_energy();
			player_statuses[0].max_value = player.max_energy();

			player_statuses[1].current_value = player.get_armor();
			player_statuses[1].max_value = player.max_armor();
		}

		inline void render() const noexcept {
			if (window.is_closing()) {
				return;
			}

			if (phase.current_phase == game_phase_t::Playing) {
				if (!draw_cursor) {
					grid_cursor.draw(camera, globals::CursorOffset);
				}

				if (draw_warp_cursor) {
					warp_cursor.draw(camera, globals::CursorOffset);
				}
			}

			draw_window_border();

			title_label.draw(renderer);

			if (phase.current_phase == game_phase_t::Playing) {
				fps_label.draw(renderer);

				player_statuses.draw(renderer);

				if (draw_cursor) {
					cursor.draw();
				}
			} else if (phase.current_phase == game_phase_t::MainMenu) {
				play_button.draw(renderer);
				config_button.draw(renderer);
				quit_button.draw(renderer);

				cursor.draw();
			} else if (phase.current_phase == game_phase_t::Exiting) {
				confirm_quit_label.draw(renderer, confirm_quit_label.box.background.dimmed(sine_wave.current_value()));
				confirm_quit_button.draw(renderer);
				cancel_quit_button.draw(renderer);

				cursor.draw();
			}
		}
	} static inline ui_registry;
} // namespace necrowarp
