#pragma once

#include <cstdint>

class XorShift32 {
	std::uint32_t state;

public:
	XorShift32();
	XorShift32(std::uint32_t);

	std::uint32_t next();
};

