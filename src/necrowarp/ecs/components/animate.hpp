#pragma once

#include <bleak.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace components {
		struct animate {};

		struct player {};

		struct npc {};

		struct good {};
		struct evil {};

		struct adventurer {};
		struct paladin {};
		struct priest {};

		struct skeleton {};
		struct wraith {};

		template<typename T> struct kill_count { u16 value; };

		struct fresh_spawn { u8 turns_remaining; };
	} // namespace components
} // namespace necrowarp
