#include <Arduino.h>
#include <XBee.h>
#include "util/sout.hpp"

constexpr static int led = LED_BUILTIN;
XBee xbee {};

void setup() {
	Serial.begin(230400);
	Serial1.begin(230400);
	xbee.setSerial(Serial1);
}

void loop() {
	static std::string payload { "BRO WHERE ARE MY BEANS????" };
	static Tx16Request req {
		0x0000,
		reinterpret_cast<std::uint8_t*>(payload.data()),
		static_cast<std::uint8_t>(payload.size())
	};

	xbee.send(req);

	delay(5000);
}
