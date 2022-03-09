#include <Arduino.h>
#include <cstdint>

#include "command.hpp"
#include "sout.hpp"

const int led = LED_BUILTIN;
bool led_on = false;

struct CommandVisitor {
	void operator()(std::monostate& _) {
		// Stops the compiler warning
		_ = _;
		sout << "Error parsing command." << std::endl;
	}

	void operator()(Command::OnOff& on_off) {
		sout << "Got ON_OFF value: " << std::boolalpha << on_off << std::endl << std::noboolalpha;
		led_on = on_off;
	}

	void operator()(Command::UtcTime& utc_time) {
		sout << "Got UTC_TIME value: " << utc_time << std::endl;
	}

	void operator()(Command::Mode& mode) {
		sout << "Got MODE value: " << mode << std::endl;
	}

	void operator()(Command::Pressure& pressure) {
		sout << "Got PRESSURE value: " << pressure << std::endl;
	}
};

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	static Command::Processor cmd_processor { Serial };
	static CommandVisitor visitor {};

	delay(300);
	auto cmd = cmd_processor.next_command();
	if (cmd) {
		std::visit(visitor, *cmd);
	}

	digitalWrite(led, led_on ? HIGH : LOW);
}
