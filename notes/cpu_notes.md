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

Let's make another pass at `main` and see if I can tighten it up a little. 

```c
#define RAM_SIZE 256
#define CLK_FREQ 1


// Struct definitions
struct register_struct {
    char instruction;   // Instruction register
    char accum;         // Accumulator
    char mar;           // Memory address register
    char index;         // Index register
    char gp;            // General purpose register
    char pc;            // Program counter
    char status;        // Status register
    char sp;            // Stack pointer
};

struct cli_struct {
    char **input_file;
};

// Function definitions
struct register_struct *init_register_struct(struct register_struct *registers);
struct cli_struct *parse_cli(int argc, char **argv);
int load_ram(char *ram, struct cli_struct *cli);
char get_next_byte(char *ram, char pc);
int execute_instruction(char instruction, char pc1, char pc2);


int main(int argc, char **argv)
{
    char ram[RAM_SIZE];
    char *p_ram = ram;
    struct register_struct regis;
    struct register_struct *p_regis = &regis;
    struct cli_struct cli_args;
    struct cli_struct *p_cli_args = &cli_args;

    // Initialization
    p_regis = init_register_struct(p_regis);
    cli_args = parse_cli(argc, argv);
    load_ram(ram, p_cli_args);
    
    // Run instructions
    while (1) {
        // Fetch instruction
        if (p_regis->instruction == NULL) {
            p_regis->instruction = get_next_byte(ram, p_regis->pc);
            p_regis->pc++;
        } 
        
        // Fetch next two bytes
        else {
            execute_instruction(p_regis->instruction, pc, pc+1);
            p_regis->instruction = NULL;
            p_regis->pc += 2;
            print_ram(ram);
            print_io(ram, IO_START);
            sleep(1 / CLK_FREQ);
        }
        
    }
}
```

I'll define my own unsigned character type, which should have a size of 1 byte
regardless of the architecture. Anyway, I think this is looking very good. I
have inputs and returns for the main functions, and the flow of information
looks natural enough. 

There's plenty of work left to do. At no point do I check the status register,
for example. Also, there's generally no error checking here. Each function will
itself be made of many different functions, too, so I'll have to manage that. 

Note that I also added a `print_ram` function and a `print_io` function. The
former prints the contents of RAM as hex values (a hex dump); the latter prints
the contents of the input output section of memory as ASCII text. 

### Steps
I've implemented the code above in C, with some extra bits for testing.
Everything's working right at the moment, which is honestly really exciting.
Now, I just need to write everything else that goes with the program. 

The good news is, I don't need to dynamically allocate any memory, since all
sizes are known in advance. That's great. I still need to: 

- Read the file and write it to "memory" (ram array)
- Decide on actual op-codes and implement each one
  - And figure out the structure of this part of the program to boot

I guess that's really it. Getting the instructions to work is a priority,
followed by reading the file and parsing the command line with any options I
want to add. Will I allow any options? One option should be to run an example
program. 

First, op-codes, second, example, and third reading any file from the user. 

### Op-codes
I've started a Google Sheet with all the instructions I want to support. See
that for more information. As this develops I might incorporate more
instructions, but I have 32 as it is, and that's a lotta switch statments. 


# Labels
It's been a while since I took notes in here, and the program as I planned it is
done. Now, I want to add some fancier features such as labels and subroutines.
A label is simply a marker for a location in memory. Also, I want to be able to
define variables. That should be pretty easy. 

I'm using the assembly simulator I found as a reference example. A label is
attached to a location in memory, whether that's an instruction or a variable.
The instruction may appear on the same line or on the next line as the label
itself. I think I'll simplify that to requiring it to be before the instruction. 

Now, this assembler is based on x86, and it behaves a little differently than
the one I'm using. It doesn't have an accumulator, only GP registers. Speaking
of which, I should really start using my GP register. 

Anyway, the functioning would look something like this: 

```
label: 
    LDA_M
    1F

hello:
    DB
    "Hello world"
```

When the assembler encounters the label, it adds it to an array of labels. I
need some sort of hash table, key-value pairs. Maybe it's enough to implement
this with a struct composed of two arrays. Let me worry about that later. First,
the instruction `LDA_M` and its operand `1F` are loaded into memory locations 00
and 01. Then, in location 02 goes the `DB` instruction, and the quotation marks
indicate that a string follows. Thus, the appropriate ASCII values for 'H', 'e',
'l', 'l', 'o', etc. are placed in locations 03-0E (or around there) and the next
values are loaded into memory sequentially after that. 

So there are two to-dos: 

1. Implement label data structure
2. Process strings sequentially

I think there's nothing stopping me from setting up a label data structure, but
I'm not set up to process strings. The assembler assumes each line can be
converted to a single integer. What I can do is process characters like 'c' and
convert them into ASCII. That should be easy enough. 



---

Charlie Gallagher, August 2021
