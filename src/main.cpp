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
	xbm.loop();
}
