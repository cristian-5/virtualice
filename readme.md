
# Virtualice VM

Stack based vm for super fast execution.
Supports threads, exceptions, and kernel extensions.

## Architecture

The architecture is capable of executing operations on 64bit integers
and 64bit floating point numbers. The instruction addresses are 32bit
long. Jumps are absolute. Function variables, function parameters are
restricted to a maximum addressing of 2^16 (65,535 variables). Global
variables are restricted to a maximum addressing of 2^32.

### Instruction Set

We are currently working on this.
The instruction set is 

``` asm

; halt, rest:

	halt
	rest              ; no operation

; push, pop, cast:

	push.[b|w|d|q]    ; push n bytes
	push.z            ; push 0
	push.o            ; push 1
	pop               ; pop a value
	pop.n [number]    ; pop n values
	top               ; duplicates stack top
	cast.[b|w|d|q]    ; value casting with size
	convert.to.[i|f]  ; value conversion with type

; arithmetics: (i = int, f = float)

	add.[i|f]
	sub.[i|f]
	mul.[i|f]
	div.[i|f]
	mod.[i|f]

	increment.[i|f]
	decrement.[i|f]

; bitwise: (l = left, r = right)

	and
	or
	not
	xor
	invert
	negate

	shift.[l|r]    [number]
	rotate.[l|r]   [number]

; functions, threads, exceptions:

	arity    [number]   ; set function arity
	call     [function]
	call.l
	call.k   [function] ; call to kernel
	return

; variables: (l = local, g = global, a = argument)

	get.[l|g|a] [variable index]
	set.[l|g|a] [variable index]

; exceptions

	raise               ; raises the exception flag
	jump.ex [address]   ; jump on exception, reset flag

; jumps:

	jump [address]
	jump.[z|nz] [address]
	jump.[e|ne|ge|le|g|l].[i|f] [address]

```

## Kernel functions

### I/O functions

| HEX | Prototype               | Description                                          |
|:---:|:-----------------------:|:-----------------------------------------------------|
|`66` | `fun estream(chr *);`   | *Prints the null terminated string to `stderr`.*     |
|`55` | `fun ostream(chr *);`   | *Prints the null terminated string to `stdout`.*     |
|`44` | `fun istream(): chr *;` | *Waits for enter, returns the last line of `stdin`.* |

### Threading management functions

| HEX | Prototype                  | Description                                             |
|:---:|:--------------------------:|:--------------------------------------------------------|
|`F0` | `fun fork(p: u32): thr *;` | *Spawns and returns a thread at instruction `p`.*       |
|`EF` | `fun join();`              | *Ends the current thread, joining the main flow.*       |
|`57` | `fun sleep(ms: u64);`      | *Sleeps for `ms` milliseconds.*                         |
|`A1` | `fun wait(t: thr *);`      | *Waits for thread `t` to complete.*                     |
|`88` | `fun lock();`              | *Locks the current thread to begin a critical section.* |
|`33` | `fun release();`           | *Releases the critical section, begun with `lock`.*     |

> The term `wait-pending` is used to describe a running thread that
> is being waited by another. Those threads can't be abruptly
> terminated and doing so results in undefined behaviour.

> The term `join` indicates the termination of the current thread, while
> `wait` will be the designated function that waits for a thread to end.

### Fast conversion functions

| HEX | Prototype                   | Description                             |
|:---:|:---------------------------:|:----------------------------------------|
|`CA` | `fun u2s(u: u64): chr *;`   | *Converts u64 `i` to string (`chr *`).* |
|`CB` | `fun i2s(i: i64): chr *;`   | *Converts i64 `i` to string (`chr *`).* |
|`CC` | `fun s2u(s: chr *): u64;`   | *Converts string `s` (`chr *`) to u64.* |
|`CD` | `fun s2i(s: chr *): i64;`   | *Converts string `s` (`chr *`) to i64.* |
|`CE` | `fun f2s(f: f64 *): chr *;` | *Converts f64 `f` to string (`chr *`).* |
|`CF` | `fun s2f(s: chr *): f64;`   | *Converts string `s` (`chr *`) to f64.* |

> The conversions from string to [`u64`, `i64`, `f64`] might fail
> and set the exception flag before returning without result.

### Memory management functions

| HEX | Prototype                             | Description                                         |
|:---:|:-------------------------------------:|:----------------------------------------------------|
|`A7` | `fun allocate(n: u64): *;`            | *Allocates `n` bytes and returns the new address.*  |
|`DA` | `fun deallocate(p: *);`               | *Deallocates memory from address `p`.*              |
|`2A` | `fun reallocate(p: *, n: u64): *;`    | *Reallocates `n` bytes trying to resize `p`.*       |
|`C0` | `fun copy(s: *, d: *, n: u64): *;`    | *Copies `n` bytes from `s` to `d`.*                 |
|`70` | `fun load(s: u32, d: *, n: u64): *;`  | *Loads `n` bytes from code index `s` to `d`.*       |
|`00` | `fun zeros(p: *, n: u64): *;`         | *Writes `0` to `p`, for `n` consecutive bytes.*     |
|`F1` | `fun fill(b: byt, p: *, n: u64): *;`  | *Fills `p` with `b`, for `n` consecutive bytes.*    |
|`C2` | `fun compare(a: *, b: *, n: u64): *;` | *Compares `a` with `b`, for `n` consecutive bytes.* |

> The usage of unallocated or unowned memory will cause
> a bed access fault resulting in a immediate crash.

### Misc functions

| HEX | Prototype                  | Description                                         |
|:---:|:--------------------------:|:----------------------------------------------------|
|`DE` | `fun debug();`             | *Prints the stack and the call frame to `stdout`.*  |
|`71` | `fun time(): u64;`         | *Returns the current unix timestamp.*               |
|`5E` | `fun seed(s: u64);`        | *Sets the seed `s` for the next random generation.* |
|`3A` | `fun random(): u64;`       | *Generates a pseudo random number using a pcg.*     |

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
