#pragma once

#include <bleak/typedef.hpp>

#include <cassert>

#include <bleak/concepts.hpp>
#include <bleak/random.hpp>
#include <bleak/sound.hpp>

namespace bleak {
	template<usize Size> struct clip_pool_t {
		static_assert(Size > 0, "size must be greater than zero!");

	  private:
		sound_t sounds[Size];
		std::uniform_int_distribution<usize> distribution{ 0, Size - 1 };

	  public:
		template<typename... Paths>
			requires is_homogeneous<cstr, Paths...>::value && (sizeof...(Paths) == Size)
		clip_pool_t(Paths... paths) noexcept : sounds{ paths... } {}

		template<typename... Sounds>
			requires is_homogeneous<sound_t, Sounds...>::value && (sizeof...(Sounds) == Size)
		constexpr clip_pool_t(rval<Sounds>... sounds) noexcept : sounds{ std::move(sounds)... } {}

		constexpr ref<sound_t> operator[](usize index) noexcept {
			assert(index < Size);

			return sounds[index];
		}

		constexpr cref<sound_t> operator[](usize index) const noexcept {
			assert(index < Size);

			return sounds[index];
		}

		constexpr void play() noexcept { sounds[std::random_device{}() % Size].play(); }

		constexpr void play(usize index) noexcept {
			assert(index < Size);

			sounds[index].play();
		}

		template<typename Generator>
			requires is_random_engine<Generator>::value
		constexpr void play(ref<Generator> generator) noexcept {
			sounds[distribution(generator)].play();
		}

		constexpr void delay(usize interval) noexcept { sounds[std::random_device{}() % Size].delay(interval); }

		constexpr void delay(usize interval, usize index) noexcept {
			assert(index < Size);

			sounds[index].delay(interval);
		}

		template<typename Generator>
			requires is_random_engine<Generator>::value
		constexpr void delay(usize interval, ref<Generator> generator) noexcept {
			sounds[distribution(generator)].delay(interval);
		}
	};
} // namespace bleak
