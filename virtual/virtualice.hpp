
#ifndef VIRTUALICE_HPP
#define VIRTUALICE_HPP

#include "../tools/ice.hpp"
#include "../tools/arr.hpp"
#include "../tools/stk.hpp"

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

class op {

	public:

	OPC halt = 0x00;
	OPC rest = 0x01;

	OPC push = 0x02 + T; // 6
	OPC pop  = 0x08 + T; // 2
	OPC top  = 0x0A;
	OPC cast = 0x0B + T; // 4

	// padding of 0x02

	OPC add = 0x10 + T; // 2
	OPC sub = 0x12 + T; // 2
	OPC mul = 0x14 + T; // 2
	OPC div = 0x16 + T; // 2
	OPC mod = 0x18 + T; // 2
	OPC inc = 0x1A + T; // 2
	OPC dec = 0x1C + T; // 2

	// padding of 0x03

	OPC b_and = 0x20;
	OPC b_or  = 0x21;
	OPC l_not = 0x22;
	OPC b_xor = 0x23;

	OPC invert = 0x24;
	OPC negate = 0x25;

	OPC shift  = 0x26 + T; // 2
	OPC rotate = 0x28 + T; // 2

};

#undef OPC
#undef TYP

union val { u64 i; f64 f; };

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

	public:

	/*!
	 *    + == VM usage ========================= +
	 *    | The code arr<u8> must end with a halt |
	 *    | instruction. The control flow will be |
	 *    | passed to threads when encountering a |
	 *    | fork instruction.                     |
	 *    + ------------------------------------- +
	!*/
	
	static void run(arr<u8> code);

};

#endif
