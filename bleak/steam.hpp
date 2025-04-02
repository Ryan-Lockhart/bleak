#pragma once

#include <bleak/typedef.hpp>

#include <cstring>

#include <steam/steam_api.h>

namespace bleak {
	namespace steam {
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

		// initialize the steam api; returns an 'init_result' enum.
		// call get_error() to interrogate why this function did not return init_result::Ok
		inline init_result initialize() noexcept {
			SteamErrMsg err{};

			init_result result = static_cast<init_result>(SteamAPI_InitEx(&err));

			if (result != init_result::Ok) {
				memset(error_str, '\0', error_str_size);
				memcpy(error_str, err, strnlen((cstr)err, error_str_size));
			}

			return result;
		}

		// initialize the steam api with a required appid; returns an 'init_result' enum.
		// call get_error() to interrogate why this function did not return init_result::Ok
		inline init_result initialize(u32 app_id) noexcept {
			if (SteamAPI_RestartAppIfNecessary(app_id)) {
				constexpr cstr invalid_launch_err_str{ "application wasn't launched through steam" };

				memset(error_str, '\0', error_str_size);
				memcpy(error_str, invalid_launch_err_str, strnlen(invalid_launch_err_str, error_str_size));

				return init_result::InvalidLaunch;
			}

			SteamErrMsg err{};

			init_result result = static_cast<init_result>(SteamAPI_InitEx(&err));

			if (result != init_result::Ok) {
				memset(error_str, '\0', error_str_size);
				memcpy(error_str, err, strnlen((cstr)err, error_str_size));
			}

			return result;
		}

		// shutdown the steam api
		inline void shutdown() noexcept { SteamAPI_Shutdown(); };

		inline bool is_steam_running() noexcept { return SteamAPI_IsSteamRunning(); }

		inline cstr get_steam_install_path() noexcept { return SteamAPI_GetSteamInstallPath(); }
	} // namespace steam
} // namespace bleak
