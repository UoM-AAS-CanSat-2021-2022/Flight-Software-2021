#include <Arduino.h>
/*
The Spycam can work in two different modes- video and snapshot.
In general, the white trigger line is pulled high.
If trigger is pulsed low for a short amount of time (less than half a second) it will take a photo.
If trigger is pulsed low for a long amount of time (a second or longer) it will start/stops video recording. 
*/


int trig = 0;
int led = 1;

void pulse_low(int);

void setup() {                
  // initialize the digital pins as output.
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);         

  digitalWrite(led, HIGH);  
  digitalWrite(trig, HIGH); 
}

// Hold HIGH and trigger quick (<250ms) LOW to take a photo. Holding LOW and trigger HIGH starts/stops video recording

void loop() {
  /*
  digitalWrite(trig, LOW);   
  digitalWrite(led, LOW);
  
  delay(1000);               

  digitalWrite(trig, HIGH);    
  digitalWrite(led, HIGH);   

  //Delay between pictures
  delay(1000);
  */

  /*
  static char buf[100] = { 0 };
  static int i = 0;
  
  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\r' && c != '\n') {
      buf[i++] = c;
      buf[i] = '\0';
    }
    Serial.println(buf);
  }
    

  if (strcmp(buf, "start") == 0) {
    start_recording();
    i = 0;
  } else if (strcmp(buf, "end") == 0) {
    end_recording();
    i = 0;
  }
  */

  Serial.println("Starting video");
  pulse_low(600);
  delay(3000);
  Serial.println("Ending video.");
  pulse_low(600);
  delay(3000);
}

void pulse_low(int duration) {
  digitalWrite(trig, LOW);
  digitalWrite(led, LOW);

  delay(duration);
  
  digitalWrite(trig, HIGH);
  digitalWrite(led, HIGH);
}

void start_recording() {
  pulse_low(200);
}

void end_recording() {
  pulse_low(600);
}
