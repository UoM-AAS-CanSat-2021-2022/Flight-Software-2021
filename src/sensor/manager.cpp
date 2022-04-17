#include <cmath>
#include "constants.hpp"
#include "sensor/manager.hpp"
#include "util/sout.hpp"

SensorManager::SensorManager() : _sim_mode(SimulationMode::Disable), _sim_pressure(101325.0) { }

void SensorManager::setup_bmp() {
    // Hardware I2C mode
    if (!_bmp.begin_I2C()) {
        sout << "Could not find a valid BMP3 sensor, check wiring!" << std::endl;
        while (true);
    }

    // Set up oversampling and filter initialization
    _bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    _bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    _bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    _bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void SensorManager::setup() {
    setup_bmp();
}

void SensorManager::set_sim_mode(SimulationMode sim_mode) {
    switch (sim_mode) {
        case SimulationMode::Disable:
            _sim_mode = SimulationMode::Disable;
            break;
        case SimulationMode::Enable:
            if (_sim_mode == SimulationMode::Disable)
                _sim_mode = SimulationMode::Enable;
            break;
        case SimulationMode::Activate:
            if (_sim_mode == SimulationMode::Enable)
                _sim_mode = SimulationMode::Activate;
            break;
    }
}

SimulationMode SensorManager::get_sim_mode() const {
    return _sim_mode;
}

void SensorManager::set_sim_pressure(std::uint32_t sim_pressure) {
    _sim_pressure = static_cast<double>(sim_pressure);
}

Telemetry SensorManager::read_telemetry() {
    const auto reading_succeeded = _bmp.performReading();
    // default to sea level pressure
    double pressure { 1013.25 };
    if (_sim_mode == SimulationMode::Activate) {
        pressure = _sim_pressure;
    } else if (reading_succeeded) {
        pressure = _bmp.pressure;
    }

    double temp = 0.0;
    if (reading_succeeded) {
        temp = _bmp.temperature;
    }

    // Adapted from readAltitude
    // Equation taken from BMP180 datasheet (page 16):
    //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

    // Note that using the equation from wikipedia can give bad results
    // at high altitude. See this thread for more information:
    //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064
    double atmospheric = pressure / 100.0F;
    double altitude = 44330.0 * (1.0 - pow(atmospheric / SEALEVELPRESSURE_HPA, 0.1903));

	constexpr double voltage { 5.02 };
	constexpr UtcTime gps_time { 13, 23, 15 };
	constexpr double gps_latitude { 69.4201 };
	constexpr double gps_longitude { -3.2635 };
	constexpr double gps_altitude { 698.2 };
	constexpr std::uint8_t gps_sats { 7 };

    return {
        altitude,
        temp,
        voltage,
        gps_latitude,
        gps_longitude,
        gps_altitude,
        gps_time,
        gps_sats
    };
}
