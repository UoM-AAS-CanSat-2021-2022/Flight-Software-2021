#pragma once

#include <cstdint>
#include <vector>
#include <Servo.h>

#include "sensor/manager.hpp"

#include <Arduino.h>

// class which manages all descent control, with a function that's called to check for when the payload is above/below 300m.
class DescentManager {
    Servo parachute_servo; // Servo obj to control servo driver
    Servo spool_servo; // Servo obj to control servo driver

    // performs internal setup for motors
    void setup();

public:
    DescentManager();

    void release_parachute();
    void close_parachute();

    void lower_payload();
    void stop_payload();
    void retract_payload();
    
    bool payload_released;
    long unsigned int payload_release_time;

    bool payload_lowered;
    bool parachute_open;
};
