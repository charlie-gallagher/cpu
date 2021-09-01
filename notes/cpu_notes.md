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
bytes to be read and executed all at once, at least as I understand it. Recall
that these are usually divided into the "instruction cycle" and the "execution
cycle". 

As for what happens when the program runs, i.e. what is in `main.c`, I haven't
decided yet. I'm leaning towards forcing the user to specify raw bytes as a
space separated stream of characters. I'm sure there's a reason that's a bad
idea, but I don't want to write a whole assembler. 

As for the ALU, this is simple arithmetic. Each instruction will do something.
But memory...

# Parts of the CPU

## Registers

- Accumulator
- Instruction register
- Memory address register
- Program counter
- Index register
- General purpose register
- Status register
- Stack pointer

The reset vector is like a register... I'll have to manage startup. That's worth
thinking about for a minute. 

### Data structure
Should the registers be individual elements of the struct, or one element of
the struct implemented as a list? That depends on what else I'll have to store
in the struct. At the moment, I'm wondering whether to implement the memory as a
global or an element of the struct. It's small, but as an element of the struct
it won't scale. 

### Simple registers
The accumulator, program counter, index register, and general purpose register
are simple registers, I think. They will contain data. The stack pointer is
similar. I'll have to refresh myself with the function of the stack before
trying to implement it, but it's a pretty basic LIFO data structure with POP and
PUSH operations. 

### Complex registers
The instruction register, memory address register, and status register are less
simple, because they're involved in some automated tasks. The instruction
register stores the instruction read during the Instruction cycle. The memory
address register stores the address the previous instruction called for. I might
decode the various addressing modes when I load the MAR, and store only the real
address. The status register contains flags such as the zero flag. The full list
of flags I plan to implement is as follows:

- Carry flag
- Zero flag
- Overflow flag
- Negative flag

How will I deal with overflow? How does C handle overflow? It throws away the
extra. I think I'll keep this behavior, but I'll have to find a way to set the
overflow flag. Hm. That sounds difficult. 

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
- RST (reset)

That's just off the top of my head, but I think it'll be enough to get started.
I'm getting rid of zero-page addressing because I can get away with an 8-bit
address bus. I have only a few addressing modes in mind: 

- Immediate addressing
- Direct addressing
- Indirect addressing

## Leave room for Assembly
I am not planning on writing an assembler right now. That's a pretty big lift,
and my C is just too rusty. Also, it's a lot of manual work. But, maybe one day
I will want to implement assembly, so I have to make sure I make it modular. It
will be in the initialization stage of the processor, when the program is being
loaded into memory. The module will input something and return the CPU's memory
loaded with the proper bytes. 

I haven't thought too much about input files yet. I was planning on using
characters rather than bytes, so I'll need a hex to integer converter. I suppose
what I'll do is conver a text file of op codes into raw bytes, and then I'll
define all instructions as symbolic constants. During the `switch` statement, it
will look for which symbolic constant was used. 

## Memory
256 bytes should do it, mapped in some convenient way. I'd like to support
sub-routines at some point, which means I'll need a stack, but maybe that's all
software. So, let's do 256 bytes of memory, and that's all included. To do this,
I'll `malloc` so many bytes and manage it myself. When the program terminates,
that memory location is freed. Part of decoding the instructions (raw
characters) will be locating the appropriate memory location and ensuring it's
within the valid range. 


## I/O
How else will I know whether the CPU is doing anything? At each clock, the
contents of some part of memory will be written to the terminal as characters. 


# Program Design

```c
int main(int argc, char **argv)
{
    // Initialization
    ram = malloc(RAM_SIZE);
    register_struct = make_register_struct();
    cli_struct = parse_cli();
    load_ram(ram, cli_struct);
    
    // Run instructions
    while (1) {
        // Fetch instruction
        if (register_struct.instruction == NULL) {
            registr_struct.instruction = get_byte(ram, register_struct.pc);
            register_struct.pc++;
        } 
        
        // Fetch next two bytes
        else {
            execute_instruction(register_struct.instruction, pc, pc+1);
            register_struct.instruction = NULL;
            register_struct.pc += 2;
            wait(1 / CLK_FREQ);
        }
        
    }
    
}
```

This pseduocode implements the flow of information as I see it right now.
Hopefully it's accurate enough to make glaring errors obvious. On the
instruction cycle, the instruction register begins as `NULL` and gets populated
with an instruction op code. 

On the next cycle, the execution cycle, the instruction gets executed with the
next two bytes, the instruction register is cleared, the PC is incremented and
the program waits for 1 second. I should plan now to make the clock frequency
variable. Adjusted it to 1 over the clock frequency. The `wait()` function units
can be fiddled with. I also forget what the function is called. 

I think this will work, and it will leave me room to add more functionality
later. I'll have to brush up on my command line parsing, but that's not too bad. 

Things to brush up on: 

- getopt




---

Charlie Gallagher, August 2021