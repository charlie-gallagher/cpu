# Virtual CPU
This is a quick, informal project emulating a processor. You can set a clock
frequecy with the `CLK_FREQ` constant defined in `main.h` -- it's given in Hz.
For watching instructions, something like 1 or 2 is fun, but pretty painfully
slow. 

I recently implemented the zero flag, so it might not behave perfectly. I have
one unconditional `JMP` instruction and two conditional jumps (`JEQ` and `JNE`).
I also have implemented loading and storing the accumulator and the index
register. See `opcodes.c` for a full list of implemented opcodes, and
`opcodes.h` for the constant definitions for all opcodes I plan on implementing.




### Compile
This program depends on the `unistd.h` header file, so it can't be compiled on
Windows. 


```
gcc -I include -o cpu src/*.c
```

### Example
Although I haven't implemented an input file, the program expects an argument
when you call it. 

```
./cpu file
```

`file` doesn't have to exist (yet). The default bytes will print "relynn Hess"
(thanks to an off-by-one error) to the "output" device, and it runs at something
like 2 Hz. 

---

Charlie Gallagher, August 2021
