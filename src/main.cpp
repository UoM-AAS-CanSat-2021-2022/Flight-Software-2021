// this must be included before Arduino.h, because Arduino.h produces colliding macro names
#include <fmt/core.h>

#include <Arduino.h>
#include <TimeLib.h>

#include "xbee/manager.hpp"
#include "telemetry/manager.hpp"
#include "sensor/bmp388.hpp"

XBeeManager xbm;
TelemetryManager telem_mgr { xbm };

time_t getTeensy3Time();

void setup() {
	Serial.begin(230400);
	bmp388::setup();
	xbm.setup(Serial1);
	xbm.set_panid(6057);
	setSyncProvider(getTeensy3Time);
}

void loop() {
	xbm.loop();

	static auto container_last_read = 0;
	if (millis() - container_last_read > 1000) {
		telem_mgr.send_container_telemetry();
		container_last_read = millis();
	}

	static auto payload_last_read = 0;
	if (millis() - payload_last_read > 250) {
		std::string_view mock_payload_relay_data { "165.2,13.7,5.02,0.18,0.08,-0.18,0.12,0.31,9.8,0.19,-0.05,0.47,12,LANDED" };
		telem_mgr.forward_payload_telemetry(mock_payload_relay_data);
		payload_last_read = millis();
	}
}

time_t getTeensy3Time() {
	return Teensy3Clock.get();
}
