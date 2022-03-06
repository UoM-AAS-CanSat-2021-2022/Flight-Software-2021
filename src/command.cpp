#include <charconv>
#include <cstdint>
#include <sstream>
#include <string>

#include <iostream>
#include <iomanip>

#include "command.hpp"

namespace Command {
	std::ostream& operator<<(std::ostream& os, const Mode& mode) {
		switch (mode) {
			case Mode::Enable:
				os << "Enable";
				break;
			case Mode::Disable:
				os << "Disable";
				break;
			case Mode::Activate:
				os << "Activate";
				break;
		}
		return os;
	}

	std::optional<Format> parse_fmt(std::string_view const s) {
		if      (s == "CX")   return Format::CX;
		else if (s == "ST")   return Format::ST;
		else if (s == "SIM")  return Format::SIM;
		else if (s == "SIMP") return Format::SIMP;
		else return {};
	}

	std::optional<UtcTime> parse_st(std::string const input) {
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

		return { UtcTime(h, m, s) };
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
	 *     - Simulation Mode Control 
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
	Value parse(std::string input) {
		std::istringstream in { input };
		std::string buf;

		// assert starts with "CMD"
		std::getline(in, buf, ',');
		if (buf != "CMD") {
			return {};
		}

		// assert command is from our team
		std::getline(in, buf, ',');
		TeamId team_id {};
		auto ec = std::from_chars(buf.data(), buf.data() + buf.size(), team_id).ec;
		if (ec != std::errc() || team_id != TEAM_NO) {
			return {};
		}

		// parse the command format
		std::getline(in, buf, ',');
		auto parsed_fmt = parse_fmt(buf);
		if (!parsed_fmt) {
			return {};
		}
		Format fmt = *parsed_fmt;

		// parse the command value
		std::getline(in, buf, ',');
		switch (fmt) {
			case Format::CX:
				if (buf == "ON") return true;
				else if (buf == "OFF") return false;
				else return {};
			case Format::ST: {
				auto time = parse_st(buf);
				if (time) return *time;
				else return {};
			}
			case Format::SIM:
				if (buf == "ENABLE") return Mode::Enable;
				else if (buf == "DISABLE") return Mode::Disable;
				else if (buf == "ACTIVATE") return Mode::Activate;
				else return {};
			case Format::SIMP:
				Pressure p {};
				ec = std::from_chars(buf.data(), buf.data() + buf.size(), p).ec;
				if (ec == std::errc()) return p;
				else return {};
		}

		// this line is unreachable so just return std::monotype
		return {};
	}
}
