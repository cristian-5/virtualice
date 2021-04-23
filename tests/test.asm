
#include "ice.asm"

start:

	push.b 35
	push.b 34

	jump multiply

multiply:

	mul.i

halt
