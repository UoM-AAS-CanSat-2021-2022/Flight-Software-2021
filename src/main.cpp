#include <Arduino.h>
#include <iostream>

#include "command.hpp"

const int led = LED_BUILTIN;
bool led_on = false;

struct CommandVisitor {
	void operator()(std::monostate& _) {
		// Stops the compiler warning
		_ = _;
		//std::cout << "Error parsing command." << std::endl;
		Serial.println("Error parsing command.");
	}

	void operator()(Command::OnOff& on_off) {
		// std::cout << "Got ON_OFF value: " << on_off << std::endl;
		Serial.println("Got ON_OFF value.");
		led_on = on_off;
	}

	void operator()(Command::UtcTime& utc_time) {
		//std::cout << "Got UTC_TIME value: " << utc_time << std::endl;
		Serial.println("Got UTC_TIME value.");
	}

	void operator()(Command::Mode& mode) {
		//std::cout << "Got MODE value: " << mode << std::endl;
		Serial.println("Got MODE value.");
	}

	void operator()(Command::Pressure& pressure) {
		//std::cout << "Got PRESSURE value: " << pressure << std::endl;
		Serial.println("Got PRESSURE value.");
	}
};

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	static auto cmd_processor = Command::Processor(Serial);

	delay(100);
	auto cmd = cmd_processor.next_command();
	if (cmd) {
		std::visit(CommandVisitor {}, *cmd);
	}
	digitalWrite(led, led_on ? HIGH : LOW);
}
