
#include "ice.asm"

virtualice 0

main:

	const.n 0

	loop:
		call.k 0xDE
		global.r 0
		inc.n
		clone
		global.w 0
	const.n 5
	jump.l.n loop

	jump end
	rest
	rest

	end:
		call.k 0xDE
		halt


; what is the kernel responsible for?
; [x] input/output
;     - print(str)
; [x] thread management
;     - fork(code: *): thr - forks a new thread with the given code
;     - join() - joins the current thread with the main thread
;     - sleep(n: nat) - sleeps for n milliseconds
;     - wait(t: thr) - waits for a thread to finish
; [x] network management
