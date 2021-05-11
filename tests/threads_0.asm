
#include "ice.asm"

; multithreading test 0:
;    + main: launches print, waits 5s, terminates print
;    + print: prints "SIGNATURE" every 300ms

main:

	push.d    thread_1         ; push thread address
	call.k    fork             ; call fork
	push.w    5000             ; push 5 seconds
	call.k    sleep            ; call sleep

	halt                       ; end program (every thread)

; ======== Thread 1 ========================================== ;

thread_1:

	push.b    string_0_length  ; push the string length
	call.k    allocate         ; allocate n bytes
	push.b    string_0_length  ; push the string length
	get.g     0                ; get allocated address
	push.d    string_0         ; get code address
	call.k    load             ; load code string in memory

loop:

	top                        ; duplicate string address
	call.k    ostream          ; print string (pops)
	push.w    300              ; push 300ms (.3s)
	call.k    sleep            ; call sleep
	jump      loop             ; loop forever

	halt                       ; safety halt (just in case)

; ======== DATA SECTION ======================================= ;

string_0:
	#d "doing something\n", 0x00
	string_0_length = $ - string_0
