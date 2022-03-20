#pragma once

#include <sstream>
#include <Arduino.h>

/**
 * String buffer that syncs by outputting over serial
 */
class SerialBuf : public std::stringbuf {
public:
    virtual int sync();
};

namespace __detail {
    extern SerialBuf sbuf;
}

// global sout which uses this buffer
extern std::ostream sout;
