#pragma once

#include <bleak.hpp>

#include <necrowarp/ui.hpp>
#include <necrowarp/game_state.hpp>
#include <necrowarp/entity_state.tpp>

namespace necrowarp {
	using namespace bleak;

	static inline label_t title_label{
		anchor_t{ { globals::ui_grid_size().w / 2, 1 }, cardinal_e::North },
		embedded_label_t{
			runes_t{ globals::GameTitle, colors::Marble },
			embedded_box_t{ colors::Black,
			border_t{ colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	static inline label_t fps_label{
		anchor_t{ { globals::ui_grid_size().w, 1 }, cardinal_e::Northeast },
		embedded_label_t{
			runes_t{ "FPS: 0000", colors::Green },
			embedded_box_t{ colors::Black, { colors::White, 1 } },
			extent_t{ 1, 1 }
		}
	};

	template<game_phase_t Phase> struct phase_state_t;

	template<> struct phase_state_t<game_phase_t::MainMenu> {
		static inline labeled_button_t credits_button{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{ "Credits", colors::Orange },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t play_button{
			anchor_t{ credits_button.position - offset_t{ credits_button.calculate_size().w / 2 + 2, 0 }, cardinal_e::East },
			embedded_label_t{
				runes_t{ "Play", colors::Green },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t quit_button{
			anchor_t{ credits_button.position + offset_t{ credits_button.calculate_size().w / 2 + 2, 0 }, cardinal_e::West },
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
				credits_button.is_hovered() ||
				quit_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::MainMenu) {
				return;
			}

			play_button.update(Mouse::button_t::Left);
			credits_button.update(Mouse::button_t::Left);
			quit_button.update(Mouse::button_t::Left);

			if (play_button.is_active()) {
				phase.transition(game_phase_t::Loading);
			} else if (credits_button.is_active()) {
				phase.transition(game_phase_t::Credits);
			} else if (quit_button.is_active()) {
				phase.transition(game_phase_t::Exiting);
			}
		}

		static inline void draw(renderer_t& renderer) noexcept {
			play_button.draw(renderer);
			credits_button.draw(renderer);
			quit_button.draw(renderer);
		}
	};

	template<> struct phase_state_t<game_phase_t::Loading> {
		static inline label_t loading_label{
			anchor_t{ { globals::ui_grid_size().w / 2, globals::ui_grid_size().h / 2 }, cardinal_e::Central },
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

		static inline labeled_button_t resume_button{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 - offset_t{ 0, 1 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{ "Resume", colors::Green },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t quit_button{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 + offset_t{ 0, 1 } }, cardinal_e::North },
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

			return resume_button.is_hovered() || quit_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::Paused) {
				return;
			}

			resume_button.update(Mouse::button_t::Left);
			quit_button.update(Mouse::button_t::Left);

			if (resume_button.is_active()) {
				phase.transition(game_phase_t::Playing);
			} else if (quit_button.is_active()) {
				phase.transition(game_phase_t::Exiting);
			}
		}

		static inline void draw(renderer_t& renderer) noexcept {
			resume_button.draw(renderer);
			quit_button.draw(renderer);
		}
	};

	template<> struct phase_state_t<game_phase_t::Credits> {
		static inline label_t credits_label{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 }, cardinal_e::Central },
			embedded_label_t{
				runes_t{
				"                   Bjarne Stroustrup and the C++ standards committee for the C++ programming language                  \n\n\n"
				"  Chris Lattner, Vikram Adve, and team responsible for the LLVM backend, Clang C++ compiler, clangd, and clang-format  \n\n\n"
				"Sam Lantinga and the team responsible for the following libraries: SDL2, SDL2_image, SDL2_mixer, SDL2_net, and SDL2_ttf\n\n\n"
				"                         Valve and the team responsible for the Steamworks API and Storefront                          \n\n\n"
				"               Tejon, the author of the 8x8 Potash bitmap font from the Dwarf Fortress Tileset Repository              \n\n\n\n\n"
				"                              My father, Jeff, and my friends Jackson, Donald, and Robert                              "
				},
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 2, 2 }
			}
		};
		static inline labeled_button_t back_button{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 + offset_t{ 0, credits_label.calculate_size().h / 2 } }, cardinal_e::North },
			embedded_label_t{
				runes_t{ "Back", colors::White },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::Credits) {
				return false;
			}

			return credits_label.is_hovered() || back_button.is_hovered();
		}

		static inline void update(Mouse::button_t button) noexcept {
			if (phase.current_phase != game_phase_t::Credits) {
				return;
			}

			back_button.update(Mouse::button_t::Left);

			if (back_button.is_active()) {
				phase.revert();
			}
		}

		static inline void draw(renderer_t& renderer) noexcept {
			credits_label.draw(renderer);

			back_button.draw(renderer);
		}
	};

	template<> struct phase_state_t<game_phase_t::GameOver> {
		static inline bool show_statistics{ false };

		static inline label_t game_over_label{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 - offset_t{ 0, 1 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{
					runes_t{
						"You were slain! Game over...",
						colors::White
					}.concatenate(
						runes_t{
							"\n\n        ...or is it?",
							colors::dark::Magenta
						}
					)
				},
				embedded_box_t{ colors::Red, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t retry_button{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 + offset_t{ -1, 1 } }, cardinal_e::Northeast },
			embedded_label_t{
				runes_t{ "Retry", colors::Green },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t quit_button{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 + offset_t{ 1, 1 } }, cardinal_e::Northwest },
			embedded_label_t{
				runes_t{ "Quit", colors::Red },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t statistics_hidden_label{
			anchor_t{ { globals::ui_grid_size().w / 2, globals::ui_grid_size().h }, cardinal_e::South },
			embedded_label_t{
				runes_t{ "Statistics", colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t statistics_expanded_label{
			anchor_t{ { globals::ui_grid_size().w / 2, globals::ui_grid_size().h }, cardinal_e::South },
			embedded_label_t{
				runes_t{
					"Depth Reached:  000\n\n\n"
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

			game_over_label.text = runes_t{ "You were slain! Game over...", colors::White };

			if (retry_button.is_hovered()) {
				game_over_label.text.concatenate(runes_t{ "\n\n        ...or is it?", colors::dark::Magenta });
			}

			show_statistics = show_statistics ? statistics_expanded_label.is_hovered() : statistics_hidden_label.is_hovered();

			if (show_statistics) {
				statistics_expanded_label.text = runes_t{
					"Depth Reached: " + std::format("{:3}", game_stats.game_depth) + "\n\n\n"
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
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 - offset_t{ 0, 1 } }, cardinal_e::South },
			embedded_label_t{
				runes_t{ "Are you sure you want to quit?", colors::White },
				embedded_box_t{ colors::Red, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t confirm_quit_button{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 + offset_t{ -1, 2 } }, cardinal_e::East },
			embedded_label_t{
				runes_t{ "Yes", colors::Green },
				embedded_box_t{ colors::Grey, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline labeled_button_t cancel_quit_button{
			anchor_t{ offset_t{ globals::ui_grid_size() / 2 + offset_t{ 1, 2 } }, cardinal_e::West },
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

	constexpr glyph_t ActiveEnergyGlyph{ characters::Energy, color_t{ 0xFF, static_cast<u8>(0xFF) } };
	constexpr glyph_t InactiveEnergyGlyph{ characters::Energy, color_t{ 0xFF, static_cast<u8>(0x80) } };

	constexpr glyph_t ActiveArmorGlyph{ characters::Armor, color_t{ 0xFF, static_cast<u8>(0xFF) } };
	constexpr glyph_t InactiveArmorGlyph{ characters::Armor, color_t{ 0xFF, static_cast<u8>(0x80) } };

	constexpr cstr depth_hidden_text{ "Depth: 000" };

	constexpr cstr depth_expanded_text{
		"Player Kills: 0000\n\n"
		"Minion Kills: 0000\n\n\n"
		"Total Kills:  0000\n\n\n"
		"    Depth: 000    "
	};

	constexpr cstr help_hidden_text{ "F1: Show Controls" };

	constexpr cstr help_expanded_text{
		" Movement:  WASD / Numpad \n\n\n"
		" Random Warp:           Q\n\n"
		" Target Warp:           E\n\n\n"
		" Calcitic Invocation:   1\n\n"
		" Spectral Invocation:   2\n\n"
		" Sanguine Invocation:   3\n\n"
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

		static inline label_t depth_hidden_label{
			anchor_t{ { globals::ui_grid_size().w / 2, globals::ui_grid_size().h }, cardinal_e::South },
			embedded_label_t{
				runes_t{ depth_hidden_text, colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t depth_expanded_label{
			anchor_t{ { globals::ui_grid_size().w / 2, globals::ui_grid_size().h }, cardinal_e::South },
			embedded_label_t{
				runes_t{ depth_expanded_text, colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t help_label{
			anchor_t{ { 1, globals::ui_grid_size().h }, cardinal_e::Southwest },
			embedded_label_t{
				runes_t{ help_hidden_text, colors::White },
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t tooltip_label{
			anchor_t{ { globals::ui_grid_size().w, globals::ui_grid_size().h }, cardinal_e::Southeast },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline label_t command_label{
			anchor_t{ { globals::ui_grid_size().w, globals::ui_grid_size().h }, cardinal_e::West },
			embedded_label_t{
				runes_t{},
				embedded_box_t{ colors::Black, { colors::White, 1 } },
				extent_t{ 1, 1 }
			}
		};

		static inline bool show_help{ false };
		static inline bool show_tooltip{ false };
		static inline bool show_command{ false };		
		static inline bool show_depth{ false };

		static constexpr offset_t random_warp_icon_position() { return offset_t{ 0, globals::icon_grid_size().h / 2 - 4 }; }
		static constexpr offset_t target_warp_icon_position() { return offset_t{ 0, globals::icon_grid_size().h / 2 - 3 }; }

		static constexpr offset_t calcitic_invocation_icon_position() { return offset_t{ 0, globals::icon_grid_size().h / 2 - 1 }; }
		static constexpr offset_t spectral_invocation_icon_position() { return offset_t{ 0, globals::icon_grid_size().h / 2 }; }
		static constexpr offset_t sanguine_invocation_icon_position() { return offset_t{ 0, globals::icon_grid_size().h / 2 + 1 }; }

		static constexpr offset_t necromantic_ascendance_icon_position() { return offset_t{ 0, globals::icon_grid_size().h / 2 + 3 }; }

		static inline bool is_hovered() noexcept {
			if (phase.current_phase != game_phase_t::Playing) {
				return false;
			}

			if (Mouse::is_inside(random_warp_icon_position() * globals::IconSize, globals::IconSize)) {
				return true;
			} else if (Mouse::is_inside(target_warp_icon_position() * globals::IconSize, globals::IconSize)) {
				return true;
			} else if (Mouse::is_inside(calcitic_invocation_icon_position() * globals::IconSize, globals::IconSize)) {
				return true;
			} else if (Mouse::is_inside(spectral_invocation_icon_position() * globals::IconSize, globals::IconSize)) {
				return true;
			} else if (Mouse::is_inside(sanguine_invocation_icon_position() * globals::IconSize, globals::IconSize)) {
				return true;
			} else if (Mouse::is_inside(necromantic_ascendance_icon_position() * globals::IconSize, globals::IconSize)) {
				return true;
			}
			
			return
				player_statuses.is_hovered() ||
				help_label.is_hovered() ||				
				(show_depth ? depth_expanded_label.is_hovered() : depth_hidden_label.is_hovered());
		}

		static inline void update() noexcept {
			if (phase.current_phase != game_phase_t::Playing) {
				return;
			}

			player_statuses[0].current_value = player.get_energy();
			player_statuses[0].max_value = player.max_energy();

			player_statuses[1].current_value = player.get_armor();
			player_statuses[1].max_value = player.max_armor();

			show_command = true;

			const offset_t mouse_pos{ Mouse::get_position() };

			if (Mouse::is_inside(random_warp_icon_position() * globals::IconSize, globals::IconSize)) {
				command_label.text = runes_t{ to_string(command_type_t::RandomWarp) };
				command_label.text
					.concatenate(runes_t{ " ["})
					.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_random_warp() ? colors::Green : colors::Red })
					.concatenate(runes_t{ "/" })
					.concatenate(runes_t{ std::format("{}", player.random_warp_cost()) })
					.concatenate(runes_t{ "]" });
				
				command_label.position = (random_warp_icon_position() + offset_t{ 1, 1 }) * globals::IconSize / globals::GlyphSize + offset_t{ 2, 0 };
			} else if (Mouse::is_inside(target_warp_icon_position() * globals::IconSize, globals::IconSize)) {
				command_label.text = runes_t{ to_string(command_type_t::TargetWarp) };
				command_label.text
					.concatenate(runes_t{ " ["})
					.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_target_warp() ? colors::Green : colors::Red })
					.concatenate(runes_t{ "/" })
					.concatenate(runes_t{ std::format("{}", player.target_warp_cost()) })
					.concatenate(runes_t{ "]" });
				
					command_label.position = (target_warp_icon_position() + offset_t{ 1, 1 }) * globals::IconSize / globals::GlyphSize + offset_t{ 2, 0 };
			} else if (Mouse::is_inside(calcitic_invocation_icon_position() * globals::IconSize, globals::IconSize)) {
				command_label.text = runes_t{ to_string(command_type_t::CalciticInvocation) };
				command_label.text
					.concatenate(runes_t{ " ["})
					.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform_calcitic_invocation() ? colors::Green : colors::Red })
					.concatenate(runes_t{ "/" })
					.concatenate(runes_t{ std::format("{}", player.calcitic_invocation_cost()) })
					.concatenate(runes_t{ "]" });
				
					command_label.position = (calcitic_invocation_icon_position() + offset_t{ 1, 1 }) * globals::IconSize / globals::GlyphSize + offset_t{ 2, 0 };
			} else if (Mouse::is_inside(spectral_invocation_icon_position() * globals::IconSize, globals::IconSize)) {
				command_label.text = runes_t{ to_string(command_type_t::SpectralInvocation) };
				command_label.text
					.concatenate(runes_t{ " ["})
					.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform_spectral_invocation() ? colors::Green : colors::Red })
					.concatenate(runes_t{ "/" })
					.concatenate(runes_t{ std::format("{}", player.spectral_invocation_cost()) })
					.concatenate(runes_t{ "]" });
				
					command_label.position = (spectral_invocation_icon_position() + offset_t{ 1, 1 }) * globals::IconSize / globals::GlyphSize + offset_t{ 2, 0 };
			} else if (Mouse::is_inside(sanguine_invocation_icon_position() * globals::IconSize, globals::IconSize)) {
				command_label.text = runes_t{ to_string(command_type_t::SanguineInvocation) };
				command_label.text
					.concatenate(runes_t{ " ["})
					.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform_sanguine_invocation() ? colors::Green : colors::Red })
					.concatenate(runes_t{ "/" })
					.concatenate(runes_t{ std::format("{}", player.sanguine_invocation_cost()) })
					.concatenate(runes_t{ "]" });
				
					command_label.position = (sanguine_invocation_icon_position() + offset_t{ 1, 1 }) * globals::IconSize / globals::GlyphSize + offset_t{ 2, 0 };
			} else if (Mouse::is_inside(necromantic_ascendance_icon_position() * globals::IconSize, globals::IconSize)) {
				command_label.text = runes_t{ to_string(command_type_t::NecromanticAscendance) };
				command_label.text
					.concatenate(runes_t{ " ["})
					.concatenate(runes_t{ std::format("{}", player.get_energy()), player.can_perform_necromantic_ascendance() ? colors::Green : colors::Red })
					.concatenate(runes_t{ "/" })
					.concatenate(runes_t{ std::format("{}", player.necromantic_ascendance_cost()) })
					.concatenate(runes_t{ "]" });
				
					command_label.position = (necromantic_ascendance_icon_position() + offset_t{ 1, 1 }) * globals::IconSize / globals::GlyphSize + offset_t{ 2, 0 };
			} else {
				show_command = false;
			}
			
			command_label.position.y = mouse_pos.y / globals::GlyphSize.h;

			show_depth = show_depth ? depth_expanded_label.is_hovered() : depth_hidden_label.is_hovered();

			if (show_depth) {
				depth_expanded_label.text = runes_t{
					std::format(
						"Player Kills: {:4}\n\n"
						"Minion Kills: {:4}\n\n\n"
						"Total Kills:  {:4}\n\n\n"
						"    Depth: {:3}    ",
						game_stats.player_kills,
						game_stats.minion_kills,
						game_stats.total_kills(),
						game_stats.game_depth
					),
					colors::White
				};
			} else {
				depth_hidden_label.text = runes_t{ std::format("Depth: {:3}", (isize)game_stats.game_depth * -1) };
			}

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

					if (entity_type == entity_type_t::Ladder) {
						tooltip_label.text
							.concatenate(runes_t{ " (" })
							.concatenate(runes_t{ to_string(entity_registry.at<ladder_t>(grid_cursor.position)->verticality) })
							.concatenate({ " | " })
							.concatenate(to_colored_string(entity_registry.at<ladder_t>(grid_cursor.position)->shackle))
							.concatenate(runes_t{ ")"});
					} else if (entity_type == entity_type_t::Skull && entity_registry.at<skull_t>(grid_cursor.position)->fresh) {
						tooltip_label.text
							.concatenate(runes_t{ " (" })
							.concatenate(runes_t{ "fresh", colors::light::Green })
							.concatenate(runes_t{ ")"});
					}
					
					tooltip_label.text
						.concatenate({ " | " })
						.concatenate({"blood", colors::materials::LightBlood });
				} else {
					if (entity_type == entity_type_t::None) {
						tooltip_label.text = runes_t{ "blood", colors::materials::LightBlood };
					} else {
						tooltip_label.text = to_colored_string(entity_type);						

						if (entity_type == entity_type_t::Ladder) {
							tooltip_label.text
								.concatenate(runes_t{ " (" })
								.concatenate(runes_t{ to_string(entity_registry.at<ladder_t>(grid_cursor.position)->verticality) })
								.concatenate({ " | " })
								.concatenate(to_colored_string(entity_registry.at<ladder_t>(grid_cursor.position)->shackle))
								.concatenate(runes_t{ ")"});
						} else if (entity_type == entity_type_t::Skull && entity_registry.at<skull_t>(grid_cursor.position)->fresh) {
							tooltip_label.text
								.concatenate(runes_t{ " (" })
								.concatenate(runes_t{ "fresh", colors::light::Green })
								.concatenate(runes_t{ ")"});
						}
					}
				}
			}

			grid_cursor.color.set_alpha(sine_wave.current_value());
			warp_cursor.color.set_alpha(sine_wave.current_value());
		}

		static inline void draw(renderer_t& renderer) noexcept {
			player_statuses.draw(renderer);

			icon_atlas.draw(glyph_t{ icons::RandomWarp.index, player.can_random_warp() ? colors::White : colors::dark::Grey }, random_warp_icon_position());
			icon_atlas.draw(glyph_t{ icons::TargetWarp.index, player.can_target_warp() ? colors::White : colors::dark::Grey }, target_warp_icon_position());

			icon_atlas.draw(glyph_t{ icons::CalciticInvocation.index, player.can_perform_calcitic_invocation() ? colors::White : colors::dark::Grey }, calcitic_invocation_icon_position());
			icon_atlas.draw(glyph_t{ icons::SpectralInvocation.index, player.can_perform_spectral_invocation() ? colors::White : colors::dark::Grey }, spectral_invocation_icon_position());
			icon_atlas.draw(glyph_t{ icons::SanguineInvocation.index, player.can_perform_sanguine_invocation() ? colors::White : colors::dark::Grey }, sanguine_invocation_icon_position());

			icon_atlas.draw(glyph_t{ icons::NecromanticAscendance.index, player.can_perform_sanguine_invocation() ? colors::White : colors::dark::Grey }, necromantic_ascendance_icon_position());

			if (show_command) {
				command_label.draw(renderer);
			}

			show_depth ?
				depth_expanded_label.draw(renderer) :
				depth_hidden_label.draw(renderer);

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
		case game_phase_t::Credits:
			return phase_state_t<game_phase_t::Credits>::is_hovered();
		case game_phase_t::GameOver:
			return phase_state_t<game_phase_t::GameOver>::is_hovered();
		case game_phase_t::Playing:
			return phase_state_t<game_phase_t::Playing>::is_hovered();
		default:
			return false;
		}
	}

	struct ui_registry_t {
		inline void update() noexcept {
			if (window.is_closing()) {
				return;
			}

			fps_label.text = {
				std::format("FPS: {:4}", static_cast<u32>(Clock::frame_time())),
				colors::Green
			};

			draw_cursor = any_hovered();

			cursor.update();

			if (phase.current_phase == game_phase_t::MainMenu) {
				phase_state_t<game_phase_t::MainMenu>::update(Mouse::button_t::Left);
			} else if (phase.current_phase == game_phase_t::Credits) {
				phase_state_t<game_phase_t::Credits>::update(Mouse::button_t::Left);
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
					grid_cursor.draw(camera, -globals::CursorOffset);
				}

				if (draw_warp_cursor) {
					warp_cursor.draw(camera, -globals::CursorOffset);
				}
			}

			title_label.draw(renderer);
			fps_label.draw(renderer);
			
			switch (phase.current_phase) {
			case game_phase_t::MainMenu:
				phase_state_t<game_phase_t::MainMenu>::draw(renderer);
				break;
			case game_phase_t::Credits:
				phase_state_t<game_phase_t::Credits>::draw(renderer);
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
