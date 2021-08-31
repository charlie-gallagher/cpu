# A Virtual CPU
I've wanted to write a CPU in C for a while, and now that I've got a
microcontroller I thought it would be a good exercise. I need to keep my C
skills sharp, so I can be ready to write C functions for my microcontroller
projects. 

The CPU is composed of a few basic parts: 

- Registers
- ALU
- Instruction decoding
- Memory
- Flash memory

I'll be writing each part of this CPU in C, piece by piece. I also want to
include some extra bits like tests, if I can. I don't know much about tests in
C, so I'll have to work that out. 

I saw someone implement the registers using a struct, and I think I'll keep that
idea. 

Instruction decoding will have to be simple, or else I'll want to blow my brains
out. The important thing is to handle the different addressing modes, not a lot
of different commands. Probably, this will just be one long `switch` statement
as a function, although some more complex treatment is probably necessary to
properly handle the addressing. 

The flow of information is something I always struggle with when I write C
programs. I'll need a section devoted to figuring that out before I write any
functions. 

Timing is an important part of a CPU's functioning, but I don't see much reason
to implement clock signals. I'm not trying to get _that_ low level. But it would
be nice to be able to slow the execution way down. In fact, that's one of the
most desirable parts of this project. Let's just assume that the operations take
no time to run and break for 1 second for each instruction. I'll need a way to
define 1 instruction. How about the instruction function reads the op code, then
as a side-effect stores the appropriate number of follow-up bytes in an
instruction register (or two). 

You would have two functions. The first loads the instruction and the
instruction registers. The second reads the registers and executes the
instruction it finds there with the data it finds in the registers. Then again,
closer to what happen in the CPU is that the instruction causes the next few
bytes to be read and executed all at once, at least as I understand it. 

As for what happens when the program runs, i.e. what is in `main.c`, I haven't
decided yet. I'm leaning towards forcing the user to specify raw bytes as a
space separated stream of characters. I'm sure there's a reason that's a bad
idea, but I don't want to write a whole assembler. 

As for the ALU, this is simple arithmetic. Each instruction will do something.
But memory...

# Parts of the CPU

## Registers

- Accumulator
- Instruction register 1
- Instruction register 2
- X register
- General purpose register

## Instructions

- Store accumulator (STA)
- Load accumulator (LDA)
- LDX and STX (store and load X register)
- JMP (jump)
- INC and DEC (increment and decrement)
- CMP (compare)
- JNE (jump if not equal to zero)
- JEQ (jump if equal to zero)
- ADD and SUB

That's just off the top of my head, but I think it'll be enough to get started.
I'm getting rid of zero-page addressing because I can get away with an 8-bit
address bus. I have only a few addressing modes in mind: 

- Immediate addressing
- Direct addressing
- Indirect addressing


## Memory
128 bytes should do it, mapped in some convenient way. I'd like to support
sub-routines at some point, which means I'll need a stack, but maybe that's all
software. So, let's do 128 bytes of memory, and that's all included. To do this,
I'll `malloc` so many bytes and manage it myself. When the program terminates,
that memory location is freed. Part of decoding the instructions (raw
characters) will be locating the appropriate memory location and ensuring it's
within the valid range. 


## I/O
How else will I know whether the CPU is doing anything? At each clock, the
contents of some part of memory will be written to the terminal as characters. 


---

Charlie Gallagher, August 2021
