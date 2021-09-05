# Virtual CPU
This is a quick, informal project emulating a processor. You can set a clock
frequecy with the `CLK_FREQ` constant defined in `main.h` -- it's given in Hz.
For watching instructions, something like 1 or 2 is fun, but pretty painfully
slow. 

See `opcodes.c` for a full list of implemented opcodes, and `opcodes.h` for the
constant definitions for all opcodes I plan on implementing.

I just implemented a very basic assembler so you don't have to know the opcodes
to use the CPU. (In otherwise, it recognizes mnemonics.) It recognizes all of
the opcodes and some symbolic constants like `IO_START`, which is the section of
memory that prints after each clock cycle. 




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

This will write the first few digits of pi to the pseudo-IO device, which is
nothing more than a block of memory reserved for such purposes. 

### Assembler
The assembler also knows to filter out blank lines and comment lines that begin
with `;`. Line-end comments are not supported, nor is white space at the
beginning or end of a line. There's no way to define variables or labels, so
jump instructions can be difficult to write. 

To write a jump address, you have to count the number of instructions and
operands since the first one (zero-indexed). If you don't use blank lines or
comments, the address of a particular line is the line number minus 1.
Otherwise, you'll have to do a sort of calculation like "line number - blank
lines - comment lines - 1". 


### Instruction set
The instructions use suffixes to indicate the addressing mode.

- "D" Direct, or absolute, addressing
- "I" Indirect addressing
- "M" Immediate addressing

Notably, I haven't written any instructions for the stack, so if you want that
type of behavior you'll have to implement it in software. 

```
ADD_D       Add number to accumulator
ADD_I
ADD_M
AND_D       AND operand with accumulator
AND_I
AND_M
CMP_D       Compare operand to accumulator
CMP_I
DEC_D       Decrement place in memory
DEC_I
DECX        Decrement X register
HLT         Halt
INC_D       Increment place in memory
INC_I
INCX        Increment X register
JEQ_D       Jump if zero flag set
JEQ_I
JEQ_M
JMP_D       Unconditional jump
JMP_I
JMP_M
JNE_D       Jump if zero flag not set
JNE_I
JNE_M
LDA_D       Load accumulator
LDA_I
LDA_M
LDX_D       Load X register
LDX_M
OR_D        OR operand with accumulator
OR_I
OR_M
STA_D       Store accumulator
STA_I
SUB_D       Subtract number from accumulator
SUB_I
SUB_M
```

---

Charlie Gallagher, August 2021
