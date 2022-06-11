#include <fmt/core.h>
#include <vector>

#include "constants.hpp"
#include "state.hpp"
#include "util/sout.hpp"

StateMachine::StateMachine() :
    _state(State::LaunchWait),
    _sim_mode(SimulationMode::Disable) {}

StateMachine::State StateMachine::get_state() const {
    return _state;
}

void StateMachine::step_state(std::optional<double> altitude) {
    // maintain altitude window of roughly last 5 seconds - 512 data points
    altitude_window.push_back(altitude.value_or(0.0));
    if (altitude_window.size() > 512) {
        altitude_window.pop_front();
    }

    static std::uint8_t calib_count;
    switch (_state) {
        case State::Calibrating: {
            // count up to 100 (>= because I wizardry happens sometimes)
            if (++calib_count >= CALIB_WINDOW_SIZE) {
                 
            }
            break;
        }
        case State::LaunchWait:
        case State::LaunchWaitCal:
            break;
        case State::Ascent:
            break;
        case State::DescentPar1:
            break;
        case State::DescentPar2:
            break;
        case State::DescentTpRel:
            break;
        case State::Landed:
            break;
    }
}

void StateMachine::operator()(const std::monostate&) {
#ifdef DEBUG_COMMANDS
    sout << "[CommandHandler] Parsing the command failed." << std::endl;
#endif
}

void StateMachine::operator()(const OnOff& on_off) {
#ifdef DEBUG_COMMANDS
    sout << fmt::format("[CommandHandler] Got ON_OFF value: {}", on_off) << std::endl;
#endif
    _telemetry_enable = on_off;
    // telem_mgr.set_enabled(on_off);
}

void StateMachine::operator()(const UtcTime&) {}
void StateMachine::operator()(const SimulationMode&) {}
void StateMachine::operator()(const Pressure&) {}
void StateMachine::operator()(const Command&) {}
