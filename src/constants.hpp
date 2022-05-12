#pragma once

#include <cstdint>

// stringification macros from https://gcc.gnu.org/onlinedocs/gcc-3.4.3/cpp/Stringification.html
#define XSTR(s) STR(s)
#define STR(s) #s

// list of compile time constants used in various parts of the code
#define TEAM_ID 1057
constexpr static std::uint16_t gcs_link_panid = TEAM_ID;
constexpr static std::uint16_t payload_link_panid = TEAM_ID + 5000;

// BMP sea level pressure constant
static constexpr double SEALEVELPRESSURE_HPA = 1013.25;

// xbee addresses
static constexpr std::uint16_t container_xbee_address = 0;
static constexpr std::uint16_t payload_xbee_address = 1;
static constexpr std::uint16_t gcs_xbee_address = 2;

// voltage divider values
static constexpr std::uint32_t ANALOG_READ_BITS = 12;
static constexpr std::uint32_t ANALOG_READ_MAX = (1 << ANALOG_READ_BITS) - 1;
static constexpr std::uint8_t VD_PIN = 23;
static constexpr double ADC_MAX_INPUT_V = 3.3;
static constexpr double VD_INPUT_V = 5.0;
static constexpr double VD_R1 = 5'000.0; // 120'000.0
static constexpr double VD_R2 = 10'000.0; // 820'000.0
