#pragma once

#include <bleak.hpp>

#include <cstddef>
#include <optional>

namespace necrowarp {
	using namespace bleak;

	struct player_t;

	struct skull_t;
	struct ladder_t;

	struct skeleton_t;
	struct wraith_t;
	struct adventurer_t;
	struct paladin_t;
	struct priest_t;

#define ALL_EVIL_NPCS \
		skeleton_t, \
		wraith_t

#define ALL_GOOD_NPCS \
		adventurer_t, \
		paladin_t, \
		priest_t

#define ALL_NPCS \
		ALL_EVIL_NPCS, \
		ALL_GOOD_NPCS

#define ALL_ANIMATE \
		player_t, \
		ALL_NPCS

#define ALL_INANIMATE \
		skull_t, \
		ladder_t

#define ALL_NON_PLAYER \
		ALL_NPCS, \
		ALL_INANIMATE

#define ALL_ENTITIES \
		ALL_ANIMATE, \
		ALL_INANIMATE

	enum struct entity_type_t : u8 {
		None = 0,
		Player,
		Skull,
		Skeleton,
		Wraith,
		Ladder,
		Adventurer,
		Paladin,
		Priest
	};

	static constexpr cstr to_string(cref<entity_type_t> type) noexcept {
		switch (type) {
		case entity_type_t::None:
			return "none";
		case entity_type_t::Player:
			return "player";
		case entity_type_t::Skull:
			return "skull";
		case entity_type_t::Skeleton:
			return "skeleton";
		case entity_type_t::Wraith:
			return "wraith";
		case entity_type_t::Ladder:
			return "ladder";
		case entity_type_t::Adventurer:
			return "adventurer";
		case entity_type_t::Paladin:
			return "paladin";
		case entity_type_t::Priest:
			return "priest";
		}
	}

	constexpr usize EntityTypeCount{ static_cast<usize>(entity_type_t::Priest) + 1 };

	template<typename T> struct is_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_entity_v = is_entity<T>::value;

	template<> struct is_entity<std::nullptr_t> {
		static constexpr bool value = true;
	};
	
	template<entity_type_t EntityType> struct to_entity_type;
	
	template<> struct to_entity_type<entity_type_t::None> {
		using type = std::nullptr_t;
	};
	
	template<typename... Types> struct entity_types_t{};

	typedef entity_types_t<skull_t, skeleton_t, wraith_t, ladder_t, adventurer_t, paladin_t, priest_t> entity_types;

	template<typename T> struct is_good_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_good_entity_v = is_good_entity<T>::value;

	template<> struct is_good_entity<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_good_entity<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_good_entity<priest_t> {
		static constexpr bool value = true;
	};

	template<typename T> struct is_evil_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_evil_entity_v = is_evil_entity<T>::value;

	template<> struct is_evil_entity<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_evil_entity<wraith_t> {
		static constexpr bool value = true;
	};

	template<typename T> struct is_npc {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_npc_v = is_npc<T>::value;

	template<typename T> struct is_animate {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_animate_v = is_animate<T>::value;

	template<> struct is_animate<skeleton_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<wraith_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<adventurer_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<paladin_t> {
		static constexpr bool value = true;
	};

	template<> struct is_animate<priest_t> {
		static constexpr bool value = true;
	};

	template<typename T> struct is_inanimate {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_inanimate_v = is_inanimate<T>::value;

	template<> struct is_inanimate<skull_t> {
		static constexpr bool value = true;
	};

	template<> struct is_inanimate<ladder_t> {
		static constexpr bool value = true;
	};

	template<typename T> struct is_non_player {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_non_player_v = is_non_player<T>::value;

	template<typename T> struct is_player {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_player_v = is_player<T>::value;

	enum struct command_type_t : u8 {
		None = 0,
		Move,
		Consume,
		Clash,
		RandomWarp,
		TargetWarp,
		ConsumeWarp,
		SummonWraith,
		GrandSummoning,
		Exorcise,
		Resurrect,
		Ordain
	};

	static constexpr cstr to_string(cref<command_type_t> command) noexcept {
		switch (command) {
		case command_type_t::None:
			return "none";
		case command_type_t::Move:
			return "move";
		case command_type_t::Consume:
			return "consume";
		case command_type_t::Clash:
			return "clash";
		case command_type_t::RandomWarp:
			return "random warp";
		case command_type_t::TargetWarp:
			return "target warp";
		case command_type_t::ConsumeWarp:
			return "consume warp";
		case command_type_t::SummonWraith:
			return "summon wraith";
		case command_type_t::GrandSummoning:
			return "grand summoning";
		case command_type_t::Exorcise:
			return "exorcise";
		case command_type_t::Resurrect:
			return "resurrect";
		case command_type_t::Ordain:
			return "ordain";
		}
	}

	struct entity_command_t {
		command_type_t type;

		std::optional<offset_t> source;
		std::optional<offset_t> target;
	};

	constexpr entity_command_t null_command{ command_type_t::None, std::nullopt, std::nullopt };

	template<typename T, entity_type_t EntityType> struct is_entity_type {
		static constexpr bool value = false;
	};

	template<> struct is_entity_type<std::nullptr_t, entity_type_t::None> {
		static constexpr bool value = true;
	};

	template<typename T, entity_type_t EntityType> constexpr bool is_entity_type_v = is_entity_type<T, EntityType>::value;

	constexpr lut_t<entity_type_t, glyph_t, EntityTypeCount> EntityGlyphs{
		pair_t<entity_type_t, glyph_t>{ entity_type_t::None, glyph_t{ 0x40, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Player, glyph_t{ 0x41, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Skull, glyph_t{ 0x43, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Skeleton, glyph_t{ 0x45, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Wraith, glyph_t{ 0x46, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Ladder, glyph_t{ 0x47, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Adventurer, glyph_t{ 0x48, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Paladin, glyph_t{ 0x49, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Priest, glyph_t{ 0x4A, colors::White } }
	};

	constexpr glyph_t PlayerArmoredGlyph{ 0x42, colors::White };

	constexpr glyph_t AnimatedSkullGlyph{ 0x44, colors::White };

	constexpr glyph_t EnergyGlyph{ 0x4B, colors::White };
	constexpr glyph_t ArmorGlyph{ 0x4C, colors::White };
} // namespace necrowarp
