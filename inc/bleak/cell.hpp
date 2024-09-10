#pragma once

#include <bleak/typedef.hpp>

#include <bit>
#include <format>
#include <random>
#include <string>

#include <bleak/atlas.hpp>
#include <bleak/extent.hpp>
#include <bleak/offset.hpp>
#include <bleak/random.hpp>
#include <bleak/renderer.hpp>
#include <bleak/zone.hpp>

#include <bleak/constants/characters.hpp>

namespace bleak {
	enum class cell_trait_t : u8 { Open, Solid, Transperant, Opaque, Seen, Explored, Unseen, Unexplored, Dry, Damp, Cold, Warm, Rough, Smooth, Recedes, Protrudes, Odorless, Smelly, Safe, Toxic };

	enum class rock_type_t : u8 {
		Limestone, // Sedimentary
		Granite,   // Igneous intrusive
		Basalt,	   // Igneous extrusive
		Marble	   // Metamorphic
	};

	constexpr cstr to_string(cref<rock_type_t> type) noexcept {
		switch (type) {
		case rock_type_t::Limestone:
			return "limestone";
		case rock_type_t::Granite:
			return "granite";
		case rock_type_t::Basalt:
			return "basalt";
		case rock_type_t::Marble:
			return "marble";
		}
	}

	enum class mineral_type_t : u8 {
		None,
		Lignite,		// Mineral of coke
		BituminousCoal, // Mineral of coke (200% of lignite)
		NativeCopper,	// Native ore of copper
		NativeSilver,	// Native ore of silver
		NativeGold,		// Native ore of gold
		NativePlatinum, // Native ore of platinum
		Limonite,		// Ore of iron
		Hematite,		// Ore of iron
		Magnetite,		// Ore of iron
		Malachite,		// Ore of copper
		Tetrahedrite,	// Ore of copper and silver (5:1)
		Garnierite,		// Ore of nickel
		Galena,			// Ore of lead and silver (2:1)
		Sphalerite,		// Ore of zinc
		Cassiterite		// Ore of tin
	};

	constexpr cstr to_string(cref<mineral_type_t> type) noexcept {
		switch (type) {
		case mineral_type_t::None:
			return "no minerals";
		case mineral_type_t::Lignite:
			return "lignite";
		case mineral_type_t::BituminousCoal:
			return "bituminous coal";
		case mineral_type_t::NativeCopper:
			return "native copper";
		case mineral_type_t::NativeSilver:
			return "native silver";
		case mineral_type_t::NativeGold:
			return "native gold";
		case mineral_type_t::NativePlatinum:
			return "native platinum";
		case mineral_type_t::Limonite:
			return "limonite";
		case mineral_type_t::Hematite:
			return "hematite";
		case mineral_type_t::Magnetite:
			return "magnetite";
		case mineral_type_t::Malachite:
			return "malachite";
		case mineral_type_t::Tetrahedrite:
			return "tetrahedrite";
		case mineral_type_t::Garnierite:
			return "garnierite";
		case mineral_type_t::Galena:
			return "galena";
		case mineral_type_t::Sphalerite:
			return "sphalerite";
		case mineral_type_t::Cassiterite:
			return "cassiterite";
		}
	}

	struct cell_state_t {
	  public:
		bool solid : 1 { false };
		bool opaque : 1 { false };
		bool seen : 1 { false };
		bool explored : 1 { false };
		bool damp : 1 { false };
		bool warm : 1 { false };
		bool smooth : 1 { false };
		bool protrudes : 1 { false };
		bool smelly : 1 { false };
		bool toxic : 1 { false };

		rock_type_t rock_type : 2 { rock_type_t::Limestone };

		mineral_type_t mineral_type : 4 { mineral_type_t::None };

		constexpr cell_state_t() noexcept = default;

		constexpr cell_state_t(cref<cell_state_t> other) noexcept = default;

		constexpr cell_state_t(rval<cell_state_t> other) noexcept = default;

		constexpr ref<cell_state_t> operator=(cref<cell_state_t> other) noexcept = default;

		constexpr ref<cell_state_t> operator=(rval<cell_state_t> other) noexcept = default;

