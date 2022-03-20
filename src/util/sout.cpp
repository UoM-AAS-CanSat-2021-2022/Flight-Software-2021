#include "util/sout.hpp"

int SerialBuf::sync() {
    Serial.print(this->str().c_str());
    this->str("");
    return 0;
}

SerialBuf __detail::sbuf {};
std::ostream sout { &__detail::sbuf };
