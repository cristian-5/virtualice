
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

mtx vm::critical;
mtx vm::global;

map<u16, val> vm::globals;

void vm::run(arr<u8> code, u64 p) {
	u8 * i = & code.data[p];
	stk<val> stack;
	stk<u8*> frame;
	val a, b;
	bln ex = false;
	pcg::seed((u64)time(nullptr));
	scope * current_scope = new scope();
	while (true) {
		switch (* i) {
			case op::halt<>: exit(0);
			case op::rest<>: break;
			case op::push<typ::b>: PUSH({ .i = getB(++i) }); break;
			case op::push<typ::w>: PUSH({ .i = getW(++i) }); SKIP(2); continue;
			case op::push<typ::d>: PUSH({ .i = getD(++i) }); SKIP(4); continue;
			case op::push<typ::q>: PUSH({ .i = getQ(++i) }); SKIP(8); continue;
			case op::push<typ::z>: PUSH({ .i = 0ull }); break;
			case op::push<typ::o>: PUSH({ .i = 1ull }); break;
			case        op::pop<>: stack.decrease(); break;
			case  op::pop<typ::n>: stack.decreaseBy(getB(++i)); break;
			case        op::top<>: PUSH(stack.top()); break;
			case op::cast<typ::b>: PUSH(V_CAST(stack.pop(), B)); break;
			case op::cast<typ::w>: PUSH(V_CAST(stack.pop(), W)); break;
			case op::cast<typ::d>: PUSH(V_CAST(stack.pop(), D)); break;
			case op::cast<typ::q>: PUSH(V_CAST(stack.pop(), Q)); break;
			case  op::factorial<>: PUSH({ .i = factorial(stack.pop().i) }); break;
			case  op::add<typ::i>: POP_BA PUSH({ .i = a.i + b.i }); break;
			case  op::add<typ::f>: POP_BA PUSH({ .f = a.f + b.f }); break;
			case  op::sub<typ::i>: POP_BA PUSH({ .i = a.i - b.i }); break;
			case  op::sub<typ::f>: POP_BA PUSH({ .f = a.f - b.f }); break;
			case  op::mul<typ::i>: POP_BA PUSH({ .i = a.i * b.i }); break;
			case  op::mul<typ::f>: POP_BA PUSH({ .f = a.f * b.f }); break;
			case  op::div<typ::i>: POP_BA PUSH({ .i = a.i / b.i }); break;
			case  op::div<typ::f>: POP_BA PUSH({ .f = a.f / b.f }); break;
			case  op::mod<typ::i>: POP_BA PUSH({ .i = a.i % b.i }); break;
			case  op::mod<typ::f>: POP_BA PUSH({ .f = F_MOD(a.f, b.f) }); break;
			case  op::pow<typ::i>: POP_BA PUSH({ .i = (u64)((i64)pow(a.i, b.i)) }); break;
			case  op::pow<typ::f>: POP_BA PUSH({ .f = pow(a.f, b.f) }); break;
			case  op::increment<>: POP_A  PUSH({ .i = a.i + 1 }); break;
			case  op::decrement<>: POP_A  PUSH({ .i = a.i - 1 }); break;
			case  op::convert<typ::i>: POP_A PUSH({ .i = (u64)a.f }); break;
			case  op::convert<typ::f>: POP_A PUSH({ .f = (f64)a.i }); break;
			case  op::b_and<>: POP_BA PUSH({ .i = (a.i & b.i) }); break;
			case   op::b_or<>: POP_BA PUSH({ .i = (a.i | b.i) }); break;
			case  op::l_not<>: POP_A  PUSH({ .i = ! a.i }); break;
			case  op::b_xor<>: POP_BA PUSH({ .i = (a.i ^ b.i) }); break;
			case op::invert<>: POP_A  PUSH({ .i = ~ a.i }); break;
			case   op::complement<>: POP_A PUSH({ .i = - a.i }); break;
			case  op::shift<typ::r>: POP_A PUSH({ .i = a.i >> getB(++i) }); break;
			case  op::shift<typ::l>: POP_A PUSH({ .i = a.i << getB(++i) }); break;
			case op::rotate<typ::r>: POP_A PUSH({ .i = rotateR(a.i, getB(++i)) }); break;
			case op::rotate<typ::l>: POP_A PUSH({ .i = rotateL(a.i, getB(++i)) }); break;
			case       op::jump<>: UPDATE_I(D); continue;
			case op::jump<jmp::t>: POP_A if (a.i)  { UPDATE_I(D); } else SKIP_NEXT(4);
			case op::jump<jmp::f>: POP_A if (!a.i) { UPDATE_I(D); } else SKIP_NEXT(4);
			case op::raise<>: ex = true; break;
			case  op::flag<>: PUSH({ .i = ex }); ex = false; break;
			case  op::swap<>: POP_AB PUSH_AB break;
			case op::compare_u<cmp::e> : POP_BA PUSH({ .i = (u64(a.i) == u64(b.i)) }); break;
			case op::compare_u<cmp::ne>: POP_BA PUSH({ .i = (u64(a.i) != u64(b.i)) }); break;
			case op::compare_u<cmp::l> : POP_BA PUSH({ .i = (u64(a.i)  < u64(b.i)) }); break;
			case op::compare_u<cmp::le>: POP_BA PUSH({ .i = (u64(a.i) <= u64(b.i)) }); break;
			case op::compare_u<cmp::g> : POP_BA PUSH({ .i = (u64(a.i)  > u64(b.i)) }); break;
			case op::compare_u<cmp::ge>: POP_BA PUSH({ .i = (u64(a.i) >= u64(b.i)) }); break;
			case op::compare_s<cmp::e> : POP_BA PUSH({ .i = (a.i == b.i) }); break;
			case op::compare_s<cmp::ne>: POP_BA PUSH({ .i = (a.i != b.i) }); break;
			case op::compare_s<cmp::l> : POP_BA PUSH({ .i = (a.i  < b.i) }); break;
			case op::compare_s<cmp::le>: POP_BA PUSH({ .i = (a.i <= b.i) }); break;
			case op::compare_s<cmp::g> : POP_BA PUSH({ .i = (a.i  > b.i) }); break;
			case op::compare_s<cmp::ge>: POP_BA PUSH({ .i = (a.i >= b.i) }); break;
			case op::compare_f<cmp::e> : POP_BA PUSH({ .i = (a.f == b.f) }); break;
			case op::compare_f<cmp::ne>: POP_BA PUSH({ .i = (a.f != b.f) }); break;
			case op::compare_f<cmp::l> : POP_BA PUSH({ .i = (a.f  < b.f) }); break;
			case op::compare_f<cmp::le>: POP_BA PUSH({ .i = (a.f <= b.f) }); break;
			case op::compare_f<cmp::g> : POP_BA PUSH({ .i = (a.f  > b.f) }); break;
			case op::compare_f<cmp::ge>: POP_BA PUSH({ .i = (a.f >= b.f) }); break;
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
					case krn::_f2s: stack.push({ .p = f2s(stack.pop().f) }); break;
					case krn::_s2f:
						try { stack.push({ .f = s2f((chr *)stack.pop().p) }); }
						catch (invalid_format) { ex = true; }
					break;

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
						/*if (!frame.isEmpty()) {
							puts("- FRAME ----------------------------------------");
							for (siz j = 0; j < frame.size(); j++) {
								printf("ret: %llu scp: %llu\n",
									fp, frame[j].lfp,
									frame[j].ret - u64(& code.data[0]),
									frame[j].ari);
							}
						}*/
						puts("================================================");
						fflush(stdout);
					break;
					case krn::sign: stack.push({ .p = sign(stack.pop().i) }); break;
					case krn::time: stack.push({ .i = (u64)time(nullptr) }); break;
					case krn::seed: pcg::seed(stack.pop().i); break;
					case krn::random: stack.push({ .i = pcg::next() }); break;
				}
			break;
			case op::call<cll::c>:
				// step 0: save return address:
				frame.push(i + 5);
				// step 1: new scope:
				current_scope = new scope(current_scope);
				// step 2: jump to the function
				i = & code.data[getD(++i) & D_MASK];
			continue;
			case op::call<cll::l>:
				// step 0: get the address of the function
				POP_A
				// step 1: save return address:
				frame.push(i + 1);
				// step 2: new scope:
				current_scope = new scope(current_scope);
				// step 3: jump to the function
				i = & code.data[u64(a.p) & D_MASK];
			continue;
			case op::ret<>: {
				scope * tmp = current_scope;
				current_scope = current_scope -> parent;
				delete tmp;
			} i = frame.pop(); continue;
			case op::negate<>: POP_A PUSH({ .f = - a.f }); break;
			case op::global<var::bc>:
				POP_A
				global.lock();
				globals.insert({ getB(++i), a });
				global.unlock();
			break;
			case op::global<var::bd>:
				global.lock();
				globals.erase(getB(++i));
				global.unlock();
			break;
			case op::global<var::bg>:
				global.lock();
				PUSH(globals[getB(++i)]);
				global.unlock();
			break;
			case op::global<var::bs>:
				POP_A
				global.lock();
				globals[getB(++i)] = a;
				global.unlock();
			break;
			case op::global<var::wc>:
				POP_A global.lock();
				globals.insert({ getB(++i), a });
				global.unlock();
				SKIP(2);
			continue;
			case op::global<var::wd>:
				global.lock();
				globals.erase(getW(++i));
				global.unlock();
				SKIP(2);
			continue;
			case op::global<var::wg>:
				global.lock();
				PUSH(globals[getW(++i)]);
				global.unlock();
				SKIP(2);
			continue;
			case op::global<var::ws>:
				POP_A
				global.lock();
				globals[getW(++i)] = a;
				global.unlock();
				SKIP(2);
			continue;
			case op::local<var::bc>: POP_A current_scope -> add(getB(++i), a); break;
			case op::local<var::bd>: current_scope -> del(getB(++i)); break;
			case op::local<var::bg>: PUSH(current_scope -> get(getB(++i))); break;
			case op::local<var::bs>: POP_A current_scope -> set(getB(++i), a); break;
			case op::local<var::wc>: POP_A current_scope -> add(getW(++i), a); SKIP(2); continue;
			case op::local<var::wd>: current_scope -> del(getW(++i)); SKIP(2); continue;
			case op::local<var::wg>: PUSH(current_scope -> get(getW(++i))); SKIP(2); continue;
			case op::local<var::ws>: POP_A current_scope -> set(getW(++i), a); SKIP(2); continue;
			case op::scope<scp::c>: current_scope = new scope(current_scope); break;
			case op::scope<scp::d>: {
				scope * tmp = current_scope;
				current_scope = current_scope -> parent;
				delete tmp;
			} break;
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

#undef PUSH

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
