// this must be included before Arduino.h, because Arduino.h produces colliding macro names
#include <fmt/core.h>
#include <optional>
#include <string>
#include <string_view>

#include "constants.hpp"
#include "runner.hpp"
#include "sensor/manager.hpp"
#include "telemetry/manager.hpp"
#include "util/sout.hpp"
#include "xbee/manager.hpp"

// Arduino has so many conflicting names smh my head
#include <Arduino.h>
#include <TimeLib.h>

XBeeManager xbee_mgr;
SensorManager sensor_mgr {};
TelemetryManager telem_mgr { xbee_mgr, sensor_mgr };
Runner runner;
const bool tp_released = false;

time_t getTeensy3Time();
void handle_response(Rx16Response&, uintptr_t);
void add_tasks_to_runner();

void setup() {
	// setup serial connections / peripherals
	Serial.begin(DEBUG_SERIAL_BAUD);
	xbee_mgr.setup(XBEE_SERIAL);
	xbee_mgr.set_panid(PAYLOAD_LINK_PANID);
	xbee_mgr.onRx16Response(handle_response);
	sensor_mgr.setup();

	// setup RTC as time provider
	setSyncProvider(getTeensy3Time);

	// add the tasks for the runner to do
	add_tasks_to_runner();
}

void loop() {
	runner.run();
}

// simple function to add tasks to the runner
// in it's own function to make setup clearer
void add_tasks_to_runner() {
	// run the xbee manager's loop every time
	// runner.schedule_task(
	// 	[]() { xbee_mgr.loop(); });
	runner.schedule_task(
		500,
		[]() { 
			sout << "hi" << std::endl;
			telem_mgr.send_payload_telemetry(); });
}

// ngl it feels weird to have this here but I couldn't really think of a better place to put it...
time_t getTeensy3Time() {
	return Teensy3Clock.get();
}

void handle_response(Rx16Response& resp, uintptr_t) {
	// check the sender's address
	if (resp.getRemoteAddress16() == CONTAINER_XBEE_ADDRESS) {
		// request is from the container, send it the telemetry
		telem_mgr.send_payload_telemetry();
	}
}
