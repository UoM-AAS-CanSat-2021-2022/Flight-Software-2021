#include <Arduino.h>
#include <XBee.h>
#include "util/sout.hpp"

constexpr static int led = LED_BUILTIN;
XBee xbee {};

void setup() {
	Serial.begin(230400);
	Serial1.begin(230400);
	xbee.setSerial(Serial1);
}

void loop() {
	static Rx16Response rx {};

	xbee.readPacket();
	
	if (xbee.getResponse().isAvailable()) {
		sout << "Got response." << std::endl;
		
		if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
			xbee.getResponse().getRx16Response(rx);

			std::string_view rx_data { (char*) rx.getData(), (size_t) rx.getDataLength() };
			sout << "Received data: " << std::quoted(rx_data) << " - ";
			for (auto i = 0; i < rx.getDataLength(); i++) {
				sout << std::hex << +rx.getData()[i] << ' ';
			}
			sout << std::endl;
		} else {
			// not something we were expecting
			sout << "Got something unexpected - API ID = " << std::hex << +xbee.getResponse().getApiId() << std::endl;
		}
	} else if (xbee.getResponse().isError()) {
		sout << "Error reading packet.  Error code: " << std::hex << +xbee.getResponse().getErrorCode() << std::endl;
	}
}
