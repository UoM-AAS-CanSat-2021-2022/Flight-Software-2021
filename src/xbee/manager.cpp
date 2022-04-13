#include <Printers.h>
#include <iomanip>

#include "xbee/manager.hpp"
#include "util/sout.hpp"

XBeeManager::XBeeManager() : _panid(0) {}

void XBeeManager::loop() {
    _xbee.loop();
}

void XBeeManager::setup(HardwareSerial& serial) {
    serial.begin(230400);
    _xbee.setSerial(serial);

    auto serial_printer = reinterpret_cast<uintptr_t>(static_cast<Print*>(&Serial));
    _xbee.onPacketError(printErrorCb, serial_printer);
    _xbee.onTxStatusResponse(printResponseCb, serial_printer);
    _xbee.onOtherResponse(printResponseCb, serial_printer);
}

std::uint8_t XBeeManager::set_panid(std::uint16_t panid, std::uint16_t timeout) {
    static std::uint8_t id_cmd[2] = { 'I', 'D' };
    static std::uint8_t wr_cmd[2] = { 'W', 'R' };
    static AtCommandRequest at_req {};

    // extract the upper and lower byte from the panid
    std::uint8_t panid_bytes[2] {
        static_cast<std::uint8_t>(panid >> 8),
        static_cast<std::uint8_t>(panid),
    };

    // send the ID command and wait for the response
    at_req.setCommand(id_cmd);
    at_req.setCommandValue(panid_bytes);
    at_req.setCommandValueLength(2);

    if (_xbee.sendAndWait(at_req, timeout) == XBEE_WAIT_TIMEOUT) {
        return XBEE_WAIT_TIMEOUT;
    }

    // send the WR command and wait for the response
    at_req.clearCommandValue();
    at_req.setCommand(wr_cmd);

    if (_xbee.sendAndWait(at_req, timeout) == XBEE_WAIT_TIMEOUT) {
        return XBEE_WAIT_TIMEOUT;
    }

    _panid = panid;

    return 0;
}

void XBeeManager::send(std::string& msg) {
    sout << "Sending msg=" << std::quoted(msg) << std::endl;
    Tx16Request req {
        0x0000,
        reinterpret_cast<std::uint8_t*>(msg.data()),
        static_cast<std::uint8_t>(msg.size())
    };
    _xbee.send(req);
}

std::uint16_t XBeeManager::get_panid() const {
    return _panid;
}

