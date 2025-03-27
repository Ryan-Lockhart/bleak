#pragma once

#include <bleak/typedef.hpp>

#include <cmath>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <bleak/log.hpp>

namespace necrowarp {
	using namespace bleak;

	class command_parser_t {
		enum class command_e : u8 {
			Unknown = 0,
			Energy,
			Armor,
			Divinity,

			NoHit,
			FreeCosts,
			BypassInvocations
		};

		enum class state_e : u8 {
			Unknown = 0,
			Enable,
			Disable
		};

		struct command_t {
			command_e type;
			state_e state;

			std::optional<i16> minimum;
			std::optional<i16> maximum;
		};

		constexpr cstr to_string(command_e type) const noexcept {
			switch (type) {
				case command_e::Energy: {
					return "vigorous";
				} case command_e::Armor: {
					return "bulwark";
				} case command_e::Divinity: {
					return "larian";
				} case command_e::NoHit: {
					return "interstice";
				} case command_e::FreeCosts: {
					return "unlimited_power";
				} case command_e::BypassInvocations: {
					return "runemaster";
				} default: {
					return "";
				}
			}
		}

		constexpr bool has_keywords(std::string_view command_str) const noexcept {
			if (command_str.empty()) {
				return false;
			}

			if (!command_str.contains("enable") && !command_str.contains("disable")) {
				return false;
			}

			for (u8 i{ 0 }; i < (u8)command_e::BypassInvocations; ++i) {
				if (!command_str.contains(to_string((command_e)i))) {
					continue;
				}

				return true;
			}

			return false;
		};

		constexpr command_e get_command(std::string_view str) const noexcept {
			if (str.empty()) {
				return command_e::Unknown;
			}

			for (u8 i{ 0 }; i < (u8)command_e::BypassInvocations; ++i) {
				if (str != to_string((command_e)i)) {
					continue;
				}

				return (command_e)i;
			}

			return command_e::Unknown;
		}

		constexpr state_e get_state(std::string_view str) const noexcept {
			if (str.empty()) {
				return state_e::Unknown;
			}

			if (str == "enable") {
				return state_e::Enable;
			} else if (str != "disable") {
				return state_e::Disable;
			} else {
				return state_e::Unknown;
			}
		}

		constexpr std::optional<command_t> parse_command(cref<std::string> command_str) const noexcept {
			if (!has_keywords(command_str)) {
				error_log.add("ERROR: \"{}\" is not a valid command; no keywords detected", command_str);
				return std::nullopt;
			}

			std::vector<std::string_view> tokens{};

			for (cauto token : std::views::split(command_str, " ")) {
				tokens.emplace_back(token);
			}

			if (tokens.size() <= 1 || tokens.size() > 4) {
				error_log.add("ERROR: \"{}\" is not a valid command; token count is out of range", command_str);

				return std::nullopt;
			}

			const command_e command{ get_command(tokens[0]) };
			const state_e state{ get_state(tokens[1]) };
			
			switch (command) {
				case command_e::NoHit:
				case command_e::FreeCosts:
				case command_e::BypassInvocations: {
					if (tokens.size() != 2 || state == state_e::Unknown) {
						error_log.add("ERROR: \"{}\" is a unary command; use \"enable\" or \"disable\" in conjunction", command_str);

						return std::nullopt;
					}

					return command_t{
						command,
						state,

						std::nullopt,
						std::nullopt
					};
				} case command_e::Energy:
				  case command_e::Armor:
				  case command_e::Divinity: {
					switch (state) {
						case state_e::Enable: {
							if (tokens.size() != 4) {
								error_log.add("ERROR: \"{}\" is a ternary command; four valid tokens must be supplied", command_str);

								return std::nullopt;
							}

							try {
								std::string temp{ tokens[2] };

								const i16 minimum{ static_cast<i16>(std::stoi(temp)) };

								temp = tokens[3];

								const i16 maximum{ static_cast<i16>(std::stoi(temp)) };

								if (minimum > maximum || minimum < 0) {
									error_log.add("ERROR: \"{}\" is a ternary command; the minimum cannot exceed the maximum", command_str);

									return std::nullopt;
								}

								return command_t{
									command,
									state,

									minimum,
									maximum
								};
							} catch (std::invalid_argument) {
								error_log.add("ERROR: failed to parse minimum or maximum of \"{}\"", command_str);
								return std::nullopt;
							}
						} case state_e::Disable: {
							if (tokens.size() > 2) {
								error_log.add("ERROR: \"{}\" is a unary command when using \"disable\"", command_str);
								return std::nullopt;
							}

							return command_t{
								command,
								state,

								std::nullopt,
								std::nullopt
							};
						} default: {
							error_log.add("ERROR: unknown state in \"{}\"; use \"enable\" or \"disable\"", command_str);

							return std::nullopt;
						}
					}
				} default: {
					error_log.add("ERROR: \"{}\" is not a valid command", command_str);

					return std::nullopt;
				}
			}
		}
	};
} // namespace necrowarp
