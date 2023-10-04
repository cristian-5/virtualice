
# Virtualice VM

Stack based vm for super fast execution.
It currently supports the following features:

✅ → signed (**integers**) and unsigned (**natural**) numbers
✅ → floating point (**real**) numbers
✅ → **complex** numbers
✅ → **control flow** jump and compare instructions
✅ → **stack based** arithmetic operations on pure types
✅ → **bitwise** and **boolean** operations
✅ → *global* and *local* variables
✅ → **functions** with parameters
✅ → opcodes for a comprehensive **math** library
✅ → **memory** management [page model](https://en.wikipedia.org/wiki/Memory_paging)
🔘 → **threads**, [fork-join model](https://en.wikipedia.org/wiki/Fork%E2%80%93join_model)
🔘 → **file system**, [unix model](https://en.wikipedia.org/wiki/Unix_filesystem)
🔘 → **files**, [posix model](https://en.wikipedia.org/wiki/Unistd.h)
🔘 → **networks** (probably [Berkeley sockets](https://en.wikipedia.org/wiki/Berkeley_sockets))

## Architecture

> ⚠️ **Attention**: Breaking Changes since last versions!

The virtual machine is stack based and the architecture is `ice`.

The architecture is capable of handling 64bit integers, floating point and
complex numbers. Arithmetic operations expect operands to be pushed in
human order. Calls should push parameters on reverse on the stack, while
it's the function's responsibility to tell the environment how many
parameters it expects using the `arity` instruction. The final bytecode is
optimized for size and sometimes even easy to read in a hex editor (for
example the `call.k debug` instruction translates to `C0 DE`, **co**de **de**bug).

### Instruction Set

``` asm

virtualice <version>        ; set magic number and version

; halt, rest:

	halt
	rest                    ; no operation

; stack operations:

	const.[b|w|d|q] <value> ; push a constant (byte, word, double, quad)
	const.[0|1|f|t|h|l]     ; push 0, 1, false, true, high, low

	swap        ; swaps the top two stack values

	drop        ; drop the top of the stack
	drop.n  <n> ; drop the top n elements of the stack (max 255)
	clone       ; clone the top of the stack
	clone.n <n> ; clone the top of the stack n times   (max 255)

; arithmetics: (natural, integer, real, complex)

	[add|sub|mul|div].[n|i|r|c] ; nat = int in some cases
	[mod|pow].[n|i|r]
	[inc|dec].[n|i|r] ; nat = int here

	project       ; project complex into real and imaginary parts
	project.[r|i] ; project complex's real or imaginary part
	magnitude     ; magnitude of a complex number
	conjugate     ; complex conjugate
	combine       ; combine 2 floating numbers into a complex

	convert.[n2r|i2r|r2i] ; convert-cast between types

; bitwise, logical operations (on 64 bits):

	mask.[b|w|d|q]  ; (byte / boolean, word, dword, qword)
	bit.[g|s|r] <n> ; (get, set, reset) nth bit
	nibble.[l|h]    ; get (low, high) nibble
	nibble.s        ; swap nibbles of a byte
	[buffer|and|or|not|nor|nand|xor|xnor|xand]
	[invert|complement|reverse]  ; invert, complement, reverse
	[rotate|shift].[r|l] <n>     ; shift / rotate (right, left) by n bits

; jumps and comparisons:

	jump [label]                        ; uncoditional jump
	jump.[1|0|z|nz|t|f|e|ne]    [label] ; if (one, zero, not zero, true, false)
	jump.[g|ge|l|le].[n|i|r]    [label] ; if condition is met
	compare.[g|ge|l|le].[n|i|r] [label] ; push result of comparison

; functions, kernel, lambda calls:

	address [address]  ; push (32bit) address (of function, lable or data)

	call    [function] ; call a normal function
	call.k  [function] ; call to kernel
	call.l             ; indirect lambda call

	arity   <n>        ; sets the parameters to n

	return             ; returns a value
	return.v           ; returns void

; global and local variables, function parameters:

	global.r       <index> ; read  a global variable
	global.w       <index> ; write a global variable

	local.r  <index>    ; read  a local variable
	local.w  <index>    ; write a local variable

	param.r  <index>    ; read  a function parameter
	param.w  <index>    ; write a function parameter

; memory load, store, pointers:

	load.[b|w|d|q]  [address] ; load  a value from memory
	store.[b|w|d|q] [address] ; store a value  in  memory

```

## Math Library ByteCode

We support the following opcodes over real values (`f64`).\
The documention for these functions can be found on the
[Mozilla Development Network](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math).

`e`, `ln10`, `ln2`, `log10e`, `log2e`, `pi`, `sqrt1_2`, `sqrt2`,
`egamma`, `phi`, `abs`, `acos`, `acosh`, `asin`, `asinh`, `atan`,
`atan2`, `atanh`, `cbrt`, `ceil`, `cos`, `cosh`, `exp`, `expm1`,
`floor`, `hypot`, `log`, `log1p`, `log10`, `log2`, `max`, `min`,
`round`, `sign`, `sin`, `sinh`, `sqrt`, `tan`, `tanh`, `trunc`.

## Access Record Information

The frame pointer points directly at the `AR` index.

```
+--------+------------------------+--------------------------------+
| 8 bits |       24 bits          |            32 bits             |
|--------+------------------------+--------------------------------+
|  arity |   old frame pointer    |         return address         |
+--------+------------------------+--------------------------------+
```

## Kernel Functions

The following tables contain codes for kernel functions.\
They work as normal functions, expecting parameters to be passed in reverse.\
To call one of these you can use their name or hex code (`call.k memory_size`).

### Memory Management Functions

|HEX | Prototype                               | Description                                                          |
|:--:|:----------------------------------------|:---------------------------------------------------------------------|
|`50`|`memory_grow(p: u64)`                    |*Grows the memory by `p` pages.*                                      |
|`54`|`memory_shrink(p: u64)`                  |*Shrinks the memory by `p` pages.*                                    |
|`51`|`memory_size(): u64`                     |*Returns the memory size in bytes.*                                   |
|`2A`|`memory_page(): u64`                     |*Returns the page size in bytes (hardware dependent, usually `4KB`).*    |
|`25`|`memory_pages(): u64`                    |*Returns the number of allotted pages.*                               |
|`C0`|`memory_copy(d: *, s: *, n: u64)`        |*Copies `n` bytes from source `s` to destination `d`.*                |
|`70`|`memory_load(d: *, s: u32, n: u64)`      |*Loads `n` bytes from code address `s` to destination `d`.*           |
|`00`|`memory_zeros(p: *, n: u64)`             |*Writes `0` to `p`, for `n` consecutive bytes.*                       |
|`F1`|`memory_fill(b: byt, p: *, n: u64)`      |*Fills `p` with `b`, for `n` consecutive bytes.*                      |
|`C2`|`memory_compare(a: *, b: *, n: u64): i64`|*Compares `n` bytes of `a`, `b`. Returns first different byte or `-1`*|

> The usage of unowned memory will cause a bad
> access fault resulting in a immediate crash.

> 🚫 **Critical**: **DO NOT** use any of the following functions as they need revision.

### I/O functions

| HEX | Prototype           | Description                                                  |
|:---:|:-------------------:|:-------------------------------------------------------------|
|`E5` | `estream([chr]);`   | *Prints the null terminated string to `stderr`.*             |
|`05` | `ostream([chr]);`   | *Prints the null terminated string to `stdout`.*             |
|`ED` | `edata(p: u32);`    | *Prints the string in program data pointer `p` to `stderr`.* |
|`0D` | `odata(p: u32);`    | *Prints the string in program data pointer `p` to `stdout`.* |
|`EF` | `eflush();`         | *Flushes `stderr`.*                                          |
|`0F` | `oflush();`         | *Flushes `stdout`.*                                          |
|`15` | `istream(): [chr];` | *Waits for the enter key, returns the last line of `stdin`.* |

### Threading management functions

| HEX | Prototype              | Description                                             |
|:---:|:----------------------:|:--------------------------------------------------------|
|`F0` | `fork(p: u32): [thr];` | *Spawns and returns a thread at instruction `p`.*       |
|`10` | `join();`              | *Ends the current thread, joining the main flow.*       |
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

### Misc functions

| HEX | Prototype              | Description                                         |
|:---:|:----------------------:|:----------------------------------------------------|
|`DE` | `debug();`             | *Prints the stack and the call frame to `stdout`.*  |
|`71` | `time(): u64;`         | *Returns the current unix timestamp.*               |
|`5E` | `seed(s: u64);`        | *Sets the seed `s` for the next random generation.* |
|`3A` | `random(): u64;`       | *Generates a pseudo random number using a pcg.*     |

## Compilation, usage and testing
 
To compile assembly files install [`customasm`](https://github.com/hlorenzi/customasm).
Include the file `ice.asm` in your code and compile it with `customasm`.
Then compile the `virtualice` binary with `ninja` and run it.

> ⚠️ ***Warning**: The test files in `/tests/` might be referring to an old version.

``` zsh

cd tests
customasm test.asm -f binary -o test.ice

cd ..
ninja

./virtualice tests/test.ice

```
