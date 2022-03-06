#include <Arduino.h>

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

	Serial.print("x: ");
	Serial.print(x);
	Serial.print(", y: ");
	Serial.println(y, HEX);

	delay(1000);
	digitalWrite(led, x ? HIGH : LOW);
}
