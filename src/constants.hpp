#pragma once

#include <cstdint>
#include <Arduino.h>
#include <Wire.h>
// prevent colliding symbol errors
#undef B1

// stringification macros from https://gcc.gnu.org/onlinedocs/gcc-3.4.3/cpp/Stringification.html
#define XSTR(s) STR(s)
#define STR(s) #s

// list of compile time constants used in various parts of the code
#define TEAM_ID 1057
static constexpr std::uint16_t GCS_LINK_PANID = TEAM_ID;
static constexpr std::uint16_t PAYLOAD_LINK_PANID = TEAM_ID + 5000;

// BMP sea level pressure constant
static constexpr double SEALEVEL_PRESSURE_HPA = 1013.25;
static constexpr double SEALEVEL_PRESSURE_PA  = SEALEVEL_PRESSURE_HPA * 100;

// xbee addresses
static constexpr std::uint16_t CONTAINER_XBEE_ADDRESS = 0;
static constexpr std::uint16_t PAYLOAD_XBEE_ADDRESS = 1;
static constexpr std::uint16_t GCS_XBEE_ADDRESS = 2;

// voltage divider values
static constexpr std::uint32_t ANALOG_READ_BITS = 12;
static constexpr std::uint32_t ANALOG_READ_MAX = (1 << ANALOG_READ_BITS) - 1;
static constexpr std::uint8_t VD_PIN = 23;
static constexpr double ADC_MAX_INPUT_V = 3.3;
static constexpr double VD_R1 = 680'000.0;
static constexpr double VD_R2 = 1'000'000.0;

// Serial interfaces and baud rates
static auto& XBEE_SERIAL = Serial1;
static auto& GPS_SERIAL = Serial5;
static constexpr long DEBUG_SERIAL_BAUD = 230400; 
static constexpr long XBEE_SERIAL_BAUD = 230400;
static constexpr long GPS_SERIAL_BAUD = 9600;

// I2C interfaces and addresses
static constexpr auto& BMP_WIRE = Wire;
static constexpr std::uint8_t BMP_ADDR = 119;

// other pins
static constexpr std::uint8_t BUZZER_PIN = 8;

// debuf flags
// #define DEBUG_GPS
// #define DEBUG_XBEE
// #define DEBUG_VD

// Servo pins
static constexpr std::uint8_t SERVO_PARACHUTE_PIN = 3;
static constexpr std::uint8_t SERVO_SPOOL_PIN = 4;
static constexpr std::uint8_t SERVO_CONTINUOUS_PIN = 5;
