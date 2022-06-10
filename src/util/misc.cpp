#include "util/misc.hpp"

namespace util {
    // ngl it feels weird to have this here but I couldn't really think of a better place to put it...
    time_t getTeensy3Time() {
        return Teensy3Clock.get();
    }
}
