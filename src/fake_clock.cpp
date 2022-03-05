#include <cstdint>
#include "fake_clock.hpp"

FakeClock::FakeClock() : time(0) {}

FakeClock::time_point FakeClock::now() {
	time += (rng.next() % 100) + 10;
	return FakeClock::time_point(FakeClock::duration(time));
}

