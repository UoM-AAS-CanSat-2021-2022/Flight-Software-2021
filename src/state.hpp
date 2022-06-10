#pragma once

#include <deque>
#include <queue>
#include "sensor/manager.hpp"


class StateMachine {
    enum class State {
        // uncalibrated, waiting for launch
        LaunchWait,
        // Calibrating...
        Calibrating,
        // Calibrated and awaiting launch
        LaunchWaitCal,
        // We going UP
        Ascent,
        // First parachute deployed
        DescentPar1,
        // Second parachute deployed
        DescentPar2,
        // Tethered Payload Released
        DescentTpRel,
        // We have landed 
        Landed,
    };

    SensorManager& _sensor_mgr;
    std::deque<double> altitude_window;

    State _state;
    bool telemetry_enable;
    SimulationMode sim_mode;
    Pressure sim_pressure;
    TetheredPayloadDepth tp_depth;

public:
    StateMachine(SensorManager&);
    State get_state() const;
    void transition();
    void reset();
    void save_to_eeprom() const;
    bool telemetry_enabled() const;

    // implement the Visitor interface for the Command::Value type
	void operator()(const std::monostate&);
	void operator()(const OnOff&);
	void operator()(const UtcTime&);
	void operator()(const SimulationMode&);
	void operator()(const Pressure&);
	void operator()(const Command&);
	void operator()(const TetheredPayloadDepth&);
};
