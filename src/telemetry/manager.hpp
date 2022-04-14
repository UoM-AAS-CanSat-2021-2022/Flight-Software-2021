#pragma once

#include "xbee/manager.hpp"
#include <string_view>

class TelemetryManager {
	XBeeManager& _xbm;
	std::uint32_t _packet_count;

	void send_telemetry(std::string&&);

public:
	TelemetryManager(XBeeManager&);

	void send_container_telemetry();
	void forward_payload_telemetry(std::string_view);
	void set_packet_count(std::uint32_t);
	std::uint32_t get_packet_count();
};
