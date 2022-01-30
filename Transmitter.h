//AAS CanSat | Sensors -- Coded by Eduritez
//Latest revision: 15/11/2021

//Full packet format: <TEAM_ID>,<MISSION_TIME>,<PACKET_COUNT>,<PACKET_TYPE>,<MODE>,<TP_RELEASED>,<ALTITUDE>,<TEMP>,<VOLTAGE>,<GPS_TIME>,<GPS_LATITUDE>,<GPS_LONGITUDE>,<GPS_ALTITUDE>,<GPS_SATS>,<SOFTWARE_STATE>,<CMD_ECHO>
//Example packet: 1057,17:48:45.91,175,C,S,R,476.2,28.3,5.02,13:23:15,69.4201,-3.2635,698.2,7,IDLE,SIMP101325

//Information
const String SOFTWARE_STATE[5] = {
  "BOOT",       //STAGE_ID 0
  "IDLE",       //STAGE_ID 1
  "LAUNCHING",  //STAGE_ID 2
  "DESCENDING", //STAGE_ID 3
  "LANDED"      //STAGE_ID 4
};

//Classes
class TransmitterClass{
  public:
    void Send() {
      //println("Packet sent");
    }
    
  private:
    void Decode(String packet) {
      
    }
};
