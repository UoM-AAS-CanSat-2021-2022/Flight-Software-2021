#include <fmt/core.h>

#include <Arduino.h>
#include <TimeLib.h>

#include "constants.hpp"
#include "sensor/bmp388.hpp"
#include "telemetry/manager.hpp"
#include "util/sout.hpp"

TelemetryManager::TelemetryManager(XBeeManager& xbm) : _xbm(xbm), _packet_count(0) { }

// ensures the correct PAN ID is set before sending the telemetry
void TelemetryManager::send_telemetry(std::string&& telemetry) {
	if (_xbm.get_panid() != gcs_link_panid) {
		_xbm.set_panid(gcs_link_panid);
	}

	_xbm.send(gcs_xbee_address, telemetry);
	_packet_count++;
}

void TelemetryManager::send_container_telemetry() {
	const auto mode = 'S';
	const auto tp_released = 'R';

	const auto readings = bmp388::read_all();
	double altitude;
	double temp;
	if (readings) {
		const auto [t, _, a] = *readings;
		altitude = a;
		temp = t;
	} else {
		altitude = 0.0;
		temp = 0.0;
	}

	double voltage = 5.02;
	const auto gps_time = "13:23:15";
	const auto gps_latitude = 69.4201;
	const auto gps_longitude = -3.2635;
	const auto gps_altitude = 698.2;
	const auto gps_sats = 7;
	const auto software_state = "IDLE";
	const auto cmd_echo = "SIMP101325";

	// 1057,17:48:45.91,175,C,S,R,476.2,28.3,5.02,13:23:15,69.4201,-3.2635,698.2,7,IDLE,SIMP101325

	// Full packet format: <TEAM_ID>,<MISSION_TIME>,<PACKET_COUNT>,<PACKET_TYPE>,
	// <MODE>,<TP_RELEASED>,<ALTITUDE>,"IDLE"<TEMP>,<VOLTAGE>,<GPS_TIME>,
	// <GPS_LATITUDE>,<GPS_LONGITUDE>,<GPS_ALTITUDE>,<GPS_SATS>,
	// <SOFTWARE_STATE>,<CMD_ECHO>
	send_telemetry(
		fmt::format(
			XSTR(TEAM_ID) ",{:02}:{:02}:{:02}.{:02},{},C,{},{},{:.1f},{:.1f},{:.2f},{},{:.4f},{:.4f},{:.1f},{},{},{}\n",
			hour(),
			minute(),
			second(),
			millis() % 100,
			_packet_count,
			mode,
			tp_released,
			altitude,
			temp,
			voltage,
			gps_time,
			gps_latitude,
			gps_longitude,
			gps_altitude,
			gps_sats,
			software_state,
			cmd_echo
        )
    );
}

void TelemetryManager::forward_payload_telemetry(std::string_view payload_telemetry) {
	send_telemetry(
		fmt::format(
			XSTR(TEAM_ID) ",{:02}:{:02}:{:02}.{:02},{},T,{}\n",
			hour(),
			minute(),
			second(),
			millis() % 100,
			_packet_count,
			payload_telemetry
        )
    );
}

std::uint32_t TelemetryManager::get_packet_count() {
	return _packet_count;
}

void TelemetryManager::set_packet_count(std::uint32_t packet_count) {
	_packet_count = packet_count;
}