		template<typename... Traits>
			requires is_homogeneous<cell_trait_t, Traits...>::value && is_plurary<Traits...>::value
		constexpr cell_state_t(Traits... traits) {
			for (cref<cell_trait_t> trait : { traits... }) {
				set(trait);
			}
		}

		constexpr inline cell_state_t operator+(cref<cell_trait_t> trait) const noexcept {
			cell_state_t state{ *this };

			state.set(trait);

			return state;
		}

		constexpr inline cell_state_t operator-(cref<cell_trait_t> trait) const noexcept {
			cell_state_t state{ *this };

			state.unset(trait);

			return state;
		}

		constexpr inline ref<cell_state_t> operator+=(cref<cell_trait_t> trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr inline ref<cell_state_t> operator-=(cref<cell_trait_t> trait) noexcept {
			unset(trait);

			return *this;
		}

		constexpr void set(cref<cell_trait_t> trait) noexcept {
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
			case cell_trait_t::Rough:
				smooth = false;
				break;
			case cell_trait_t::Smooth:
				smooth = true;
				break;
			case cell_trait_t::Recedes:
				protrudes = false;
				break;
			case cell_trait_t::Protrudes:
				protrudes = true;
				break;
			case cell_trait_t::Odorless:
				smelly = false;
				break;
			case cell_trait_t::Smelly:
				smelly = true;
				break;
			case cell_trait_t::Safe:
				toxic = false;
				break;
			case cell_trait_t::Toxic:
				toxic = true;
				break;
			default:
				break;
			}
		}

		constexpr void unset(cref<cell_trait_t> trait) noexcept {
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
			case cell_trait_t::Rough:
				smooth = true;
				break;
			case cell_trait_t::Smooth:
				smooth = false;
				break;
			case cell_trait_t::Protrudes:
				protrudes = true;
				break;
			case cell_trait_t::Recedes:
				protrudes = false;
				break;
			case cell_trait_t::Odorless:
				smelly = true;
				break;
			case cell_trait_t::Smelly:
				smelly = false;
				break;
			case cell_trait_t::Safe:
				toxic = true;
				break;
			case cell_trait_t::Toxic:
				toxic = false;
				break;
			default:
				break;
			}
		}

		constexpr ref<cell_state_t> operator=(cref<cell_trait_t> trait) noexcept {
			set(trait);

			return *this;
		}

		constexpr ref<cell_state_t> operator=(cref<rock_type_t> type) noexcept {
			rock_type = type;

			return *this;
		}

		constexpr ref<cell_state_t> operator=(cref<mineral_type_t> type) noexcept {
			mineral_type = type;

			return *this;
		}

		constexpr bool operator==(cref<cell_state_t> other) const noexcept {
			return solid == other.solid && opaque == other.opaque && seen == other.seen && explored == other.explored && damp == other.damp && warm == other.warm && smooth == other.smooth && protrudes == other.protrudes
				&& smelly == other.smelly && toxic == other.toxic && rock_type == other.rock_type && mineral_type == other.mineral_type;
		}

		constexpr bool operator!=(cref<cell_state_t> other) const noexcept {
			return solid != other.solid || opaque != other.opaque || seen != other.seen || explored != other.explored || damp != other.damp || warm != other.warm || smooth != other.smooth || protrudes != other.protrudes
				|| smelly != other.smelly || toxic != other.toxic || rock_type != other.rock_type || mineral_type != other.mineral_type;
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
			case cell_trait_t::Smooth:
				return smooth;
			case cell_trait_t::Rough:
				return !smooth;
			case cell_trait_t::Protrudes:
				return protrudes;
			case cell_trait_t::Recedes:
				return !protrudes;
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
			if (seen && explored) {
				return std::format(
					"The cell is physically {} and visibility is {}.\nIt is {} and {} by the player.\nThe {}{} {} is {}, {}, and {} to the touch.{}{}",
					solid ? "blocked" : "open",
					opaque ? "obscured" : "unobscured",
					seen ? "in view" : "not in view",
					explored ? "was explored" : "remains unexplored",
					solid ? protrudes ? "protruding " : "receding " : "",
					to_string(rock_type),
					solid ? "wall" : "floor",
					damp ? "damp" : "dry",
					warm ? "warm" : "cold",
					smooth ? "smooth" : "rough",
					mineral_type == mineral_type_t::None || !solid ? "" : std::format("\nIts surface is encrusted with {}", to_string(mineral_type)),
					solid ? "" : std::format("\nThe air within is {} and {}.", smelly ? "pungent" : "odorless", toxic ? "toxic" : "innocuous")
				);
			} else if (explored) {
				return std::format(
					"The cell is physically {} and visibility is {}.\nIt is {} and {} by the player.",
					solid ? "blocked" : "open",
					opaque ? "obscured" : "unobscured",
					seen ? "in view" : "not in view",
					explored ? "was explored" : "remains unexplored"
				);
			} else {
				return "The cell remains unexplored.";
			}
		}

