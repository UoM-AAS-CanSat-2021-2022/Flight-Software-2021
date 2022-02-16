//AAS CanSat | Container Logic

#include <time.h>
#include <stdio.h>
#include <EEPROM.h>

typedef enum class SOFTWARE_STATE {
  BOOT,
  IDLE,
  LAUNCHING,
  DESCENDING,
  LANDED
} SOFTWARE_STATE;
typedef enum class PAYLOAD_STATE {
  IDLE,
  RELEASED,
  LOWERED,
} PAYLOAD_STATE;
typedef enum class SIMULATION_MODE {
  DISABLE,
  ENABLE,
  ACTIVATE,
} SIMULATION_MODE;
const int core_frequency = 40;

double time_elapsed = 0;
int iteration = 0;

#include "memory_manage.h"
MemoryClass memory;
ConfigClass config;

//Setup
#include "sensor_control.h"
#include "transmitter.h"
#include "descent_control.h"

TransmitterClass transmitter;
SensorClass sensor;
DescentController descent_control;

void serialFlush() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}
double customRound(double val) {
  return (floor(val * 100 + 0.1) / 100);
}
void setup() {
  Serial.begin(57600);
  serialFlush();
  Serial.setTimeout(200);

  pinMode(2, OUTPUT); //For Buzzer

  Serial.println("CanSat Container booted");

  //Recovery process
  ConfigClass* saved_config = memory.read();
  if (memory.recover_memory == true && (saved_config->state != SOFTWARE_STATE::BOOT || saved_config->start_time != "")) {
    //println("Previous data found!");
    Serial.println("Recovering data...");
    config = *saved_config;
  }
}

boolean atFrequency(float hz) {
  if (floor(core_frequency / hz) != customRound(core_frequency / hz)) {
    String error = "Unsupported frequency of " + String(hz) + "Hz, needs to be a remainder of " + String(core_frequency) + "Hz";
    Serial.println(error);
    return false;
  }
  double t = customRound(time_elapsed * hz);
  return ((floor(t) == t) ? true : false);
}

//Main logic (40Hz)
void loop() {

  //Transmitter connections:
  if (Serial.available() > 0) {
    transmitter.receive();
  }
  if (atFrequency(1) == true) {
    if (config.container_telemetry == true) {
      transmitter.send();
    }
  }
  
  if (atFrequency(0.5) == true) {
    Serial.println("Saving");
    memory.write(&config); //Saving
    Serial.println(time_elapsed);
  }

  if (atFrequency(4) == true) {
    if (config.payload_telemetry == true) {
      transmitter.poll();
    }
    //Serial.println("Voltage: " + String(sensor.voltage/1000.0)+"V");
  }
  if (atFrequency(5) == true) {
    sensor.read();
    descent_control.check();
    //Serial.println("Voltage: " + String(sensor.voltage/1000.0)+"V");
  }

  
  //Increase time elapsed.
  iteration += 1;
  delay(10^3 / core_frequency);
  
  time_elapsed = customRound(time_elapsed + customRound(1.00 / core_frequency));
}
