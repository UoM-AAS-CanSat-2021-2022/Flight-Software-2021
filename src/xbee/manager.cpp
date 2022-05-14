#include <algorithm>

#include <Printers.h>

#include "constants.hpp"
#include "xbee/manager.hpp"
#include "util/sout.hpp"

XBeeManager::XBeeManager() : _panid(0) {}

void XBeeManager::loop() {
    _xbee.loop();
}

void XBeeManager::setup(HardwareSerial& serial) {
    serial.begin(XBEE_SERIAL_BAUD);
    _xbee.setSerial(serial);

    auto serial_printer = reinterpret_cast<uintptr_t>(static_cast<Print*>(&Serial));
    _xbee.onPacketError(printErrorCb, serial_printer);
    _xbee.onTxStatusResponse(printErrorCb, serial_printer);
    _xbee.onOtherResponse(printResponseCb, serial_printer);
}

std::uint8_t XBeeManager::set_panid(std::uint16_t panid, std::uint16_t timeout) {
    static std::uint8_t id_cmd[2] = { 'I', 'D' };
    static std::uint8_t wr_cmd[2] = { 'W', 'R' };
    static std::uint8_t ac_cmd[2] = { 'A', 'C' };
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

    at_req.setCommand(ac_cmd);
    if (_xbee.sendAndWait(at_req, timeout) == XBEE_WAIT_TIMEOUT) {
        return XBEE_WAIT_TIMEOUT;
    }

    _panid = panid;

    return 0;
}

void XBeeManager::send(std::uint16_t dest_addr, std::string& msg) {
    static Tx16Request req;
    req.setAddress16(dest_addr);
    auto data_ptr = reinterpret_cast<std::uint8_t*>(msg.data());
    auto data_len = static_cast<std::uint32_t>(msg.size());

    // send message in 100 byte blocks
    // hopefully should never be necessary but i wrote it just in case
    while (data_len > 0) {
        // length of the data to be sent in this request
        auto req_len = std::min(data_len, 100UL);

        req.setPayload(data_ptr);
        req.setPayloadLength(req_len);
        _xbee.send(req);

        data_len -= req_len;
        data_ptr += req_len;
    }

    // clear out old invalid pointers
    req.setPayload(NULL);
    req.setPayloadLength(0);
}

std::uint16_t XBeeManager::get_panid() const {
    return _panid;
}

void XBeeManager::onRx16Response(void (*func)(Rx16Response &, uintptr_t), uintptr_t data) {
    _xbee.onRx16Response(func, data);
}
