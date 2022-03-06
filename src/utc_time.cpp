#include <cstdint>
#include "utc_time.hpp"
#include "fake_clock.hpp"

template <typename Clock>
typename Clock::duration UtcTimePoint<Clock>::time_since() {
	return clock.now() - clock_value;
}
