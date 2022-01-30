//AAS CanSat | Sensors -- Coded by Eduritez
//Latest revision: 15/11/2021

//Libraries
#include <basicMPU6050.h>

//#include <BMP388_DEV.h>
//BMP388_DEV bmp388;  

//Variables
//basicMPU6050<> mpu;

//Classes
class SensorClass {
  public:
    void Setup() {
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
    void Read() {
      if (Config.SIMULATION_MODE==2) { 
        altitude=Config.SIMULATION_VALUE;
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
