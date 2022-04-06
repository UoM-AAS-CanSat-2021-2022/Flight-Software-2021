#include <Arduino.h>
#include "util/sout.hpp"
#include "sensor/bmp388.hpp"

void setup() {
    // Initialise serial and wait till it is ready
    Serial.begin(9600);
    while (!Serial);

    bmp388::setup();    
}

void loop() {
    const auto readings = bmp388::read_all();
    if (!readings) return;

    const auto [temp, press, alt] = *readings;

    sout << "Temperature = " << temp << " *C\n";
    sout << "Pressure = " << press / 100.0 << " hPa\n";
    sout << "Approx Alt = " << alt << " m" << std::endl;

    delay(10);
}
