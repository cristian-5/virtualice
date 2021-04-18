
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

#define Q_MASK 0xFFFFFFFFFFFFFFFFull
#define D_MASK 0x00000000FFFFFFFFull
#define W_MASK 0x000000000000FFFFull
#define B_MASK 0x00000000000000FFull

#define V_CAST(V, T) { .i = V.i & T##_MASK }

class vm {

	private:

	// ========= MICRO OPTIMISATIONS =========
	// Equivalent to: "mov reg, dword ptr [p]"
	// where 'reg' is the right register size,
	// the compiler will always remove N_MASK.

	[[gnu::always_inline]]
	inline static val getQ(void * p) {
		return { .i = * static_cast<u64 *>(p) };
	}

	[[gnu::always_inline]]
	inline static val getD(void * p) {
		return { .i = (* static_cast<u32 *>(p)) & D_MASK };
	}

	[[gnu::always_inline]]
	inline static val getW(void * p) {
		return { .i = (* static_cast<u16 *>(p)) & W_MASK };
	}

	[[gnu::always_inline]]
	inline static val getB(void * p) {
		return { .i = (* static_cast<u8 *>(p)) & B_MASK };
	}

	public:

	/*!
	 *    + == VM usage ========================= +
	 *    | The code arr<u8> must end with a halt |
	 *    | instruction. The control flow will be |
	 *    | passed to threads when encountering a |
	 *    | fork instruction.                     |
	 *    + ------------------------------------- +
	!*/
	
	void run(arr<u8> code) {
		u8 * i = code.data;
		stk<val> stack;
		val a, b;
		while (true) {
			switch (* i) {
				case op::_halt: return;
				case op::_rest: break;
				case op::_push_b: stack.push(getB(++i)); break;
				case op::_push_w: stack.push(getW(++i)); break;
				case op::_push_d: stack.push(getD(++i)); break;
				case op::_push_q: stack.push(getQ(++i)); break;
				case op::_push_z: stack.push({ .i = 0ull }); break;
				case op::_push_o: stack.push({ .i = 1ull }); break;
				case    op::_pop: stack.decrease(); break;
				case  op::_pop_n: stack.decrease(getQ(++i).i); break;
				case    op::_top: stack.push(stack.top()); break;
				case op::_cast_b: stack.push(V_CAST(stack.pop(), B));
				case op::_cast_w: stack.push(V_CAST(stack.pop(), W));
				case op::_cast_d: stack.push(V_CAST(stack.pop(), D));
				case op::_cast_q: stack.push(V_CAST(stack.pop(), Q));
				case  op::_add_i:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = a.i + b.i });
				break;
				case  op::_add_f:
					b = stack.pop(); a = stack.pop();
					stack.push({ .f = a.f + b.f });
				break;
				case  op::_sub_i:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = a.i - b.i });
				break;
				case  op::_sub_f:
					b = stack.pop(); a = stack.pop();
					stack.push({ .f = a.f - b.f });
				break;
				case  op::_mul_i:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = a.i * b.i });
				break;
				case  op::_mul_f:
					b = stack.pop(); a = stack.pop();
					stack.push({ .f = a.f * b.f });
				break;
				case  op::_div_i:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = a.i / b.i });
				break;
				case  op::_div_f:
					b = stack.pop(); a = stack.pop();
					stack.push({ .f = a.f / b.f });
				break;
				case  op::_mod_i:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = a.i % b.i });
				break;
				case  op::_mod_f:
					b = stack.pop(); a = stack.pop();
					stack.push({ .f = fmod(a.f, b.f) });
				break;
				case  op::_inc_i:
					a = stack.pop();
					stack.push({ .i = a.i + 1 });
				break;
				case  op::_inc_f:
					a = stack.pop();
					stack.push({ .f = a.f + 1 });
				break;
				case  op::_dec_i:
					a = stack.pop();
					stack.push({ .i = a.i - 1 });
				break;
				case  op::_dec_f:
					a = stack.pop();
					stack.push({ .f = a.f - 1 });
				break;
				case  op::_xand:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = ~ (a.i ^ b.i) });
				break;
				case  op::_xor:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = (a.i ^ b.i) });
				break;
				case  op::_and:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = (a.i & b.i) });
				break;
				case  op::_or:
					b = stack.pop(); a = stack.pop();
					stack.push({ .i = (a.i | b.i) });
				break;
				case  op::_not:
					a = stack.pop();
					stack.push({ .i = ! a.i });
				break;
				case  op::_inv:
					a = stack.pop();
					stack.push({ .i = ~ a.i });
				break;
				case  op::_neg:
					a = stack.pop();
					stack.push({ .i = (~ a.i) + 1});
				break;
			}
			++i;
		}
	}

};

#undef Q_MASK
#undef D_MASK
#undef W_MASK
#undef B_MASK

#endif
