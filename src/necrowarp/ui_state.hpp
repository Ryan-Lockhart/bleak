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

	static inline label_t fps_label{
		anchor_t{ { globals::UIGridSize.w, 1 }, cardinal_e::Northeast },
		embedded_label_t{
			runes_t{ "FPS: 0000", colors::Green },
			embedded_box_t{ colors::Black, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	template<game_phase_t Phase> struct phase_state_t;

	template<> struct phase_state_t<game_phase_t::MainMenu> {
		static inline labeled_button_t config_button{
			anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Config", colors::Orange },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t play_button{
			anchor_t{ { config_button.position.x, config_button.position.y - config_button.calculate_size().h - 3 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Play", colors::Green },
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

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::MainMenu) {
				return false;
			}

			return
				play_button.is_hovered() ||
				config_button.is_hovered() ||
				quit_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::MainMenu) {
				return;
			}

			play_button.update(Mouse::button_t::Left);
			config_button.update(Mouse::button_t::Left);
			quit_button.update(Mouse::button_t::Left);

			if (play_button.is_active()) {
				phase.transition(game_phase_t::Loading);
			} else if (config_button.is_active()) {
				phase.transition(game_phase_t::ConfigMenu);
			} else if (quit_button.is_active()) {
				phase.transition(game_phase_t::Exiting);
			}
		}

		static inline void draw(renderer_t& renderer) noexcept {
			play_button.draw(renderer);
			config_button.draw(renderer);
			quit_button.draw(renderer);
		}
	};

	template<> struct phase_state_t<game_phase_t::Loading> {
		static inline label_t loading_label{
			anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Loading...", colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::Loading) {
				return false;
			}

			return loading_label.is_hovered();
		}

		static inline void draw(renderer_t& renderer) noexcept {
			loading_label.draw(renderer);
		}
	};

	template<> struct phase_state_t<game_phase_t::Paused> {
		static inline labeled_button_t config_button{
			anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Config", colors::Orange },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t resume_button{
			anchor_t{ { config_button.position.x, config_button.position.y - config_button.calculate_size().h - 3 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Resume", colors::Green },
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

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::Paused) {
				return false;
			}

			return
				resume_button.is_hovered() ||
				config_button.is_hovered() ||
				quit_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::Paused) {
				return;
			}

			resume_button.update(Mouse::button_t::Left);
			config_button.update(Mouse::button_t::Left);
			quit_button.update(Mouse::button_t::Left);

			if (resume_button.is_active()) {
				phase.transition(game_phase_t::Playing);
			} else if (config_button.is_active()) {
				phase.transition(game_phase_t::ConfigMenu);
			} else if (quit_button.is_active()) {
				phase.transition(game_phase_t::Exiting);
			}
		}

		static inline void draw(renderer_t& renderer) noexcept {
			resume_button.draw(renderer);
			config_button.draw(renderer);
			quit_button.draw(renderer);
		}
	};

	template<> struct phase_state_t<game_phase_t::ConfigMenu> {
		static inline labeled_button_t back_button{
			anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Back", colors::White },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::ConfigMenu) {
				return false;
			}

			return back_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::ConfigMenu) {
				return;
			}

			back_button.update(Mouse::button_t::Left);

			if (back_button.is_active()) {
				phase.revert();
			}
		}

		static inline void draw(renderer_t& renderer) noexcept {
			back_button.draw(renderer);
		}
	};

	template<> struct phase_state_t<game_phase_t::GameOver> {
		static inline bool show_statistics{ false };

		static inline label_t game_over_label{
			anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ runes_t{ "You were slain! Game over... ", colors::White }.concatenate(runes_t{ " or is it?", colors::dark::Magenta }) },
				embedded_box_t{ colors::Red, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t retry_button{
			anchor_t{ { game_over_label.position.x - 1, game_over_label.position.y + 4 }, cardinal_e::East },
			embedded_label_t{
				runes_t{ "Retry", colors::Green },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t quit_button{
			anchor_t{ { game_over_label.position.x + 1, game_over_label.position.y + 4 }, cardinal_e::West },
			embedded_label_t{
				runes_t{ "Quit", colors::Red },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t statistics_hidden_label{
			anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h }, cardinal_e::South },
			embedded_label_t{
				runes_t{ "Statistics", colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t statistics_expanded_label{
			anchor_t{ { globals::UIGridSize.w / 2, globals::UIGridSize.h }, cardinal_e::South },
			embedded_label_t{
				runes_t{
					"Player Kills: 0000\n\n"
					"Minion Kills: 0000\n\n\n"
					"Total Kills:  0000\n\n\n"
					"    Statistics    ",
					colors::White
				},
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::GameOver) {
				return false;
			}

			return
				game_over_label.is_hovered() ||
				retry_button.is_hovered() ||
				quit_button.is_hovered() ||
				(show_statistics ? statistics_expanded_label.is_hovered() : statistics_hidden_label.is_hovered());
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::GameOver) {
				return;
			}

			retry_button.update(Mouse::button_t::Left);
			quit_button.update(Mouse::button_t::Left);

			if (retry_button.is_active()) {
				phase.transition(game_phase_t::Loading);
			} else if (quit_button.is_active()) {
				phase.transition(game_phase_t::Exiting);
			}

			show_statistics = show_statistics ? statistics_expanded_label.is_hovered() : statistics_hidden_label.is_hovered();

			if (show_statistics) {
				statistics_expanded_label.text = runes_t{
					"Player Kills: " + std::format("{:4}", game_stats.player_kills) + "\n\n"
					"Minion Kills: " + std::format("{:4}", game_stats.minion_kills) + "\n\n\n"
					"Total Kills:  " + std::format("{:4}", game_stats.total_kills()) + "\n\n\n"
					"    Statistics    ",
					colors::White
				};
			}
		}

		static inline void draw(renderer_t& renderer) noexcept {
			title_label.draw(renderer);
			game_over_label.draw(renderer);
			retry_button.draw(renderer);
			quit_button.draw(renderer);

			show_statistics ?
				statistics_expanded_label.draw(renderer) :
				statistics_hidden_label.draw(renderer);
		}
	};

	template<> struct phase_state_t<game_phase_t::Exiting> {
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

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::Exiting) {
				return false;
			}

			return
				confirm_quit_label.is_hovered() ||
				confirm_quit_button.is_hovered() ||
				cancel_quit_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::Exiting) {
				return;
			}

			confirm_quit_button.update(Mouse::button_t::Left);
			cancel_quit_button.update(Mouse::button_t::Left);

			if (confirm_quit_button.is_active()) {
				window.close();
			} else if (cancel_quit_button.is_active()) {
				phase.revert();
			}
		}

		static inline void draw(renderer_t& renderer) noexcept {
			title_label.draw(renderer);

			confirm_quit_label.draw(renderer, confirm_quit_label.box.background.dimmed(sine_wave.current_value()));

			confirm_quit_button.draw(renderer);
			cancel_quit_button.draw(renderer);
		}
	};

	constexpr glyph_t ActiveEnergyGlyph{ EnergyGlyph.index, color_t{ 0xFF, static_cast<u8>(0xFF) } };
	constexpr glyph_t InactiveEnergyGlyph{ EnergyGlyph.index, color_t{ 0xFF, static_cast<u8>(0x80) } };

	constexpr glyph_t ActiveArmorGlyph{ ArmorGlyph.index, color_t{ 0xFF, static_cast<u8>(0xFF) } };
	constexpr glyph_t InactiveArmorGlyph{ ArmorGlyph.index, color_t{ 0xFF, static_cast<u8>(0x80) } };

	constexpr cstr help_hidden_text{ "F1: Show Controls" };

	constexpr cstr help_expanded_text{
		" Movement:  WASD / Numpad \n\n\n"
		" Random Warp:           Q\n\n"
		" Target Warp:           E\n\n\n"
		" Calcitic Invocation:   1\n\n"
		" Spectral Invocation:   2\n\n"
		" Sanguinary Invocation: 3\n\n"
		" Necromantic Ascension: 4\n\n\n"
		"F1: Hide Controls"
	};

	template<> struct phase_state_t<game_phase_t::Playing> {
		static inline status_bar_t<2> player_statuses{
			anchor_t{ offset_t{ 1, 1 }, cardinal_e::Northwest},
			std::array<status_t, 2>{
				status_t{ player_t::MaximumEnergy, ActiveEnergyGlyph, InactiveEnergyGlyph },
				status_t{ player_t::MaximumArmor, ActiveArmorGlyph, InactiveArmorGlyph }
			},
			embedded_box_t{ colors::Black, { colors::White, 1 } },
			extent_t{ 1, 1 }
		};

		static inline label_t help_label{
			anchor_t{ { 1, globals::UIGridSize.h }, cardinal_e::Southwest },
			embedded_label_t{
				runes_t{ help_hidden_text, colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t tooltip_label{
			anchor_t{ { globals::UIGridSize.w, globals::UIGridSize.h }, cardinal_e::Southeast },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool show_help{ false };
		static inline bool show_tooltip{ false };

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::Playing) {
				return false;
			}

			return player_statuses.is_hovered() || help_label.is_hovered();
		}

		static inline void update() noexcept {
			if (phase.current_phase != game_phase_t::Playing) {
				return;
			}

			player_statuses[0].current_value = player.get_energy();
			player_statuses[0].max_value = player.max_energy();

			player_statuses[1].current_value = player.get_armor();
			player_statuses[1].max_value = player.max_armor();

			if (Keyboard::is_key_down(bindings::ToggleControls)) {
				show_help = !show_help;

				help_label.text = runes_t{ show_help ? help_expanded_text : help_hidden_text, colors::White };
			}

			if (gamepad_enabled && gamepad_active) {
				if (primary_gamepad->left_stick.current_state != cardinal_e::Central && cursor_timer.ready()) {
					grid_cursor.update(primary_gamepad->left_stick.current_state);

					cursor_timer.record();
				}
			} else {
				grid_cursor.update(camera.get_position());
			}

			const entity_type_t entity_type{ entity_registry.at(grid_cursor.position) };
			const bool bloodied{ game_map[grid_cursor.position].bloodied };

			show_tooltip = entity_type != entity_type_t::None || bloodied;

			if (show_tooltip) {
				if (entity_type != entity_type_t::None && bloodied) {
					tooltip_label.text = to_colored_string(entity_type);
					tooltip_label.text
						.concatenate({ " | ", colors::White })
						.concatenate({"bloodied", colors::materials::LightBlood });
				} else {
					tooltip_label.text = entity_type == entity_type_t::None ? runes_t{ "bloodied", colors::materials::LightBlood } : to_colored_string(entity_type);
				}
			}

			grid_cursor.color.set_alpha(sine_wave.current_value());
			warp_cursor.color.set_alpha(sine_wave.current_value());
		}

		static inline void draw(renderer_t& renderer) noexcept {
			player_statuses.draw(renderer);

			help_label.draw(renderer);

			if (!draw_cursor && show_tooltip) {
				tooltip_label.draw(renderer);
			}
		}
	};

	static inline bool any_hovered() noexcept {
		if (title_label.is_hovered() || fps_label.is_hovered()) {
			return true;
		}

		switch (phase.current_phase) {
		case game_phase_t::MainMenu:
			return phase_state_t<game_phase_t::MainMenu>::is_hovered();
		case game_phase_t::Exiting:
			return phase_state_t<game_phase_t::Exiting>::is_hovered();
		case game_phase_t::Paused:
			return phase_state_t<game_phase_t::Paused>::is_hovered();
		case game_phase_t::Loading:
			return phase_state_t<game_phase_t::Loading>::is_hovered();
		case game_phase_t::ConfigMenu:
			return phase_state_t<game_phase_t::ConfigMenu>::is_hovered();
		case game_phase_t::GameOver:
			return phase_state_t<game_phase_t::GameOver>::is_hovered();
		case game_phase_t::Playing:
			return phase_state_t<game_phase_t::Playing>::is_hovered();
		default:
			return false;
		}
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

			fps_label.text = {
				std::format("FPS: {:2}", static_cast<u32>(Clock::frame_time())),
				colors::Green
			};

			draw_cursor = any_hovered() || on_border();

			cursor.update();

			if (phase.current_phase == game_phase_t::MainMenu) {
				phase_state_t<game_phase_t::MainMenu>::update(Mouse::button_t::Left);
			} else if (phase.current_phase == game_phase_t::ConfigMenu) {
				phase_state_t<game_phase_t::ConfigMenu>::update(Mouse::button_t::Left);
			} else if (phase.current_phase == game_phase_t::Exiting) {
				phase_state_t<game_phase_t::Exiting>::update(Mouse::button_t::Left);
			} else if (phase.current_phase == game_phase_t::Playing) {
				phase_state_t<game_phase_t::Playing>::update();
			} else if (phase.current_phase == game_phase_t::Paused) {
				phase_state_t<game_phase_t::Paused>::update(Mouse::button_t::Left);
			} else if (phase.current_phase == game_phase_t::GameOver) {
				phase_state_t<game_phase_t::GameOver>::update(Mouse::button_t::Left);
			}
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
			fps_label.draw(renderer);
			
			switch (phase.current_phase) {
			case game_phase_t::MainMenu:
				phase_state_t<game_phase_t::MainMenu>::draw(renderer);
				break;
			case game_phase_t::ConfigMenu:
				phase_state_t<game_phase_t::ConfigMenu>::draw(renderer);
				break;
			case game_phase_t::Exiting:
				phase_state_t<game_phase_t::Exiting>::draw(renderer);
				break;
			case game_phase_t::Playing:
				phase_state_t<game_phase_t::Playing>::draw(renderer);
				break;
			case game_phase_t::Paused:
				phase_state_t<game_phase_t::Paused>::draw(renderer);
				break;
			case game_phase_t::Loading:
				phase_state_t<game_phase_t::Loading>::draw(renderer);
				break;
			case game_phase_t::GameOver:
				phase_state_t<game_phase_t::GameOver>::draw(renderer);
				break;
			default:
				break;
			}

			if (phase.current_phase != game_phase_t::Playing || draw_cursor) {
				cursor.draw();
			}
		}
	} static inline ui_registry;
} // namespace necrowarp
