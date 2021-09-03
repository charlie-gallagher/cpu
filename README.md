# Virtual CPU
This is a quick, informal project emulating a processor. You can set a clock
frequecy with the `CLK_FREQ` constant defined in `main.h` -- it's given in Hz.
For watching instructions, something like 1 or 2 is fun, but pretty painfully
slow. 

See `opcodes.c` for a full list of implemented opcodes, and `opcodes.h` for the
constant definitions for all opcodes I plan on implementing.

I just implemented a very basic assembler so you don't have to know the opcodes
to use the CPU. It recognizes all of the opcodes and some symbolic constants
like `IO_START`, which is the section of memory that prints after each clock
cycle. 

In progress: 

- Code comments




### Compile
This program depends on the `unistd.h` header file, so it can't be compiled on
Windows. 


```
gcc -I include -o cpu src/*.c
```

### Example
I've included a test file, which is the best way to run this for the first time
so you know what to expect. 

```
./cpu test/test_file.txt
```

This will write "Brelynn Hess" to the pseudo-IO device, which is nothing more
than a block of memory reserved for such purposes. 


---

Charlie Gallagher, August 2021
