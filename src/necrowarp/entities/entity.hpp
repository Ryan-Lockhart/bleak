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

	template<typename T> concept Entity = is_entity<T>::value;

	template<> struct is_entity<std::nullptr_t> {
		static constexpr bool value = true;
	};
	
	template<entity_type_t EntityType> struct to_entity_type;
	
	template<> struct to_entity_type<entity_type_t::None> {
		using type = std::nullptr_t;
	};

	template<entity_type_t EntityType> using to_entity_type_t = typename to_entity_type<EntityType>::type;

	template<typename T> struct is_good_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_good_entity_v = is_good_entity<T>::value;

	template<typename T> concept GoodEntity = is_entity<T>::value && is_good_entity<T>::value;

	template<typename T> struct is_evil_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_evil_entity_v = is_evil_entity<T>::value;

	template<typename T> concept EvilEntity = is_entity<T>::value && is_evil_entity<T>::value;

	template<typename T> struct is_npc_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_npc_entity_v = is_npc_entity<T>::value;

	template<typename T> concept NPCEntity = is_entity<T>::value && is_npc_entity<T>::value;

	template<typename T> struct is_animate {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_animate_v = is_animate<T>::value;

	template<typename T> concept AnimateEntity = is_entity<T>::value && is_animate<T>::value;

	template<typename T> struct is_inanimate {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_inanimate_v = is_inanimate<T>::value;

	template<typename T> concept InanimateEntity = is_entity<T>::value && is_inanimate<T>::value;

	template<typename T> struct is_non_player_entity {
		static constexpr bool value = true;
	};

	template<typename T> constexpr bool is_non_player_v = is_non_player_entity<T>::value;

	template<typename T> concept NonPlayerEntity = is_entity<T>::value && is_non_player_entity<T>::value;

	template<typename T> struct is_player {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_player_v = is_player<T>::value;

	template<typename T> concept PlayerEntity = is_entity<T>::value && is_player<T>::value;

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

	template<Entity EntityType> inline constexpr glyph_t entity_glyphs;

	template<> inline constexpr glyph_t entity_glyphs<std::nullptr_t>{ 0x40, colors::White };

	constexpr glyph_t PlayerArmoredGlyph{ 0x42, colors::White };

	constexpr glyph_t AnimatedSkullGlyph{ 0x44, colors::White };

	constexpr glyph_t EnergyGlyph{ 0x4B, colors::White };
	constexpr glyph_t ArmorGlyph{ 0x4C, colors::White };
} // namespace necrowarp
