#include <cmath>
#include "constants.hpp"
#include "sensor/manager.hpp"
#include "util/sout.hpp"

SensorManager::SensorManager() : _sim_mode(SimulationMode::Disable), _sim_pressure(101325.0) { }

void SensorManager::setup_bmp() {
    // Hardware I2C mode
    if (!_bmp.begin_I2C()) {
        sout << "[BMP] Could not find a valid BMP3 sensor, check wiring!" << std::endl;
        while (true);
    }

    // Set up oversampling and filter initialization
    _bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    _bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    _bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    _bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}
void SensorManager::setup_imu() {
    if(!_icm.begin_I2C()) {
        sout << "[IMU] Could not find a valid ICM20X sensor, check wiring!" << std::endl;
        while (true);
    }
}

void SensorManager::setup_gps() {
    GPS.begin(9600);

    // Initialization.
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); 
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    //GPS.sendCommand(PGCMD_ANTENNA); Should be only needed if there's an external antenna.
}

void SensorManager::setup() {
    setup_bmp();
    setup_gps();
    setup_imu();
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
    //BPM readings
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

    //IMU readings
    sensors_event_t _accel;
    sensors_event_t _gyro;
    sensors_event_t _mag;
    //sensors_event_t temp; GPS also measures temperature.
    _icm.getEvent(&_accel, &_gyro, nullptr, &_mag);

    std::vector<double> gyro{
        _gyro.gyro.x, 
        _gyro.gyro.y, 
        _gyro.gyro.z
    };
    std::vector<double> acc{
        _accel.acceleration.x, 
        _accel.acceleration.y, 
        _accel.acceleration.z
    };
    std::vector<double> mag{
        _mag.magnetic.x, 
        _mag.magnetic.y, 
        _mag.magnetic.z
    };
    
    //GPS
    UtcTime gps_time { 0, 0, 0 };
	double gps_latitude { 0 };
	double gps_longitude { 0 };
	double gps_altitude { 0 };
	std::uint8_t gps_sats { 0 };

    if (GPS.newNMEAreceived() && GPS.fix) {
        GPS.parse(GPS.lastNMEA());

        UtcTime gps_time;

        gps_latitude = GPS.latitude;
        gps_longitude = GPS.longitude;
        gps_altitude = GPS.altitude;
        gps_sats = (int)GPS.satellites;

        // GPS.speed --Speed in knots
        // GPS.angle --Angle
    } else {
        Serial.println("[GPS] No data received");
    }

    //Voltage
    double voltage = analogRead(VOLTAGE_PIN);

    return {
        altitude,
        temp,
        voltage,
        
        gps_latitude,
        gps_longitude,
        gps_altitude,
        gps_time,
        gps_sats,

        gyro,
        acc,
        mag
    };
}
