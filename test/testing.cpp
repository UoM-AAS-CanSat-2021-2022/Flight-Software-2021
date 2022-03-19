#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <variant>

#include "command.hpp"

struct CommandVisitor {
	void operator()(const std::monostate&) {
		std::cout << "Error parsing command." << std::endl;
	}

	void operator()(const Command::OnOff& on_off) {
		std::cout << "Got ON_OFF value: " << on_off << std::endl;
	}

	void operator()(const Command::UtcTime& utc_time) {
		std::cout << "Got UTC_TIME value: " << utc_time << std::endl;
	}

	void operator()(const Command::Mode& mode) {
		std::cout << "Got MODE value: " << mode << std::endl;
	}

	void operator()(const Command::Pressure& pressure) {
		std::cout << "Got PRESSURE value: " << pressure << std::endl;
	}
};

int main() {
	constexpr std::array<std::string_view, 4> inps = {
		"CMD,1057,CX,ON",
		"CMD,1057,ST,12:43:51",
		"CMD,1057,SIM,ACTIVATE",
		"CMD,1057,SIMP,111364",
	};

	constexpr auto longest = std::max_element(
		inps.begin(),
		inps.end(),
		[](auto a, auto b) {
			return a.size() < b.size();
		})->size();

	std::cout << std::boolalpha << std::left;
	for (const auto & inp : inps) {
		std::cout << "Parsing " << std::setw(longest + 3) << std::quoted(inp);
		const std::string s{ inp };
		auto val = Command::parse(s);
		std::visit(CommandVisitor {}, val);
	}

	return 0;
}
