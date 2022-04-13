#include <Arduino.h>
#include "rng/xorshift.hpp"

const int led = LED_BUILTIN;

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	static XorShift32 rng { 42 };

	digitalWrite(led, rng.next() & 1 ? HIGH : LOW);
	delay(300);
}
