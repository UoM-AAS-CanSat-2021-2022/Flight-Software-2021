#pragma once

#include <cstdint>

#include "xorshift.hpp"

class FakeClock {
	std::uint32_t time;
	XorShift32 rng;

public:
	FakeClock();

	using time_point = std::uint32_t;
	using duration = std::uint32_t;

	time_point now();	
};

