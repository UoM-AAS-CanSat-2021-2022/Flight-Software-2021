#pragma once

#include <string_view>
#include "xbee/manager.hpp"
#include "sensor/manager.hpp"

#include "sensor/manager.fwd.hpp"

class TelemetryManager {
	XBeeManager& _xbee_mgr;
	SensorManager& _sensor_mgr;

	void send_telemetry(std::string&&);

public:
	TelemetryManager(XBeeManager&, SensorManager&);

	void send_payload_telemetry();
	void set_packet_count(std::uint32_t);
	std::uint32_t get_packet_count();
};
