
#ifndef VIRTUALICE_HPP
#define VIRTUALICE_HPP

#include <mutex>
#include <unordered_map>

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
	TYP t = 1;
	TYP f = 2;
};

class cmp {
	public:
	TYP e  = 0x0;
	TYP ne = 0x1;
	TYP l  = 0x2;
	TYP le = 0x3;
	TYP g  = 0x4;
	TYP ge = 0x5;
};

class cll {
	public:
	TYP k = 0;
	TYP c = 1;
	TYP l = 2;
};

class var {
	public:
	TYP bc = 0;
	TYP bd = 1;
	TYP bg = 2;
	TYP bs = 3;
	TYP wc = 4;
	TYP wd = 5;
	TYP wg = 6;
	TYP ws = 7;
};

class scp {
	public:
	TYP c = 0;
	TYP d = 1;
};

class krn {
	public:

	TYP estream = 0xE5;
	TYP ostream = 0x05;
	TYP istream = 0x15;

	TYP fork      = 0xF0;
	TYP join      = 0xEF;
	TYP sleep     = 0x57;
	TYP wait      = 0xA1;
	TYP lock      = 0x88;
	TYP release   = 0x33;

	TYP _u2s = 0xCA;
	TYP _i2s = 0xCB;
	TYP _s2u = 0xCC;
	TYP _s2i = 0xCD;
	TYP _f2s = 0xCE;
	TYP _s2f = 0xCF;

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

	// ==========================================

	OPC factorial = 0x0F;

	OPC add = 0x10 + T; // 2
	OPC sub = 0x12 + T; // 2
	OPC mul = 0x14 + T; // 2
	OPC div = 0x16 + T; // 2
	OPC mod = 0x18 + T; // 2
	OPC pow = 0x1A + T; // 2

	OPC increment = 0x1C;
	OPC decrement = 0x1D;

	// ==========================================

	OPC convert = 0x1E + T; // 2

	// ==========================================

	OPC b_and = 0x20;
	OPC b_or  = 0x21;
	OPC l_not = 0x22;
	OPC b_xor = 0x23;

	OPC invert     = 0x24;
	OPC complement = 0x25;

	OPC shift  = 0x26 + T; // 2
	OPC rotate = 0x28 + T; // 2

	// ==========================================

	OPC jump    = 0x2A + T; // 3

	// ==========================================

	OPC raise = 0x2D;
	OPC flag  = 0x2E;

	// ==========================================

	OPC swap = 0x2F;

	// === PADDING OF 10 INSTRUCTIONS ===========

	OPC compare_u = 0x3A + T; // 6, padding of 10
	OPC compare_s = 0x4A + T; // 6, padding of 10
	OPC compare_f = 0x5A + T; // 6, padding of 10

	// === PADDING OF 10 INSTRUCTIONS ===========

	OPC call  = 0x6A + T; // 3
	OPC ret   = 0x6D;

	// === PADDING OF 01 INSTRUCTIONS ===========

	OPC negate = 0x6F;

	// ==========================================

	OPC global = 0x70 + T; // 8
	OPC local  = 0x78 + T; // 8

	// padding of 9 instructions

	OPC scope  = 0x8C + T; // 2

	// ==========================================

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

	static mtx critical, global;

	static map<u16, val> globals;

	class scope {
		public:
		scope * parent = nullptr;
		map<u16, val> locals;
		scope() = default;
		scope(scope * p) { parent = p; };
		inline void add(u16 i, val v) {
			locals.insert({ i, v });
		}
		inline void del(u16 i) {
			auto search = locals.find(i);
			if (search != locals.end()) locals.erase(search);
			else if (parent) parent -> del(i);
			else {
				global.lock();
				globals.erase(i);
				global.unlock();
			}
		}
		inline val get(u16 i) {
			auto search = locals.find(i);
			if (search != locals.end()) return search -> second;
			if (parent) return parent -> get(i);
			global.lock();
			val v = globals[i];
			global.unlock();
			return v;
		}
		inline void set(u16 i, val v) {
			auto search = locals.find(i);
			if (search != locals.end()) locals[i] = v;
			else if (parent) parent -> set(i, v);
			else {
				global.lock();
				globals[i] = v;
				global.unlock();
			}
		}
	};

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
