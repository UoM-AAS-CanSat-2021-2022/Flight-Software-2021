#include <algorithm>
#include <charconv>
#include <cstdint>

#include "constants.hpp"
#include "command/parser.hpp"
#include "telemetry/manager.hpp"

CommandParser::CommandParser(TelemetryManager& telem_mgr) : _telem_mgr(telem_mgr) {}

std::optional<CommandParser::Format> CommandParser::parse_fmt(const std::string_view s) const {
	if      (s == "CX")   return Format::CX;
	else if (s == "ST")   return Format::ST;
	else if (s == "SIM")  return Format::SIM;
	else if (s == "SIMP") return Format::SIMP;
	else if (s == "RES")  return Format::RES;
	else if (s == "CAL")  return Format::CAL;
	else if (s == "TPD")  return Format::TPD;
	else return {};
}

std::optional<UtcTime> CommandParser::parse_st(const std::string& input) const {
	std::istringstream in { input };
	std::string buf;
	std::uint8_t h, m, s;
	std::errc ec;

	std::getline(in, buf, ':');
	ec = std::from_chars(buf.data(), buf.data() + buf.size(), h).ec;
	if (ec != std::errc() || h >= 24) return {};

	std::getline(in, buf, ':');
	ec = std::from_chars(buf.data(), buf.data() + buf.size(), m).ec;
	if (ec != std::errc() || m >= 60) return {};

	std::getline(in, buf, ':');
	ec = std::from_chars(buf.data(), buf.data() + buf.size(), s).ec;
	if (ec != std::errc() || m >= 60) return {};

	return {{ h, m, s }};
}

/*
	* Container Command Formats:
	*   - CX
	*     - Container Telemetry On/Off
	*     - Activates/Deactivates container telemetry transmission.
	*     - CMD,<TEAM_ID>,CX,<ON_OFF>
	*   - ST
	*     - Set Time
	*     - Sets the mission time to a given value.
	*     - CMD,<TEAM_ID>,ST,<UTC_TIME>
	*   - SIM
	*     - Simulation SimulationMode Control 
	*     - Enables/Disables/Activates simulation mode.
	*     - CMD,<TEAM_ID>,SIM,<MODE>
	*   - SIMP
	*     - Simulated Pressure Data
	*     - Provides a pressure simulation value to the container which is used in simulation mode.
	*     - CMD,<TEAM_ID>,SIMP,<PRESSURE>
	* 
	* Command Field Description:
	*   - <TEAM_ID>
	*     - Unique assigned Team ID
	*   - <ON_OFF>
	*     - The string ‘ON’ to activate or the string ‘OFF’ to deactivate packet transmissions.
	*   - <UTC_TIME>
	*     - Time to which the container mission time will be set in UTC format
	*   - <MODE>
	*     - The string ‘ENABLE’, ‘DISABLE’ or ‘ACTIVATE’ to enable, disable or activate
	*       simulation mode respectively.
	*   - <PRESSURE>
	*     - An integer holding the simulated atmospheric pressure in Pascals.
	*/
Value CommandParser::parse(const std::string& input) const {
	std::istringstream in { input };
	std::string buf;

	// assert starts with "CMD"
	std::getline(in, buf, ',');
	if (buf != "CMD") {
		return {};
	}

	// assert command is from our team
	std::getline(in, buf, ',');
	std::uint16_t team_id {};
	auto ec = std::from_chars(buf.data(), buf.data() + buf.size(), team_id).ec;
	if (ec != std::errc() || team_id != TEAM_ID) {
		return {};
	}

	// parse the command format
	std::getline(in, buf, ',');
	std::string cmd_id { buf };
	auto parsed_fmt = parse_fmt(buf);
	if (!parsed_fmt) {
		return {};
	}
	Format fmt = *parsed_fmt;

	// parse the command value
	std::getline(in, buf, ',');
	cmd_id += buf;
	Value retval {};
	switch (fmt) {
		case Format::CX:
			if (buf == "ON") retval = { true };
			else if (buf == "OFF") retval = { false };
			else return {};
			break;
		case Format::ST: {
			auto time = parse_st(buf);
			if (time) retval = { *time };
			else return {};
			break;
		}
		case Format::SIM:
			if (buf == "ENABLE") retval = { SimulationMode::Enable };
			else if (buf == "DISABLE") retval = { SimulationMode::Disable };
			else if (buf == "ACTIVATE") retval = { SimulationMode::Activate };
			else return {};
			break;
		case Format::SIMP: {
			Pressure p {};
			ec = std::from_chars(buf.data(), buf.data() + buf.size(), p).ec;
			if (ec == std::errc()) retval = { p };
			else return {};
			break;
		}
		case Format::RES:
			retval = { Command::Reset };
			break;
		case Format::CAL:
			retval = { Command::Calibrate };
			break;
		case Format::TPD:
			TetheredPayloadDepth tpd {};
			ec = std::from_chars(buf.data(), buf.data() + buf.size(), tpd).ec;
			if (ec == std::errc()) retval = { tpd };
			else return {};
			break;
	}

	// if we parsed a valid command set the cmd id
	if (!std::holds_alternative<std::monostate>(retval)) {
		_telem_mgr.set_last_cmd_id(cmd_id);
	}

	return retval;
}
