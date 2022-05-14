#include <fmt/core.h>

#include <Arduino.h>
#include <TimeLib.h>

#include "constants.hpp"
#include "telemetry/manager.hpp"
#include "util/sout.hpp"

TelemetryManager::TelemetryManager(XBeeManager& xbee_mgr, SensorManager& sensor_mgr)
	: _xbee_mgr(xbee_mgr)
	, _sensor_mgr(sensor_mgr)
	, _packet_count(0)
	, _last_cmd_id("")
	, _enabled(false) { }

// ensures the correct PAN ID is set before sending the telemetry
void TelemetryManager::send_telemetry(std::string&& telemetry) {
	if (_xbee_mgr.get_panid() != GCS_LINK_PANID) {
		_xbee_mgr.set_panid(GCS_LINK_PANID);
	}

	_xbee_mgr.send(GCS_XBEE_ADDRESS, telemetry);
	_packet_count++;
}

void TelemetryManager::send_container_telemetry() {
	if (!_enabled) return;

	const auto mode = _sensor_mgr.get_sim_mode();
	const auto tp_released = 'R';

	const auto telem = _sensor_mgr.read_container_telemetry();
	const auto software_state = "IDLE";

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
			telem.altitude,
			telem.temp,
			telem.voltage,
			telem.gps_time,
			telem.gps_latitude,
			telem.gps_longitude,
			telem.gps_altitude,
			telem.gps_sats,
			software_state,
			_last_cmd_id
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

void TelemetryManager::set_last_cmd_id(std::string cmd_id) {
	_last_cmd_id = cmd_id;
}

void TelemetryManager::set_enabled(bool enabled) {
	_enabled = enabled;
}
