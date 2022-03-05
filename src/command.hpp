#pragma once

#include <cstdint>
#include <string>
#include <variant>

#include "utc_time.hpp"
#include "fake_clock.hpp"

namespace Command {
	using OnOff = bool;

	// TODO: move to using the Teensy's RTC
	using UtcTime = UtcTimePoint<FakeClock>;

	enum class Mode {
		Enable,
		Disable,
		Activate,
	};

	std::ostream& operator<<(std::ostream&, const Mode&);

	using Pressure = std::uint32_t;

	using Value = std::variant<std::monostate, OnOff, UtcTime, Mode, Pressure>;
	Value parse(std::string);
}

