#pragma once

#include <EEPROM.h>
#include "eeprom/crc.hpp"
#include <cstdint>
#include <cstring>
#include <optional>

namespace eeprom {
	// load an object from a given address in the EEPROM
	// returns nullopt if the 4 byte checksum after the value isn't correct
	template<typename T>
	std::optional<T> load(const std::uint32_t base_addr) {
		T obj;
		EEPROM.get(base_addr, obj);

		std::uint32_t checksum;
		EEPROM.get(base_addr + sizeof(T), checksum);

		bool valid = crc(obj) == checksum;

		return valid ? std::optional<T>{ obj } : std::nullopt;
	}

	// store an object at the given address in EEPROM, and it's checksum in the 4 bytes following
	template<typename T>
	void store(const T& obj, const uint32_t base_addr) {
		EEPROM.put(base_addr, obj);
        auto checksum = crc(obj);
        EEPROM.put(base_addr + sizeof(T), checksum);
	}

    namespace __detail {
        // updates a value in memory
        template<typename T>
        void update_value(const T& obj, const uint32_t base_addr) {
            std::uint8_t bytearray[sizeof(T)];
            memcpy(bytearray, static_cast<const void*>(&obj), sizeof(T));
            for (std::uint32_t i = 0; i < sizeof(T); ++i) {
                EEPROM[base_addr + i] = bytearray[i];
            }
        }
    }

	// update the value of an object at the given address in EEPROM, and it's checksum
	template<typename T>
	void update(const T& obj, const uint32_t base_addr) {
        __detail::update_value(obj, base_addr);
        auto checksum = crc(obj);
        __detail::update_value(checksum, base_addr + sizeof(T));
	}
}
