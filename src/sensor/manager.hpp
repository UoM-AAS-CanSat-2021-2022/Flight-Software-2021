#pragma once
#include <cstdint>
#include <vector>

#include "command/parser.hpp"

#include <Arduino.h>

//Sensor libraries
#include <Adafruit_BMP3XX.h>

#include <HardwareSerial.h>
#include <Adafruit_GPS.h>

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>

#define VOLTAGE_PIN 12 //Random value

// sensor readings / telemetry which the sensor manager is responsible for
struct Telemetry {
    double altitude;
    double temp;
    double voltage;

    double gps_latitude;
    double gps_longitude;
    double gps_altitude;
    UtcTime gps_time;
    double gps_sats;

    std::vector<double> gyro;
    std::vector<double> acc;
    std::vector<double> mag;

    UtcTime gps_time;
    std::uint8_t gps_sats;
};

// class which manages all the sensors, all sensors should be accessed through this class
class SensorManager {
    // sensor objects
    HardwareSerial* GPSSerial = &Serial1;
    Adafruit_GPS GPS(HardwareSerial* Serial1);
    Adafruit_BMP3XX _bmp;
    Adafruit_ICM20948 _icm;

    SimulationMode _sim_mode;
    double _sim_pressure;

    double _temp;
    double _gps_latitude;
    double _gps_longitude;
    double _gps_altitude;
    double _gps_time;
    double _gps_sats;

    // performs setup for the BMP sensor
    void setup_bmp();
    void setup_gps();
    void setup_imu();

public:
    SensorManager();

    // runs all the setup functions for each sensor
    void setup();
    void set_sim_mode(SimulationMode);
    SimulationMode get_sim_mode() const;
    void set_sim_pressure(std::uint32_t sim_pressure);
    Telemetry read_telemetry();
};
