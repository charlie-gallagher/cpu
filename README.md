# Virtual CPU
This is a quick, informal project emulating a processor. You can set a clock
frequecy with the `CLK_FREQ` constant defined in `main.h` -- it's given in Hz.
For watching instructions, something like 1 or 2 is fun, but pretty painfully
slow. 

See `opcodes.c` for a full list of implemented opcodes, and `opcodes.h` for the
constant definitions for all opcodes I plan on implementing.

I've implemented a basic assembler that understands mnemonics, comments, and
labels. It's not smart enough to figure out addressing modes, so these are
included in the mnemonics explicitly. See the section "Assembler" below.



### Compile
This program depends on the `unistd.h` header file, so it can't be compiled on
Windows. 


```
gcc -I include -o cpu src/*.c

# Or just make
make
```

### Example
I've included a few test files, which are the best way to run this for the first
time.

```
./cpu test/test_file.txt
./cpu test/subroutine_test.txt
```

They are good examples of the structure of the programs you can write for this
CPU. Some of the other files are for testing various features, trying to trick
the compiler and so on. 

### Assembler
The assembler is basic. The user is responsible for writing machine
code instructions using mnemonics of the form "INSTRUCTION\_MODE". MODE can be
one of "D", "I", or "M" for Direct, Indirect, and iMmediate address modes. The
operands may be in hex (format `xxh`, e.g. `4fh`) or decimal if the hex format
is not used. So, for example, `25` will be read as decimal, `2fh` as
hexadecimal, and `2f` as `2`, because `atoi` is used to convert numbers.
Alphabetical hex characters may be uppercase or lowercase. 

You can add up to 20 labels per file. I can't imagine you being able to use many
more than this when you only have 256 bytes of memory. The array of labels
is printed at startup so you can troubleshoot if anything goes wrong. Labels are
most useful for subroutines, jumping to the start of the program, and defining
variables. See `test/subroutine_test.txt` for an example. Labels are processed
before the instructions, so they can be used before they are defined. 

```
JMP
start

; Define some subroutines and data
mysubroutine:
    ...

start:
    ...   ; Start of program
```

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

Conditional branch instructions go to address PC + OPERAND, so to jump backwards
you must pass a negative number or the appropriate hex conversion for a 1-byte
2's complement number. Conditional jumps are also relative to the instruction,
not the operand. Unconditional jumps use absolute addressing. 

```
LDA_M
10
STA_I
IO_START
BNE
-4      ; Branch back to "LDA_M"
```

Every instruction takes an operand, even implied addressing instructions like
`INCX`. If you use `INCX`, you must follow it with a null byte (or any other
byte, but for clarity's sake use a null byte). 

The types of input allowed are: 

- Mnemonics (see next section for instruction set)
- Decimal numbers (no suffix)
- Hexadecimal numbers (`h` suffix)
- The `IO_START` builtin constant
- Character constants surrounded by single quotes (e.g., `'m'` or `'c'`)
- A label definition of the form `[A-Za-z_]+:` (e.g., `a:` or `hello_world:`)
- A label name defined in your file

Labels may include characters or underscores (`_`). They are attached to the
address of the next RAM entry, so you won't run into trouble if you define
multiple labels for a single address. 

```
one:
two:
    LDA_M
    10h
```

In this case, both `one` and `two` will be associated with address `0` in RAM,
a.k.a. the instruction `LDA_M`. Labels of course do not affect the arrangement
of memory in RAM. 


### Instruction set
The instructions use suffixes to indicate the addressing mode.

- "D" Direct, or absolute, addressing
- "I" Indirect addressing
- "M" Immediate addressing

I have tested many but not all of the instructions. Report bugs if you find
them. 

```
ADC_D       Add number to accumulator
ADC_I
ADC_M
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
JMP         Unconditional jump
/* NOTE: Conditional jumps go to address PC + OPERAND
BEQ         Jump if zero flag set
BNE         Jump if zero flag not set
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
SBC_D       Subtract number from accumulator
SBC_I
SBC_M
SEC         Set carry flag
CLC         Clear carry flag
JSR         Jump to subroutine at operand address
RTS         Return from subroutine
PHA         Push accumulator onto stack
PHP         Push status register onto stack
PLA         Pull accumulator from stack
PLP         Pull status register from stack
TXA         Transfer from index to accumulator...
TAX         ...and vice versa
TXS         Transfer from index to stack pointer...
TSX         ...and vice versa
```

## Known problems and deficiencies
While this is close to the Motorolla 6502 instruction set, it is not a copy of
it. 

- Only the negative and zero status flags are implemented, but besides
  conditional branch instructions there's no way to test these
- The 6502 has two registers besides the accumulator, X and Y, but I only
  included X (index register)
- All instructions must be 2 bytes, even if they do not take an operand
- I haven't implemented bit shift or testing instructions (yet)



Report bugs to charlesjgallagher15@gmail.com.

---

Charlie Gallagher, September 2021
