//AAS CanSat | Container Logic -- Coded by Eduritez
//Latest revision: 15/11/2021

//Libraries
#include <time.h> 
//#include <thread>
#include "CustomFunctions.h" //Mainly to deal with C++ rounding

#include <EEPROM.h>

//Macros (For whoever wants it eh)
#define println Serial.println

//Information
const int Frequency=20;

//Variables
bool saveMemory = true;

#include "MemoryManage.h"
MemoryClass Memory;
ConfigClass Config;

double TIME_ELAPSED = 0; 

//Setup
#include "ProcessCommand.h"
#include "SensorControl.h"
#include "DescentControl.h"
#include "Transmitter.h"

TransmitterClass Transmitter;
SensorClass Sensor;

void setup() {
  pinMode(2,OUTPUT); //For Buzzer
  
  Serial.begin(57600);
  serialFlush();
  Serial.setTimeout(100);
  
  //Recovery process
  ConfigClass SavedConfig = Memory.Read();
  if (SavedConfig.STAGE_ID != 0 || SavedConfig.GLOBAL_TIME_ELAPSED != 0) {
    //println("Previous data found!");
    println("Recovering data...");
    Config = SavedConfig;
  }
  SetTime(Config);
  
}

boolean AtFrequency(int Hz) { 
  //Hz=Round(Hz);
  if (floor(Frequency/Hz)!=Frequency/Hz) {
    String error = "Unsupported frequency of " + String(Hz) + "Hz, needs to be a remainder of " + String(Frequency) +"Hz";
    println(error);
    return false;
  }
  double t=Round(TIME_ELAPSED*Hz); 
  return ((floor(t)==t)?true:false);
}

//Main logic (20Hz)
void loop() {
  if (Serial.available() > 0 && Config.RECEIVE_DATA==true) {
    processCommand(Serial.readString());
  }

  if (AtFrequency(5)==true) {
    Sensor.Read();
    println("Voltage: "+String(Sensor.voltage/1000.0)+"V");
  }

  if (AtFrequency(1)==true) {
    println(Config.GLOBAL_TIME_ELAPSED); //Just a neat way to keep track of time.
    Memory.Write(Config); //Saving
    
    Transmitter.Send();
  }
  
  //Increase time elapsed.
  double iteration = Round(1.00/Frequency);
  delay(1000/Frequency);
  
  TIME_ELAPSED = Round(TIME_ELAPSED+iteration);
  Config.GLOBAL_TIME_ELAPSED = Round(Config.GLOBAL_TIME_ELAPSED+iteration);
}
