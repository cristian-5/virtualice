
#ifndef VIRTUALICE_HPP
#define VIRTUALICE_HPP

#include <mutex>

#include "../tools/ice.hpp"
#include "../tools/arr.hpp"
#include "../tools/stk.hpp"
#include "../tools/pcg.hpp"

#define TYP static constexpr u8
#define OPC template <u8 T = 0> TYP

class typ {
	public:
	TYP b = 0;
	TYP w = 1;
	TYP d = 2;
	TYP q = 3;
	TYP z = 4;
	TYP o = 5;
	TYP r = 0;
	TYP l = 1;
	TYP n = 1;
	TYP i = 0;
	TYP f = 1;
};

class jmp {
	public:
	TYP z = 1;
	TYP nz = 2;
	TYP li = 3;
	TYP gi = 4;
	TYP ei = 5;
	TYP nei = 6;
	TYP lei = 7;
	TYP gei = 8;
	TYP lf = 9;
	TYP gf = 10;
	TYP ef = 11;
	TYP nef = 12;
	TYP lef = 13;
	TYP gef = 14;
	TYP ex = 15;
};

class cll {
	public:
	TYP l = 1;
	TYP k = 2;
};

class var {
	public:
	TYP g = 0;
	TYP l = 1;
	TYP a = 2;
};

class krn {
	public:

	TYP estream = 0x66;
	TYP ostream = 0x55;
	TYP istream = 0x44;

	TYP fork      = 0xF0;
	TYP join      = 0xEF;
	TYP sleep     = 0x57;
	TYP wait      = 0xA1;
	TYP lock      = 0x88;
	TYP release   = 0x33;

	TYP allocate   = 0xA7;
	TYP deallocate = 0xDA;
	TYP reallocate = 0x2A;
	TYP copy       = 0xC0;
	TYP load       = 0x70;
	TYP zeros      = 0x00;
	TYP fill       = 0xF1;
	TYP compare    = 0xC2;

	TYP debug  = 0xDE;
	TYP sign   = 0x51;
	TYP time   = 0x71;
	TYP seed   = 0x5E;
	TYP random = 0x3A;

	TYP factorial = 0xFA;

};

class op {

	public:

	OPC halt = 0x00;
	OPC rest = 0x01;

	OPC push = 0x02 + T; // 6
	OPC pop  = 0x08 + T; // 2
	OPC top  = 0x0A;
	OPC cast = 0x0B + T; // 4

	// padding of 0x00

	OPC raise = 0x0F;

	// padding of 0x00

	OPC add = 0x10 + T; // 2
	OPC sub = 0x12 + T; // 2
	OPC mul = 0x14 + T; // 2
	OPC div = 0x16 + T; // 2
	OPC mod = 0x18 + T; // 2
	OPC inc = 0x1A + T; // 2
	OPC dec = 0x1C + T; // 2

	// padding of 0x00

	OPC convert = 0x1E + T; // 2

	// padding of 0x00

	OPC b_and = 0x20;
	OPC b_or  = 0x21;
	OPC l_not = 0x22;
	OPC b_xor = 0x23;

	OPC invert     = 0x24;
	OPC complement = 0x25;

	OPC shift  = 0x26 + T; // 2
	OPC rotate = 0x28 + T; // 2

	// padding of 0x00

	OPC jump = 0x2A + T; // 16

	// padding of 0x00

	OPC call  = 0x3A + T; // 3
	OPC ret   = 0x3D;
	OPC arity = 0x3E;

	// padding of 0x00

	OPC negate = 0x3F;

	// padding of 0x00

	OPC get  = 0x40 + T; // 3
	OPC set  = 0x43 + T; // 3

};

#undef OPC
#undef TYP

union val { u64 i; f64 f; void * p; };

static_assert(sizeof(val) == 8, "Please adjust the (val) union size!");

struct cal {
	u64 lfp = 0; // last frame pointer
	u64 ret = 0; // return address
	u8  ari = 0; // function arity
};

class vm {

	private:

	// ========= MICRO OPCIMISATIONS =========
	// Equivalent to: "mov reg, dword ptr [p]"
	// where 'reg' is the right register size,
	// the compiler will always remove N_MASK.

	[[gnu::always_inline]]
	inline static u64 getQ(void * p);

	[[gnu::always_inline]]
	inline static u32 getD(void * p);

	[[gnu::always_inline]]
	inline static u16 getW(void * p);

	[[gnu::always_inline]]
	inline static u8  getB(void * p);

	static std::mutex critical;

	public:

	/*!
	 *    + == VM usage ========================= +
	 *    | The code arr<u8> must end with a halt |
	 *    | instruction. The control flow will be |
	 *    | passed to threads when encountering a |
	 *    | fork instruction.                     |
	 *    + ------------------------------------- +
	!*/
	
	static void run(arr<u8> code, u64 p = 0);

};

#endif
