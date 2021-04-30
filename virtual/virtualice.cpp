
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

#define CMP_I(_) if (a.i _ b.i)
#define CMP_F(_) if (a.f _ b.f)

#define UPDATE_I(T) i = & code.data[get##T(++i)]; continue
#define SKIP(N) i += N
#define SKIP_NEXT(N) i += N + 1; continue

[[gnu::always_inline]]
u64 vm::getQ(void * p) { return (* ((u64 *)p)); }
[[gnu::always_inline]]
u32 vm::getD(void * p) { return (* ((u32 *)p)) & D_MASK; }
[[gnu::always_inline]]
u16 vm::getW(void * p) { return (* ((u16 *)p)) & W_MASK; }
[[gnu::always_inline]]
u8  vm::getB(void * p) { return (* ((u8  *)p)) & B_MASK; }

void vm::run(arr<u8> code) {
	u8 * i = code.data;
	stk<val> stack;
	val a, b;
	while (true) {
		switch (* i) {
			case op::halt<>: return;
			case op::rest<>: break;
			case op::push<typ::b>: stack.push({ .i = getB(++i) }); break;
			case op::push<typ::w>: stack.push({ .i = getW(++i) }); break;
			case op::push<typ::d>: stack.push({ .i = getD(++i) }); break;
			case op::push<typ::q>: stack.push({ .i = getQ(++i) }); break;
			case op::push<typ::z>: stack.push({ .i = 0ull }); break;
			case op::push<typ::o>: stack.push({ .i = 1ull }); break;
			case        op::pop<>: stack.decrease(); break;
			case  op::pop<typ::n>: stack.decrease(getQ(++i)); break;
			case        op::top<>: stack.push(stack.top()); break;
			case op::cast<typ::b>: stack.push(V_CAST(stack.pop(), B));
			case op::cast<typ::w>: stack.push(V_CAST(stack.pop(), W));
			case op::cast<typ::d>: stack.push(V_CAST(stack.pop(), D));
			case op::cast<typ::q>: stack.push(V_CAST(stack.pop(), Q));
			case  op::add<typ::i>: POP_BA stack.push({ .i = a.i + b.i }); break;
			case  op::add<typ::f>: POP_BA stack.push({ .f = a.f + b.f }); break;
			case  op::sub<typ::i>: POP_BA stack.push({ .i = a.i - b.i }); break;
			case  op::sub<typ::f>: POP_BA stack.push({ .f = a.f - b.f }); break;
			case  op::mul<typ::i>: POP_BA stack.push({ .i = a.i * b.i }); break;
			case  op::mul<typ::f>: POP_BA stack.push({ .f = a.f * b.f }); break;
			case  op::div<typ::i>: POP_BA stack.push({ .i = a.i / b.i }); break;
			case  op::div<typ::f>: POP_BA stack.push({ .f = a.f / b.f }); break;
			case  op::mod<typ::i>: POP_BA stack.push({ .i = a.i % b.i }); break;
			case  op::mod<typ::f>: POP_BA stack.push({ .f = fmod(a.f, b.f) }); break;
			case  op::inc<typ::i>: POP_A  stack.push({ .i = a.i + 1 }); break;
			case  op::inc<typ::f>: POP_A  stack.push({ .f = a.f + 1 }); break;
			case  op::dec<typ::i>: POP_A  stack.push({ .i = a.i - 1 }); break;
			case  op::dec<typ::f>: POP_A  stack.push({ .f = a.f - 1 }); break;
			case      op::b_and<>: POP_BA stack.push({ .i = (a.i & b.i) }); break;
			case       op::b_or<>: POP_BA stack.push({ .i = (a.i | b.i) }); break;
			case      op::l_not<>: POP_A  stack.push({ .i = ! a.i }); break;
			case      op::b_xor<>: POP_BA stack.push({ .i = (a.i ^ b.i) }); break;
			case     op::invert<>: POP_A  stack.push({ .i = ~ a.i }); break;
			case     op::negate<>: POP_A  stack.push({ .i = (~ a.i) + 1}); break;
			case  op::shift<typ::r>: POP_A stack.push({ .i = a.i >> getB(++i) }); break;
			case  op::shift<typ::l>: POP_A stack.push({ .i = a.i << getB(++i) }); break;
			case op::rotate<typ::r>: POP_A stack.push({ .i = rotateR(a.i, getB(++i)) }); break;
			case op::rotate<typ::l>: POP_A stack.push({ .i = rotateL(a.i, getB(++i)) }); break;
			case         op::jump<>: UPDATE_I(Q);
			case   op::jump<jmp::t>: POP_A if (a.i)   { UPDATE_I(Q); } else SKIP_NEXT(8);
			case   op::jump<jmp::f>: POP_A if (!a.i)  { UPDATE_I(Q); } else SKIP_NEXT(8);
			case  op::jump<jmp::li>: POP_BA CMP_I(<)  { UPDATE_I(Q); } else SKIP_NEXT(8);
			case  op::jump<jmp::gi>: POP_BA CMP_I(>)  { UPDATE_I(Q); } else SKIP_NEXT(8);
			case  op::jump<jmp::ei>: POP_BA CMP_I(==) { UPDATE_I(Q); } else SKIP_NEXT(8);
			case op::jump<jmp::nei>: POP_BA CMP_I(!=) { UPDATE_I(Q); } else SKIP_NEXT(8);
			case op::jump<jmp::lei>: POP_BA CMP_I(<=) { UPDATE_I(Q); } else SKIP_NEXT(8);
			case op::jump<jmp::gei>: POP_BA CMP_I(>=) { UPDATE_I(Q); } else SKIP_NEXT(8);
			case  op::jump<jmp::lf>: POP_BA CMP_F(<)  { UPDATE_I(Q); } else SKIP_NEXT(8);
			case  op::jump<jmp::gf>: POP_BA CMP_F(>)  { UPDATE_I(Q); } else SKIP_NEXT(8);
			case  op::jump<jmp::ef>: POP_BA CMP_F(==) { UPDATE_I(Q); } else SKIP_NEXT(8);
			case op::jump<jmp::nef>: POP_BA CMP_F(!=) { UPDATE_I(Q); } else SKIP_NEXT(8);
			case op::jump<jmp::lef>: POP_BA CMP_F(<=) { UPDATE_I(Q); } else SKIP_NEXT(8);
			case op::jump<jmp::gef>: POP_BA CMP_F(>=) { UPDATE_I(Q); } else SKIP_NEXT(8);
			case op::call<>: stack.push({ .i = (u64)(i + 9) }); i = (u8 *)getQ(++i); continue;
			case  op::ret<>: POP_BA i = (u8 *)a.i; stack.decrease(stack.pop().i); stack.push(b); continue;
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

#undef CMP_I
#undef CMP_F

#undef UPDATE_I
#undef SKIP
#undef SKIP_NEXT

#endif
