
#include "ice.asm"

main:

    push.q      10
    push.a      fibonacci
    call.l      ; lambda call
	call.k      debug       ; should print 55
	halt

; let fibonacci = ƒ(n: nat) -> nat
fibonacci: n = 0 ; n is the argument 0

	arity     1           ; number of arguments
	
    get.a     n           ; get n
	push.b    2

    compare.i.l
	jump.t    base_case   ; if (n < 2) jump base_case
						  ; else:
	get.a     n           ; get n
	decrement             ; n - 1
	push.a    fibonacci
    call.l

	get.a     n           ; get n
	decrement
	decrement             ; n - 2
	push.a    fibonacci
    call.l

	add.i                 ; add the 2 calls
	return

	base_case:
        get.a     n
        return            ; else return n
