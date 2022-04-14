// this must be included before Arduino.h, because Arduino.h produces colliding macro names
#include <fmt/core.h>

#include <Arduino.h>
#include <TimeLib.h>

#include "xbee/manager.hpp"
#include "sensor/bmp388.hpp"
#include "util/sout.hpp"

void send_container_telemetry();
time_t getTeensy3Time();

XBeeManager xbm;

void setup() {
	Serial.begin(230400);
	bmp388::setup();
	xbm.setup(Serial1);
	xbm.set_panid(6057);
	setSyncProvider(getTeensy3Time);
}

void loop() {
	xbm.loop();

	static auto last_read = 0;
	if (millis() - last_read > 1000) {
		send_container_telemetry();
		last_read = millis();
	}
}

// TOOD: create a TelemetryManager class to handle all of this
void send_container_telemetry() {
	constexpr static auto team_id = 1057;
	static auto packet_count = 0;
	const auto packet_type = 'C';
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
	auto telemetry = fmt::format(
		"{:04},{:02}:{:02}:{:02}.{:02},{},{},{},{},{:.1f},{:.1f},{:.2f},{},{:.4f},{:.4f},{:.1f},{},{},{}\n",
		team_id,
		hour(),
		minute(),
		second(),
		millis() % 100,
		packet_count,
		packet_type,
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
	);

	xbm.send(telemetry);
	packet_count++;
}

time_t getTeensy3Time() {
	return Teensy3Clock.get();
}
