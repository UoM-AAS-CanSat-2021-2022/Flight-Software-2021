#pragma once

#include <cstdint>

#include "command/parser.hpp"

#include "telemetry/manager.fwd.hpp"
#include "sensor/manager.fwd.hpp"
#include "command/parser.fwd.hpp"

#include <Arduino.h>
#include <Adafruit_BMP3XX.h>

// sensor readings / telemetry which the sensor manager is responsible for
struct Telemetry {
    double altitude;
    double temp;
    double voltage;
    double gps_latitude;
    double gps_longitude;
    double gps_altitude;
    UtcTime gps_time;
    std::uint8_t gps_sats;
};

// class which manages all the sensors, all sensors should be accessed through this class
class SensorManager {
    // sensor objects
    Adafruit_BMP3XX _bmp;

    SimulationMode _sim_mode;
    double _sim_pressure;

    // performs setup for the BMP sensor
    void setup_bmp();

public:
    SensorManager();

    // runs all the setup functions for each sensor
    void setup();
    void set_sim_mode(SimulationMode);
    SimulationMode get_sim_mode() const;
    void set_sim_pressure(std::uint32_t sim_pressure);
    Telemetry read_telemetry();
};
