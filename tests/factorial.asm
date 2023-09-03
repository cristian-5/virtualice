
#include "ice.asm"

input:

	push.a    string_0           ; push string_0 address
	call.k    odata              ; print string_0

	call.k    istream            ; get user input
	call.k    s2i                ; convert string to integer

	call      factorial
	call.k    i2s
	top
	push.a    string_1           ; push string_1 address
	call.k    odata              ; print string_1
	call.k    ostream
	call.k    deallocate
	halt

; --------------------------------

factorial: n = 0
	param.b n
	local.b.g n  ; get n
	push.o       ; push 1
	compare.i.e  ; if (n == 1) goto base case
	jump.t       factorial_bc
	local.b.g n  ; get n
	top          ; duplicate n
	decrement    ; n - 1
	call         factorial
	mul.i        ; n * factorial(n - 1)
	return
	factorial_bc: ; base case
		push.o      ; push 1
		return      ; return 1
factorial_end:

; --------------------------------

string_0:
	#d "insert integer: ", 0x00
	string_0_length = $ - string_0

string_1:
	#d "the factorial is: ", 0x00
	string_1_length = $ - string_1
