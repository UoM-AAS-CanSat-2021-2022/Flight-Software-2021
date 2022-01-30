//AAS CanSat | Memory Manage -- Coded by Eduritez
//Latest revision: 15/11/2021

//Macros
#define ForEEPROM for (int i=0; i<Size; i++)

//Config
class ConfigClass { //Whatever is saved goes here.
  public:
    int STAGE_ID = 0; //Keeps track of stage and allows for recovery. 

    String START_TIME = "";
    double GLOBAL_TIME_ELAPSED = 0;                                 
    
    boolean RECEIVE_DATA = true;                                  
    
    //Simulation stuff
    byte SIMULATION_MODE = 0;
    double SIMULATION_VALUE = 0;
};
int startingAddress=0;
int Size=40;

//Functions
void SetTime(ConfigClass c) {
   if (c.START_TIME=="") {
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    String timeResult = asctime(timeinfo);
    c.START_TIME = timeResult;
  }
}

//Class
class MemoryClass {
  public:
    ConfigClass Read() { //Used for retrieving recovery data.
      ConfigClass newConfig;
      EEPROM.get(startingAddress,newConfig);
      return newConfig;
    }
    void Write(ConfigClass c) { //Saving data
      if (saveMemory==true) {
        EEPROM.put(startingAddress,c); //Just updates the values, doesn't keep rewriting.
        Size = sizeof(c);
      }
    }
    void Clear() { //Resets data
      ForEEPROM {
        if (EEPROM.read(i) != 0) { EEPROM.write(i, 0); }
      }
      //println("Memory cleared");
    }
};
