#include <Arduino.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include <cstdint>
#include <iomanip>
#include "util/sout.hpp"
#include "xbee/setup.hpp"
#include "xbee/panid_changer.hpp"

void setup() {
	Serial.begin(230400);
	xbee::setup();	
}

void loop() {
	static xbee::PanidChanger pc {};
	static AtCommandResponse at_resp {};
	static TxStatusResponse tx_stat_resp {};
	static std::string payload { "BRO WHERE ARE MY BEANS????" };
	static Tx16Request req {
		0x0000,
		reinterpret_cast<std::uint8_t*>(payload.data()),
		static_cast<std::uint8_t>(payload.size())
	};
	static bool ran = false;

	if (!ran) {
		pc.change_to(6057);
	}

	pc.loop_logic();

	if (pc.changed()) {
		xb.send(req);
		delay(2000);
	}

    xb.readPacket();
    if (xb.getResponse().isAvailable()) {
		switch (xb.getResponse().getApiId()) {
			case AT_COMMAND_RESPONSE:
				xb.getResponse().getAtCommandResponse(at_resp);
				sout << "Got AT Command Response: ok=" << std::boolalpha << at_resp.isOk() << ", cmd=\"" << at_resp.getCommand()[0] << at_resp.getCommand()[1] << '"' << std::endl;
				if (at_resp.isOk()) {
					pc.ok_received();
				}
				break;
			case TX_STATUS_RESPONSE:
				xb.getResponse().getTxStatusResponse(tx_stat_resp);
				sout << "TX Status: success=" << std::hex << +tx_stat_resp.getStatus() << std::endl;
				break;
			default:
				// not something we were expecting
				sout << "Got something unexpected - API ID = " << std::hex << +xb.getResponse().getApiId() << std::endl;
		}
    } else if (xb.getResponse().isError()) {
		sout << "Error reading packet.  Error code: " << std::hex << +xb.getResponse().getErrorCode() << std::endl;
    }
}
