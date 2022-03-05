#include <Arduino.h>

const int led = LED_BUILTIN;

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	Serial.setTimeout(100);	
}

struct ret_type {
    bool a;
    unsigned long b;
};

ret_type get_val() {
    return {true, 100};
}

void loop() {
    auto [x, y] = get_val();

	delay(1000);
	digitalWrite(led, HIGH);

	delay(1000);

	digitalWrite(led, LOW);
}
