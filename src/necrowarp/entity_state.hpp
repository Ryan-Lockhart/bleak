#pragma once

#include <necrowarp/entities.hpp>

namespace necrowarp {
	using namespace bleak;

	static inline player_t player{};

	static inline std::unordered_map<offset_t, skull_t, offset_t::hasher> skulls{};
	static inline std::unordered_map<offset_t, skeleton_t, offset_t::hasher> skeletons{};
	static inline std::unordered_map<offset_t, adventurer_t, offset_t::hasher> adventurers{};

	struct occupation_map_t {
		inline bool operator[](cref<offset_t> position) const noexcept { return player.position == position || skulls.contains(position) || skeletons.contains(position) || adventurers.contains(position); }
	} static inline occupation_map{};

	struct entity_registry_t {
		inline entity_type_t operator[](cref<offset_t> position) const noexcept {
			if (player.position == position) {
				return entity_type_t::Player;
			} else if (skulls.contains(position)) {
				return entity_type_t::Skull;
			} else if (skeletons.contains(position)) {
				return entity_type_t::Skeleton;
			} else if (adventurers.contains(position)) {
				return entity_type_t::Adventurer;
			} else {
				return entity_type_t::None;
			}
		}

		template<typename T> requires is_entity<T>::value inline bool add(rval<T> entity, cref<offset_t> position) noexcept {
			if (occupation_map[position]) {
				return false;
			}
			
			if constexpr (is_entity_type<T, entity_type_t::Skull>::value) {
				skulls.emplace(position, entity);
				return true;
			} else if constexpr (is_entity_type<T, entity_type_t::Skeleton>::value) {
				skeletons.emplace(position, entity);
				return true;
			} else if constexpr (is_entity_type<T, entity_type_t::Adventurer>::value) {
				adventurers.emplace(position, entity);
				return true;
			}

			return false;
		}

		template<entity_type_t EntityType> inline bool remove(cref<offset_t> position) noexcept {
			if (!occupation_map[position]) {
				return false;
			}
			
			if constexpr (EntityType == entity_type_t::Skull) {
				skulls.erase(position);
				return true;
			} else if constexpr (EntityType == entity_type_t::Skeleton) {
				skeletons.erase(position);
				return true;
			} else if constexpr (EntityType == entity_type_t::Adventurer) {
				adventurers.erase(position);
				return true;
			}

			return false;
		}

		template<entity_type_t EntityType> inline bool update(cref<offset_t> current, cref<offset_t> target) noexcept {
			if (!occupation_map[current] || occupation_map[target]) {
				return false;
			}

			if constexpr (EntityType == entity_type_t::Skull) {
				if (!skulls.contains(current)) {
					return false;
				}

				rvauto skull = skulls.extract(current);

				if (skull.empty()) {
					return false;
				}
				
				skulls.emplace(current, skull.mapped());

				return true;
			} else if constexpr (EntityType == entity_type_t::Skeleton) {
				if (!skeletons.contains(current)) {
					return false;
				}

				rvauto skeleton = skeletons.extract(current);

				if (skeleton.empty()) {
					return false;
				}

				skeletons.emplace(current, skeleton.mapped());

				return true;
			} else if constexpr (EntityType == entity_type_t::Adventurer) {
				if (!adventurers.contains(current)) {
					return false;
				}

				rvauto adventurer = adventurers.extract(current);

				if (adventurer.empty()) {
					return false;
				}

				adventurers.emplace(current, adventurer.mapped());

				return true;
			}

			return false;
		}
	} static inline entity_registry{};
}