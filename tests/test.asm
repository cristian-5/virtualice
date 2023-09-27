
#include "ice.asm"

virtualice 0

main:

	const.b 1
	call.k memory_grow

	const.w 30
	store.w  [0x50] ; store `30` in address 0x50

	call.k 0xDE ; debug (nothing on the stack)

	load.w   [0x50] ; load from address 0x50

	call.k 0xDE ; debug (30 on the stack)

	halt




; what is the kernel responsible for?
; [x] input/output
;     - print(str)
; [x] memory management
;     - pages(): nat - returns number of pages
;     - page(): nat - returns page size in bytes
;     - grow(n: nat): bln - gets n new pages, returns true if successful
;     - shrink(n: nat) - shrinks the memory by n pages
; [x] thread management
;     - fork(code: *): thr - forks a new thread with the given code
;     - join() - joins the current thread with the main thread
;     - sleep(n: nat) - sleeps for n milliseconds
;     - wait(t: thr) - waits for a thread to finish
; [x] network management
