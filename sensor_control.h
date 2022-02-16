//AAS CanSat | Sensors

//Libraries
#include <basicMPU6050.h>

//#include <BMP388_DEV.h>
//BMP388_DEV bmp388;  

//basicMPU6050<> mpu;
class Vector { //Not an actual vector class, just to allow for better storage of IMU data.
public:
  float X=0;
  float Y=0;
  float Z=0;
  void Update(int x, int y, int z) {
    X=x; Y=y; Z=z;
  }
};

class SensorClass {
public:
  void setup() {
    //BMP388 setup
    /*
    bmp388.begin();
    bmp388.setTimeStandby(TIME_STANDBY_1280MS);
    bmp388.startNormalConversion();

    
    */

    //MPU setup
    //mpu.setup();
    //mpu.setBias();
  }
  void read() {
    if (config.simulation_mode == SIMULATION_MODE::ACTIVATE) { 
      altitude = config.simulation_pressure;
    }

    //BMP388
    /*
    bmp388.getMeasurements(temp, pressure, altitude);
    */
    
    //MPU
    /*
    println(mpu.temp());
    temp = mpu.temp();
    Acc.Update(mpu.ax(), mpu.ay(), mpu.az());
    Gyro.Update(mpu.gx(), mpu.gy(), mpu.gz());
    */

    //Voltage sensor
    voltage = (analogRead(A0)*5)/1024;
  }
  
  //BMP388 data
  float temp;
  float pressure;
  float altitude;

  //MPU data
  Vector Acc;
  Vector Gyro;
  Vector Field;

  /*GPS   
  float latitude;
  float longitude;
  int nSatellites;
  */
  
  //Others
  float voltage=0;
};
