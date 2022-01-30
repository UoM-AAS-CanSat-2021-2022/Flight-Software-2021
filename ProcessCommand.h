//AAS CanSat | Process Command -- Coded by Eduritez
//Latest revision: 15/11/2021

void(* resetFunc) (void) = 0;

void Restart() {
  delay(200);
  resetFunc();
}

//Class
void processCommand(String cmdName) {
  cmdName.trim();

  println("Command input: "+cmdName);
  
  if (cmdName=="SIMULATION ACTIVATE") {
    Config.SIMULATION_MODE=1;
    
  } else if (cmdName=="SIMULATION DEACTIVATE" || cmdName=="SIMULATION DISABLE") {
    Config.SIMULATION_MODE=0;
    
  } else if (cmdName=="SIMULATION ENABLE") {
    if (Config.SIMULATION_MODE!=0) {
     Config.SIMULATION_MODE=2;
    } else {
      println("Simulation Mode needs to be activated first.");
    }
      
  } else if (cmdName=="BUZZER ENABLE") {
    digitalWrite(2,HIGH);
    
  } else if (cmdName=="BUZZER DISABLE") {
    digitalWrite(2,LOW);

  } else if (cmdName=="RESET") {
    saveMemory=false;
    Memory.Clear();
    Restart();
    
  } else if (cmdName=="RESTART") {
    saveMemory=false;
    Restart();
    
  } else {
    println("Command "+cmdName+" not found.");
  }
}
