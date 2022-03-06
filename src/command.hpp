#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include "utc_time.hpp"
#include "fake_clock.hpp"

namespace Command {
	enum class Format {
		CX,
		ST,
		SIM,
		SIMP,
	};

	std::optional<Format> parse_fmt(std::string_view const);

	using OnOff = bool;

	// TODO: move to using the Teensy's RTC
	using UtcTime = UtcTimePoint<FakeClock>;

	std::optional<UtcTime> parse_st(std::string_view const);

	enum class Mode {
		Enable,
		Disable,
		Activate,
	};

	std::ostream& operator<<(std::ostream&, const Mode&);

	using TeamId = std::uint32_t;

	using Pressure = std::uint32_t;

	using Value = std::variant<std::monostate, OnOff, UtcTime, Mode, Pressure>;
	Value parse(std::string);
}
