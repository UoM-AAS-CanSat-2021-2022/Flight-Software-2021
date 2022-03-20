#include <Arduino.h>
#include "util/sout.hpp"

const int led = LED_BUILTIN;

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
}

struct ret_type {
	bool a;
	unsigned int b;
};

ret_type get_val() {
	static unsigned int x = 42;

	x ^= x << 17;
	x ^= x >> 13;
	x ^= x << 5;

	return {(x & 1) == 0, x};
}

void loop() {
	auto [x, y] = get_val();

	sout << "x: " << x
	     << ", y: " << std::hex << y
	     << std::dec
		 << std::endl;

	delay(300);
	digitalWrite(led, x ? HIGH : LOW);
}
