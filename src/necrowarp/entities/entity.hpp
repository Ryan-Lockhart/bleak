#pragma once

#include <bleak.hpp>

#include <cstddef>
#include <optional>

namespace necrowarp {
	using namespace bleak;

	enum struct entity_type_t : u8 {
		None = 0,
		Player,
		Skull,
		Skeleton,
		Adventurer,
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
		case entity_type_t::Adventurer:
			return "adventurer";
		}
	}

	constexpr usize EntityTypeCount{ static_cast<usize>(entity_type_t::Adventurer) + 1 };

	template<typename T> struct is_entity {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_entity_v = is_entity<T>::value;

	template<> struct is_entity<std::nullptr_t> {
		static constexpr bool value = true;
	};

	enum struct command_type_t : u8 {
		None = 0,
		Move,
		Consume,
		Clash,
		RandomWarp,
		TargetWarp,
		ConsumeWarp,
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
		pair_t<entity_type_t, glyph_t>{ entity_type_t::None, glyph_t{ 0x00, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Player, glyph_t{ 0x40, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Skull, glyph_t{ 0x46, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Skeleton, glyph_t{ 0x43, colors::White } },
		pair_t<entity_type_t, glyph_t>{ entity_type_t::Adventurer, glyph_t{ 0x42, colors::White } }
	};

	constexpr glyph_t PlayerArmored{ 0x41, colors::White };
} // namespace necrowarp
