#include <EEPROM.h>
#include <cmath>
#include <ctime>

#include "custom_functions.h"
#include "descent_control.h"
#include "memory_manage.h"
#include "process_command.h"
#include "sensor_control.h"
#include "transmitter.h"

const int frequency = 20;
double TIME_ELAPSED = 0;
memory_class memory;
config_class config;
bool save_memory = true;

transmitter_class transmitter;
sensor_class sensor;

void setup() {
	pin_mode(2, OUTPUT); // for buzzer

	Serial.begin(57600);
	serialFlush();
	Serial.setTimeout(100);

	// recovery process
	config_class saved_config = Memory.Read();
	if (saved_config.STAGE_ID != 0 || saved_config.GLOBAL_TIME_ELAPSED != 0) {
		// Serial.println("previous data found!");
		Serial.println("recovering data...");
		config = saved_config;
	}

	set_time(config);
}

boolean at_frequency(int hz) {
	// hz=round(hz);
	if (floor(frequency / hz) != frequency / hz) {
		string error = "unsupported frequency of " + string(hz) + "hz, needs to be a remainder of " + string(frequency) + "hz";
		Serial.println(error);
		return false;
	}
	double t = round(TIME_ELAPSED * hz);
	return ((floor(t) == t) ? true : false);
}

// main logic (20hz)
void loop() {
	if (Serial.available() > 0 && config.RECEIVE_DATA == true) {
		process_command(Serial.read_string());
	}

	if (at_frequency(5) == true) {
		sensor.read();
		println("voltage: " + string(sensor.voltage / 1000.0) + "V");
	}

	if (at_frequency(1) == true) {
		println(config.GLOBAL_TIME_ELAPSED); // just a neat way to keep track of time.
		memory.write(config); // saving

		transmitter.send();
	}

	// increase time elapsed.
	double iteration = round(1.00 / frequency);
	delay(1000 / frequency);

	TIME_ELAPSED = round(TIME_ELAPSED + iteration);
	config.GLOBAL_TIME_ELAPSED = round(config.GLOBAL_TIME_ELAPSED + iteration);
}
