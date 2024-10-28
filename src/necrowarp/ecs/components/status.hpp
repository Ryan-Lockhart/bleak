#pragma once

#include <bleak.hpp>

namespace necrowarp {
	using namespace bleak;

	namespace components {
		struct healthy {
			u16 value{ 0 };
		};

		struct health_affected {};

		struct dead {};

		struct armoured {
			u16 value{ 0 };
		};

		struct armour_affected {};

		struct unarmoured {};

		struct energized {
			u16 value{ 0 };
		};

		struct energy_affected {};

		struct enervated {};

		struct pious {
			u16 value{ 0 };
		};

		struct piety_affected {};

		struct impious {};

		struct fresh {};
		struct rotted {};
	} // namespace components
} // namespace necrowarp
