// this must be included before Arduino.h, because Arduino.h produces colliding macro names
#include <optional>
#include <string_view>
#include <string>
#include <fmt/core.h>

#include <Arduino.h>
#include <TimeLib.h>

#include "constants.hpp"
#include "xbee/manager.hpp"
#include "telemetry/manager.hpp"
#include "sensor/bmp388.hpp"
#include "command/parser.hpp"
#include "util/sout.hpp"

XBeeManager xbm;
TelemetryManager telem_mgr { xbm };
Command::Mode sim_mode { Command::Mode::Disable };
bool telemetry_enable { false };

time_t getTeensy3Time();
void handle_response(Rx16Response&, uintptr_t);

void setup() {
	Serial.begin(230400);
	bmp388::setup();
	xbm.setup(Serial1);
	xbm.set_panid(gcs_link_panid);
	setSyncProvider(getTeensy3Time);
	xbm.onRx16Response(handle_response);
}

void loop() {
	xbm.loop();

	static auto container_last_read = 0;
	if (telemetry_enable && millis() - container_last_read > 1000) {
		telem_mgr.send_container_telemetry();
		container_last_read = millis();
	}

	const bool tp_released = false;
	static auto payload_last_read = 0;
	if (tp_released && millis() - payload_last_read > 250) {
		xbm.set_panid(payload_link_panid);
		// poll the container for the data, for now just mock it out
		std::string_view mock_payload_relay_data { "165.2,13.7,5.02,0.18,0.08,-0.18,0.12,0.31,9.8,0.19,-0.05,0.47,12,LANDED" };
		xbm.set_panid(payload_link_panid);
		telem_mgr.forward_payload_telemetry(mock_payload_relay_data);
		payload_last_read = millis();
	}
}

time_t getTeensy3Time() {
	return Teensy3Clock.get();
}

struct CommandHandler {
	void operator()(const std::monostate&) {
		// parse failed, ignore it
		sout << "Parsing the command failed." << std::endl;
	}

	void operator()(const Command::OnOff& on_off) {
		sout << "Got ON_OFF value: " << std::boolalpha << on_off << std::endl;

		telemetry_enable = on_off;
	}

	void operator()(const Command::UtcTime& utc_time) {
		const auto [h, m, s] = utc_time;
		sout << fmt::format("Got UtcTime value: {:02}:{:02}:{:02}", h, m, s) << std::endl;

		// we don't care about the full date, just the hour minute and second
		setTime(h, m, s, 0, 0, 0);
	}

	void operator()(const Command::Mode& mode) {
		sout << "Got MODE value: " << mode << std::endl;

		switch (mode) {
			case Command::Mode::Disable:
				sim_mode = Command::Mode::Disable;
				break;
			case Command::Mode::Enable:
				if (sim_mode == Command::Mode::Disable)
					sim_mode = Command::Mode::Enable;
				break;
			case Command::Mode::Activate:
				if (sim_mode == Command::Mode::Enable)
					sim_mode = Command::Mode::Activate;
				break;
		}

		sout << "sim_mode=" << sim_mode << std::endl;
	}

	void operator()(const Command::Pressure& pressure) {
		sout << "Got PRESSURE value: " << pressure << std::endl;
		// eventually we can integrate this with other things
	}
};

void handle_response(Rx16Response& resp, uintptr_t) {
	// make sure we are listening on the gcs link as much as possible
	if (xbm.get_panid() != gcs_link_panid) {
		xbm.set_panid(gcs_link_panid);
	}

	std::string_view data_view {
		reinterpret_cast<char*>(resp.getData()),
		static_cast<size_t>(resp.getDataLength())
	};

	switch (resp.getRemoteAddress16()) {
		case gcs_xbee_address: {
			// data is from the ground station, interpret it as a command
			std::string cmd_string { data_view };
			std::visit(CommandHandler {}, Command::parse(cmd_string));
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
