#pragma once

#include <cstdint>
#include <iomanip>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

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
	struct UtcTime {
		std::uint8_t h;
		std::uint8_t m;
		std::uint8_t s;
	};

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
	Value parse(const std::string&);
}
