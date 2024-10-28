#pragma once

#include <bleak.hpp>

#include <flecs.h>

namespace necrowarp {
	using namespace bleak;

	namespace components {
		struct awaiting_input {};

		struct player_input {
			sdl::key key;
		};

		struct awaiting_turn {};

		struct move_command {
			cardinal_t direction;
		};

		struct consume_command {
			flecs::entity target;
		};

		struct attack_command {
			flecs::entity target;
		};

		struct random_warp_command {};

		struct target_warp_command {
			offset_t target;
		};

		struct consume_warp_command {
			flecs::entity target;
		};

		struct summon_wraith_command {};

		struct grand_summoning_command {};

		struct exorcise_command {
			flecs::entity target;
		};

		struct resurrect_command {
			flecs::entity target;
		};

		struct annoint_command {
			flecs::entity target;
		};
	} // namespace components
} // namespace necrowarp
