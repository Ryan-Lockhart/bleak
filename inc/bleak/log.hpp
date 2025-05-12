#pragma once

#include <bleak/typedef.hpp>

#include <format>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include <SDL.h>

namespace bleak {
	namespace sdl {
		static inline cstr get_error() { return SDL_GetError(); }
	}

	class log_t {
	  private:
		std::string name;
		std::list<std::string> messages;
		usize max_messages;

	  public:
		using iterator = std::list<std::string>::iterator;
		using const_iterator = std::list<std::string>::const_iterator;

		inline log_t(cref<std::string> name, usize maxMessages = 16) : name { name }, messages {}, max_messages { maxMessages } {}

		inline log_t(cref<log_t> other) noexcept : name { other.name }, messages { other.messages }, max_messages { other.max_messages } {}

		inline log_t(rval<log_t> other) noexcept : name { std::move(other.name) }, messages(std::move(other.messages)), max_messages(std::move(other.max_messages)) {}

		inline ref<log_t> operator=(cref<log_t> other) noexcept {
			name = other.name;
			messages = other.messages;
			max_messages = other.max_messages;

			return *this;
		}

		inline ref<log_t> operator=(rval<log_t> other) noexcept {
			name = std::move(other.name);
			messages = std::move(other.messages);
			max_messages = std::move(other.max_messages);

			return *this;
		}

		inline iterator begin() { return messages.begin(); }

		inline iterator end() { return messages.end(); }

		inline const_iterator begin() const { return messages.begin(); }

		inline const_iterator end() const { return messages.end(); }

		inline iterator operator[](usize index) { return std::next(messages.begin(), index); }

		inline const_iterator operator[](usize index) const { return std::next(messages.begin(), index); }

		inline ref<std::string> at(usize index) {
			if (index >= messages.size()) {
				throw(std::out_of_range("Index out of range"));
			}

			return *std::next(messages.begin(), index);
		}

		inline cref<std::string> at(usize index) const {
			if (index >= messages.size()) {
				throw(std::out_of_range("Index out of range"));
			}

			return *std::next(messages.begin(), index);
		}

		inline usize size() const { return messages.size(); }

		inline usize capacity() const { return max_messages; }

		inline ref<std::string> front() { return messages.front(); }

		inline ref<std::string> back() { return messages.back(); }

		inline cref<std::string> front() const { return messages.front(); }

		inline cref<std::string> back() const { return messages.back(); }

		inline void prune() {
			if (max_messages <= 0) {
				return;
			}

			while (messages.size() > max_messages) {
				messages.pop_front();
			}
		}

		inline void prune(usize count) {
			if (count <= 0) {
				return;
			}

			while (messages.size() > count) {
				messages.pop_front();
			}
		}

		inline void clear() { messages.clear(); }

		inline void add(cstr message) { messages.push_back(message); }

		inline void add(std::string message) { messages.push_back(message); }

		inline void add(cstr message, cstr time, cstr file, i32 line) { messages.push_back(std::format("[{}]: \"{}\" ({}): {}", time, file, line, message)); }

		inline void add(std::string message, cstr time, cstr file, i32 line) {
			messages.push_back(std::format("[{}]: \"{}\" ({}): {}", time, file, line, message));
		}

		template<typename... Args> inline void add(const std::format_string<Args...> format, rval<Args>... args) {
			messages.push_back(std::format(format, std::forward<Args>(args)...));
		}

		template<typename... Args> inline void add(const std::format_string<Args...> format, cstr time, cstr file, i32 line, rval<Args>... args) {
			messages.push_back(std::format(std::format("[{}]: \"{}\" ({}): {}", time, file, line, format), std::forward<Args>(args)...));
		}

		inline operator std::string() const {
			std::string result;

			for (cref<std::string> message : messages) {
				result += message + "\n";
			}

			return result;
		}

		inline void flush_to_file() {
			std::ofstream file { "./log/" + name + ".log", std::ios::out | std::ios::trunc };

			usize idx{ 0 };
			for (cref<std::string> message : messages) {
				file << message << ((idx++ < messages.size()) ? "\n" : "");
			}

			file.close();
		}
		
		inline void flush_to_console(ref<std::ostream> stream = std::cout) {
			usize idx{ 0 };
			for (cref<std::string> message : messages) {
				stream << message << ((idx++ < messages.size()) ? "\n" : "");
			}

			stream.flush();
		}
	};

	static inline log_t message_log { "message" };
	static inline log_t error_log { "error" };
} // namespace Bleakdepth
