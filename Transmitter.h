//AAS CanSat | Transmitter

//Full packet format: <TEAM_ID>,<MISSION_TIME>,<PACKET_COUNT>,<PACKET_TYPE>,<MODE>,<TP_RELEASED>,<ALTITUDE>,<TEMP>,<VOLTAGE>,<GPS_TIME>,<GPS_LATITUDE>,<GPS_LONGITUDE>,<GPS_ALTITUDE>,<GPS_SATS>,<SOFTWARE_STATE>,<CMD_ECHO>
//Example packet: 1057,17:48:45.91,175,C,S,R,476.2,28.3,5.02,13:23:15,69.4201,-3.2635,698.2,7,IDLE,SIMP101325

void trim(char* input) {
  char* p = strchr(input, ' ');  
  while (p) {
    *p = 0;
    char* p = strchr(input, ' ');
  }
}
void(* resetFunc) (void) = NULL; //Function pointer to address 0, which allows for the arduino/teensy to be reset.

class TransmitterClass {
public:
  void restart();
  void process_command();
  
  void decode(char* output[]) {
    int len=50;
    char* input = malloc(sizeof(char)*len);
    (Serial.readString()).toCharArray(input, len);
    trim(input);
      
    int this_size = sizeof(*output)/sizeof(*output[0])
    int i=0;
    *output[i] = strtok(input, ",");
    while (output[i] != NULL || i<=(this_size-1)) {

      i++;
      *output[i] = strtok(NULL, ",");
    }
  }
};

void TransmitterClass::send() {
    //println("Packet sent");
 }
void TransmitterClass::poll() {
  //Polls data from payload, data will be received separately in process_command.
}
  
void TransmitterClass::restart() {
  delay(200);
  resetFunc();
}

void TransmitterClass::receive() {
  char* cmd_parts[2];
  
  this->decode(cmd_parts);
  //if (sizeof(cmd_parts)/sizeof(cmd_parts[0]) != this_size) { return; } //Just making sure decoding went well.
  
  char* cmd_name = cmd_parts[0];
  char* arg = cmd_parts[1];
  Serial.println(strcat("Command input: ",cmd_name));

  //SIM - Simulation Mode Control Command
  if (cmd_name == "SIM") {
    if (arg == "DISABLE") {
      config.simulation_mode = SIMULATION_MODE::DISABLE;
    } else if (arg == "ENABLE") {
      config.simulation_mode = SIMULATION_MODE::ENABLE;
    } else if (arg == "ACTIVATE") {
      if (config.simulation_mode == SIMULATION_MODE::ENABLE) {
        config.simulation_mode = SIMULATION_MODE::ACTIVATE;
      } else {
        Serial.println("simulation_mode needs to be enabled first.");
      } 
    }
  
  //SIMP - Simulated Pressure Data
  } else if (cmd_name == "SIMP") {
    float f;
    
    config.simulation_pressure = float(atof(arg)); //String to float.
      
  //ST - Set Time
  } else if (cmd_name == "ST") {
    config.start_time = arg; //Process arg into c_time class
      
  //CX - Container Telemetry On/Off Command
  } else if (cmd_name == "CX") {
    config.container_telemetry = (arg == "ON"); //Process arg into boolean

  //Other unofficial commands
  } else if (cmd_name == "BUZZER") {
    if (arg == "ENABLE") {
      digitalWrite(2,HIGH);
      digitalWrite(2,LOW);
    } else if (arg == "DISABLE") {
    }
  
  } else if (cmd_name == "STATE") {
    if (arg == "RESET") {
      memory.save_memory=false;
      memory.clear();
      restart();
    } else if (arg == "RESTART") {
      memory.save_memory=false;
      restart();
    }
  
  //Just in case we mispelled a command in the process.
  } else {
    Serial.println("Command not found.");
    /*
     char buffer[60];
     sprintf(buffer,"Command %s not found.",cmd_name)
     Serial.println(buffer);
     */
  }
}
