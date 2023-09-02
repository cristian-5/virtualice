
#ifndef PCG_PURE
#define PCG_PURE

#include "ice.hpp"

// PCG Based on Ph.D. Professor Melissa E. O'Neill research.

class pcg {

	private:

		static const u64 n = 6364237223846893005;
		static const u64 inc = 0xDA3E39AB94B95BDB;
		inline static u64 state = 0x953C49E3748FEA9B;
		static const u64 s = 0xD20580F23E7CE3FB;

	public:

	static u64 next() {
		u64 old = state;
		state = old * n + inc;
		u64 shifted = ((old >> 18) ^ old) >> 27;
		u64 rot = old >> 59;
		return ((shifted >> rot) | (shifted << ((~rot + 1) & 0x1F))) ^ s;
	}

	static void seed(u64 s) {
		state = 0x878BA8145FC0274A ^ s;
	}

};

#endif
