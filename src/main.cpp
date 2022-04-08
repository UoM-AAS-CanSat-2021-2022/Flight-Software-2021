#include <Arduino.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include <cstdint>
#include <iomanip>
#include "util/sout.hpp"

constexpr static int led = LED_BUILTIN;
SoftwareSerial XBeeSerial { PIN_SERIAL_RX, PIN_SERIAL_TX };
XBee xbee {};

uint8_t id_cmd[] = { 'I', 'D' };
uint8_t wr_cmd[] = { 'W', 'R' };
uint8_t ac_cmd[] = { 'A', 'C' };
uint8_t id_payload[] = { 0x69, 0x69 };
AtCommandRequest at_req { id_cmd, id_payload, 2 };
AtCommandResponse at_resp {};
int cmds_sent = 0;
int oks_received = 0;

void setup() {
	pinMode(led, OUTPUT);

	Serial.begin(230400);
	XBeeSerial.begin(230400);
	xbee.setSerial(XBeeSerial);
}

void loop() {
	// try to send the next packet
	switch (cmds_sent) {
		case 0:
			xbee.send(at_req);
			cmds_sent++;
			break;
		case 1:
			if (oks_received != 1) break;
			at_req.clearCommandValue();
			at_req.setCommand(wr_cmd);
			xbee.send(at_req);
			cmds_sent++;
			break;
		case 2:
			if (oks_received != 2) break;
			at_req.setCommand(ac_cmd);
			xbee.send(at_req);
			cmds_sent++;
			break;
		default:
			// do nothing we've sent all the commands	
			break;
	}

    xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
		if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
			xbee.getResponse().getAtCommandResponse(at_resp);
			sout << "Got AT Command Response: ok=" << std::boolalpha << at_resp.isOk() << ", cmd=\"" << at_resp.getCommand()[0] << at_resp.getCommand()[1] << '"' << std::endl;
			oks_received += at_resp.isOk();
		} else {
			// not something we were expecting
			sout << "Got something unexpected - API ID = " << std::hex << +xbee.getResponse().getApiId() << std::endl;
		}
    } else if (xbee.getResponse().isError()) {
		sout << "Error reading packet.  Error code: " << std::hex << +xbee.getResponse().getErrorCode() << std::endl;
    }
}