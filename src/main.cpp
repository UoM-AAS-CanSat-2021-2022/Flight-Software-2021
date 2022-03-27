#include <Arduino.h>
#include <TimeLib.h>
#include <iomanip>
#include "util/sout.hpp"

const int led = LED_BUILTIN;

time_t getTeensy3Time();

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	setSyncProvider(getTeensy3Time);
}

void loop() {
	auto prev_fill = sout.fill('0');
	sout << "[c] "
		 << std::setw(2) << hour()
		 << ':'
		 << std::setw(2) << minute()
		 << ':'
		 << std::setw(2) << second()
		 << ' '
		 << std::setw(2) << year()
		 << '/'
		 << std::setw(2) << month()
		 << '/'
		 << std::setw(2) << day()
		 << std::endl;
	sout.fill(prev_fill);

	digitalWrite(led, second() & 1 ? HIGH : LOW);

	delay(300);
}

time_t getTeensy3Time() {
	return Teensy3Clock.get();
}
