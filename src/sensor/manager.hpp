#pragma once

#include <cstdint>
#include <vector>

#include "command/parser.hpp"

#include <Arduino.h>

//Sensor libraries
#include <Adafruit_BMP3XX.h>

#include <HardwareSerial.h>
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>

// sensor readings / telemetry which the sensor manager is responsible for
struct Telemetry {
    double altitude;
    double temp;
    double voltage;
    UtcTime gps_time;
    double gps_latitude;
    double gps_longitude;
    double gps_altitude;
    std::uint8_t gps_sats;
};

// class which manages all the sensors, all sensors should be accessed through this class
class SensorManager {
    // sensor objects
    Adafruit_GPS _gps;
    Adafruit_BMP3XX _bmp;

    SimulationMode _sim_mode;
    double _sim_pressure;

    // performs setup for the BMP sensor
    void setup_gps();
    void setup_bmp();

    void calibrate();
    float correction = 0;

    constexpr double pressure2altitude(const double pressure) {
        // Adapted from readAltitude
        // Equation taken from BMP180 datasheet (page 16):
        //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

        // Note that using the equation from wikipedia can give bad results
        // at high altitude. See this thread for more information:
        //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064
        double atmospheric = pressure / 100.0F;
        return 44330.0 * (1.0 - pow(atmospheric / SEALEVEL_PRESSURE_HPA, 0.1903));
    }

public:
    SensorManager();

    // runs all the setup functions for each sensor
    void setup();
    void set_sim_mode(SimulationMode);
    SimulationMode get_sim_mode() const;
    void set_sim_pressure(std::uint32_t);
    Telemetry read_container_telemetry();
};
