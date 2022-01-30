// you should REALLY not be using function pointers, please try and remove this...
void (*reset_func)(void) = NULL;

void restart() {
	delay(200);
	if (reset_func != NULL) {
		reset_func();
	}
}

// this does match the spec, don't change it for now as I will fix it when I start working on my code
void process_command(string cmd_name) {
	cmd_name.trim();

	println("command input: " + cmd_name);

	if (cmd_name == "SIMULATION ENABLE") {
		config.SIMULATION_MODE = SimState::SIMULATION_ENABLE;
	} else if (cmd_name == "SIMULATION DISABLE") {
		config.SIMULATION_MODE = SimState::SIMULATION_DISABLE;
	} else if (cmd_name == "SIMULATION ACTIVATE") {
		if (config.SIMULATION_MODE == SimState::SIMULATION_ENABLE) {
			config.SIMULATION_MODE = SimState::SIMULATION_ACTIVATE;
		} else {
			println("simulation mode needs to be enabled first.");
		}
	} else if (cmd_name == "BUZZER ENABLE") {
		digital_write(2, HIGH);
	} else if (cmd_name == "BUZZER DISABLE") {
		digital_write(2, LOW);
	} else if (cmd_name == "RESET") {
		save_memory = false;
		memory.clear();
		restart();
	} else if (cmd_name == "RESTART") {
		save_memory = false;
		restart();
	} else {
		println("command " + cmd_name + " not found.");
	}
}
