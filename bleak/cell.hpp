#pragma once
#include <bleak/typedef.hpp>

#include <format>
#include <string>

#include <bleak/atlas.hpp>
#include <bleak/extent.hpp>
#include <bleak/offset.hpp>
#include <bleak/renderer.hpp>

#include "constants/characters.hpp"

namespace bleak {
	enum class cell_trait_t : u8 { Open, Solid, Transperant, Opaque, Seen, Explored, Unseen, Unexplored, Dry, Damp, Cold, Warm, Odorless, Smelly, Safe, Toxic };

	struct cell_state_t {
	  public:
		bool solid : 1 { false };
		bool opaque : 1 { false };
		bool seen : 1 { false };
		bool explored : 1 { false };
		bool damp : 1 { false };
		bool warm : 1 { false };
		bool smelly : 1 { false };
		bool toxic : 1 { false };

		constexpr cell_state_t() noexcept = default;

		constexpr cell_state_t(cref<cell_state_t> other) noexcept :
			solid{ other.solid },
			opaque{ other.opaque },
			seen{ other.seen },
			explored{ other.explored },
			damp{ other.damp },
			warm{ other.warm },
			smelly{ other.smelly },
			toxic{ other.toxic } {}

		constexpr cell_state_t(rval<cell_state_t> other) noexcept :
			solid{ other.solid },
			opaque{ other.opaque },
			seen{ other.seen },
			explored{ other.explored },
			damp{ other.damp },
			warm{ other.warm },
			smelly{ other.smelly },
			toxic{ other.toxic } {}

		constexpr ref<cell_state_t> operator=(cref<cell_state_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			solid = other.solid;
			opaque = other.opaque;
			seen = other.seen;
			explored = other.explored;
			damp = other.damp;
			warm = other.warm;
			smelly = other.smelly;
			toxic = other.toxic;

			return *this;
		}

		constexpr ref<cell_state_t> operator=(rval<cell_state_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			solid = other.solid;
			opaque = other.opaque;
			seen = other.seen;
			explored = other.explored;
			damp = other.damp;
			warm = other.warm;
			smelly = other.smelly;
			toxic = other.toxic;

			return *this;
		}

		template<typename... Traits>
		constexpr cell_state_t(Traits... traits)
			requires(sizeof...(Traits) > 1) && (std::is_same_v<Traits, cell_trait_t> && ...)
		{
			for (cell_trait_t trait : { traits... }) {
				set(trait);
			}
		}

		constexpr inline cell_state_t operator+(cell_trait_t trait) const noexcept {
			cell_state_t state{ *this };

			state.set(trait);

			return state;
		}

