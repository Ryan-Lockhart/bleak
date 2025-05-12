#pragma once

#include <bleak/typedef.hpp>

#include <cstring>

#include <bleak/constants.hpp>
#include <bleak/log.hpp>

#include <steam/steam_api.h>
#include <steam/steam_api_flat.h>

namespace bleak::steam {
    enum class init_result : u8 {
        Ok = ESteamAPIInitResult::k_ESteamAPIInitResult_OK,
        FailedGeneric = ESteamAPIInitResult::k_ESteamAPIInitResult_FailedGeneric,
        NoSteamClient = ESteamAPIInitResult::k_ESteamAPIInitResult_NoSteamClient,
        VersionMismatch = ESteamAPIInitResult::k_ESteamAPIInitResult_VersionMismatch,
        InvalidLaunch
    };

    static constexpr usize error_str_size{ sizeof(SteamErrMsg) };

    static inline char error_str[error_str_size];

    inline cstr get_error() noexcept { return error_str; }

    inline void set_error(cstr message) noexcept {
        memset(error_str, '\0', error_str_size);
        memcpy(error_str, message, strnlen(message, error_str_size));
    }

    extern "C" inline void __cdecl steam_api_debug_hook(i32 severity, cstr contents) noexcept {
        severity ? error_log.add("[MESSAGE]: {}", contents) : message_log.add("[WARNING]: {}", contents);
    }

    // initialize the steam api; returns an 'init_result' enum.
    // call get_error() to interrogate why this function did not return init_result::Ok
    inline init_result initialize() noexcept {
		if constexpr (IsSteamless) {
			set_error("the application is running in \"steamless\" mode.");
			return init_result::NoSteamClient;
		} else {
			SteamErrMsg err{};

			init_result result = static_cast<init_result>(SteamAPI_InitEx(&err));

			if (result != init_result::Ok) {
				set_error(err);
			} else {
				SteamUtils()->SetWarningMessageHook(&steam_api_debug_hook);
			}

			return result;
		}
    }

    // initialize the steam api with a required appid; returns an 'init_result' enum.
    // call get_error() to interrogate why this function did not return init_result::Ok
    inline init_result initialize(u32 app_id) noexcept {
		if constexpr (IsSteamless) {
			set_error("[WARNING]: the application is running in \"steamless\" mode. to use the steam api, remove the STEAMLESS flag from compilation arguments...");
			return init_result::NoSteamClient;
		} else {
			if (SteamAPI_RestartAppIfNecessary(app_id)) {
				constexpr cstr invalid_launch_err_str{ "application wasn't launched through steam" };
				
				set_error(invalid_launch_err_str);
	
				return init_result::InvalidLaunch;
			}
	
			SteamErrMsg err{};
	
			init_result result = static_cast<init_result>(SteamAPI_InitEx(&err));
	
			if (result != init_result::Ok) {				
				set_error(err);
			} else {
				SteamUtils()->SetWarningMessageHook(&steam_api_debug_hook);
			}
	
			return result;	
		}
    }

    // shutdown the steam api
    inline void shutdown() noexcept {
		if constexpr (IsSteamless) {
			return;
		} else {
			SteamAPI_Shutdown();
		}
	};

    inline bool is_steam_running() noexcept {
		if constexpr (IsSteamless) {
			return false;
		} else {
			return SteamAPI_IsSteamRunning();
		}
	}

    inline cstr get_steam_install_path() noexcept {
		if constexpr (IsSteamless) {
			return "";
		} else {
			return SteamAPI_GetSteamInstallPath();
		}
	}
}
