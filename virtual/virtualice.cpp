
#ifndef VIRTUALICE_CPP
#define VIRTUALICE_CPP

#include <chrono>
#include <thread>
#include <numbers>
#include <iostream>

using namespace std;

#include "virtualice.hpp"

#define Q_MASK 0xFFFFFFFFFFFFFFFFull
#define D_MASK 0x00000000FFFFFFFFull
#define W_MASK 0x000000000000FFFFull
#define B_MASK 0x00000000000000FFull
#define NL_MASK 0x0Full // low  4 bits (nibble)
#define NH_MASK 0xF0ull // high 4 bits (nibble)

#define V_CAST(V, T) { .n = V.n & T##_MASK }

#define POP_A  a = stack.pop();
#define POP_BA b = stack.pop(); a = stack.pop();
#define POP_AB a = stack.pop(); b = stack.pop();

#define PUSH stack.push

#define PUSH_A stack.push(a);
#define PUSH_B stack.push(b);
#define PUSH_BA stack.push(b); stack.push(a);
#define PUSH_AB stack.push(a); stack.push(b);

#define DECREASE stack.decrease(stack.pop().i);

#define CMP_I(_) if (a.i _ b.i)
#define CMP_F(_) if (a.f _ b.f)

#define UPDATE_I(T) i = & code.data[get##T(++i)]; continue
#define SKIP(N) i += N
#define SKIP_NEXT(N) i += N + 1; continue

#define ARITY (stack[fp].n >> 56)
#define OFP   (stack[fp].n >> 32 & 0x00FFFFFF) // 24 bits only
#define RA    (stack[fp].n & D_MASK)

#define PU8 const u8 * n = (u8 *)p;

[[gnu::always_inline]]
u64 vm::getQ(void * p) {
	if constexpr (little_endian) {
		PU8 return (
			(u64)n[0] << 56 | (u64)n[1] << 48 |
			(u64)n[2] << 40 | (u64)n[3] << 32 |
			(u64)n[4] << 24 | (u64)n[5] << 16 |
			(u64)n[6] << 8  | (u64)n[7]
		);
	} else return (* ((u64 *)p));
}
[[gnu::always_inline]]
u32 vm::getD(void * p) {
	if constexpr (little_endian) {
		PU8 return (n[0] << 24 | n[1] << 16 | n[2] << 8 | n[3]) & D_MASK;
	} else return (* ((u32 *)p)) & D_MASK;
}
[[gnu::always_inline]]
u16 vm::getW(void * p) {
	if constexpr (little_endian) {
		PU8 return (n[0] << 8 | n[1]) & W_MASK;
	} else return (* ((u16 *)p)) & W_MASK;
}
[[gnu::always_inline]]
u8  vm::getB(void * p) { return (* ((u8  *)p)) & B_MASK; }

// BUG: this does not work!
[[gnu::always_inline]]
void vm::setQ(void * p, u64 v) {
    if constexpr (little_endian) {
        (* ((u64 *)p)) = (v << 56) |
			((v << 40) & 0x00FF000000000000) |
			((v << 24) & 0x0000FF0000000000) |
			((v << 8)  & 0x000000FF00000000) |
			((v >> 8)  & 0x00000000FF000000) |
			((v >> 24) & 0x0000000000FF0000) |
			((v >> 40) & 0x000000000000FF00) |
			 (v >> 56);
    } else (* ((u64 *)p)) = v;
}
// BUG: this does not work!
[[gnu::always_inline]]
void vm::setD(void * p, u32 v) {
    if constexpr (little_endian) {
        (* ((u32 *)p)) = (v << 24) |
			((v << 16) & 0x00FF0000) |
			((v << 8)  & 0x0000FF00) |
			 (v >> 24);
    } else (* ((u32 *)p)) = v;
}
[[gnu::always_inline]]
void vm::setW(void * p, u16 v) {
	if constexpr (little_endian) {
		(* ((u16 *)p)) = (v << 8 | v >> 8) & W_MASK;
	} else (* ((u16 *)p)) = v;
}
[[gnu::always_inline]]
void vm::setB(void * p, u8 v) { (* ((u8  *)p)) = v; }

#undef PU8

mtx vm::critical;
mtx vm::global;

arr<u8> vm::memory;

