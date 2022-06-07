// this must be included before Arduino.h, because Arduino.h produces colliding macro names
#include <fmt/core.h>
#include <optional>
#include <string>
#include <string_view>

#include "command/parser.hpp"
#include "constants.hpp"
#include "runner.hpp"
#include "sensor/manager.hpp"
#include "mechanisms/manager.hpp"
#include "telemetry/manager.hpp"
#include "util/sout.hpp"
#include "xbee/manager.hpp"

// Arduino has so many conflicting names smh my head
#include <Arduino.h>
#include <TimeLib.h>

XBeeManager xbee_mgr;
SensorManager sensor_mgr {};
DescentManager descent_mgr {};
TelemetryManager telem_mgr { xbee_mgr, sensor_mgr };
CommandParser cmd_parser { telem_mgr };
Runner runner;

double current_pressure = 90000;

time_t getTeensy3Time();
void handle_response(Rx16Response&, uintptr_t);
void add_tasks_to_runner();

void setup() {
	// setup pins
	pinMode(VD_PIN, INPUT);
	pinMode(RETRACT_PIN, INPUT);
	analogReadResolution(ANALOG_READ_BITS);

	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, LOW);

	// setup serial connections / peripherals
	Serial.begin(DEBUG_SERIAL_BAUD);
	xbee_mgr.setup(XBEE_SERIAL);
	xbee_mgr.set_panid(GCS_LINK_PANID);
	xbee_mgr.onRx16Response(handle_response);
	sensor_mgr.setup();

	// setup RTC as time provider
	setSyncProvider(getTeensy3Time);

	// add the tasks for the runner to do
	add_tasks_to_runner();
	
	// Initializes altitude to 1000.
	sensor_mgr.set_sim_mode(SimulationMode::Enable);
	sensor_mgr.set_sim_mode(SimulationMode::Activate);

	sensor_mgr.set_sim_pressure(current_pressure);
	delay(2000);
}

void loop() {
	current_pressure+=0.5;
	sensor_mgr.set_sim_pressure(current_pressure);
	//sensor_mgr.read_container_telemetry();

	Serial.print("Altitude at ");
	Serial.print(current_pressure);
	Serial.println(":");
	Serial.println(sensor_mgr.altitude);

	if (descent_mgr.parachute_open == false && sensor_mgr.altitude <= 400) {
		Serial.println("< 400");
		descent_mgr.release_parachute();
		
	} else if (descent_mgr.payload_lowered == false && sensor_mgr.altitude <= 300) {
		Serial.println("< 300");
		descent_mgr.lower_payload();

	} else if (sensor_mgr.altitude <= 5) {
		Serial.println("< 5");
		digitalWrite(BUZZER_PIN, HIGH);
		//descent_mgr.retract_payload();
	}

	runner.run(); 
}

// simple function to add tasks to the runner
// in it's own function to make setup clearer
void add_tasks_to_runner() {
	// run the xbee manager's loop every time
	runner.schedule_task(
		[]() { xbee_mgr.loop(); });

	// send the container telemetry once a second if its enabled
	runner.schedule_task(
		1000,
		[]() { telem_mgr.send_container_telemetry(); });

	// send the payload telemetry four times a second if the
	// payload is released
	runner.schedule_task(
		250,
		[]() {
			if (!descent_mgr.payload_released)
				return;

			xbee_mgr.set_panid(PAYLOAD_LINK_PANID);
			// poll the container for the data, for now just mock it out
			// NOTE: eventually none of this will be needed as the handle_response method
			//       will get called for the response from the XBee
			std::string_view mock_payload_relay_data { "165.2,13.7,5.02,0.18,0.08,-0.18,0.12,0.31,9.8,0.19,-0.05,0.47,12,LANDED" };
			xbee_mgr.set_panid(GCS_LINK_PANID);
			telem_mgr.forward_payload_telemetry(mock_payload_relay_data);
		});

	runner.schedule_task(
		125,
		[]() {
			sensor_mgr.read_container_telemetry();
		});

	

}

// ngl it feels weird to have this here but I couldn't really think of a better place to put it...
time_t getTeensy3Time() {
	return Teensy3Clock.get();
}

// remove the logging messages once we aren't testing any more
struct CommandHandler {
	void operator()(const std::monostate&) {
		// parse failed, ignore it
		sout << "Parsing the command failed." << std::endl;
	}

	void operator()(const OnOff& on_off) {
		sout << "Got ON_OFF value: " << std::boolalpha << on_off << std::endl;
		telem_mgr.set_enabled(on_off);
	}

	void operator()(const UtcTime& utc_time) {
		const auto [h, m, s] = utc_time;
		sout << fmt::format("Got UtcTime value: {:02}:{:02}:{:02}", h, m, s) << std::endl;

		// we don't care about the full date, just the hour minute and second
		setTime(h, m, s, 0, 0, 0);
	}

	void operator()(const SimulationMode& mode) {
		sout << "Got MODE value: " << mode << std::endl;

		sensor_mgr.set_sim_mode(mode);
		sout << "sim_mode=" << sensor_mgr.get_sim_mode() << std::endl;
	}

	void operator()(const Pressure& pressure) {
		sout << "Got PRESSURE value: " << pressure << std::endl;
		sensor_mgr.set_sim_pressure(pressure);
	}
};

void handle_response(Rx16Response& resp, uintptr_t) {
	// make sure we are listening on the gcs link as much as possible
	if (xbee_mgr.get_panid() != GCS_LINK_PANID) {
		xbee_mgr.set_panid(GCS_LINK_PANID);
	}

	std::string_view data_view {
		reinterpret_cast<char*>(resp.getData()),
		static_cast<size_t>(resp.getDataLength())
	};

	// sout << "BUZZER_PIN, HIGH" << std::endl;
	// digitalWriteFast(BUZZER_PIN, HIGH);
	// runner.run_after(5'000, []() {
	// 	sout << "BUZZER_PIN, LOW" << std::endl;
	// 	digitalWrite(BUZZER_PIN, LOW);
	// });

	switch (resp.getRemoteAddress16()) {
	case GCS_XBEE_ADDRESS: {
		// data is from the ground station, interpret it as a command
		std::string cmd_string { data_view };
		std::visit(CommandHandler {}, cmd_parser.parse(cmd_string));
		break;
	}
	case PAYLOAD_XBEE_ADDRESS:
		// data is from payload, forward it to the ground station
		telem_mgr.forward_payload_telemetry(data_view);
		break;
	default:
		// invalid PANID, maybe log in future?
		break;
	}
}
