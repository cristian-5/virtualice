
# Virtualice VM

Stack based vm for super fast execution.
Supports threads, exceptions, and kernel extensions.

## Architecture

The architecture is capable of executing operations on 64bit integers
and 64bit floating point numbers. The instruction addresses are 32bit
long. Jumps are absolute. Function variables, function parameters and
global variables are restricted to a maximum addressing of 2^16.

When a call occurs, a new scope gets generated automatically. It is a
function's responsibility to move the parameters from the stack into
the scope using `param.[b|w] $name`. The `return` instruction will
automatically drop the current scope, invoking the garbage collector.

Variable shadowing is automatically implemented and if `local.[b|w].g $name`
does not find the variable in the local scope, it will be fetched from
the global scope if present. If not found then undefined behaviour.
For variable shadowing to properly work the varible names should be
the same for varibles that share the same identifier.

Global variables are thread shared and thread safe.

It is possible to force the creation of a new scope with the instruction
`scope.c` and the deletion follows `scope.d`. A scope deletion will
always invoke the garbage collector.

### Instruction Set

``` asm

; halt, rest:

	halt
	rest              ; no operation

; push, pop, cast:

	push.[b|w|d|q]    ; push n bytes
	push.a [function] ; push function address
	push.z            ; push 0
	push.o            ; push 1
	pop               ; pop a value
	pop.n [number]    ; pop n values
	top               ; duplicates stack top
	cast.[b|w|d|q]    ; value casting with size
	convert.to.[i|f]  ; value conversion with type

; arithmetics: (i = int, f = float)

	factorial

	add.[i|f]
	sub.[i|f]
	mul.[i|f]
	div.[i|f]
	mod.[i|f]
	pow.[i|f]
	
	increment
	decrement

; bitwise: (l = left, r = right)

	and
	or
	not
	xor
	invert
	negate

	swap

	shift.[l|r]    [number]
	rotate.[l|r]   [number]

; functions, threads, exceptions:

	call.k   [function] ; call to kernel
	call     [function]
	call.l              ; indirect 'lamda' call
	
	return

; variables:

	; the variable name is numeric
	; [b|w] refers to the name length in bytes

	global.[b|w].c [variable name] ; create global var
	global.[b|w].d [variable name] ; delete global var
	global.[b|w].g [variable name] ; get global var
	global.[b|w].s [variable name] ; set global var

	local.[b|w].c [variable name] ; create local var
	local.[b|w].d [variable name] ; delete local var
	local.[b|w].g [variable name] ; get local var
	local.[b|w].s [variable name] ; set local var

	param.[b|w]   [variable name] ; make a parameter

	scope.c ; create scope
	scope.d ; delete scope

; exceptions

	raise  ; raises the exception flag
	flag   ; moves the exception flag on the stack

; jumps:

	jump       [address]
	jump.[t|f] [address]

; compare:

	compare.[i|f].[e|ne]
	compare.[s|u|f].[ge|le|g|l]

```

## Kernel functions

### I/O functions

| HEX | Prototype           | Description                                                  |
|:---:|:-------------------:|:-------------------------------------------------------------|
|`66` | `estream([chr]);`   | *Prints the null terminated string to `stderr`.*             |
|`55` | `ostream([chr]);`   | *Prints the null terminated string to `stdout`.*             |
|`44` | `istream(): [chr];` | *Waits for the enter key, returns the last line of `stdin`.* |

### Threading management functions

| HEX | Prototype              | Description                                             |
|:---:|:----------------------:|:--------------------------------------------------------|
|`F0` | `fork(p: u32): [thr];` | *Spawns and returns a thread at instruction `p`.*       |
|`EF` | `join();`              | *Ends the current thread, joining the main flow.*       |
|`57` | `sleep(ms: u64);`      | *Sleeps for `ms` milliseconds.*                         |
|`A1` | `wait(t: [thr]);`      | *Waits for thread `t` to complete.*                     |
|`88` | `lock();`              | *Locks the current thread to begin a critical section.* |
|`33` | `release();`           | *Releases the critical section, begun with `lock`.*     |

> The term `wait-pending` is used to describe a running thread that
> is being waited by another. Those threads can't be abruptly
> terminated and doing so results in undefined behaviour.

> The term `join` indicates the termination of the current thread, while
> `wait` will be the designated function that waits for a thread to end.

### Fast conversion functions

| HEX | Prototype               | Description                             |
|:---:|:-----------------------:|:----------------------------------------|
|`CA` | `u2s(u: u64): [chr];`   | *Converts u64 `i` to string (`[chr]`).* |
|`CB` | `i2s(i: i64): [chr];`   | *Converts i64 `i` to string (`[chr]`).* |
|`CC` | `s2u(s: [chr]): u64;`   | *Converts string `s` (`[chr]`) to u64.* |
|`CD` | `s2i(s: [chr]): i64;`   | *Converts string `s` (`[chr]`) to i64.* |
|`CE` | `f2s(f: f64): [chr];`   | *Converts f64 `f` to string (`[chr]`).* |
|`CF` | `s2f(s: [chr]): f64;`   | *Converts string `s` (`[chr]`) to f64.* |

> The strings are always null terminated *heap* arrays of characters `[chr]`.
> The conversions from string to [`u64`, `i64`, `f64`] might fail
> and set the exception flag before returning without result.

### Memory management functions

| HEX | Prototype                               | Description                                      |
|:---:|:---------------------------------------:|:-------------------------------------------------|
|`A7` | `allocate(n: u64): [*];`                | *Allocates `n` bytes, returns the new address.*  |
|`DA` | `deallocate(p: [*]);`                   | *Deallocates memory from address `p`.*           |
|`2A` | `reallocate(p: [*], n: u64): [*];`      | *Reallocates `n` bytes trying to resize `p`.*    |
|`C0` | `copy(s: [*], d: [*], n: u64): [*];`    | *Copies `n` bytes from `s` to `d`.*              |
|`70` | `load(s: u32, d: [*], n: u64): [*];`    | *Loads `n` bytes from code index `s` to `d`.*    |
|`00` | `zeros(p: [*], n: u64): [*];`           | *Writes `0` to `p`, for `n` consecutive bytes.*  |
|`F1` | `fill(b: byt, p: [*], n: u64): [*];`    | *Fills `p` with `b`, for `n` consecutive bytes.* |
|`C2` | `compare(a: [*], b: [*], n: u64): [*];` | *Compares `a`, `b`, for `n` consecutive bytes.*  |

> The usage of unallocated or unowned memory will cause
> a bad access fault resulting in a immediate crash.

### Misc functions

| HEX | Prototype              | Description                                         |
|:---:|:----------------------:|:----------------------------------------------------|
|`DE` | `debug();`             | *Prints the stack and the call frame to `stdout`.*  |
|`51` | `sign(n: u64): [chr];` | *Returns the string signature of `n`.*              |
|`71` | `time(): u64;`         | *Returns the current unix timestamp.*               |
|`5E` | `seed(s: u64);`        | *Sets the seed `s` for the next random generation.* |
|`3A` | `random(): u64;`       | *Generates a pseudo random number using a pcg.*     |

> Every VM instantiation will always set `seed(time());`.

## Testing

For testing purposes install [`customasm`](https://github.com/hlorenzi/customasm).

``` zsh

cd tests
customasm test.asm -f binary -o test.ice

cd ..
ninja

./virtualice tests/test.ice

```
