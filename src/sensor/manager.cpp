#include "constants.hpp"
#include "sensor/manager.hpp"
#include "util/misc.hpp"
#include "util/sout.hpp"

SensorManager::SensorManager() :
    _gps(&GPS_SERIAL),
    _sim_mode(SimulationMode::Disable),
    _sim_pressure(SEALEVEL_PRESSURE_PA) { }

void SensorManager::setup_gps() {
    gps_valid = _gps.begin(GPS_SERIAL_BAUD);

    // Initialization.
    _gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); 
    _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    //_gps.sendCommand(PGCMD_ANTENNA); Should be only needed if there's an external antenna.
}

void SensorManager::setup_bmp() {
    bmp_valid = _bmp.begin_I2C();

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
    setup_gps();
    setup_vd();
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

void SensorManager::calibrate() {
    std::vector<float> v;

    for (int i = 0; i < 100; i++) {
        if (bmp_valid && _bmp.performReading()) {
            v.emplace_back(_bmp.pressure);
        }

        delay(10);
    }

    auto med = util::median(v);
    ground_altitude = pressure2altitude(med.value_or(SEALEVEL_PRESSURE_HPA));
}

Telemetry SensorManager::read_container_telemetry() {
    // BMP readings
    const auto reading_succeeded = bmp_valid && _bmp.performReading();

    // default to sea level pressure
    double temp { 0.0 };
    double pressure { SEALEVEL_PRESSURE_PA };
    if (reading_succeeded) {
        temp = _bmp.temperature;
        pressure = _bmp.pressure;
    }

    // replace the pressure with the simulation pressure 
    if (_sim_mode == SimulationMode::Activate) {
        pressure = _sim_pressure;
    }
    const auto altitude = pressure2altitude(pressure);

    // GPS readings
    UtcTime gps_time { 0 };
	double gps_latitude { 0.0 }, gps_longitude { 0.0 }, gps_altitude { 0.0 };
	std::uint8_t gps_sats { 0 };
    const bool new_nmea = _gps.newNMEAreceived();

#ifdef DEBUG_GPS
    sout << fmt::format("[read_container_telemetry] gps_valid={}, new_nmea={}, _gps.fix={}", gps_valid, new_nmea, _gps.fix) << std::endl;
#endif

    if (gps_valid && new_nmea && _gps.fix) {
        _gps.parse(_gps.lastNMEA());

        gps_time.h = _gps.hour;
        gps_time.m = _gps.minute;
        gps_time.s = _gps.seconds;
        gps_latitude = _gps.latitude;
        gps_longitude = _gps.longitude;
        gps_altitude = _gps.altitude;
        gps_sats = _gps.satellites;
    }

#ifdef DEBUG_GPS
    sout << fmt::format("[read_container_telemetry] gps_time=\"{:02}:{:02}:{:02}\", gps_latitude={:.4f}, gps_longitude={:.4f}, gps_altitude={:.2f}, gps_sats={}",
            gps_time.h, gps_time.m, gps_time.s, gps_latitude, gps_longitude, gps_altitude, gps_sats) << std::endl;
#endif

    // voltage maths
    static constexpr auto multiplier = (ADC_MAX_INPUT_V / ANALOG_READ_MAX) * ((VD_R1 + VD_R2) / VD_R2);

    const auto pin_value = analogRead(VD_PIN);
    const auto voltage = static_cast<double>(pin_value) * multiplier;

    return {
        altitude,
        temp,
        voltage,
        gps_time,
        gps_latitude,
        gps_longitude,
        gps_altitude,
        gps_sats,
    };
}
