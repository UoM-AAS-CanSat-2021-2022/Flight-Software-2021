#include <Arduino.h>
#include <XBee.h>
#include <string>

XBee xbee {};

void setup() {
	Serial.begin(230400);
	Serial1.begin(230400);
	xbee.setSerial(Serial1);
}

void loop() {
	static std::string buf;
	bool send = false;
	if (Serial.available()) {
		char c = static_cast<char>(Serial.read());
		buf.push_back(c);
		if (c == '\n') {
			send = true;
		}
	}
	if (send) {
		Tx16Request req {
			0x0000,
			reinterpret_cast<std::uint8_t*>(buf.data()),
			static_cast<std::uint8_t>(buf.size())
		};
		xbee.send(req);
		buf.erase();
	}
}
