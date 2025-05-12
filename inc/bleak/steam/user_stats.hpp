#pragma once

#include <bleak/typedef.hpp>

#include <optional>

#include <bleak/concepts.hpp>
#include <bleak/constants.hpp>
#include <bleak/log.hpp>

#include <bleak/steam/api.hpp>
#include <bleak/steam/user.hpp>

namespace bleak::steam::user_stats {
    static inline constexpr ptr<ISteamUserStats> get_interface() {
		if constexpr (IsSteamless) {
			return nullptr;
		} else {
			ptr<ISteamUserStats> interface = SteamUserStats();

			if (interface == nullptr) {
				error_log.add("[ERROR]: unable to access steam user stats api!");
			}

			return interface;
		}
    }

    template<typename T>
        requires is_one_of<T, i32, f32>::value
    static inline constexpr std::optional<T> get_stat(cstr api_name) noexcept {
		if constexpr (IsSteamless) {
			return std::nullopt;
		} else {
			T value{}; bool success{};

			if constexpr (std::is_same<T, i32>::value) {
				success = SteamAPI_ISteamUserStats_GetStatInt32(get_interface(), api_name, &value);
			} else if constexpr (std::is_same<T, f32>::value) {
				success = SteamAPI_ISteamUserStats_GetStatFloat(get_interface(), api_name, &value);
			}

			if (!success) {
				error_log.add("failed to get value of stat \"{}\"", api_name);

				return std::nullopt;
			}

			return value;
		}
    }

    template<typename T>
        requires is_one_of<T, i32, f32>::value
    static inline constexpr std::optional<T> get_stat(usize user_id, cstr api_name) noexcept {
		if constexpr (IsSteamless) {
			return std::nullopt;
		} else {
			T value{}; bool success{};

			if constexpr (std::is_same<T, i32>::value) {
				success = SteamAPI_ISteamUserStats_GetUserStatInt32(get_interface(), user_id, api_name, &value);
			} else if constexpr (std::is_same<T, f32>::value) {
				success = SteamAPI_ISteamUserStats_GetUserStatFloat(get_interface(), user_id, api_name, &value);
			}

			if (!success) {
				error_log.add("failed to get value of stat \"{}\" for steam user \"{}\"", api_name, user_id);

				return std::nullopt;
			}

			return value;
		}
    }

    template<typename T>
        requires is_one_of<T, i32, f32>::value
    static inline constexpr bool set_stat(cstr api_name, T value) noexcept {
        if constexpr (IsSteamless) {
			return false;
		} else {
			bool success{};

			if constexpr (std::is_same<T, i32>::value) {
				success = SteamAPI_ISteamUserStats_SetStatInt32(get_interface(), api_name, value);
			} else if constexpr (std::is_same<T, f32>::value) {
				success = SteamAPI_ISteamUserStats_SetStatFloat(get_interface(), api_name, value);
			}

			if (!success) {
				error_log.add("failed to set value of stat \"{}\" to \"{}\"", api_name, value);
			}

			return success;
		}
    }
} // namespace bleak
