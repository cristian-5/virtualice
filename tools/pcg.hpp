
#ifndef PCG_PURE
#define PCG_PURE

#include "ice.hpp"

// PCG Based on Doctor Melissa E. O'Neill paper.

static u32 pcg32() {
	static u64 state, inc;
	u64 oldstate = state;
	// Advance internal state (at least by 1 if inc == 0).
	state = oldstate * 6364136223846793005ull + (inc | 1);
	// Calculate output function xshrr (XOR SHIFT RIGHT RIGHT)
	// Using old state for maximum ilp (Instruction Level Parallelism)
	u32 xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
	u32 rot = oldstate >> 59u;
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static u64 pcg64() {
	return ((u64)pcg32() << 32) | (u64)pcg32();
}

#endif
