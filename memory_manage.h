#include <ctime>

class enum sim_state {
	SIMULATION_DISABLE,
	SIMULATION_ENABLE,
	SIMULATION_ACTIVATE,
};

struct config_class {
	int STAGE_ID;

	string START_TIME;
	double GLOBAL_TIME_ELAPSED;

	boolean RECEIVE_DATA;

	// simulation stuff
	enum sim_state SIMULATION_MODE;
	double SIMULATION_VALUE;

	config_class() : START_TIME(""), RECEIVE_DATA(true) { }

	void set_time() {
		if (this->START_TIME == "") {
			time_t rawtime;
			struct tm* timeinfo;
			time(&rawtime);
			timeinfo = gmtime(&rawtime);
			string time_result = asctime(timeinfo);
			this->START_TIME = time_result;
		}
	}
};

struct memory_class {
	int starting_address;

	memory_class() { }

	config_class read() {
		config_class new_config;
		EEPROM.get(starting_address, new_config);
		return new_config;
	}

	void write(config_class c) {
		if (save_memory == true) {
			EEPROM.put(starting_address, c);
		}
	}

	void clear() {
		for (int i = 0; i < sizeof(config_class); i++) {
			if (EEPROM.read(i) != 0) {
				EEPROM.write(i, 0);
			}
		}
	}
};
