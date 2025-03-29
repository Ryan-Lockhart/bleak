#pragma once

#include <bleak.hpp>

namespace necrowarp {
	using namespace bleak;

	enum struct game_phase_t : u8 { None, MainMenu, Credits, NewGame, Loading, Playing, Paused, GameOver, Exiting };

	struct phase_t {
		game_phase_t previous_phase{ game_phase_t::None };
		game_phase_t current_phase{ game_phase_t::MainMenu };

		constexpr phase_t() noexcept{};
		
		constexpr phase_t(game_phase_t phase) noexcept : previous_phase{ game_phase_t::None }, current_phase{ phase }{}

		constexpr void transition(game_phase_t phase) noexcept {
			previous_phase = current_phase;
			current_phase = phase;
		}

		constexpr void revert() noexcept {
			current_phase = previous_phase;
			previous_phase = game_phase_t::None;
		}
	};
} // namespace necrowarp
