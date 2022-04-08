#include <SoftwareSerial.h>
#include "xbee/setup.hpp"

XBee xb {};
SoftwareSerial XBeeSerial { PIN_SERIAL_RX, PIN_SERIAL_TX };

namespace xbee {
    void setup() {
        XBeeSerial.begin(230400);
        xb.setSerial(XBeeSerial);
    }
}