		inline constexpr operator std::string() const {
			return std::format(
				"[{}, {}, {}, {}, {}, {}, {}, {}, {}]",
				solid ? "Solid" : "Open",
				opaque ? "Opaque" : "Transperant",
				seen ? "Seen" : "Unseen",
				explored ? "Explored" : "Unexplored",
				damp ? "Damp" : "Dry",
				warm ? "Warm" : "Cold",
				smooth ? "Smooth" : "Rough",
				protrudes ? "Protrudes" : "Recedes",
				smelly ? "Smelly" : "Odorless",
				toxic ? "Toxic" : "Safe"
			);
		}

		template<extent_t AtlasSize> inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<offset_t> position) const noexcept {
			if (!explored) {
				return;
			}

			const u8 alpha{ seen ? u8{ 0xFF } : u8{ 0x80 } };

			atlas.draw(glyph_t{ characters::Floor, color_t{ 0x40, alpha } }, position);

			if (!solid) {
				return;
			}

			atlas.draw(glyph_t{ characters::Wall, color_t{ 0xC0, alpha } }, position);
		}

		template<extent_t AtlasSize> inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<offset_t> position, cref<offset_t> offset) const noexcept {
			if (!explored) {
				return;
			}

			const u8 alpha{ seen ? u8{ 0xFF } : u8{ 0x80 } };

			atlas.draw(glyph_t{ characters::Floor, color_t{ 0x40, alpha } }, position + offset);

			if (!solid) {
				return;
			}

			atlas.draw(glyph_t{ characters::Wall, color_t{ 0xC0, alpha } }, position + offset);
		}

		template<extent_t AtlasSize, typename T, extent_t ZoneSize, extent_t ZoneBorder>
		inline constexpr void draw(cref<atlas_t<AtlasSize>> atlas, cref<zone_t<T, ZoneSize, ZoneBorder>> zone, cref<offset_t> position, cref<offset_t> offset) const noexcept {
			if (!explored) {
				return;
			}

			const u8 alpha{ seen ? u8{ 0xFF } : u8{ 0x80 } };

			atlas.draw(glyph_t{ characters::Floor, color_t{ 0x40, alpha } }, position + offset);

			if (!solid) {
				return;
			}

			const u8 glyph{ characters::auto_set(smooth, protrudes, zone.template calculate_index<neighbourhood_solver_t::Melded>(position, cell_trait_t::Solid)) };

			atlas.draw(glyph_t{ glyph, color_t{ 0xC0, alpha } }, position + offset);
		}

		struct hasher {
			static constexpr usize operator()(cref<cell_state_t> cell_state) noexcept { return hash_combine(std::bit_cast<u16>(cell_state)); }
		};

		template<typename Generator>
			requires is_random_engine<Generator>::value
		struct randomizer {
			template<typename T> static constexpr T operator()(ref<Generator> generator) noexcept {
				if constexpr (std::is_same<T, rock_type_t>::value) {
					static std::uniform_int_distribution<i32> dis{ static_cast<i32>(rock_type_t::Limestone), static_cast<i32>(rock_type_t::Marble) };

					return static_cast<rock_type_t>(dis(generator));
				} else if constexpr (std::is_same<T, mineral_type_t>::value) {
					static std::uniform_int_distribution<i32> dis{ static_cast<i32>(mineral_type_t::None), static_cast<i32>(mineral_type_t::Tetrahedrite) };

					return static_cast<mineral_type_t>(dis(generator));
				}

				return T{};
			}
		};
	};

	template<> struct is_drawable<cell_state_t> {
		static bool constexpr value = true;
	};
} // namespace bleak