		constexpr inline ref<cell_state_t> operator+=(cell_trait_t trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr inline cell_state_t operator-(cell_trait_t trait) const noexcept {
			cell_state_t state{ *this };

			state.unset(trait);

			return state;
		}

		constexpr inline ref<cell_state_t> operator-=(cell_trait_t trait) noexcept {
			unset(trait);

			return *this;
		}

		constexpr void set(cell_trait_t trait) noexcept {
			switch (trait) {
			case cell_trait_t::Open:
				solid = false;
				break;
			case cell_trait_t::Solid:
				solid = true;
				break;
			case cell_trait_t::Transperant:
				opaque = false;
				break;
			case cell_trait_t::Opaque:
				opaque = true;
				break;
			case cell_trait_t::Unseen:
				seen = false;
				break;
			case cell_trait_t::Seen:
				seen = true;
				break;
			case cell_trait_t::Unexplored:
				explored = false;
				break;
			case cell_trait_t::Explored:
				explored = true;
				break;
			case cell_trait_t::Dry:
				damp = false;
				break;
			case cell_trait_t::Damp:
				damp = true;
				break;
			case cell_trait_t::Cold:
				warm = false;
				break;
			case cell_trait_t::Warm:
				warm = true;
				break;
			case cell_trait_t::Smelly:
				smelly = true;
				break;
			case cell_trait_t::Odorless:
				smelly = false;
				break;
			case cell_trait_t::Toxic:
				toxic = true;
				break;
			case cell_trait_t::Safe:
				toxic = false;
				break;
			default:
				break;
			}
		}

		constexpr void unset(cell_trait_t trait) noexcept {
			switch (trait) {
			case cell_trait_t::Open:
				solid = true;
				break;
			case cell_trait_t::Solid:
				solid = false;
				break;
			case cell_trait_t::Transperant:
				opaque = true;
				break;
			case cell_trait_t::Opaque:
				opaque = false;
				break;
			case cell_trait_t::Unseen:
				seen = true;
				break;
			case cell_trait_t::Seen:
				seen = false;
				break;
			case cell_trait_t::Unexplored:
				explored = true;
				break;
			case cell_trait_t::Explored:
				explored = false;
				break;
			case cell_trait_t::Dry:
				damp = true;
				break;
			case cell_trait_t::Damp:
				damp = false;
				break;
			case cell_trait_t::Cold:
				warm = true;
				break;
			case cell_trait_t::Warm:
				warm = false;
				break;
			case cell_trait_t::Smelly:
				smelly = false;
				break;
			case cell_trait_t::Odorless:
				smelly = true;
				break;
			case cell_trait_t::Toxic:
				toxic = false;
				break;
			case cell_trait_t::Safe:
				toxic = true;
				break;
			default:
				break;
			}
		}

		constexpr ref<cell_state_t> operator=(cref<cell_trait_t> other) noexcept {
			set(other);
			return *this;
		}

		constexpr bool operator==(cref<cell_state_t> other) const noexcept {
			return solid == other.solid && opaque == other.opaque && seen == other.seen && explored == other.explored && damp == other.damp && warm == other.warm && smelly == other.smelly && toxic == other.toxic;
		}

		constexpr bool operator!=(cref<cell_state_t> other) const noexcept {
			return solid != other.solid || opaque != other.opaque || seen != other.seen || explored != other.explored || damp != other.damp || warm != other.warm || smelly != other.smelly || toxic != other.toxic;
		}

		constexpr bool operator==(cref<cell_trait_t> other) const noexcept { return contains(other); }

		constexpr bool operator!=(cref<cell_trait_t> other) const noexcept { return !contains(other); }

		constexpr bool contains(cell_trait_t trait) const noexcept {
			switch (trait) {
			case cell_trait_t::Solid:
				return solid;
			case cell_trait_t::Open:
				return !solid;
			case cell_trait_t::Opaque:
				return opaque;
			case cell_trait_t::Transperant:
				return !opaque;
			case cell_trait_t::Seen:
				return seen;
			case cell_trait_t::Unseen:
				return !seen;
			case cell_trait_t::Explored:
				return explored;
			case cell_trait_t::Unexplored:
				return !explored;
			case cell_trait_t::Damp:
				return damp;
			case cell_trait_t::Dry:
				return !damp;
			case cell_trait_t::Warm:
				return warm;
			case cell_trait_t::Cold:
				return !warm;
			case cell_trait_t::Smelly:
				return smelly;
			case cell_trait_t::Odorless:
				return !smelly;
			case cell_trait_t::Toxic:
				return toxic;
			case cell_trait_t::Safe:
				return !toxic;
			default:
				return false;
			}
		}

		inline constexpr std::string to_tooltip() const {
			return std::format(
				"The cell is physically {} and visibly {}.\nThe cell is {} and is {} by the player.\nIt is {} and {} to the touch.\nThe air within is {} and "
				"{}.",
				solid ? "blocked" : "open",
				seen ? "blocked" : "open",
				solid ? "in view" : "not in view",
				explored ? "explored" : "unexplored",
				damp ? "damp" : "dry",
				warm ? "warm" : "cold",
				smelly ? "pungent" : "odorless",
				toxic ? "toxic" : "harmless"
			);
		}

		inline constexpr operator std::string() const {
			return std::format(
				"[{}, {}, {}, {}, {}, {}, {}, {}]",
				solid ? "Solid" : "Open",
				opaque ? "Opaque" : "Transperant",
				seen ? "Seen" : "Unseen",
				explored ? "Explored" : "Unexplored",
				damp ? "Damp" : "Dry",
				warm ? "Warm" : "Cold",
				smelly ? "Smelly" : "Odorless",
				toxic ? "Toxic" : "Safe"
			);
		}

		template<extent_t AtlasSize> inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<offset_t> position) const noexcept {
			if (!explored) {
				return;
			}

			const u8 rgb{ solid ? u8{ 0xC0 } : u8{ 0x40 } };
			const u8 alpha{ seen ? u8{ 0xFF } : u8{ 0x80 } };
			const u8 glyph{ solid ? Characters::Wall : Characters::Floor };

			atlas.draw(glyph_t{ glyph, { rgb, rgb, rgb, alpha } }, position);
		}
	};

	template<> struct is_drawable<cell_state_t> {
		static bool constexpr value = true;
	};
} // namespace bleak
