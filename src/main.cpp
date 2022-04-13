#include <Arduino.h>
#include "xbee/manager.hpp"
#include "util/sout.hpp"

XBeeManager xbm;

void setup() {
	Serial.begin(9600);
	xbm.setup(Serial1);
	xbm.set_panid(6057);
}

void loop() {
	static std::string message = "Hi there!";
	static auto last_sent = 0;
	if ((millis() - last_sent) > 1000) {
		xbm.send(message);
		last_sent = millis();
	}

	xbm.loop();
}
