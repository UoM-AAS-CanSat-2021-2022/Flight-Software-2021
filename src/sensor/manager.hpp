#pragma once

#include <cstdint>
#include <vector>

#include <Arduino.h>

//Sensor libraries
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_ICM20948.h>

// sensor readings / telemetry which the sensor manager is responsible for
struct Telemetry {
    double altitude;
    double temp;
    double voltage;

    double gyro_r;
    double gyro_p;
    double gyro_y;

    double accel_r;
    double accel_p;
    double accel_y;

    double mag_r;
    double mag_p;
    double mag_y;

    double pointing_error;
};

// class which manages all the sensors, all sensors should be accessed through this class
class SensorManager {
    // sensor objects
    Adafruit_ICM20948 _icm;
    Adafruit_BMP3XX _bmp;

    // did the sensor intialize correctly
    bool icm_valid;
    bool bmp_valid;

    void setup_imu();
    void setup_bmp();

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
    // runs all the setup functions for each sensor
    void setup();
    Telemetry read_payload_telemetry();
};
