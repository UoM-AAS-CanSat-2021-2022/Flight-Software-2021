//AAS CanSat | Memory Manage

int starting_address=0;
int clear_size;

//Variables to be saved between shutdown.
struct ConfigClass { //Whatever is saved goes here.
  SOFTWARE_STATE state = SOFTWARE_STATE::BOOT; //Keeps track of stage and allows for recovery. 
  String start_time = "";                               

  bool container_telemetry = false;       
  bool payload_telemetry = false;                            
  int packet_count = 0;
  
  double current_altitude = 0;

  //Descent control
  PAYLOAD_STATE payload_state = PAYLOAD_STATE::IDLE;
  float payload_extension = 0;
  
  bool parachute1_deployed = false;
  bool parachute2_deployed = false;
   
  //Simulation stuff
  SIMULATION_MODE simulation_mode = SIMULATION_MODE::DISABLE;
  double simulation_pressure = 0;

private:
  void set_time() {
    if (this->start_time == "") {
      time_t rawtime;
      struct tm* timeinfo;
      time(&rawtime);
      timeinfo = gmtime(&rawtime);
      String time_result = asctime(timeinfo);
      this->start_time = time_result;
    }
  }
};

struct MemoryClass {
  bool save_memory = true;
  bool recover_memory = true;
  
  ConfigClass* read() { //Used for retrieving recovery data.
    ConfigClass new_config;
    EEPROM.get(starting_address,new_config);
    return &new_config;
  }
  void write(ConfigClass* c) { //Saving data
    if (save_memory == true) {
      EEPROM.put(starting_address,c); //Just updates the values, doesn't keep rewriting.
      clear_size = sizeof(c);
    }
  }
  void clear() { //Resets data
    for (int i=0; i < sizeof(ConfigClass); i++) {
      if (EEPROM.read(i) != 0) { EEPROM.write(i, 0); }
    }
    //Serial.println("Memory cleared");
  }
};
