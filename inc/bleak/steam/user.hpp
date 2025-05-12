#pragma once

#include <bleak/typedef.hpp>

#include <bleak/constants.hpp>
#include <bleak/steam.hpp>

namespace bleak::steam::user {
    static inline constexpr ptr<ISteamUser> get_interface() {
		if constexpr (IsSteamless) {
			return 0;
		} else {
			ptr<ISteamUser> interface = SteamUser();

			if (interface == nullptr) {
				error_log.add("[ERROR]: unable to access steam user api!");
			}

			return interface;
		}
    }

    constexpr usize cast_steam_id(CSteamID steam_id) noexcept { return std::bit_cast<usize>(steam_id); }

    inline usize get_steam_id() noexcept {
		if constexpr (IsSteamless) {
			return 0;
		} else {
			return SteamAPI_ISteamUser_GetSteamID(get_interface());
		}
	}
} // namespace bleak
