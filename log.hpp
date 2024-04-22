#pragma once

#include "typedef.hpp"

#include <list>
#include <string>

#include <fmt/format.h>

class Log
{
private:
	std::list<std::string> messages;
	usize maxMessages;

public:
	using iterator = std::list<std::string>::iterator;
	using const_iterator = std::list<std::string>::const_iterator;

	inline Log(usize maxMessages = 16) : messages(), maxMessages(maxMessages) {}

	inline Log(cref<Log> other) noexcept : messages(other.messages), maxMessages(other.maxMessages) {}
	inline Log(rval<Log> other) noexcept : messages(std::move(other.messages)), maxMessages(std::move(other.maxMessages)) {}

	inline ref<Log> operator=(cref<Log> other) noexcept
	{
		messages = other.messages;
		maxMessages = other.maxMessages;

		return *this;
	}
	inline ref<Log> operator=(rval<Log> other) noexcept
	{
		messages = std::move(other.messages);
		maxMessages = std::move(other.maxMessages);

		return *this;
	}

	inline iterator begin() { return messages.begin(); }
	inline iterator end() { return messages.end(); }

	inline const_iterator begin() const { return messages.begin(); }
	inline const_iterator end() const { return messages.end(); }

	inline iterator operator[](usize index) { return std::next(messages.begin(), index); }
	inline const_iterator operator[](usize index) const { return std::next(messages.begin(), index); }

	inline ref<std::string> at(usize index)
	{
		if (index >= messages.size())
			throw(std::out_of_range("Index out of range"));

		return *std::next(messages.begin(), index);
	}
	inline cref<std::string> at(usize index) const
	{
		if (index >= messages.size())
			throw(std::out_of_range("Index out of range"));

		return *std::next(messages.begin(), index);
	}

	inline usize size() const { return messages.size(); }
	inline usize capacity() const { return maxMessages; }

	inline ref<std::string> front() { return messages.front(); }
	inline ref<std::string> back() { return messages.back(); }

	inline cref<std::string> front() const { return messages.front(); }
	inline cref<std::string> back() const { return messages.back(); }

	inline void prune()
	{
		if (maxMessages <= 0)
			return;

		while (messages.size() > maxMessages)
			messages.pop_front();
	}

	inline void prune(usize count)
	{
		if (count <= 0)
			return;

		while (messages.size() > count)
			messages.pop_front();
	}

	inline void clear() { messages.clear(); }

	inline void log(cstr message)
	{
		messages.push_back(message);
	}

	inline void log(std::string message)
	{
		messages.push_back(message);
	}

	inline void debug(cstr message, cstr time, cstr file, usize line)
	{
		messages.push_back(
			fmt::format(
				"[{}]: \"{}\" ({}): {}",
				time,
				file,
				line,
				message
			)
		);
	}

	inline void debug(std::string message, cstr time, cstr file, usize line)
	{
		messages.push_back(
			fmt::format(
				"[{}]: \"{}\" ({}): {}",
				time,
				file,
				line,
				message
			)
		);
	}
	
	template<typename... Args, typename = cstr>
	inline void log(cstr format, Args... args) { messages.push_back(fmt::format(format, { args... })); }

	template<typename... Args, typename = std::string>
	inline void log(std::string format, Args... args) { messages.push_back(fmt::format(format, { args... })); }

	template<typename... Args, typename = cstr>
	inline void debug(cstr format, cstr time, cstr file, usize line, Args... args)
	{
		messages.push_back(
			fmt::format(
				fmt::format(
					"[{}]: \"{}\" ({}): {}",
					time,
					file,
					line,
					format
				).c_str(),
				{ args... }
			)
		);
	}

	template<typename... Args, typename = std::string>
	inline void debug(std::string format, cstr time, cstr file, usize line, Args... args)
	{
		messages.push_back(
			fmt::format(
				fmt::format(
					"[{}]: \"{}\" ({}): {}",
					time,
					file,
					line,
					format
				).c_str(),
				{ args... }
			)
		);
	}

	inline operator std::string() const
	{
		std::string result;

		for (cref<std::string> message : messages)
			result += message + "\n";

		return result;
	}
};
