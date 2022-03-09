#include "utc_time.hpp"

template <typename Clock>
typename Clock::duration UtcTimePoint<Clock>::time_since() {
	return clock.now() - clock_value;
}
