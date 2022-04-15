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
