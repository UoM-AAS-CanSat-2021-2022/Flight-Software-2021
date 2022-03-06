#include <cstdint>
#include "utc_time.hpp"
#include "fake_clock.hpp"

template <typename Clock>
UtcTimePoint<Clock>::UtcTimePoint(std::uint8_t h, std::uint8_t m, std::uint8_t s) : h(h), m(m), s(s) {
	clock_value = clock.now();
}

template <typename Clock>
typename Clock::duration UtcTimePoint<Clock>::time_since() {
	return clock.now() - clock_value;
}
