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

std::ostream& operator<<(std::ostream&, const SimulationMode&);
template<>
struct fmt::formatter<SimulationMode> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template<typename FormatContext>
	auto format(SimulationMode const& mode, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "{}", (mode == SimulationMode::Activate) ? 'S' : 'F');
	}
};

struct UtcTime {
	std::uint8_t h;
	std::uint8_t m;
	std::uint8_t s;
};
using OnOff = bool;
using Pressure = std::uint32_t;
using Value = std::variant<std::monostate, OnOff, UtcTime, SimulationMode, Pressure>;

template<>
struct fmt::formatter<UtcTime> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template<typename FormatContext>
	auto format(UtcTime const& time, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "{:02}:{:02}:{:02}", time.h, time.m, time.s);
	}
};

class CommandParser {
	TelemetryManager& _telem_mgr;

	enum class Format {
		CX,
		ST,
		SIM,
		SIMP,
	};

	std::optional<Format> parse_fmt(std::string_view const) const;
	std::optional<UtcTime> parse_st(const std::string&) const;

public:
	CommandParser(TelemetryManager&);
	Value parse(const std::string&) const;
};
