//AAS CanSat | Descent Control

class DescentController {
public:
  void check() {
    if (sensor.altitude <= 400 && config.parachute2_deployed==false) {
      //Deploy second parachute.
      parachute2_deployed=true;
    }

    if (sensor.altitude <= 300) {
      if (config.payload_state == PAYLOAD_STATE::IDLE) {
        config.payload_state = PAYLOAD::RELEASED;
        config.payload_telemetry = true;

        //Turn on and setup camera
        config.container_camera = true;
      } 
      if (config.payload_extension<=9.9) {
        
        //Lower payload
      } else {
        config.payload_lowered = true
      }
      
    }
  }
  
};
