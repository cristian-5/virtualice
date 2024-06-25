
# Virtualice VM

Stack based vm for super fast execution.
It currently supports the following features:

✅ → signed (**integers**) and unsigned (**natural**) numbers\
✅ → floating point (**real**) numbers\
✅ → **complex** numbers\
✅ → **control flow** jump and compare instructions\
✅ → **stack based** arithmetic operations on pure types\
✅ → **bitwise** and **boolean** operations\
✅ → *global* and *local* variables\
✅ → **functions** with parameters\
✅ → opcodes for a comprehensive **math** library\
✅ → **exceptions** handling, flag register\
✅ → **memory**, [page model](https://en.wikipedia.org/wiki/Memory_paging)\
🔘 → **threads**, [fork-join model](https://en.wikipedia.org/wiki/Fork%E2%80%93join_model)\
🔘 → **file system**, [unix model](https://en.wikipedia.org/wiki/Unix_filesystem)\
🔘 → **files**, [posix model](https://en.wikipedia.org/wiki/Unistd.h)\
🔘 → **networks**, (probably [Berkeley sockets](https://en.wikipedia.org/wiki/Berkeley_sockets))



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

|HEX | Prototype           | Description                                       |
|:--:|:--------------------|:--------------------------------------------------|
|`DE`|`debug()`            |*Prints the stack and the call frame to `stdout`.* |
|`EC`|`exit(c: u32)`       |*Exits with code `c`.*                             |
|`71`|`time(): u64`        |*Returns the current unix timestamp.*              |
|`5E`|`seed(s: u64)`       |*Sets the seed `s` for the next random generation.*|
|`3A`|`random(): u64`      |*Generates a pseudo random number using a pcg.*    |

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
