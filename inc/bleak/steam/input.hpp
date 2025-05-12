#pragma once

#include <bleak/typedef.hpp>

#include <bleak/constants.hpp>
#include <bleak/steam.hpp>

namespace bleak::steam::input {
    static inline constexpr ptr<ISteamInput> get_interface() {
		if constexpr (IsSteamless) {
			return nullptr;
		} else {
			ptr<ISteamInput> interface = SteamInput();

			if (interface == nullptr) {
				error_log.add("[ERROR]: unable to access steam input api!");
			}

			return interface;
		}
    }
} // namespace bleak
