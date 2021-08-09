
#include "ice.asm"

push.b    6
call      factorial
call.k    debug
halt

; let factorial = ƒ(n: nat): nat
factorial: n = 0

	arity     1           ; number of arguments
	push.o                ; temporary variable (f)

	_while__A:
	
		get.a     n           ; get n
		top
		; if (n == 1) stop:
		push.o
		jump.i.e    _end_while__A
		; else:
		top
		get.l       0     ; local variable 0 (f)
		mul.i             ; f * n
		set.l       0     ; f = f * n
		dec.i             ; n--
		set.a       n
		jump        _while__A

	_end_while__A:

	pop
	get.l         0 ; f
	return

end_factorial:

halt

; --------------------------------

r_factorial:
	arity      1
	get.a      0
	push.o
	jump.i.e   r_factorial_bc
	get.a      0
	top
	dec.i
	call       r_factorial
	mul.i
	return
	r_factorial_bc: ; base case
		push.o
		return
r_factorial_end:

; --------------------------------
