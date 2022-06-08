#include <cmath>
#include <fmt/core.h>
#include "constants.hpp"
#include "sensor/manager.hpp"
#include "util/sout.hpp"

void SensorManager::setup_imu() {
    icm_valid = _icm.begin_I2C(ICM_ADDR, &I2C_WIRE);
}

void SensorManager::setup_bmp() {
    bmp_valid = _bmp.begin_I2C(BMP_ADDR, &I2C_WIRE);

    // Set up oversampling and filter initialization
    if (bmp_valid) {
        _bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        _bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
        _bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
        _bmp.setOutputDataRate(BMP3_ODR_50_HZ);
    }
}

void SensorManager::setup_vd() {
    pinMode(VD_PIN, INPUT);
    analogReadResolution(ANALOG_READ_BITS);
}

void SensorManager::setup() {
    setup_bmp();
    setup_imu();
    setup_vd();
}

void SensorManager::calibrate() {
    if (bmp_valid && _bmp.performReading()) {
        ground_pressure = _bmp.pressure;
    } else {
        // default to sea level pressure
        ground_pressure = SEALEVEL_PRESSURE_HPA;
    }

    // read the magnetometer's z axis value to get an initial value for true south
    sensors_event_t _a, _g, _t, mag;
    if (icm_valid) {
        _icm.getEvent(&_a, &_g, &_t, &mag);
    } else {
        mag.magnetic = { 0 };
    }
    true_south = mag.magnetic.z;
}

Telemetry SensorManager::read_payload_telemetry() {
    // BMP readings
    const auto reading_succeeded = bmp_valid && _bmp.performReading();

    // default to sea level pressure
    double temp { 0.0 };
    double pressure { SEALEVEL_PRESSURE_PA };
    if (reading_succeeded) {
        temp = _bmp.temperature;
        pressure = _bmp.pressure;
    }
    const auto altitude = std::max(
        static_cast<double>(0.0),
        pressure2altitude(pressure) - pressure2altitude(ground_pressure)
    );
    
    // IMU readings
    sensors_event_t accel, gyro, mag, _t;
    if (icm_valid) {
        _icm.getEvent(&accel, &gyro, &_t, &mag);
    } else {
        accel.acceleration = { 0 };
        gyro.gyro = { 0 };
        mag.magnetic = { 0 };
    }

    // voltage maths
    static constexpr auto multiplier = (ADC_MAX_INPUT_V / ANALOG_READ_MAX) * ((VD_R1 + VD_R2) / VD_R2);
    const auto pin_value = analogRead(VD_PIN);
    const auto voltage = static_cast<double>(pin_value) * multiplier;

    // calculate pointing error
    const auto yaw_pointing_error = true_south - mag.magnetic.z; // THIS IS WRONG
    // static constexpr auto yaw_pointing_error = 0;

    return {
        altitude,
        temp,
        voltage,

        gyro.gyro.x,
        gyro.gyro.y,
        gyro.gyro.z,

        accel.acceleration.x,
        accel.acceleration.y,
        accel.acceleration.z,

        mag.magnetic.x,
        mag.magnetic.y,
        mag.magnetic.z,

        yaw_pointing_error
    };
}
