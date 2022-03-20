#include <Arduino.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include "util/sout.hpp"

constexpr static int led = LED_BUILTIN;
SoftwareSerial XBeeSerial { PIN_SERIAL_RX, PIN_SERIAL_TX };
XBee xbee {};

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	XBeeSerial.begin(9600);
	xbee.setSerial(XBeeSerial);
}

void loop() {
	static std::uint8_t payload[] { "beans" };
	static Tx16Request tx { 1057, payload, sizeof(payload) };
	xbee.send(tx);
	delay(1000);
}
