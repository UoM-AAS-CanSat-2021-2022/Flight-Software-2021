#pragma once

#include <chrono>
#include <cstdint>
#include <ratio>

#include "xorshift.hpp"

namespace chrono = std::chrono;

class FakeClock {
	std::uint32_t time;
	XorShift32 rng;

public:
	FakeClock();

	typedef std::uint32_t rep;
	typedef std::ratio<1> period;
	typedef chrono::duration<FakeClock::rep, FakeClock::period> duration;
	typedef chrono::time_point<FakeClock> time_point;

	const bool is_steady = false;

	FakeClock::time_point now();	
};

