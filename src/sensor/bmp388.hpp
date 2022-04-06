#pragma once

#include <Arduino.h>
#include <Adafruit_BMP3XX.h>
#include <cstdint>
#include <optional>

extern Adafruit_BMP3XX bmp;

namespace bmp388 {
    static constexpr double SEALEVELPRESSURE_HPA = 1013.25;

    // setup the BMP device, must be called in setup()
    void setup();

    // calculates the altitude from a pressure reading
    double calc_altitude(double);

    struct AllReadings {
        double temperature;
        double pressure;
        double approx_altitude;
    };

    std::optional<AllReadings> read_all();
}
