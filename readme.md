
# Virtualice VM

Stack based vm for super fast execution.
It currently supports the following features:

- [x] 64bit signed and unsigned **integers**
- [x] 64bit **floating** point numbers
- [x] 64bit **complex** numbers (`f32` real, `f32` imaginary)
- [x] **control flow** jump and compare instructions
- [x] **stack based** arithmetic operations on pure types
- [x] **bitwise** and **boolean** operations
- [x] *global* and *local* variables
- [x] **functions** with parameters
- [x] comprehensive **math** library
- [ ] **memory** management [page model](https://en.wikipedia.org/wiki/Memory_paging)
- [ ] **threads**, [fork-join model](https://en.wikipedia.org/wiki/Fork%E2%80%93join_model)
- [ ] **networks** (probably [Berkeley sockets](https://en.wikipedia.org/wiki/Berkeley_sockets))

## Architecture

> ⚠️ **Attention**: Breaking Changes since last version!

The virtual machine is stack based.

The architecture is capable of handling 64bit integers, floating point and
complex numbers. Arithmetic operations expect operands to be pushed in
human order. Addresses are 32 bit absolute. Calls should push parameters
on reverse on the stack, while it's the function's responsibility to tell
the environment how many parameters it expects using the `arity` instruction.

### Instruction Set

``` asm

virtualice <n>  ; set magic number and version

; halt, rest:

	halt
	rest        ; no operation

; stack operations:

	const.[b|w|d|q] <value> ; push a constant (byte, word, double, quad)
	const.[0|1|f|t|h|l] ; push 0, 1, false, true, high, low

	swap        ; swaps the top two stack values

	drop        ; drop the top of the stack
	drop.n  <n> ; drop the top n elements of the stack (max 255)
	clone       ; clone the top of the stack
	clone.n <n> ; clone the top of the stack n times   (max 255)

; arithmetics: (natural, integer, real, complex)

	[add|sub|mul|div].[n|i|r|c] ; nat = int in some cases
	[mod|pow].[n|i|r]
	[inc|dec].[n|i|r] ; nat = int here

	magn       ; magnitude of a complex number
	conj       ; complex conjugate
	comb       ; combine 2 floating numbers into a complex
	proj       ; project complex into real and imaginary parts
	proj.[r|i] ; project complex's real or imaginary part

	conv.[n2r|i2r|r2i] ; convert-cast between types

; bitwise, logical operations (on 64 bits):

	mask.[b|w|d|q]  ; (byte / boolean, word, dword, qword)
	bit.[g|s|r] <n> ; (get, set, reset) nth bit
	nibble.[l|h]    ; get (low, high) nibble
	nibble.s        ; swap nibbles of a byte
	[buffer|and|or|not|nor|nand|xor|xnor|xand]
	[invert|complement|reverse]  ; invert, complement, reverse
	[rotate|shift].[r|l] <n> ; shift / rotate (right, left) by n bits

; jumps and comparisons:

	jump [label]                     ; uncoditional jump
	jump.[1|0|z|nz|t|f|e|ne]        [label] ; if (one, zero, not zero, true, false)
	jump.[g|ge|l|le].[n|i|r] [label] ; if condition is met
	compare.[g|ge|l|le].[n|i|r] [label] ; push result of comparison

; functions, kernel, lambda calls:

	address [address]  ; push (32bit) address (of function, lable or data)

	call    [function] ; call a normal function
	call.k  [function] ; call to kernel
	call.l             ; indirect lambda call

	arity   <n>        ; sets the parameters to n
	arity.[0...7]      ; shorthand for arity up to 8

	return             ; returns a value
	return.v           ; returns void

; global and local variables, function parameters:

	global.r       <index> ; read  a global variable (8 bits operand)
	global.w       <index> ; write a global variable (8 bits operand)
	global.e.[r|w] <index> ; read / write a global (extended 16 bits operand)

	local.r  <index>    ; read  a local variable
	local.w  <index>    ; write a local variable
	local.[r|w].[0...7] ; shorthand for reading / writing first 8 locals

	param.r  <index>    ; read  a function parameter
	param.w  <index>    ; write a function parameter
	param.[r|w].[0...7] ; shorthand for reading / writing first 8 parameters

; program data, main memory:

	memory.l [address] ; load  a value from memory (address is 32-bits)
	memory.s [address] ; store a value  in  memory (address is 32-bits)

```

## Math Library [NEW]
|  Code  |  Operands  |  Description  |
|  ----: | :--------: | :-----------  |
|   e    |    None    |  Euler's number and the base of natural logarithms; approximately 2.718. |
| euler  |    None    |  Euler's number and the base of natural logarithms; approximately 2.718. |
|  ln10  |    None    |  Natural logarithm of 10; approximately 2.303. |
|  ln2   |    None    |  Natural logarithm of 2; approximately 0.693.  |
| log10e |    None    |  Base-10 logarithm of E; approximately 0.434.  |
| log2e  |    None    |  Base-2 logarithm of E; approximately 1.443.   |
|   pi   |    None    |  Ratio of a circle's circumference to its diameter; approximately 3.14159. |
| sqrt1_2|    None    |  Square root of ½; approximately 0.707. |
| sqrt2  |    None    |  Square root of 2; approximately 1.414. |
| egamma |    None    |  ???, not documented. |
| phi    |    None    |  ???, not documented. |
| abs    |  x: real   |  Returns the absolute value of `x`. |
| acos   |  x: real   |  Returns the arccosine of `x`. |
| acosh  |  x: real   |  Returns the hyperbolic arccosine of `x`. |
| asin   |  x: real   |  Returns the arcsine of `x`. |
| asinh  |  x: real   |  Returns the hyperbolic arcsine of a `x`. |
| atan   |  x: real   |  Returns the arctangent of `x`. |
| atan2  |  y: real, x: real  |  Returns the arctangent of `y/x`.  |
| atanh  |  x: real   |  Returns the hyperbolic arctangent of `x`. |
| cbrt   |  x: real   |  Returns the cube root of `x`. |
| ceil   |  x: real   |  Returns the smallest integer greater than or equal to `x`. |
| cos    |  x: real   |  Returns the cosine of `x`. |
| cosh   |  x: real   |  Returns the hyperbolic cosine of `x`. |
| exp    |  x: real   |  Returns `e^x`, where `x` is the argument, and `e` is Euler's number (2.718…, the base of the natural logarithm). |
| expm1  |  x: real   |  Returns subtracting 1 from `exp(x)`. |
| floor  |  x: real   |  Returns the largest integer less than or equal to `x`. |
| hypot  |  x: real, y: real |  Returns the `sqrt(x^2+y^2)`. |
| log    |  x: real   |  Returns the natural logarithm (base e) of a `x`. |
| log1p  |  x: real   |  Returns the natural logarithm (log_e; also ㏑) of `1 + x` for the number `x`. |
| log10  |  x: real   |  Returns the base-10 logarithm of `x`. |
| log2   |  x: real   |  Returns the base-2 logarithm of `x`. |
| max    |  x: real, y: real |  Returns the largest of `x` and `y`. |
| min    |  x: real, y: real |  Returns the smallest of `x` and `y`. |
| rand?  |  TODO |  TODO |
| round  |  x: real   |  Returns the value of the number `x` rounded to the nearest integer. |
| sign   |  x: real   | Returns the sign of the `x`, indicating whether `x` is positive, negative, or zero. |
| sin    |  x: real   | Returns the sine of `x`. |
| sinh   |  x: real   | Returns the hyperbolic sine of `x`. |
| sqrt   |  x: real   | Returns the positive square root of `x`. |
| tan    |  x: real   | Returns the tangent of `x`. |
| tanh   |  x: real   | Returns the hyperbolic tangent of `x`. |
| trunc  |  x: real   | Returns the integer portion of `x`, removing any fractional digits. |

## Access Record Information

```
+--------+------------------------+--------------------------------+
| 8 bits |       24 bits          |            32 bits             |
|--------+------------------------+--------------------------------+
|  arity |   old frame pointer    |         return address         |
+--------+------------------------+--------------------------------+
```

## Kernel functions

> 🚫 **Critical**: **DO NOT** use any of these as they need revision.

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
