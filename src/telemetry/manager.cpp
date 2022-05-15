#include <fmt/core.h>

#include <Arduino.h>
#include <TimeLib.h>

#include "constants.hpp"
#include "telemetry/manager.hpp"
#include "util/sout.hpp"

TelemetryManager::TelemetryManager(XBeeManager& xbee_mgr, SensorManager& sensor_mgr)
	: _xbee_mgr(xbee_mgr)
	, _sensor_mgr(sensor_mgr) { }

// ensures the correct PAN ID is set before sending the telemetry
void TelemetryManager::send_telemetry(std::string&& telemetry) {
	if (_xbee_mgr.get_panid() != PAYLOAD_LINK_PANID) {
		_xbee_mgr.set_panid(PAYLOAD_LINK_PANID);
	}

	_xbee_mgr.send(CONTAINER_XBEE_ADDRESS, telemetry);
}

void TelemetryManager::send_payload_telemetry() {
	sout << "hii" << std::endl;
	const auto telem = _sensor_mgr.read_payload_telemetry();
	sout << "hiii" << std::endl;
	// const auto software_state = "VIBING";

	// 1057,17:48:45.91,175,C,S,R,476.2,28.3,5.02,13:23:15,69.4201,-3.2635,698.2,7,IDLE,SIMP101325

	// Full packet format: 
	// <TP_ALTITUDE>,<TP_TEMP>,<TP_VOLTAGE>,<GYRO_R>,<GYRO_P>,<GYRO_Y>,
	// <ACCEL_R>,<ACCEL_P>,<ACCEL_Y>,<MAG_R>,<MAG_P>,<MAG_Y>,
	// <POINTING_ERROR>,<TP_SOFTWARE_STATE>
	send_telemetry(
		fmt::format(
			"{:.1f},{:.1f},{:.2f},{:.0f},{:.0f},{:.0f},{:.0f},{:.0f},{:.0f},{:.0f},{:.0f},{:.0f},{:.0f},vibing",
			telem.altitude,
			telem.temp,
			telem.voltage,
			telem.gyro_r,
			telem.gyro_p,
			telem.gyro_y,
			telem.accel_r,
			telem.accel_p,
			telem.accel_y,
			telem.mag_r,
			telem.mag_p,
			telem.mag_y,
			telem.pointing_error
			// software_state
		)
	);
}
