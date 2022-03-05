#include <cstdint>
#include <sstream>
#include <string>

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
	Command::Value parse(std::string input) {
		return false;
	}
}

