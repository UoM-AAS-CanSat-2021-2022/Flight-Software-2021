#include <sstream>
#include <iomanip>
#include <Arduino.h>
#include <TimeLib.h>
#include "xbee/manager.hpp"
#include "sensor/bmp388.hpp"
#include "util/sout.hpp"

void send_container_telemetry();
time_t getTeensy3Time();

XBeeManager xbm;

void setup() {
	Serial.begin(9600);
	bmp388::setup();
	xbm.setup(Serial1);
	xbm.set_panid(6057);
	setSyncProvider(getTeensy3Time);
}

void loop() {
	xbm.loop();

	static auto last_read = 0;
	if (millis() - last_read > 1000) {
		const auto readings = bmp388::read_all();
		if (!readings) return;

		const auto [temp, press, alt] = *readings;

		sout << "Temperature = " << temp << " *C\n";
		sout << "Pressure = " << press / 100.0 << " hPa\n";
		sout << "Approx Alt = " << alt << " m" << std::endl;

		last_read = millis();
	}
}

void send_container_telemetry() {
	static std::ostringstream out;
	out.clear();

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

	// 165.2,13.7,5.02,0.18,0.08,-0.18,0.12,0.31,9.8,0.19,-0.05,0.47,12,LANDED
	double tp_volta

	// Full packet format: <TEAM_ID>,<MISSION_TIME>,<PACKET_COUNT>,<PACKET_TYPE>,
	// <MODE>,<TP_RELEASED>,<ALTITUDE>,<TEMP>,<VOLTAGE>,<GPS_TIME>,
	// <GPS_LATITUDE>,<GPS_LONGITUDE>,<GPS_ALTITUDE>,<GPS_SATS>,
	// <SOFTWARE_STATE>,<CMD_ECHO>
}

time_t getTeensy3Time() {
	return Teensy3Clock.get();
}
