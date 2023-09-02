
#include "ice.asm"

main:

	push.b    10
	call      fibonacci
	call.k    debug       ; should print 55
	halt

fibonacci: n = 0 ; n is the argument 0

	param.b   n

	local.b.g n           ; get n
	push.b    2

	compare.u.l           ; if (n < 2) jump base_case
	jump.t base_case
						  ; else:
	local.b.g  n          ; get n
	decrement             ; n - 1
	call      fibonacci

	local.b.g  n          ; get n
	decrement
	decrement             ; n - 2
	call      fibonacci

	add.i                 ; add the 2 calls
	return

	base_case:
		local.b.g  n
		return            ; else return n
