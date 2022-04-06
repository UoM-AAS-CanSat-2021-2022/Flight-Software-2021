#include <Arduino.h>
#include "util/sout.hpp"

#include "sensor/camera.hpp"

/*
The Spycam can work in two different modes- video and snapshot.
In general, the white trigger line is pulled high.
If trigger is pulsed low for a short amount of time (less than half a second) it will take a photo.
If trigger is pulsed low for a long amount of time (a second or longer) it will start/stops video recording. 
*/

void Camera::pulse_low(const int duration) {
    digitalWriteFast(trigger_pin, LOW);
    delay(duration);
    digitalWriteFast(trigger_pin, HIGH);
}

Camera::Camera(int trigger_pin) : recording(false), trigger_pin(trigger_pin) {}

void Camera::setup_pins() {
    pinMode(trigger_pin, OUTPUT);
    digitalWriteFast(trigger_pin, HIGH);
}
void Camera::start_recording() {
    if (recording) {
        sout << "Already recording." << std::endl;
        return;
    }

    pulse_low(start_delay);
    recording = true;
}

void Camera::stop_recording() {
    if (!recording) {
        sout << "Not recording." << std::endl;
        return;
    }

    pulse_low(stop_delay);
    recording = false;
}
