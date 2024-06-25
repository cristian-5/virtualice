
#ifndef VIRTUALICE_HPP
#define VIRTUALICE_HPP

#include <mutex>
#include <unordered_map>

#include "../tools/ice.hpp"
#include "../tools/arr.hpp"
#include "../tools/stk.hpp"
#include "../tools/pcg.hpp"
#include "../tools/cpx.hpp"

#define TYP static constexpr u8
#define COP static constexpr u8     // constant operand opcode
#define VOP template <u8 T = 0> TYP // variable operand opcode

class dta {
	public:
	TYP b = 0;
	TYP w = 1;
	TYP d = 2;
	TYP q = 3;
};

class typ {
	public:
	TYP b = 0; // byt / bln
	TYP n = 0; // nat
	TYP i = 1; // int
	TYP r = 2; // rea
	TYP c = 3; // cpx
};

class krn {
	public:

	TYP estream = 0xE5;
	TYP ostream = 0x05;
	TYP edata   = 0xED;
	TYP odata   = 0x0D;
	TYP eflush  = 0xEF;
	TYP oflush  = 0x0F;
	TYP istream = 0x15;

	TYP fork      = 0xF0;
	TYP join      = 0x10;
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
	TYP time   = 0x71;
	TYP seed   = 0x5E;
	TYP random = 0x3A;

	TYP factorial = 0xFA;

};

class mem {
	public:
	TYP grow    = 0x50;
	TYP shrink  = 0x54;
	TYP size    = 0x51;
	TYP page    = 0x2A;
	TYP pages   = 0x25;
	TYP copy    = 0xC0;
	TYP load    = 0x70;
	TYP zeros   = 0x00;
	TYP fill    = 0xF1;
	TYP compare = 0xC2;
};

enum math: u8 {

	_i = 0,
	_e,
	_ln10,
	_ln2,
	_log10e,
	_log2e,
	_pi,
	_sqrt1_2,
	_sqrt2,
	_egamma,
	_phi,

	_abs,
	_acos,
	_acosh,
	_asin,
	_asinh,
	_atan,
	_atan2,
	_atanh,
	_cbrt,
	_ceil,
	_cos,
	_cosh,
	_exp,
	_expm1,
	_floor,
	_hypot,
	_log,
	_log1p,
	_log10,
	_log2,
	_max,
	_min,
	_round,
	_sign,
	_sin,
	_sinh,
	_sqrt,
	_tan,
	_tanh,
	_trunc,

};

class op {

	public:

	COP _halt_00 = 0x00;
	COP _rest    = 0x01;

	VOP _const   = 0x02 + T;
	COP _const_0 = 0x06;
	COP _const_1 = 0x07;

	COP _swap    = 0x08;

	COP _clone_n = 0x0A;
	COP _drop_n  = 0x0B;
	COP _clone   = 0x0C;
	COP _drop    = 0x0D;

	COP _fail    = 0x0E;
	COP _flag    = 0x0F;

	// ==========================================

	VOP _add = 0x10 + T;
	VOP _sub = 0x14 + T;
	VOP _mul = 0x18 + T;
	VOP _div = 0x1C + T;

	VOP _mod = 0x20 + T;
	VOP _pow = 0x24 + T;

	VOP _inc = 0x28 + T;
	VOP _dec = 0x2C + T;

	// ==========================================

	VOP _memory_l = 0x32 + T;
	VOP _memory_s = 0x36 + T;

	// ==========================================

	COP _shift    = 0x3A;
	COP _shift_r  = 0x3B;
	COP _shift_l  = 0x3C;
	COP _rotate   = 0x3D;
	COP _rotate_r = 0x3E;
	COP _rotate_l = 0x3F;

	VOP _mask     = 0x40 + T;

	COP _bit      = 0x44;
	COP _bit_0    = 0x45;
	COP _bit_1    = 0x46;

	COP _nibble_l = 47;
	COP _nibble_h = 48;
	COP _nibble_s = 49;

