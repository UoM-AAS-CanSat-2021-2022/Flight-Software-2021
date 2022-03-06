#pragma once

#include <chrono>
#include <cstdint>
#include <ostream>

namespace chrono = std::chrono;

template <typename Clock>
class UtcTimePoint {
	std::uint8_t h;
	std::uint8_t m;
	std::uint8_t s;

	Clock clock;

	typename Clock::time_point clock_value;

public:
	UtcTimePoint(std::uint8_t h, std::uint8_t m, std::uint8_t s) : h(h), m(m), s(s) {
		clock_value = clock.now();
	}

	typename Clock::duration time_since();

	friend std::ostream& operator<<(std::ostream& os, const UtcTimePoint<Clock>& utc_time) {
		os << +utc_time.h << ':' << +utc_time.m << ':' << +utc_time.s;
		return os;
	}
};
