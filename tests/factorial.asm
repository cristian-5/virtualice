
#include "ice.asm"

STRPTR = 0
; allocate enough memory:
push.b     string_0_length
call.k     allocate
global.b.c STRPTR            ; store allocation ptr

; load the string in memory:
push.b     string_0_length   ; number of bytes to copy
global.b.g STRPTR            ; get allocation ptr (destination)
push.d     string_0          ; code pointer (source)
call.k     load              ; fun load(s: u32, d: *, n: u64): *;

; print the string and cleanup:
global.b.g STRPTR            ; get allocation ptr
top                          ; duplicate string pointer
call.k    ostream
call.k    deallocate         ; deallocate leftover pointer

; --------------------------------

call.k    istream            ; get user input
call.k    s2i                ; convert string to integer

flag                         ; get exception flag
jump.f    main               ; if (flag == 0) goto main

; print error message:

; allocate enough memory:
push.b     string_1_length
call.k     allocate
global.b.s STRPTR            ; store allocation ptr

; load the string in memory:
push.b     string_1_length   ; number of bytes to copy
global.b.g STRPTR            ; get allocation ptr (destination)
push.d     string_1          ; code pointer (source)
call.k     load              ; fun load(s: u32, d: *, n: u64): *;

; print the string and cleanup:
global.b.g STRPTR            ; get allocation ptr
top                          ; duplicate string pointer
call.k    ostream
call.k    deallocate         ; deallocate leftover pointer

halt

main:

	call      factorial
	call.k    i2s
	top
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
	#d "invalid integer!\n", 0x00
	string_1_length = $ - string_0
