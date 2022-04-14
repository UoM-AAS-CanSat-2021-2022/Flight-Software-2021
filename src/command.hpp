#pragma once

#include <cstdint>
#include <iomanip>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include "fake_clock.hpp"
#include "utc_time.hpp"

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
	Value parse(const std::string&);

	template <typename Stream>
	class Processor {
		std::string buf;
		Stream& stream;

	public:
		Processor(Stream& stream) : stream(stream) {}
		std::optional<Value> next_command() {
			while (stream.available()) {
				char c = static_cast<char>(stream.read());

				// ignore the carriage return character
				if (c == '\r') continue;

				if (c == '\n') {
					// if we hit a newline try and parse the command
					auto cmd = parse(buf);
					buf.clear();
					return cmd;
				} else {
					// otherwise just append to the buffer
					buf.push_back(c);
				}
			}

			return {};
		}
	};
}