void vm::run(arr<u8> code) {
	u8 * i = code.data;
	// check for magic number:
	if (code.count < 4 || (getD(i) & 0xFFFFFF00) != 0x69636500) return;
	SKIP(4); // skip magic number
	stk<val> stack;
	u64 fp = 0;
	val a, b;
	// pcg::seed((u64)time(nullptr));
	while (true) {
		switch (* i) {
			case op::_halt_00: exit(0);
			case op::_rest: break;
			case op::_const<dta::b>: PUSH({ .n = getB(++i) }); break;
			case op::_const<dta::w>: PUSH({ .n = getW(++i) }); SKIP(2); continue;
			case op::_const<dta::d>: PUSH({ .n = getD(++i) }); SKIP(4); continue;
			case op::_const<dta::q>: PUSH({ .n = getQ(++i) }); SKIP(8); continue;
			case op::_const_0: PUSH({ .n = 0ull }); break;
			case op::_const_1: PUSH({ .n = 1ull }); break;
			case op::_swap: POP_AB PUSH_AB break;
			case op::_clone_n: for (u64 j = getB(++i) + 1; j > 0; j--) PUSH(stack.top()); break;
			case  op::_drop_n: stack.decreaseBy(getB(++i)); break;
			case   op::_clone: PUSH(stack.top()); break;
			case    op::_drop: stack.decrease(); break;
			case op::_add<typ::n>: POP_BA PUSH({ .n = a.n + b.n }); break;
			case op::_add<typ::i>: POP_BA PUSH({ .i = a.i + b.i }); break;
			case op::_add<typ::r>: POP_BA PUSH({ .r = a.r + b.r }); break;
			case op::_add<typ::c>: POP_BA PUSH({ .c = a.c + b.c }); break;
			case op::_sub<typ::n>: POP_BA PUSH({ .n = a.n - b.n }); break;
			case op::_sub<typ::i>: POP_BA PUSH({ .i = a.i - b.i }); break;
			case op::_sub<typ::r>: POP_BA PUSH({ .r = a.r - b.r }); break;
			case op::_sub<typ::c>: POP_BA PUSH({ .c = a.c - b.c }); break;
			case op::_mul<typ::n>: POP_BA PUSH({ .n = a.n * b.n }); break;
			case op::_mul<typ::i>: POP_BA PUSH({ .i = a.i * b.i }); break;
			case op::_mul<typ::r>: POP_BA PUSH({ .r = a.r * b.r }); break;
			case op::_mul<typ::c>: POP_BA PUSH({ .c = a.c * b.c }); break;
			case op::_div<typ::n>: POP_BA PUSH({ .n = a.n / b.n }); break;
			case op::_div<typ::i>: POP_BA PUSH({ .i = a.i / b.i }); break;
			case op::_div<typ::r>: POP_BA PUSH({ .r = a.r / b.r }); break;
			case op::_div<typ::c>: POP_BA PUSH({ .c = a.c / b.c }); break;
			case op::_mod<typ::n>: POP_BA PUSH({ .n = a.n % b.n }); break;
			case op::_mod<typ::i>: POP_BA PUSH({ .i = a.i % b.i }); break;
			case op::_mod<typ::r>: POP_BA PUSH({ .r = fmod(a.r, b.r) }); break;
			// op::_mod<typ::c> is not defined
			case op::_pow<typ::n>: POP_BA PUSH({ .n = power_u64(a.n, b.n) }); break;
			case op::_pow<typ::i>: POP_BA PUSH({ .i = (i64)pow(a.i, b.i) }); break;
			case op::_pow<typ::r>: POP_BA PUSH({ .r = pow(a.r, b.r) }); break;
			// op::_pow<typ::c> is not defined
			case op::_inc<typ::n>: POP_A PUSH({ .n = a.n + 1 }); break;
			case op::_inc<typ::i>: POP_A PUSH({ .i = a.i + 1 }); break;
			case op::_inc<typ::r>: POP_A PUSH({ .r = a.r + 1.0 }); break;
			// op::_inc<typ::c> is not defined
			case op::_dec<typ::n>: POP_A PUSH({ .n = a.n - 1 }); break;
			case op::_dec<typ::i>: POP_A PUSH({ .i = a.i - 1 }); break;
			case op::_dec<typ::r>: POP_A PUSH({ .r = a.r - 1.0 }); break;
			// op::_dec<typ::c> is not defined
			case op::_memory_l<dta::b>: PUSH({ .n = getB(memory.data + getD(++i)) }); SKIP(4); continue;
			case op::_memory_l<dta::w>: PUSH({ .n = getW(memory.data + getD(++i)) }); SKIP(4); continue;
			case op::_memory_l<dta::d>: PUSH({ .n = getD(memory.data + getD(++i)) }); SKIP(4); continue;
			case op::_memory_l<dta::q>: PUSH({ .n = getQ(memory.data + getD(++i)) }); SKIP(4); continue;
			case op::_memory_s<dta::b>: POP_A setB(memory.data + getD(++i), a.n); SKIP(4); continue;
			case op::_memory_s<dta::w>: POP_A setW(memory.data + getD(++i), a.n); SKIP(4); continue;
			case op::_memory_s<dta::d>: POP_A setD(memory.data + getD(++i), a.n); SKIP(4); continue;
			case op::_memory_s<dta::q>: POP_A setQ(memory.data + getD(++i), a.n); SKIP(4); continue;
			case op::_magnitude: POP_A PUSH({ .r = absolute(a.c) }); break;
			case op::_conjugate: POP_A PUSH({ .c = ~ a.c }); break;
			case op::_combine: POP_BA PUSH({ .c = { (f32)a.r, (f32)b.r } }); break;
			case op::_project:
				POP_A
				PUSH({ .r = (f64)a.c.r });
				PUSH({ .r = (f64)a.c.i });
			break;
			case op::_project_r: POP_A PUSH({ .r = (f64)a.c.r }); break;
			case op::_project_i: POP_A PUSH({ .r = (f64)a.c.i }); break;
			case op::_mask<dta::b>: PUSH(V_CAST(stack.pop(), B)); break;
			case op::_mask<dta::w>: PUSH(V_CAST(stack.pop(), W)); break;
			case op::_mask<dta::d>: PUSH(V_CAST(stack.pop(), D)); break;
			case op::_mask<dta::q>: PUSH(V_CAST(stack.pop(), Q)); break;
			case   op::_bit: POP_A PUSH({ .n = get_bit(a.n, getB(++i)) }); break;
			case op::_bit_0: POP_A PUSH({ .n = reset_bit(a.n, getB(++i)) }); break;
			case op::_bit_1: POP_A PUSH({ .n = set_bit(a.n, getB(++i)) }); break;
			case op::_nibble_l: POP_A PUSH({ .n =  a.n & NL_MASK }); break;
			case op::_nibble_h: POP_A PUSH({ .n = (a.n & NH_MASK) >> 4 }); break;
			case op::_nibble_s: POP_A PUSH({ .n = ((a.n & NL_MASK) << 4) | ((a.n & NL_MASK) >> 4) }); break;
			case  op::_and: POP_BA PUSH({ .n = (a.n & b.n) }); break;
			case   op::_or: POP_BA PUSH({ .n = (a.n | b.n) }); break;
			case  op::_not: POP_A  PUSH({ .n = ! a.n }); break;
			case  op::_nor: POP_BA PUSH({ .n = !(a.n | b.n) }); break;
			case op::_nand: POP_BA PUSH({ .n = !(a.n & b.n) }); break;
			case  op::_xor: POP_BA PUSH({ .n = (a.n ^ b.n) }); break;
			case op::_xnor: POP_BA PUSH({ .n = !(a.n ^ b.n) }); break;
			case     op::_invert: POP_A PUSH({ .n = ~ a.n }); break;
			case op::_complement: POP_A PUSH({ .i = - a.i }); break;
			case    op::_reverse: POP_A PUSH({ .n = reverse(a.n) }); break;
			case   op::_rotate_r: POP_A PUSH({ .n = rotateR(a.n, getB(++i)) }); break;
			case   op::_rotate_l: POP_A PUSH({ .n = rotateL(a.n, getB(++i)) }); break;
			case    op::_shift_r: POP_A PUSH({ .n = a.n >> getB(++i) }); break;
			case    op::_shift_l: POP_A PUSH({ .n = a.n << getB(++i) }); break;
			case    op::_jump: UPDATE_I(D); continue;
			case  op::_jump_z: POP_A if (a.i == 0) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_jump_o: POP_A if (a.i == 1) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case op::_jump_nz: POP_A if (a.i != 0) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_jump_e: POP_BA if (a.n == b.n) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case op::_jump_ne: POP_BA if (a.n != b.n) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_jump_l<typ::n>: POP_BA if (a.n <  b.n) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_jump_l<typ::i>: POP_BA if (a.i <  b.i) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_jump_l<typ::r>: POP_BA if (a.r <  b.r) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case op::_jump_le<typ::n>: POP_BA if (a.n <= b.n) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case op::_jump_le<typ::i>: POP_BA if (a.i <= b.i) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case op::_jump_le<typ::r>: POP_BA if (a.r <= b.r) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case op::_jump_ge<typ::n>: POP_BA if (a.n >= b.n) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case op::_jump_ge<typ::i>: POP_BA if (a.i >= b.i) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case op::_jump_ge<typ::r>: POP_BA if (a.r >= b.r) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_jump_g<typ::n>: POP_BA if (a.n >  b.n) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_jump_g<typ::i>: POP_BA if (a.i >  b.i) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_jump_g<typ::r>: POP_BA if (a.r >  b.r) { UPDATE_I(D); } else SKIP_NEXT(4); continue;
			case  op::_compare_e: POP_BA PUSH({ .n = (a.n == b.n) }); break;
			case op::_compare_ne: POP_BA PUSH({ .n = (a.n != b.n) }); break;
			case  op::_compare_l<typ::n>: POP_BA PUSH({ .n = (a.n <  b.n) }); break;
			case  op::_compare_l<typ::i>: POP_BA PUSH({ .i = (a.i <  b.i) }); break;
			case  op::_compare_l<typ::r>: POP_BA PUSH({ .i = (a.r <  b.r) }); break;
			case op::_compare_le<typ::n>: POP_BA PUSH({ .n = (a.n <= b.n) }); break;
			case op::_compare_le<typ::i>: POP_BA PUSH({ .i = (a.i <= b.i) }); break;
			case op::_compare_le<typ::r>: POP_BA PUSH({ .i = (a.r <= b.r) }); break;
			case op::_compare_ge<typ::n>: POP_BA PUSH({ .n = (a.n >= b.n) }); break;
			case op::_compare_ge<typ::i>: POP_BA PUSH({ .i = (a.i >= b.i) }); break;
			case op::_compare_ge<typ::r>: POP_BA PUSH({ .i = (a.r >= b.r) }); break;
			case  op::_compare_g<typ::n>: POP_BA PUSH({ .n = (a.n >  b.n) }); break;
			case  op::_compare_g<typ::i>: POP_BA PUSH({ .i = (a.i >  b.i) }); break;
			case  op::_compare_g<typ::r>: POP_BA PUSH({ .i = (a.r >  b.r) }); break;
			case op::_isnan: POP_A PUSH({ .n = isnan(a.r) }); break;
			case op::_isinf: POP_A PUSH({ .n = isinf(a.r) }); break;
			case op::_global_r: PUSH(stack[getB(++i)]); break;
			case op::_global_w: stack[getB(++i)] = stack.pop(); break;
			case op::_global_e_r: PUSH(stack[getW(++i)]); SKIP(2); continue;
			case op::_global_e_w: stack[getW(++i)] = stack.pop(); SKIP(2); continue;
			case op::_local_r_: PUSH(stack[fp + 1 + getB(++i)]); break;
			case op::_local_w_: stack[fp + 1 + getB(++i)] = stack.pop(); break;
			case op::_local_r<0>: PUSH(stack[fp + 1]); break;
			case op::_local_r<1>: PUSH(stack[fp + 2]); break;
			case op::_local_r<2>: PUSH(stack[fp + 3]); break;
			case op::_local_r<3>: PUSH(stack[fp + 4]); break;
			case op::_local_r<4>: PUSH(stack[fp + 5]); break;
			case op::_local_r<5>: PUSH(stack[fp + 6]); break;
			case op::_local_r<6>: PUSH(stack[fp + 7]); break;
			case op::_local_r<7>: PUSH(stack[fp + 8]); break;
			case op::_local_w<0>: stack[fp + 1] = stack.pop(); break;
			case op::_local_w<1>: stack[fp + 2] = stack.pop(); break;
			case op::_local_w<2>: stack[fp + 3] = stack.pop(); break;
			case op::_local_w<3>: stack[fp + 4] = stack.pop(); break;
			case op::_local_w<4>: stack[fp + 5] = stack.pop(); break;
			case op::_local_w<5>: stack[fp + 6] = stack.pop(); break;
			case op::_local_w<6>: stack[fp + 7] = stack.pop(); break;
			case op::_local_w<7>: stack[fp + 8] = stack.pop(); break;
			case    op::_arity<>: stack[fp].n |= (u64)getB(++i) << 56; break;
			case op::_arity<0x1>: stack[fp].n |= 0x1ull << 56; break;
			case op::_arity<0x2>: stack[fp].n |= 0x2ull << 56; break;
			case op::_arity<0x3>: stack[fp].n |= 0x3ull << 56; break;
			case op::_arity<0x4>: stack[fp].n |= 0x4ull << 56; break;
			case op::_arity<0x5>: stack[fp].n |= 0x5ull << 56; break;
			case op::_arity<0x6>: stack[fp].n |= 0x6ull << 56; break;
			case op::_arity<0x7>: stack[fp].n |= 0x7ull << 56; break;
			case op::_arity<0x8>: stack[fp].n |= 0x8ull << 56; break;
			case op::_param_r_: PUSH(stack[fp - 1 - getB(++i)]); break;
			case op::_param_w_: stack[fp - 1 - getB(++i)] = stack.pop(); break;
			case op::_param_r<0>: PUSH(stack[fp - 1]); break;
			case op::_param_r<1>: PUSH(stack[fp - 2]); break;
			case op::_param_r<2>: PUSH(stack[fp - 3]); break;
			case op::_param_r<3>: PUSH(stack[fp - 4]); break;
			case op::_param_r<4>: PUSH(stack[fp - 5]); break;
			case op::_param_r<5>: PUSH(stack[fp - 6]); break;
			case op::_param_r<6>: PUSH(stack[fp - 7]); break;
			case op::_param_r<7>: PUSH(stack[fp - 8]); break;
			case op::_param_w<0>: stack[fp - 1] = stack.pop(); break;
			case op::_param_w<1>: stack[fp - 2] = stack.pop(); break;
			case op::_param_w<2>: stack[fp - 3] = stack.pop(); break;
			case op::_param_w<3>: stack[fp - 4] = stack.pop(); break;
			case op::_param_w<4>: stack[fp - 5] = stack.pop(); break;
			case op::_param_w<5>: stack[fp - 6] = stack.pop(); break;
			case op::_param_w<6>: stack[fp - 7] = stack.pop(); break;
			case op::_param_w<7>: stack[fp - 8] = stack.pop(); break;
			case op::_return: // return value:
				// this is only safe because decreasing the stack does not
				// shrink it or delete its old values, allowing us to access
				// the old frame pointer after we set the stack pointer.
				POP_A                         // save return value
				i = & code.data[RA];          // restore instruction pointer
				stack.decreaseTo(fp - ARITY); // restore stack pointer
				fp = OFP;                     // restore old frame pointer
				PUSH_A                        // put back the return value
			continue;
			case op::_return_v: // return void:
				i = & code.data[RA];          // restore instruction pointer
				stack.decreaseTo(fp - ARITY); // restore stack pointer
				fp = OFP;                     // restore old frame pointer
			continue;
			case op::_call: // normal call (address operand):
				// Access Record: | arity (8b) | OFP (24b) | RA (32b) |
				PUSH({ .n = (fp << 32) | (u64)(i - code.data + 5) }); // push fp and ra together
				fp = stack.size() - 1;               // set frame pointer
				i = & code.data[getD(++i) & D_MASK]; // set instruction pointer
			continue;
			case op::_call_l: // lambda call (address on the stack):
				// Access Record: | arity (8b) | OFP (24b) | RA (32b) |
				POP_A // get the address of the function
				PUSH({ .n = (fp << 32) | (u64)(i - code.data + 1) }); // push fp and ra together
				fp = stack.size() - 1; // set frame pointer
				i = & code.data[a.n];  // set instruction pointer
			continue;
			case op::_call_k:
				switch (getB(++i)) {
					// ==== Memory Functions ===================================
					case mem::grow:
						POP_A
						// works in both cases because count is initially 0:
						memory.count += a.n * 4096; // 4 KB
						if (memory.count != 0)
							 memory.data = (u8 *) realloc(memory.data, memory.count);
						else memory.data = (u8 *)  malloc(memory.count);
					break;
					case mem::shrink:
						POP_A
						memory.count -= a.n * 4096; // 4 KB
						if (memory.count != 0)
							memory.data = (u8 *) realloc(memory.data, memory.count);
					break;
					case mem::size:  PUSH({ .n = memory.count }); break;
					case mem::page:  PUSH({ .n = 4096 /* 4 KB */ }); break;
					case mem::pages: PUSH({ .n = memory.count / 4096 }); break;
					case mem::copy: POP_AB memcpy(b.p, a.p, stack.pop().n); break;
					case mem::load: POP_AB memcpy(b.p, & code.data[a.n], stack.pop().n); break;
					case mem::zeros: memset(stack.pop().p, 0, stack.pop().n); break;
					case mem::fill: POP_AB memset(b.p, (u8)a.n, stack.pop().n); break;
					case mem::compare:
						stack.push({ .i = compare(
							(u8 *) stack.pop().p,
							(u8 *) stack.pop().p,
							stack.pop().n
						) });
					break;
					/*case krn::estream: e_stream(stack.pop().p); break;
					case krn::ostream: o_stream(stack.pop().p); break;
					case krn::edata:   e_stream(& code.data[stack.pop().i]); break;
					case krn::odata:   o_stream(& code.data[stack.pop().i]); break;
					case krn::eflush:  fflush(stderr); break;
					case krn::oflush:  fflush(stdout); break;
					case krn::istream: {
						char * tmp = new char[256];
						std::cin.getline(tmp, 256);
						PUSH({ .p = tmp });
					} break;
					case krn::fork: {
						thread * t = new thread(run, code, stack.pop().i);
						PUSH({ .p = t });
					} break;
					case krn::join: return;
					case krn::sleep:
						this_thread::sleep_for(
							chrono::milliseconds(stack.pop().i)
						);
					break;
					case krn::wait: ((thread *)stack.pop().p) -> join(); break;
					case krn::lock: critical.lock(); break;
					case krn::release: critical.unlock(); break;

					case krn::_u2s: stack.push({ .p = u2s(stack.pop().i) }); break;
					case krn::_i2s: stack.push({ .p = i2s(stack.pop().i) }); break;
					case krn::_s2u:
						try { stack.push({ .i = s2u((chr *)stack.pop().p) }); }
						catch (invalid_format) { ex = true; }
					break;
					case krn::_s2i:
						try { stack.push({ .i = (u64)s2i((chr *)stack.pop().p) }); }
						catch (invalid_format) { ex = true; }
					break;
					case krn::_f2s: stack.push({ .p = f2s(stack.pop().r) }); break;
					case krn::_s2f:
						try { stack.push({ .r = s2f((chr *)stack.pop().p) }); }
						catch (invalid_format) { throw; }
					break;*/
					case krn::debug:
						puts("= STACK ========================================");
						for (siz j = 0; j < stack.size(); j++) printf("%llu\n", stack[j].n);
						puts("================================================");
						fflush(stdout);
					break;
					//case krn::time: stack.push({ .i = (u64)time(nullptr) }); break;
					//case krn::seed: pcg::seed(stack.pop().i); break;
					//case krn::random: stack.push({ .i = pcg::next() }); break;
				}
			break;
			case op::_convert_n2r: POP_A PUSH({ .r = (f64)a.n }); break;
			case op::_convert_i2r: POP_A PUSH({ .r = (f64)a.i }); break;
			case op::_convert_r2i: POP_A PUSH({ .i = (i64)a.r }); break;
			// === MATH FUNCTIONS ==============================================
			case op::_math<math::_e>:       PUSH({ .r = numbers::e_v<f64> }); break;
			case op::_math<math::_ln10>:    PUSH({ .r = numbers::ln10_v<f64> }); break;
			case op::_math<math::_ln2>:     PUSH({ .r = numbers::ln2_v<f64> }); break;
			case op::_math<math::_log10e>:  PUSH({ .r = numbers::log10e_v<f64> }); break;
			case op::_math<math::_log2e>:   PUSH({ .r = numbers::log2e_v<f64> }); break;
			case op::_math<math::_pi>:      PUSH({ .r = numbers::pi_v<f64> }); break;
			case op::_math<math::_sqrt1_2>: PUSH({ .r = sqrt(1 / 2) }); break;
			case op::_math<math::_sqrt2>:   PUSH({ .r = numbers::sqrt2_v<f64> }); break;
			case op::_math<math::_egamma>:  PUSH({ .r = numbers::egamma_v<f64> }); break;
			case op::_math<math::_phi>:     PUSH({ .r = numbers::phi_v<f64> }); break;
			case op::_math<math::_abs>:   POP_A  PUSH({ .r = abs(a.r) }); break;
			case op::_math<math::_acos>:  POP_A  PUSH({ .r = acos(a.r) }); break;
			case op::_math<math::_acosh>: POP_A  PUSH({ .r = acosh(a.r) }); break;
			case op::_math<math::_asin>:  POP_A  PUSH({ .r = asin(a.r) }); break;
			case op::_math<math::_asinh>: POP_A  PUSH({ .r = asinh(a.r) }); break;
			case op::_math<math::_atan>:  POP_A  PUSH({ .r = atan(a.r) }); break;
			case op::_math<math::_atan2>: POP_BA PUSH({ .r = atan2(a.r, b.r) }); break;
			case op::_math<math::_atanh>: POP_A  PUSH({ .r = atanh(a.r) }); break;
			case op::_math<math::_cbrt>:  POP_A  PUSH({ .r = cbrt(a.r) }); break;
			case op::_math<math::_ceil>:  POP_A  PUSH({ .r = ceil(a.r) }); break;
			case op::_math<math::_cos>:   POP_A  PUSH({ .r = cos(a.r) }); break;
			case op::_math<math::_cosh>:  POP_A  PUSH({ .r = cosh(a.r) }); break;
			case op::_math<math::_exp>:   POP_A  PUSH({ .r = exp(a.r) }); break;
			case op::_math<math::_expm1>: POP_A  PUSH({ .r = expm1(a.r) }); break;
			case op::_math<math::_floor>: POP_A  PUSH({ .r = floor(a.r) }); break;
			case op::_math<math::_hypot>: POP_BA PUSH({ .r = hypot(a.r, b.r) }); break;
			case op::_math<math::_log>:   POP_A  PUSH({ .r = log(a.r) }); break;
			case op::_math<math::_log1p>: POP_A  PUSH({ .r = log1p(a.r) }); break;
			case op::_math<math::_log10>: POP_A  PUSH({ .r = log10(a.r) }); break;
			case op::_math<math::_log2>:  POP_A  PUSH({ .r = log2(a.r) }); break;
			case op::_math<math::_max>:   POP_BA PUSH({ .r = max(a.r, b.r) }); break;
			case op::_math<math::_min>:   POP_BA PUSH({ .r = min(a.r, b.r) }); break;
			case op::_math<math::_round>: POP_A  PUSH({ .r = round(a.r) }); break;
			case op::_math<math::_sign>:  POP_A  PUSH({ .r = sign(a.r) }); break;
			case op::_math<math::_sin>:   POP_A  PUSH({ .r = sin(a.r) }); break;
			case op::_math<math::_sinh>:  POP_A  PUSH({ .r = sinh(a.r) }); break;
			case op::_math<math::_sqrt>:  POP_A  PUSH({ .r = sqrt(a.r) }); break;
			case op::_math<math::_tan>:   POP_A  PUSH({ .r = tan(a.r) }); break;
			case op::_math<math::_tanh>:  POP_A  PUSH({ .r = tanh(a.r) }); break;
			case op::_math<math::_tri>:   POP_A  PUSH({ .r = tri(a.r) }); break;
			case op::_math<math::_trunc>: POP_A  PUSH({ .r = trunc(a.r) }); break;
			case op::_math<math::_imaginary>: PUSH({ .c = { 0.0, 1.0 } }); break;
			case op::_halt_FF: exit(0);
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

#undef PUSH

#undef PUSH_A
#undef PUSH_B
#undef PUSH_BA
#undef PUSH_AB

#undef DECREASE

#undef RA
#undef OFP
#undef ARITY

#undef CMP_I
#undef CMP_F

#undef UPDATE_I
#undef SKIP
#undef SKIP_NEXT

#endif
