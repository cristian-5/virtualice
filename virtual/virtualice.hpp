
#ifndef VIRTUALICE_HPP
#define VIRTUALICE_HPP

#include "../tools/ice.hpp"
#include "../tools/arr.hpp"
#include "../tools/stk.hpp"

enum op: u8 {
	_halt,
	_rest,
	_push_b,
	_push_w,
	_push_d,
	_push_q,
	_push_z,
	_push_o,
	_pop,
	_pop_n,
	_top,
	_cast_b,
	_cast_w,
	_cast_d,
	_cast_q,
	_add_i,
	_add_f,
	_sub_i,
	_sub_f,
	_mul_i,
	_mul_f,
	_div_i,
	_div_f,
	_mod_i,
	_mod_f,
	_inc_i,
	_inc_f,
	_dec_i,
	_dec_f,
	_xand,
	_xor,
	_and,
	_or,
	_not,
	_inv,
	_neg,
	_shift_l,
	_shift_r,
	_rotate_l,
	_rotate_r,
	_get_l,
	_get_g,
	_set_l,
	_set_g,
	_call,
	_kernel,
	_return,
	_fork,
	_join,
	_abort,
	_abort_t,
	_lock,
	_release,
	_wait_t,
	_wait_a,
	_wait_s,
	_throw,
	_catch,
	_jump,
	_jump_t,
	_jump_f,
	_jump_e,
	_jump_n,
	_jump_ge,
	_jump_le,
	_jump_g,
	_jump_l,
};

union val { u64 i; f64 f; };

class vm {

	private:

	// ========= MICRO OPTIMISATIONS =========
	// Equivalent to: "mov reg, dword ptr [p]"
	// where 'reg' is the right register size,
	// the compiler will always remove N_MASK.

	[[gnu::always_inline]]
	inline static val getQ(void * p);

	[[gnu::always_inline]]
	inline static val getD(void * p);

	[[gnu::always_inline]]
	inline static val getW(void * p);

	[[gnu::always_inline]]
	inline static val getB(void * p);

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
