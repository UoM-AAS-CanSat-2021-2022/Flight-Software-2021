#pragma once

#include <string_view>
#include "xbee/manager.hpp"
#include "sensor/manager.hpp"

#include "telemetry/manager.fwd.hpp"
#include "sensor/manager.fwd.hpp"
#include "command/parser.fwd.hpp"

class TelemetryManager {
	XBeeManager& _xbee_mgr;
	SensorManager& _sensor_mgr;
	std::uint32_t _packet_count;
	std::string _last_cmd_id;
	bool _enabled;

	void send_telemetry(std::string&&);

public:
	TelemetryManager(XBeeManager&, SensorManager&);

	void send_container_telemetry();
	void forward_payload_telemetry(std::string_view);
	void set_packet_count(std::uint32_t);
	std::uint32_t get_packet_count();
	void set_last_cmd_id(std::string);
	void set_enabled(bool);
};
