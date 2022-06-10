#include <fmt/core.h>
#include "state.hpp"
#include "util/sout.hpp"

StateMachine::StateMachine(SensorManager& sensor_mgr) : _sensor_mgr(sensor_mgr), _state(State::LaunchWait) {}

StateMachine::State StateMachine::get_state() const {
    return _state;
}

void StateMachine::transition() {
    static std::vector<float> alts;

    switch (_state) {
        case State::Calibrating: {
            if (alts.size() < 100) {
                alts.emplace_back(_sensor_mgr.altitude().value_or(0.0));
            } else {
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
    telemetry_enable = on_off;
    // telem_mgr.set_enabled(on_off);
}

void StateMachine::operator()(const UtcTime&) {}
void StateMachine::operator()(const SimulationMode&) {}
void StateMachine::operator()(const Pressure&) {}
void StateMachine::operator()(const Command&) {}
void StateMachine::operator()(const TetheredPayloadDepth&) {}
