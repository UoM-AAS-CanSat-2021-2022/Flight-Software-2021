#include <Arduino.h>
#include <sout.hpp>

const int led = LED_BUILTIN;

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	sout << std::boolalpha;
}

void loop() {
	static bool x = true;
	delay(1000);
	//digitalWrite(led, (x = !x) ? HIGH : LOW);
	//sout << "x: " << x << std::endl;
}
