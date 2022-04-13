#include <cstdint>
#include <XBee.h>

class XBeeManager {
    XBeeWithCallbacks _xbee;

public:
    // XBeeManager();
    void setup(HardwareSerial&);
    void set_panid(std::uint16_t);
};
