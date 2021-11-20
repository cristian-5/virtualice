
#include "ice.asm"

main:

	push.b 10
	push.a fibonacci
	call.l
	call.k debug
	halt

fibonacci:

	param.b $n    ; make parameter $n

	local.b.g $n  ; get $n
	push.b    2
	compare.u.l   ; if n < 2: base case
	jump.t base_case

	local.b.g $n  ; get $n
	decrement     ; $n--
	push.a fibonacci
	call.l

	local.b.g $n  ; get $n
	decrement
	decrement     ; $n = $n - 2
	push.a fibonacci
	call.l

	add.i
	return

	base_case:
	
		local.b.g $n
		return


; varibles table
$n = 0x2A