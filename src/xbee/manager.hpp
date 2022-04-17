#pragma once

#include <cstdint>
#include <string>
#include <XBee.h>

class XBeeManager {
    XBeeWithCallbacks _xbee;
    std::uint16_t _panid;

public:
    XBeeManager();
    void loop();
    void setup(HardwareSerial&);
    std::uint8_t set_panid(std::uint16_t, std::uint16_t timeout=100);
    std::uint16_t get_panid() const;
    void send(std::uint16_t, std::string&);
    // expose this function from the XBee so that responses can be handled by functions outside of this class
    void onRx16Response(void (*)(Rx16Response &, uintptr_t), uintptr_t data = 0U);
};
