
#include "ice.asm"

main:

	; allocate enough memory:
	push.b    string_0_length
	call.k    allocate

	; load the string in memory:
	push.b    string_0_length   ; number of bytes to copy
	get.g     0                 ; get allocation ptr (destination)
	push.d    string_0          ; code pointer (source)
	call.k    load              ; fun load(s: u32, d: *, n: u64): *;

	; print the string:
	call.k    ostream

	halt

string_0:
	#d "hello world!", 0x00
	string_0_length = $ - string_0
