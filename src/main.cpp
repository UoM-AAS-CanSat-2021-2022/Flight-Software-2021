#include <Arduino.h>
#include "util/sout.hpp"
#include "xbee/manager.hpp"

XBeeManager xbm;

void setup() {
	Serial.begin(230400);
	xbm.setup(Serial1);
}

void loop() {
	static bool ran = false;
	if (!ran) {
		xbm.set_panid(0x6969);
		ran = true;
	}
}
