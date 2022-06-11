#pragma once

#include <cstdint>
#include <iomanip>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <fmt/format.h>

#include "telemetry/manager.fwd.hpp"

enum class SimulationMode {
	Enable,
	Disable,
	Activate,
};

struct UtcTime {
	std::uint8_t h;
	std::uint8_t m;
	std::uint8_t s;
};
using OnOff = bool;
using Pressure = std::uint32_t;
enum class Command {
	Reset,
	Calibrate,
	ReleaseTetheredPayload,
	ReleaseParachute2,
};
using Value = std::variant<std::monostate, OnOff, UtcTime, SimulationMode, Pressure, Command>;

class CommandParser {
	TelemetryManager& _telem_mgr;

	enum class Format {
		CX,
		ST,
		SIM,
		SIMP,
		RES,
		CAL,
		TPR,
		RP2,
	};

	std::optional<Format> parse_fmt(std::string_view const) const;
	std::optional<UtcTime> parse_st(const std::string&) const;

public:
	CommandParser(TelemetryManager&);
	Value parse(const std::string&) const;
};

template<>
struct fmt::formatter<SimulationMode> {
	bool debug_repr;

	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		debug_repr = false;
		if (it != end && (*it == '?')) {
			debug_repr = true;
			it++;
		}

		// Return an iterator past the end of the parsed range:
		return it;
  	}

	template<typename FormatContext>
	auto format(const SimulationMode& mode, FormatContext& ctx) -> decltype(ctx.out()) {
		if (debug_repr) {
			switch (mode) {
				case SimulationMode::Activate:
					return fmt::format_to(ctx.out(), "Activate");
				case SimulationMode::Disable:
					return fmt::format_to(ctx.out(), "Disable");
				case SimulationMode::Enable:
					return fmt::format_to(ctx.out(), "Enable");
			}
		}

		return fmt::format_to(ctx.out(), (mode == SimulationMode::Activate) ? "S" : "F");
	}
};


template<>
struct fmt::formatter<Command> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
  	}

	template<typename FormatContext>
	auto format(const Command& cmd, FormatContext& ctx) -> decltype(ctx.out()) {
		switch (cmd) {
			case Command::Calibrate:
				return fmt::format_to(ctx.out(), "Calibrate");
			case Command::Reset:
				return fmt::format_to(ctx.out(), "Reset");
			case Command::ReleaseTetheredPayload:
				return fmt::format_to(ctx.out(), "ReleaseTetheredPayload");
			case Command::ReleaseParachute2:
				return fmt::format_to(ctx.out(), "ReleaseParachute2");
		}
		return ctx.out();
	}
};

template<>
struct fmt::formatter<UtcTime> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
  	}

	template<typename FormatContext>
	auto format(const UtcTime& time, FormatContext& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{:02}:{:02}:{:02}", time.h, time.m, time.s);
	}
};
