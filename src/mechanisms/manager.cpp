#include <cmath>
#include "constants.hpp"
#include "mechanisms/manager.hpp"
#include "sensor/manager.hpp"
#include "util/sout.hpp"

#include <Arduino.h>

DescentManager::DescentManager() {
    parachute_servo.attach(22);
    spool_servo.attach(23, 1000, 2000);

    digitalWrite(BUZZER_PIN, LOW);
    close_parachute();
    stop_payload();

    payload_released = false; 
    payload_release_time = -1; //-1 set as the inactive state, enums would have been better but when I have more time.
    payload_lowered = false;
    
    parachute_open = false;
} 

void DescentManager::setup() {
    
}

//Payload functions
long unsigned int payload_runtime = 10;

void DescentManager::stop_payload() {
    spool_servo.write(92);
}
void DescentManager::lower_payload() {
    if (payload_release_time <= 0) {
        payload_release_time = millis();
        payload_released = true;    

        Serial.println("Moving spool.");
        spool_servo.write(0);
    }

    if (millis()-payload_release_time >= payload_runtime*1000) {
        payload_lowered = true;
        stop_payload();
    }
}
void DescentManager::retract_payload() {
    payload_release_time = -1;
    spool_servo.write(180);
}

//Parachute functions
void DescentManager::release_parachute() {
    parachute_servo.write(70);
    Serial.println("Opening parachute!!");
    parachute_open = true;
}
void DescentManager::close_parachute() {
    parachute_servo.write(0);
    parachute_open = false;
}