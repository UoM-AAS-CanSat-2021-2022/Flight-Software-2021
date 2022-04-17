// this must be included before Arduino.h, because Arduino.h produces colliding macro names
#include <optional>
#include <string_view>
#include <string>
#include <fmt/core.h>

#include "constants.hpp"
#include "xbee/manager.hpp"
#include "telemetry/manager.hpp"
#include "sensor/manager.hpp"
#include "command/parser.hpp"
#include "util/sout.hpp"

// Arduino has so many conflicting names smh my head
#include <Arduino.h>
#include <TimeLib.h>

XBeeManager xbee_mgr;
SensorManager sensor_mgr {};
TelemetryManager telem_mgr { xbee_mgr, sensor_mgr };
CommandParser cmd_parser { telem_mgr };

time_t getTeensy3Time();
void handle_response(Rx16Response&, uintptr_t);

void setup() {
	Serial.begin(230400);
	xbee_mgr.setup(Serial1);
	xbee_mgr.set_panid(gcs_link_panid);
	xbee_mgr.onRx16Response(handle_response);
	sensor_mgr.setup();
	setSyncProvider(getTeensy3Time);
}

void loop() {
	xbee_mgr.loop();

	static auto container_last_read = 0;
	if (millis() - container_last_read > 1000) {
		container_last_read = millis();
		telem_mgr.send_container_telemetry();
	}

	const bool tp_released = false;
	static auto payload_last_read = 0;
	if (tp_released && millis() - payload_last_read > 250) {
		payload_last_read = millis();
		xbee_mgr.set_panid(payload_link_panid);
		// poll the container for the data, for now just mock it out
		// NOTE: eventually none of this will be needed as the handle_response method
		//       will get called for the response from the XBee
		std::string_view mock_payload_relay_data { "165.2,13.7,5.02,0.18,0.08,-0.18,0.12,0.31,9.8,0.19,-0.05,0.47,12,LANDED" };
		xbee_mgr.set_panid(gcs_link_panid);
		telem_mgr.forward_payload_telemetry(mock_payload_relay_data);
	}
}

// ngl it feels weird to have this here but I couldn't really think of a better place to put it...
time_t getTeensy3Time() {
	return Teensy3Clock.get();
}

// remove the logging messages once we aren't testing any more
struct CommandHandler {
	void operator()(const std::monostate&) {
		// parse failed, ignore it
		sout << "Parsing the command failed." << std::endl;
	}

	void operator()(const OnOff& on_off) {
		sout << "Got ON_OFF value: " << std::boolalpha << on_off << std::endl;
		telem_mgr.set_enabled(on_off);
	}

	void operator()(const UtcTime& utc_time) {
		const auto [h, m, s] = utc_time;
		sout << fmt::format("Got UtcTime value: {:02}:{:02}:{:02}", h, m, s) << std::endl;

		// we don't care about the full date, just the hour minute and second
		setTime(h, m, s, 0, 0, 0);
	}

	void operator()(const SimulationMode& mode) {
		sout << "Got MODE value: " << mode << std::endl;

		sensor_mgr.set_sim_mode(mode);
		sout << "sim_mode=" << sensor_mgr.get_sim_mode() << std::endl;
	}

	void operator()(const Pressure& pressure) {
		sout << "Got PRESSURE value: " << pressure << std::endl;
		sensor_mgr.set_sim_pressure(pressure);
	}
};

void handle_response(Rx16Response& resp, uintptr_t) {
	// make sure we are listening on the gcs link as much as possible
	if (xbee_mgr.get_panid() != gcs_link_panid) {
		xbee_mgr.set_panid(gcs_link_panid);
	}

	std::string_view data_view {
		reinterpret_cast<char*>(resp.getData()),
		static_cast<size_t>(resp.getDataLength())
	};

	switch (resp.getRemoteAddress16()) {
		case gcs_xbee_address: {
			// data is from the ground station, interpret it as a command
			std::string cmd_string { data_view };
			std::visit(CommandHandler {}, cmd_parser.parse(cmd_string));
			break;
		}
		case payload_xbee_address:
			// data is from payload, forward it to the ground station
			telem_mgr.forward_payload_telemetry(data_view);
			break;
		default:
			// invalid PANID, maybe log in future?
			break;
	}
}
