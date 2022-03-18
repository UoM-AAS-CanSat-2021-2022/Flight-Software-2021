#include <iostream>
#include <iomanip>
#include <variant>

#include "command.hpp"

struct CommandVisitor {
	void operator()([[maybe_unused]] std::monostate& _) {
		std::cout << "Error parsing command." << std::endl;
	}

	void operator()(Command::OnOff& on_off) {
		std::cout << "Got ON_OFF value: " << on_off << std::endl;
	}

	void operator()(Command::UtcTime& utc_time) {
		std::cout << "Got UTC_TIME value: " << utc_time << std::endl;
	}

	void operator()(Command::Mode& mode) {
		std::cout << "Got MODE value: " << mode << std::endl;
	}

	void operator()(Command::Pressure& pressure) {
		std::cout << "Got PRESSURE value: " << pressure << std::endl;
	}
};

int main() {
	auto inps = {
		"CMD,1057,CX,ON",
		"CMD,1057,ST,12:43:51",
		"CMD,1057,SIM,ACTIVATE",
		"CMD,1057,SIMP,111364",
	};

	std::cout << std::boolalpha;
	for (const auto & a : inps) {
		std::cout << "Parsing " << std::quoted(a) << '\t';
		auto val = Command::parse(a);
		std::visit(CommandVisitor {}, val);
	}

	return 0;
}
