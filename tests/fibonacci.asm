
#include "ice.asm"

virtualice 0

main:

	const.n    10


	call       fibonacci
	; the kernel debug function prints the stack:
	call.k     0xDE       ; should print 55
	halt

fibonacci:

	; functions have automatic garbage collection
	; so we need to specify arity:
	arity 1               ; we have 1 parameter

	call.k    0xDE

	param.r 0             ; get n
	const.n 2             ; push 2

	jump.l.n   base_case ; if (n < 2) jump base_case
	; else:
	param.r 0             ; get n
	dec.n                 ; (decrement natural) n - 1
	call       fibonacci

	param.r 0             ; get n
	dec.n
	dec.n                 ; n - 2
	call       fibonacci

	add.n                 ; add the 2 calls
	return

	base_case:
		param.r 0         ; get n
		return            ; return n
