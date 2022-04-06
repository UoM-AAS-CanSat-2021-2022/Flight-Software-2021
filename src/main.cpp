#include <Arduino.h>
#include "util/sout.hpp"
#include "sensor/camera.hpp"

#include <cctype>
#include <iomanip>

Camera camera { 0 };

void setup() {                
  // initialize the digital pins as output.
  Serial.begin(9600);

  // setup camera pins
  camera.setup_pins();
}

void loop() {
  static std::string buf;

  bool read_data = false;
  if (Serial.available()) {
    read_data = true;
    auto c = Serial.read();
    if (std::isalpha(c)) {
      buf.push_back(c);
      sout << "buf = " << std::quoted(buf) << std::endl;
    }
  }

  if (!read_data) return;

  if (buf == "start") {
    sout << "Starting video recording." << std::endl;
    camera.start_recording();
    buf.clear();
  } else if (buf == "stop") {
    sout << "Stopping video recording." << std::endl;
    camera.stop_recording();
    buf.clear();
  } else if (buf.size() > 5) {
    sout << "buf is garbage, flushing." << std::endl;
    buf.clear();
  }
}
