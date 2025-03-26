#pragma once

#include <necrowarp/entities/entity.hpp>

#include <necrowarp/game_state.hpp>

namespace necrowarp {
	using namespace bleak;

	template<> struct is_entity<ladder_t> {
		static constexpr bool value = true;
	};

	template<> struct is_entity_type<ladder_t, entity_type_t::Ladder> {
		static constexpr bool value = true;
	};

	template<> struct to_entity_type<entity_type_t::Ladder> {
		using type = ladder_t;
	};

	template<> struct is_inanimate<ladder_t> {
		static constexpr bool value = true;
	};

	template<> inline constexpr glyph_t entity_glyphs<ladder_t>{ glyphs::UpLadder };

	enum class verticality_t : u8 {
		Up,
		Down
	};

	constexpr cstr to_string(verticality_t type) noexcept {
		switch (type) {
			case verticality_t::Up: {
				return "up";
			} case verticality_t::Down: {
				return "down";
			} default: {
				return "unknown";
			}
		}
	}

	enum class shackle_type_t : u8 {
		None,
		Calcitic,
		Spectral,
		Sanguine
	};

	constexpr cstr to_string(shackle_type_t type) noexcept {
		switch (type) {
			case shackle_type_t::Calcitic: {
				return "spooky shackle";
			} case shackle_type_t::Spectral: {
				return "eldritch shackle";
			} case shackle_type_t::Sanguine: {
				return "bloody shackle";
			} default: {
				return "unshackled";
			}
		}
	}

	constexpr runes_t to_colored_string(shackle_type_t type) noexcept {
		switch (type) {
			case shackle_type_t::Calcitic: {
				return runes_t{ to_string(type), colors::metals::shackles::Calcitic };
			} case shackle_type_t::Spectral: {
				return runes_t{ to_string(type), colors::metals::shackles::Spectral };
			}case shackle_type_t::Sanguine: {
				return runes_t{ to_string(type), colors::metals::shackles::Sanguine };
			} default: {
				return runes_t{ to_string(type), colors::metals::Iron };
			}
		}
	}

	struct ladder_t {
	  private:
		inline void draw_shackle(offset_t draw_position) const noexcept {
			switch (shackle) {
				case shackle_type_t::Calcitic: {
					game_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Calcitic }, draw_position);
					return;
				} case shackle_type_t::Spectral: {
					game_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Spectral }, draw_position);
					return;
				} case shackle_type_t::Sanguine: {
					game_atlas.draw(glyph_t{ characters::Shackle, colors::metals::shackles::Sanguine }, draw_position);
					return;
				} default: {
					return;
				}
			}
		}
		
	  public:
		offset_t position;
		
		verticality_t verticality;
		shackle_type_t shackle;

		inline ladder_t(cref<offset_t> position) noexcept : position{ position }, verticality{ verticality_t::Up }, shackle{ shackle_type_t::None } {}

		inline ladder_t(cref<offset_t> position, verticality_t verticality, bool random = false) noexcept :
			position{ position },
			verticality{ verticality },
			shackle{
				[random]() -> shackle_type_t {
					return random ?
						(shackle_type_t)std::uniform_int_distribution<u16>{ (u16)shackle_type_t::Calcitic, (u16)shackle_type_t::Sanguine }(random_engine) :
						shackle_type_t::None;
				}()
			}
		{}

		inline ladder_t(cref<offset_t> position, verticality_t verticality, shackle_type_t shackle) noexcept : position{ position }, verticality{ verticality }, shackle{ shackle } {}

		inline bool is_up_ladder() const noexcept { return verticality == verticality_t::Up; }

		inline bool is_down_ladder() const noexcept { return verticality == verticality_t::Down; }

		inline bool has_shackle() const noexcept { return shackle != shackle_type_t::None; }

		inline glyph_t current_glyph() const noexcept { return is_up_ladder() ? entity_glyphs<ladder_t> : glyphs::DownLadder; }

		inline void enshackle() noexcept {
			if (has_shackle()) {
				return;
			}
			
			shackle = (shackle_type_t)std::uniform_int_distribution<u16>{ (u16)shackle_type_t::Calcitic, (u16)shackle_type_t::Sanguine }(random_engine);
		}

		inline void enshackle(shackle_type_t type) noexcept {
			if (has_shackle() || type == shackle_type_t::None) {
				return;
			}
			
			shackle = type;
		}

		inline void unshackle() noexcept {
			if (!has_shackle()) {
				return;
			}

			shackle = shackle_type_t::None;
		};

		inline void draw() const noexcept {
			game_atlas.draw(current_glyph(), position);

			if (!has_shackle()) {
				return;
			}

			draw_shackle(position);
		}

		inline void draw(cref<offset_t> offset) const noexcept {
			const offset_t draw_position{ position + offset };

			game_atlas.draw(current_glyph(), draw_position);

			if (!has_shackle()) {
				return;
			}

			draw_shackle(draw_position);
		}

		inline void draw(cref<camera_t> camera) const noexcept {
			const offset_t draw_position{ position + camera.get_offset() };

			game_atlas.draw(current_glyph(), draw_position);

			if (!has_shackle()) {
				return;
			}

			draw_shackle(draw_position);
		}

		inline void draw(cref<camera_t> camera, cref<offset_t> offset) const noexcept {
			const offset_t draw_position{ position + camera.get_offset() + offset };

			game_atlas.draw(current_glyph(), draw_position);

			if (!has_shackle()) {
				return;
			}

			draw_shackle(draw_position);
		}

		constexpr operator entity_type_t() const noexcept { return entity_type_t::Ladder; }

		struct hasher {
			struct offset {
				using is_transparent = void;

				static constexpr usize operator()(cref<ladder_t> ladder) noexcept { return offset_t::hasher::operator()(ladder.position); }

				static constexpr usize operator()(cref<offset_t> position) noexcept { return offset_t::hasher::operator()(position); }
			};
		};

		struct comparator {
			struct offset {
				using is_transparent = void;
			
				static constexpr bool operator()(cref<ladder_t> lhs, cref<ladder_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs.position); }

				static constexpr bool operator()(cref<ladder_t> lhs, cref<offset_t> rhs) noexcept { return offset_t::hasher::operator()(lhs.position) == offset_t::hasher::operator()(rhs); }

				static constexpr bool operator()(cref<offset_t> lhs, cref<ladder_t> rhs) noexcept { return offset_t::hasher::operator()(lhs) == offset_t::hasher::operator()(rhs.position); }
			};
		};
	};
} // namespace necrowarp
