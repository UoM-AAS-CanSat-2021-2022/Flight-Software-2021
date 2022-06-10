#pragma once

#include <cstdint>
#include <vector>

#include <Arduino.h>
#undef B1
#include <Adafruit_BMP3XX.h>
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>
#include <HardwareSerial.h>

#include "command/parser.hpp"
#include "constants.hpp"


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

    // corrections
    double ground_altitude;

    bool gps_valid;
    bool bmp_valid;

    // individual sensor setup functions
    void setup_gps();
    void setup_bmp();
    void setup_vd();

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
    void setup();
    void calibrate();
    void set_sim_mode(SimulationMode);
    SimulationMode get_sim_mode() const;
    void set_sim_pressure(std::uint32_t);
    Telemetry read_container_telemetry();
    std::optional<double> altitude();
};
