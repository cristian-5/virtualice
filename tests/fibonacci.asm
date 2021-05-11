
#include "ice.asm"

main:

	push.b    10
	call      fibonacci
	call.k    ostream     ; should print 55
	halt

; func fibonacci(n: Natural): Natural;
fibonacci: n = 0 ; n is the argument 0

	arity     1           ; number of arguments

	get.a     n           ; get n
	push.b    2

	jump.i.l  base_case   ; if (n < 2) jump base_case
						  ; else:
	get.a     n           ; get n
	dec.i                 ; n - 1
	call      fibonacci

	get.a     n           ; get n
	dec.i
	dec.i                 ; n - 2
	call      fibonacci

	add.i                 ; add the 2 calls
	return

	base_case:
	get.a     n
	return                ; else return n
