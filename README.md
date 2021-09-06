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
memory that prints after each clock cycle. This is defined in `include/main.h`,
so you can set your own value there. Currently, it's set to the last 16 bytes in
RAM. 



### Compile
This program depends on the `unistd.h` header file, so it can't be compiled on
Windows. 


```
gcc -I include -o cpu src/*.c

# Or just make
make
```

### Example
I've included a test file, which is the best way to run this for the first time.

```
./cpu test/test_file.txt
```

This will write the first few digits of pi to the pseudo-IO device, which is
nothing more than a block of memory reserved for such purposes. 

### Assembler
The assembler is extremely basic. The user is responsible for writing machine
code instructions using mnemonics of the form "INSTRUCTION\_MODE". MODE can be
one of "D", "I", or "M" for Direct, Indirect, and iMmediate address modes. The
operands may be in hex (format `xxh`, e.g. `4fh`) or decimal if the hex format
is not used. So, for example, `25` will be read as decimal, `2fh` as
hexadecimal, and `2f` as `2`, because `atoi` is used to convert numbers.
Alphabetical hex characters may be uppercase or lowercase. 

Comments begin with `;` and may be included on a line by itself or at the end of
a line. Any part of a line after the ";" is dropped during processing. 

Whitespace is also stripped from the lines, so you can indent lines for clarity. 

Instructions are placed in the first addresses in RAM. Every operation is 2
bytes (1 byte instruction, 1 byte operand), and blank lines and comment lines do
not affect the numbering of address locations. So, for example,

```
; Begin program
LDA_M  ; Load accumulator
2fh

; Store value in 3F
STA_D  ; Store accumulator
3Fh
```

will be entered into RAM as

```
0: LDA_M
1: 2fh
2: STA_D
3: 3fh
```

(Note: there's no typo in the first block, you can use uppercase or lowercase
characters in your hex.)

Thus, to write a jump address, you have to count the number of instructions and
operands since the first one (zero-indexed). If you don't use blank lines or
comments, the address of a particular line is the line number minus 1.
Otherwise, you'll have to do a sort of calculation like "line number - blank
lines - comment lines - 1". 

Every instruction takes an operand, even implied addressing instructions like
`INCX`. If you use `INCX`, you must follow it with a null byte (or any other
byte, but for clarity's sake use a null byte). 

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

Charlie Gallagher, September 2021
