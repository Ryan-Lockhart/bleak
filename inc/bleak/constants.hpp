#pragma once

// IWYU pragma: begin_exports
#include <bleak/constants/bindings.hpp>
#include <bleak/constants/characters.hpp>
#include <bleak/constants/colors.hpp>
#include <bleak/constants/glyphs.hpp>
#include <bleak/constants/keys.hpp>
#include <bleak/constants/numeric.hpp>
#include <bleak/constants/octants.hpp>
// IWYU pragma: end_exports

#if defined(STEAMLESS)
	#if defined(BLEAK_RELEASE)
		#error [ERROR]: "steamless" mode cannot be used for release builds.
	#else
		#warning [WARNING]: "steamless" mode is enabled. the steam api will not be used at runtime...
	#endif
#endif

namespace bleak {
	constexpr bool IsReleaseBuild{
#if defined(BLEAK_RELEASE)
		true
#elif defined(BLEAK_DEBUG)
		false
#else
	#error [ERROR]: a debug or release symbol must be defined...
#endif
	};

	constexpr bool IsSteamless{
#if defined(STEAMLESS)
		true
#else
		false
#endif
	};
} //namespace bleak
