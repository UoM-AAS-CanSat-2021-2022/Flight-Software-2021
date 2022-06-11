#pragma once

#include <deque>
#include <optional>
#include "command/parser.hpp"


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

    std::deque<double> altitude_window;

    State _state;
    bool _telemetry_enable;
    SimulationMode _sim_mode;
    Pressure _sim_pressure;

public:
    StateMachine();
    State get_state() const;
    void step_state(std::optional<double>);
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
};
