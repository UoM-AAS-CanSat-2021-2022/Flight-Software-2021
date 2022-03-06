#pragma once

#include <chrono>
#include <cstdint>
#include <ostream>
#include "fake_clock.hpp"

namespace chrono = std::chrono;

template <typename Clock>
class UtcTimePoint {
	std::uint8_t h;
	std::uint8_t m;
	std::uint8_t s;

	Clock clock;

	typename Clock::time_point clock_value;

public:
	UtcTimePoint(std::uint8_t, std::uint8_t, std::uint8_t);

	typename Clock::duration time_since();

	// NOTE: this is implemented here because templates are funky
	// https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
	friend std::ostream& operator<<(std::ostream& os, const UtcTimePoint<Clock>& utc_time) {
		os << utc_time.h << ':' << utc_time.m << ':' << utc_time.s;
		return os;
	}
};
