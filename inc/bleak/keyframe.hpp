#pragma once

#include <bleak/typedef.hpp>

#include <bleak/random.hpp>

namespace bleak {
	struct keyframe_t {
		static constexpr usize index_bits{ 8 };
		static constexpr usize frame_bits{ 4 };

		static constexpr usize length{ frame_bits * frame_bits };

		u8 index : index_bits;

	  private:
		mutable u8 frame : frame_bits;
		bool running : 1;

		static inline std::uniform_int_distribution<u16> frame_dis{ 0, length - 1 };

		template<RandomEngine Generator> static inline u8 random_frame(ref<Generator> generator) noexcept { return static_cast<u8>(frame_dis(generator)); }

	  public:
		inline keyframe_t(u8 index) noexcept : index{ index }, frame{ 0 }, running{ false } {}

		inline keyframe_t(u8 index, u8 frame, bool start = false) noexcept : index{ index }, frame{ frame }, running{ start } {}

		template<RandomEngine Generator> inline keyframe_t(u8 index, ref<Generator> generator, bool start = false) noexcept : index{ index }, frame{ random_frame<Generator>(generator) }, running{ start } {}

		inline bool is_running() const noexcept { return running; }

		inline u8 current_frame() const noexcept { return frame; }

		inline void start() noexcept { running = true; }
		inline void stop() noexcept { running = false; }

		inline void reset() const noexcept { frame = 0; }

		inline void advance() const noexcept {
			if (!is_running()) {
				return;
			}

			++frame;
		}

		inline void retreat() const noexcept {
			if (!is_running()) {
				return;
			}

			--frame;
		}
	};
} // namespace bleak
