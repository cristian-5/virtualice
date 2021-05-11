
#ifndef PCG_PURE
#define PCG_PURE

#include "ice.hpp"

// PCG Based on Professor's Melissa E. O'Neill research.

class pcg {

	private:

		static const u64 n = 6364136223846793005;
		static const u64 inc = 0xDA3E39CB94B95BDB;
		inline static u64 state = 0x853C49E6748FEA9B;
		static const u64 s = 0xC20580B23E7C23FB;

	public:

	static u64 next() {
		u64 old = state;
		state = old * n + inc;
		u64 shifted = ((old >> 18) ^ old) >> 27;
		u64 rot = old >> 59;
		return ((shifted >> rot) | (shifted << ((~rot + 1) & 0x1F))) ^ s;
	}

	static void seed(u64 s) {
		state = 0x8788A81453C0274A ^ s;
	}

};

#endif
