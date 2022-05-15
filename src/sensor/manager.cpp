#include <cmath>
#include "constants.hpp"
#include "sensor/manager.hpp"
#include "util/sout.hpp"

void SensorManager::setup_imu() {
    icm_valid = _icm.begin_I2C();
}

void SensorManager::setup_bmp() {
    if (bmp_valid = _bmp.begin_I2C()) {
        // Set up oversampling and filter initialization
        if (bmp_valid)
        _bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        _bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
        _bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
        _bmp.setOutputDataRate(BMP3_ODR_50_HZ);
    }
}

void SensorManager::setup() {
    setup_bmp();
    setup_imu();
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
    const auto altitude = pressure2altitude(pressure);

    // IMU readings
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t mag;
    if (icm_valid) {
        _icm.getEvent(&accel, &gyro, nullptr, &mag);
    } else {
        accel.acceleration = { 0 };
        gyro.gyro = { 0 };
        mag.magnetic = { 0 };
    }

    // voltage maths
    // static constexpr auto multiplier = (ADC_MAX_INPUT_V / ANALOG_READ_MAX) * ((VD_R1 + VD_R2) / VD_R2);

    // const auto pin_value = analogRead(VD_PIN);
    // const auto voltage = static_cast<double>(pin_value) * multiplier;
    constexpr double voltage = 5.02;

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

        0.0
    };
}
