
#include "ice.asm"

main: STRPTR = 0

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

	halt

string_0:
	#d "hello world!", 0x00
	string_0_length = $ - string_0
