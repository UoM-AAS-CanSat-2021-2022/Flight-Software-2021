#include <Arduino.h>
#include <EEPROM.h>
#include "util/sout.hpp"
#include "eeprom/util.hpp"
#include "eeprom/crc.hpp"

const int led = LED_BUILTIN;

struct ComplexExample {
	std::int64_t _this;
	std::int32_t _struct;
	std::uint64_t _has;
	std::uint16_t _many;
	std::int8_t _fields;
};

std::ostream& operator<<(std::ostream& os, const ComplexExample& cx) {
	os << cx._this << ", "
	   << cx._struct << ", "
	   << cx._has << ", "
	   << cx._many << ", "
	   << +cx._fields;

	return os;
}

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	sout << std::boolalpha << std::hex;
	eeprom::store(ComplexExample { 1, 2, 3, 4, 5 }, 0);
}

void loop() {
	delay(1000);

	auto cx = eeprom::load<ComplexExample>(0);

	if (cx) {
		//digitalWrite(led, *x ? HIGH : LOW);
		sout << "complex_example: " << *cx;
		std::uint32_t checksum;
		EEPROM.get(0 + sizeof(ComplexExample), checksum);
		sout << " - size = 0x" << sizeof(ComplexExample)
			<< " - checksum = 0x" << checksum << std::endl;

		cx->_has++;
		eeprom::update(*cx, 0);
	}
}
