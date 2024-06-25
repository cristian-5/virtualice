
#include "../ice.inc"

virtualice 0

input:

	const.n   5
	call      factorial
	call.k    0xDE
	halt

; --------------------------------

factorial: n = 0
	arity 1
	param.r   n  ; get n
	const.n 1    ; push 1
	jump.e       factorial_bc
	param.r   n  ; get n
	clone        ; duplicate n
	dec.n        ; n - 1
	call         factorial
	mul.n        ; n * factorial(n - 1)
	return
	factorial_bc: ; base case
		const.n 1 ; push 1
		return    ; return 1
factorial_end:
