//GPS and IMU

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_GPS.h>

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);
uint32_t timer = millis();

Adafruit_ICM20948 icm;

void setup()
{
  Serial.begin(115200);
  GPSSerial.begin(9600);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); 

  GPS.sendCommand(PGCMD_ANTENNA);

  if(!icm.begin_I2C())
  {
    while (1) {
      Serial.println("IMU failed initializing - Not found");
      delay(10);
    }
  }
}

void loop()
{
  //  /* Get a new normalized sensor event */
  // sensors_event_t accel;
  // sensors_event_t gyro;
  // sensors_event_t mag;
  // sensors_event_t temp;
  // icm.getEvent(&accel, &gyro, &temp, &mag);

  // Serial.print("\t\tTemperature ");
  // Serial.print(temp.temperature);
  // Serial.println(" deg C");

  // /* Display the results (acceleration is measured in m/s^2) */
  // Serial.print("\t\tAccel X: ");
  // Serial.print(accel.acceleration.x);
  // Serial.print(" \tY: ");
  // Serial.print(accel.acceleration.y);
  // Serial.print(" \tZ: ");
  // Serial.print(accel.acceleration.z);
  // Serial.println(" m/s^2 ");

  // Serial.print("\t\tMag X: ");
  // Serial.print(mag.magnetic.x);
  // Serial.print(" \tY: ");
  // Serial.print(mag.magnetic.y);
  // Serial.print(" \tZ: ");
  // Serial.print(mag.magnetic.z);
  // Serial.println(" uT");

  // /* Display the results (acceleration is measured in m/s^2) */
  // Serial.print("\t\tGyro X: ");
  // Serial.print(gyro.gyro.x);
  // Serial.print(" \tY: ");
  // Serial.print(gyro.gyro.y);
  // Serial.print(" \tZ: ");
  // Serial.print(gyro.gyro.z);
  // Serial.println(" radians/s ");
  // Serial.println();

  if (GPSSerial.available()) {
    char c = GPS.read();
    Serial1.println(c);
  }
 
  Serial.println(GPS.fix);
  if (GPS.newNMEAreceived()) {
    GPS.parse(GPS.lastNMEA());

    if (millis() - timer > 2000 && GPS.fix) {
      timer = millis(); // reset the timer
      Serial.println(GPS.day);
      Serial.println(GPS.month);
      Serial.println(GPS.year);
    } 
  } else {
    Serial.println("[GPS] No data received");
    delay(300);
  }
}