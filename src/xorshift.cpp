#include <cstdint>
#include "xorshift.hpp"

XorShift32::XorShift32() : state(42) {}

XorShift32::XorShift32(std::uint32_t seed) : state(42) {
	// cannot have zero state
	if (seed != 0) {
		state = seed;
	}
}

std::uint32_t XorShift32::next() {
	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;
	return state;
}

