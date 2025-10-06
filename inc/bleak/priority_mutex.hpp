#pragma once

#include <bleak/typedef.hpp>

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <bleak/concepts.hpp>

namespace bleak {
	template<typename T> struct is_mutex {
		static constexpr bool value = false;
	};

	template<typename T> constexpr bool is_mutex_v = is_mutex<T>::value;

	template<typename T> concept Mutex = is_mutex<T>::value;

	template<> struct is_mutex<std::mutex> { static constexpr bool value = true; };
	template<> struct is_mutex<std::timed_mutex> { static constexpr bool value = true; };

	template<> struct is_mutex<std::recursive_mutex> { static constexpr bool value = true; };
	template<> struct is_mutex<std::recursive_timed_mutex> { static constexpr bool value = true; };

	struct priority_mutex {
	  private:
		std::condition_variable cv{};
		std::mutex access{};

		std::thread::id priority_tid{};

		bool locked{};

	  public:
		inline priority_mutex() noexcept : locked{ false } {}

		inline ~priority_mutex() noexcept { assert(!locked); }

		inline priority_mutex(cref<priority_mutex> other) noexcept = delete;
		inline ref<priority_mutex> operator=(cref<priority_mutex> other) noexcept = delete;

		inline bool is_locked() const noexcept { return locked; }

		inline bool has_priority_tid() const noexcept { return priority_tid != std::thread::id{}; }

		inline bool is_priority_tid() const noexcept { return priority_tid == std::this_thread::get_id(); }
		inline bool is_priority_tid(std::thread::id tid) const noexcept { return priority_tid == tid; }

		inline void set_priority_tid() noexcept { priority_tid = std::this_thread::get_id(); }
		inline void set_priority_tid(std::thread::id tid) noexcept { priority_tid = tid; }

		inline void reset_priority_tid() noexcept { priority_tid = std::thread::id{}; }

		template<bool Prioritize = false> inline void lock() noexcept {
			cauto current_tid{ std::this_thread::get_id() };

			std::unique_lock lock{ access };

			if constexpr (Prioritize) {
				set_priority_tid(current_tid);
			}

			cv.wait(lock, [&]() -> bool {
				return !is_locked() && (!has_priority_tid() || is_priority_tid(current_tid));
			});

			locked = true;
		}

		inline bool try_lock() noexcept {
			if (is_locked()) {
				return false;
			}

			cauto current_tid{ std::this_thread::get_id() };

			std::unique_lock lock{ access };

			cv.wait(lock, [&]() -> bool {
				return !is_locked() && (!has_priority_tid() || is_priority_tid(current_tid));
			});

			locked = true;

			return true;
		}

		template<typename Period> inline bool try_lock(Period period) noexcept {
			std::unique_lock<std::mutex> lock{ access };

			const std::cv_status result = cv.wait_for(lock, period);

			switch (result) {
				case std::cv_status::timeout: {
					return false;
				} default: {
					locked = true;

					return true;
				}
			}
		}

		inline void unlock() noexcept {
			cauto current_tid{ std::this_thread::get_id() };

			std::lock_guard lock{ access };

			if (is_priority_tid(current_tid)) {
				reset_priority_tid();
			}

			locked = false;

			cv.notify_all();
		}
	};

	template<> struct is_mutex<priority_mutex> { static constexpr bool value = true; };
} //namespace bleak
