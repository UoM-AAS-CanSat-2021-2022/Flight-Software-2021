#include <Arduino.h>
#include "xbee/manager.hpp"
#include "sensor/bmp388.hpp"
#include "util/sout.hpp"

XBeeManager xbm;

void setup() {
	Serial.begin(9600);
	bmp388::setup();
	xbm.setup(Serial1);
	xbm.set_panid(6057);
}

void loop() {
	xbm.loop();

	const auto readings = bmp388::read_all();
	if (!readings) return;
	
	const auto [temp, press, alt] = *readings;
	
	sout << "Temperature = " << temp << " *C\n";
	sout << "Pressure = " << press / 100.0 << " hPa\n";
	sout << "Approx Alt = " << alt << " m" << std::endl;
}
