#pragma once

#include <array>
#include <cstdint>
#include <cstring>

namespace eeprom {
	constexpr static std::array<std::uint32_t, 16> crc_table = {
		0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
		0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
		0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
		0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
	};

	// adapted from https://www.arduino.cc/en/Tutorial/EEPROMCrc
	template<typename T>
	std::uint32_t crc(const T& data) {
		std::uint32_t crc = ~0L;

		std::uint8_t bytearray[sizeof(T)];
		memcpy(bytearray, static_cast<const void*>(&data), sizeof(T));

		for (auto&& byte : bytearray) {
			crc = crc_table[(crc ^ byte) & 0x0f] ^ (crc >> 4);
			crc = crc_table[(crc ^ (byte >> 4)) & 0x0f] ^ (crc >> 4);
			crc = ~crc;
		}

		return crc;
	}
}
