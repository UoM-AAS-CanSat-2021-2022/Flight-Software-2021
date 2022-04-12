#include <Arduino.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include <cstdint>
#include <iomanip>
#include <string>
#include "util/sout.hpp"

constexpr static int led = LED_BUILTIN;
SoftwareSerial XBeeSerial { PIN_SERIAL_RX, PIN_SERIAL_TX };
XBee xbee {};

void setup() {
	pinMode(led, OUTPUT);

	Serial.begin(230400);
	XBeeSerial.begin(230400);
	xbee.setSerial(XBeeSerial);
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
