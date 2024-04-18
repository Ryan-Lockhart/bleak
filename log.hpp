#pragma once

#include "typedef.hpp"

#include <list>
#include <string>
#include <initializer_list>

class Log
{
private:
	std::list<std::string> messages;
	usize maxMessages;

public:
	Log(usize maxMessages = 16) : messages(), maxMessages(maxMessages) {}

	template<typename... Messages, typename = cstr>
	Log(Messages... messages, usize maxMessages = 16) : messages({ messages... }), maxMessages(maxMessages) { prune(); }

	template<typename... Messages, typename = std::string>
	Log(Messages... messages, usize maxMessages = 16) : messages({ messages... }), maxMessages(maxMessages) { prune(); }

	void prune();

	void log(cstr message);
	void log(std::string message);

	template<typename... Messages, typename = cstr>
	void log(cstr format, Messages... args);

	template<typename... Messages, typename = std::string>
	void log(std::string format, Messages... args);

};

void log(cstr message) { MESSAGES.emplace(message); }
void log(cref<string> message) { MESSAGES.emplace(message); }

static void Prune(ref<std::queue<string>> queue, usize count) { while (queue.size() > count) queue.pop(); }

#include <fmt/core.h>

void log(cstr message, cstr file, usize line) { ERRORS.emplace(fmt::format("[{}]: \"{}\" ({}): {}", __TIME__, file, line, message)); }
void log(cref<string> message, cstr file, usize line) { ERRORS.emplace(fmt::format("[{}]: \"{}\" ({}): {}", __TIME__, file, line, message)); }