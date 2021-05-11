
#include "ice.asm"

; multithreading test 0:
;    + main: launches print, waits 5s, terminates print
;    + print: prints "SIGNATURE" every 300ms

main:

	push.d print
	call.k fork
	push.w 5000
	call.k sleep

	halt

print:

	call.k sign
	push.w 300
	call.k sleep

	jump print
