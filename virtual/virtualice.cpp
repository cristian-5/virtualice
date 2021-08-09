
#ifndef VIRTUALICE_CPP
#define VIRTUALICE_CPP

#include <chrono>
#include <thread>

using namespace std;

#include "virtualice.hpp"

#define F_MOD(N, D) N - (u64)(D * N / D)

#define Q_MASK 0xFFFFFFFFFFFFFFFFull
#define D_MASK 0x00000000FFFFFFFFull
#define W_MASK 0x000000000000FFFFull
#define B_MASK 0x00000000000000FFull

#define V_CAST(V, T) { .i = V.i & T##_MASK }

#define POP_A  a = stack.pop();
#define POP_BA b = stack.pop(); a = stack.pop();
#define POP_AB a = stack.pop(); b = stack.pop();

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

#undef PU8

mutex vm::critical;

void vm::run(arr<u8> code, u64 p) {
	u8 * i = & code.data[p];
	stk<val> stack;
	stk<cal> frame;
	val a, b;
	siz fp = 0;
	bln ex = false;
	pcg::seed((u64)time(nullptr));
	while (true) {
		switch (* i) {
			case op::halt<>: exit(0);
			case op::rest<>: break;
			case op::push<typ::b>: stack.push({ .i = getB(++i) }); break;
			case op::push<typ::w>: stack.push({ .i = getW(++i) }); SKIP(2); continue;
			case op::push<typ::d>: stack.push({ .i = getD(++i) }); SKIP(4); continue;
			case op::push<typ::q>: stack.push({ .i = getQ(++i) }); SKIP(8); continue;
			case op::push<typ::z>: stack.push({ .i = 0ull }); break;
			case op::push<typ::o>: stack.push({ .i = 1ull }); break;
			case        op::pop<>: stack.decrease(); break;
			case  op::pop<typ::n>: stack.decreaseBy(getB(++i)); break;
			case        op::top<>: stack.push(stack.top()); break;
			case op::cast<typ::b>: stack.push(V_CAST(stack.pop(), B)); break;
			case op::cast<typ::w>: stack.push(V_CAST(stack.pop(), W)); break;
			case op::cast<typ::d>: stack.push(V_CAST(stack.pop(), D)); break;
			case op::cast<typ::q>: stack.push(V_CAST(stack.pop(), Q)); break;
			case      op::raise<>: ex = true; break;
			case  op::add<typ::i>: POP_BA stack.push({ .i = a.i + b.i }); break;
			case  op::add<typ::f>: POP_BA stack.push({ .f = a.f + b.f }); break;
			case  op::sub<typ::i>: POP_BA stack.push({ .i = a.i - b.i }); break;
			case  op::sub<typ::f>: POP_BA stack.push({ .f = a.f - b.f }); break;
			case  op::mul<typ::i>: POP_BA stack.push({ .i = a.i * b.i }); break;
			case  op::mul<typ::f>: POP_BA stack.push({ .f = a.f * b.f }); break;
			case  op::div<typ::i>: POP_BA stack.push({ .i = a.i / b.i }); break;
			case  op::div<typ::f>: POP_BA stack.push({ .f = a.f / b.f }); break;
			case  op::mod<typ::i>: POP_BA stack.push({ .i = a.i % b.i }); break;
			case  op::mod<typ::f>: POP_BA stack.push({ .f = F_MOD(a.f, b.f) }); break;
			case  op::inc<typ::i>: POP_A  stack.push({ .i = a.i + 1 }); break;
			case  op::inc<typ::f>: POP_A  stack.push({ .f = a.f + 1 }); break;
			case  op::dec<typ::i>: POP_A  stack.push({ .i = a.i - 1 }); break;
			case  op::dec<typ::f>: POP_A  stack.push({ .f = a.f - 1 }); break;
			case  op::convert<typ::i>: POP_A stack.push({ .i = (u64)a.f }); break;
			case  op::convert<typ::f>: POP_A stack.push({ .f = (f64)a.i }); break;
			case      op::b_and<>: POP_BA stack.push({ .i = (a.i & b.i) }); break;
			case       op::b_or<>: POP_BA stack.push({ .i = (a.i | b.i) }); break;
			case      op::l_not<>: POP_A  stack.push({ .i = ! a.i }); break;
			case      op::b_xor<>: POP_BA stack.push({ .i = (a.i ^ b.i) }); break;
			case     op::invert<>: POP_A  stack.push({ .i = ~ a.i }); break;
			case   op::complement<>: POP_A stack.push({ .i = - a.i }); break;
			case  op::shift<typ::r>: POP_A stack.push({ .i = a.i >> getB(++i) }); break;
			case  op::shift<typ::l>: POP_A stack.push({ .i = a.i << getB(++i) }); break;
			case op::rotate<typ::r>: POP_A stack.push({ .i = rotateR(a.i, getB(++i)) }); break;
			case op::rotate<typ::l>: POP_A stack.push({ .i = rotateL(a.i, getB(++i)) }); break;
			case         op::jump<>: UPDATE_I(D); continue;
			case   op::jump<jmp::z>: POP_A if (a.i)   { UPDATE_I(D); } else SKIP_NEXT(4);
			case  op::jump<jmp::nz>: POP_A if (!a.i)  { UPDATE_I(D); } else SKIP_NEXT(4);
			case  op::jump<jmp::li>: POP_BA CMP_I(<)  { UPDATE_I(D); } else SKIP_NEXT(4);
			case  op::jump<jmp::gi>: POP_BA CMP_I(>)  { UPDATE_I(D); } else SKIP_NEXT(4);
			case  op::jump<jmp::ei>: POP_BA CMP_I(==) { UPDATE_I(D); } else SKIP_NEXT(4);
			case op::jump<jmp::nei>: POP_BA CMP_I(!=) { UPDATE_I(D); } else SKIP_NEXT(4);
			case op::jump<jmp::lei>: POP_BA CMP_I(<=) { UPDATE_I(D); } else SKIP_NEXT(4);
			case op::jump<jmp::gei>: POP_BA CMP_I(>=) { UPDATE_I(D); } else SKIP_NEXT(4);
			case  op::jump<jmp::lf>: POP_BA CMP_F(<)  { UPDATE_I(D); } else SKIP_NEXT(4);
			case  op::jump<jmp::gf>: POP_BA CMP_F(>)  { UPDATE_I(D); } else SKIP_NEXT(4);
			case  op::jump<jmp::ef>: POP_BA CMP_F(==) { UPDATE_I(D); } else SKIP_NEXT(4);
			case op::jump<jmp::nef>: POP_BA CMP_F(!=) { UPDATE_I(D); } else SKIP_NEXT(4);
			case op::jump<jmp::lef>: POP_BA CMP_F(<=) { UPDATE_I(D); } else SKIP_NEXT(4);
			case op::jump<jmp::gef>: POP_BA CMP_F(>=) { UPDATE_I(D); } else SKIP_NEXT(4);
			case  op::jump<jmp::ex>: if (ex) { ex = false; UPDATE_I(D); } else SKIP_NEXT(4);
			case op::call<>:
				// step 1: save lfp, ret, arity = 0:
				frame.push({
					.lfp = fp,
					.ret = (u64)(i + 5),
					.ari = 0
				});
				// step 2: set the new fp
				fp = stack.size();
				// step 3: jump to the function
				i = & code.data[getD(++i) & D_MASK];
			continue;
			case op::call<cll::k>:
				switch (getB(++i)) {
					case krn::estream: e_stream(stack.pop().p); break;
					case krn::ostream: o_stream(stack.pop().p); break;
					case krn::istream: break;
					
					case krn::fork: {
						thread * t = new thread(run, code, stack.pop().i);
						stack.push({ .p = t });
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

					case krn::allocate:
						stack.push({ .p = malloc(stack.pop().i) });
					break;
					case krn::deallocate: free(stack.pop().p); break;
					case krn::reallocate:
						stack.push({ .p = realloc(stack.pop().p, stack.pop().i) });
					break;
					case krn::copy: POP_AB memcpy(b.p, a.p, stack.pop().i);
					break;
					case krn::load:
						POP_AB memcpy(b.p, & code.data[a.i], stack.pop().i);
					break;
					case krn::zeros:
						memset(stack.pop().p, 0x00, stack.pop().i);
					break;
					case krn::fill:
						POP_AB memset(b.p, (u8)a.i, stack.pop().i);
					break;
					case krn::compare:
						stack.push({ .i = memcmp(
							stack.pop().p,
							stack.pop().p,
							stack.pop().i
						) & 1ull });
					break;

					case krn::debug:
						puts("= STACK ========================================");
						for (siz j = 0; j < stack.size(); j++) {
							printf("%llu\n", stack[j].i);
						}
						if (!frame.isEmpty()) {
							puts("- FRAME ----------------------------------------");
							for (siz j = 0; j < frame.size(); j++) {
								printf("fp: %llu lfp: %llu ret: %llu arity: %d\n",
									fp, frame[j].lfp, frame[j].ret, frame[j].ari);
							}
						}
						puts("================================================");
					break;
					case krn::sign: stack.push({ .p = sign(stack.pop().i) }); break;
					case krn::time: stack.push({ .i = (u64)time(nullptr) }); break;
					case krn::seed: pcg::seed(stack.pop().i); break;
					case krn::random: stack.push({ .i = pcg::next() }); break;
				}
			break;
			case op::call<cll::l>: break;
			case op::ret<>: {
				// step 1: get return value
				POP_A
				// step 2: pop frame
				const cal current = frame.pop();
				// step 3: pop parameters and variables
				stack.decreaseTo(fp - current.ari);
				// step 4: reset fp
				fp = current.lfp;
				// step 5: push return value
				PUSH_A
				// step 6: jump to return address
				i = (u8 *)current.ret;
			} continue;
			case     op::arity<>: frame.top().ari = getB(++i); break;
			case    op::negate<>: POP_A stack.push({ .f = - a.f }); break;
			case op::get<var::g>: stack.push(stack.at(getW(++i))); SKIP(2); continue;
			case op::get<var::l>: stack.push(stack.at(fp + getB(++i))); break;
			case op::get<var::a>: stack.push(stack.at(fp - getB(++i) - 1)); break;
			case op::set<var::g>: POP_A stack.edit(getW(++i),  a); SKIP(2); continue;
			case op::set<var::l>: POP_A stack.edit(fp + getB(++i),  a); break;
			case op::set<var::a>: POP_A stack.edit(fp - getB(++i) - 1,  a); break;
		}
		++i;
	}
}

#undef F_MOD

#undef Q_MASK
#undef D_MASK
#undef W_MASK
#undef B_MASK

#undef POP_A
#undef POP_BA
#undef POP_AB

#undef PUSH_A
#undef PUSH_B
#undef PUSH_BA
#undef PUSH_AB

#undef DECREASE

#undef CMP_I
#undef CMP_F

#undef UPDATE_I
#undef SKIP
#undef SKIP_NEXT

#endif
