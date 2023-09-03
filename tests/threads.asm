
#include "ice.asm"

; multithreading test 0:
;    + main: launches print, waits 5s, terminates print
;    + print: prints "doing something" every 300ms

main:

	push.d    thread_1         ; push thread address
	call.k    fork             ; call fork
	push.w    5000             ; push 5 seconds
	call.k    sleep            ; call sleep

	halt                       ; end program (every thread)

; ======== Thread 1 ========================================== ;

thread_1:

	loop:

		push.a    string_0         ; push string address
		call.k    odata            ; print string
		push.w    300              ; push 300ms (.3s)
		call.k    sleep            ; call sleep
		jump      loop             ; loop forever

	halt                       ; safety halt (just in case)

; ======== DATA SECTION ======================================= ;

string_0:
	#d "doing something\n", 0x00
	string_0_length = $ - string_0
