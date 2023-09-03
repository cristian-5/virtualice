
#include "ice.asm"

main:

	push.a    string_0           ; push string_0 onto the stack
	call.k    odata              ; print string from data segment

	halt

string_0:
	#d "hello world!", 0x00
	string_0_length = $ - string_0
