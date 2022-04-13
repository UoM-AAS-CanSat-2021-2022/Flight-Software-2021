#include "xbee/manager.hpp"
#include "util/sout.hpp"

void XBeeManager::setup(HardwareSerial& serial) {
    serial.begin(230400);
}

void XBeeManager::set_panid(std::uint16_t panid) {
    static std::uint8_t id_cmd[2] = { 'I', 'D' };
    static std::uint8_t wr_cmd[2] = { 'W', 'R' };
    static AtCommandRequest at_req {};
    std::uint8_t panid_bytes[2] { (panid >> 8) & 0xFF, panid & 0xFF };

    // send the ID command and wait for the response
    at_req.setCommand(id_cmd);
    at_req.setCommandValue(panid_bytes);
    at_req.setCommandValueLength(2);

    auto resp = _xbee.sendAndWait(at_req, 1000);
    sout << "1. resp=" << +resp << std::endl;

    // send the WR command and wait for the response
    at_req.setCommand(wr_cmd);
    at_req.clearCommandValue();

    resp = _xbee.sendAndWait(at_req, 1000);
    sout << "2. resp=" << +resp << std::endl;;
}