	// buffer = rest = nop
	COP _and      = 0x4A;
	COP _or       = 0x4B;
	COP _not      = 0x4C;
	COP _nor      = 0x4D;
	COP _nand     = 0x4E;
	COP _xor      = 0x4F;
	COP _xnor     = 0x50; // xand = xnor

	COP _invert     = 0x51;
	COP _reverse    = 0x52;
	COP _complement = 0x53;

	// ==========================================

	COP _jump_r  = 0x59;
	COP _jump_a  = 0x5A;
	COP _jump_z  = 0x5B;
	COP _jump_o  = 0x5C;
	COP _jump_nz = 0x5D;

	COP _jump_e  = 0x5E;
	COP _jump_ne = 0x5F;

	VOP _jump_l  = 0x60 + T;
	VOP _jump_le = 0x64 + T;
	VOP _jump_ge = 0x68 + T;
	VOP _jump_g  = 0x6C + T;

	COP _compare_e  = 0x70;
	COP _compare_ne = 0x71;

	VOP _compare_l  = 0x72 + T;
	VOP _compare_le = 0x76 + T;
	VOP _compare_ge = 0x80 + T;
	VOP _compare_g  = 0x84 + T;

	COP _is_nan = 0x88;
	COP _is_inf = 0x89;

	// ==========================================

	COP _global_r   = 0x8A;
	COP _global_w   = 0x8B;
	COP _global_e_r = 0x8C;
	COP _global_e_w = 0x8D;

	COP _local_r_   = 0x8E; // _ to distinguish from _local_r
	COP _local_w_   = 0x8F; // _ to distinguish from _local_w

	VOP _local_r    = 0x90 + T;
	VOP _local_w    = 0x98 + T;

	// ==========================================

	VOP _arity = 0xA0 + T;

	// ==========================================

	COP _param_r_ = 0xAE; // _ to distinguish from _param_r
	COP _param_w_ = 0xAF; // _ to distinguish from _param_w

	VOP _param_r  = 0xB0 + T;
	VOP _param_w  = 0xB8 + T;

	// ==========================================

	COP _call_k   = 0xC0;
	COP _call_l   = 0xC7;
	COP _call_e   = 0xC9; // extended call (3 bytes)
	COP _call     = 0xCA; // 2 byte call

	COP _return   = 0xCB;
	COP _return_v = 0xCC;

	// ==========================================

	COP _convert_n2r = 0xCD;
	COP _convert_i2r = 0xCE;
	COP _convert_r2i = 0xCF;

	// ==========================================

	VOP _math = 0xCF + T;

	// ==========================================

	COP _imaginary = 0xF8;
	COP _combine   = 0xF9;
	COP _project   = 0xFA;
	COP _project_r = 0xFB;
	COP _project_i = 0xFC;
	COP _magnitude = 0xFD;
	COP _conjugate = 0xFE;

	// ==========================================

	COP _halt_FF = 0xFF;

};

#undef VOP
#undef TYP

union val { u64 n; i64 i; f64 r; cpx c; void * p; };

static_assert(sizeof(val) == 8, "Please adjust the (val) union size!");

class vm {

	private:

	[[gnu::always_inline]]
	inline static u64 loadQ(void * p);
	[[gnu::always_inline]]
	inline static u32 loadD(void * p);
	[[gnu::always_inline]]
	inline static u16 loadW(void * p);
	[[gnu::always_inline]]
	inline static u8  loadB(void * p);

	[[gnu::always_inline]]
	inline static void storeQ(void * p, u64 v);
	[[gnu::always_inline]]
	inline static void storeD(void * p, u32 v);
	[[gnu::always_inline]]
	inline static void storeW(void * p, u16 v);
	[[gnu::always_inline]]
	inline static void storeB(void * p, u8  v);

	static mtx critical, global;

	static arr<u8> memory;

	public:

	///  + == VM usage ========================= +
	///  | The code arr<u8> must end with a halt |
	///  | instruction. The control flow will be |
	///  | passed to threads when encountering a |
	///  | fork instruction.                     |
	///  + ------------------------------------- +
	static void run(arr<u8> code);

};

#endif
