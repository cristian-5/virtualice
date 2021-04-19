
#ifndef VIRTUALICE_CPP
#define VIRTUALICE_CPP

#include "virtualice.hpp"

#define Q_MASK 0xFFFFFFFFFFFFFFFFull
#define D_MASK 0x00000000FFFFFFFFull
#define W_MASK 0x000000000000FFFFull
#define B_MASK 0x00000000000000FFull

#define V_CAST(V, T) { .i = V.i & T##_MASK }

#define POP_A  a = stack.pop();
#define POP_BA b = stack.pop(); a = stack.pop();
#define POP_AB a = stack.pop(); b = stack.pop();

[[gnu::always_inline]]
val vm::getQ(void * p) {
	return { .i = * ((u64 *)p) };
}

[[gnu::always_inline]]
val vm::getD(void * p) {
	return { .i = (* ((u32 *)p)) & D_MASK };
}

[[gnu::always_inline]]
inline static val getW(void * p) {
	return { .i = (* ((u16 *)p)) & W_MASK };
}

[[gnu::always_inline]]
inline static val getB(void * p) {
	return { .i = (* ((u8 *)p)) & B_MASK };
}

void vm::run(arr<u8> code) {
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
			case  op::_add_i: POP_BA stack.push({ .i = a.i + b.i }); break;
			case  op::_add_f: POP_BA stack.push({ .f = a.f + b.f }); break;
			case  op::_sub_i: POP_BA stack.push({ .i = a.i - b.i }); break;
			case  op::_sub_f: POP_BA stack.push({ .f = a.f - b.f }); break;
			case  op::_mul_i: POP_BA stack.push({ .i = a.i * b.i }); break;
			case  op::_mul_f: POP_BA stack.push({ .f = a.f * b.f }); break;
			case  op::_div_i: POP_BA stack.push({ .i = a.i / b.i }); break;
			case  op::_div_f: POP_BA stack.push({ .f = a.f / b.f }); break;
			case  op::_mod_i: POP_BA stack.push({ .i = a.i % b.i }); break;
			case  op::_mod_f: POP_BA stack.push({ .f = fmod(a.f, b.f) }); break;
			case  op::_inc_i: POP_A  stack.push({ .i = a.i + 1 }); break;
			case  op::_inc_f: POP_A  stack.push({ .f = a.f + 1 }); break;
			case  op::_dec_i: POP_A  stack.push({ .i = a.i - 1 }); break;
			case  op::_dec_f: POP_A  stack.push({ .f = a.f - 1 }); break;
			case   op::_xand: POP_BA stack.push({ .i = ~ (a.i ^ b.i) }); break;
			case    op::_xor: POP_BA stack.push({ .i = (a.i ^ b.i) }); break;
			case    op::_and: POP_BA stack.push({ .i = (a.i & b.i) }); break;
			case     op::_or: POP_BA stack.push({ .i = (a.i | b.i) }); break;
			case    op::_not: POP_A  stack.push({ .i = ! a.i }); break;
			case    op::_inv: POP_A  stack.push({ .i = ~ a.i }); break;
			case    op::_neg: POP_A  stack.push({ .i = (~ a.i) + 1}); break;
		}
		++i;
	}
}

#undef Q_MASK
#undef D_MASK
#undef W_MASK
#undef B_MASK

#undef POP_A
#undef POP_BA
#undef POP_AB

#endif
