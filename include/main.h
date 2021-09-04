#ifdef MAIN_CPU
#else 
#define MAIN_CPU
#define RAM_SIZE 256
#define CLK_FREQ 5
#define IO_START 200
#define INSTRUCT_PASS 254


// Struct definitions
struct register_struct {
    unsigned char instruction;   // Instruction register
    unsigned char accum;   	// Accumulator
    unsigned char mar;           // Memory address register
    unsigned char index;         // Index register
    unsigned char gp;            // General purpose register
    unsigned char pc;            // Program counter
    unsigned char status;        // Status register
    unsigned char sp;            // Stack pointer
};

struct cli_struct {
    char *input_file;
};

// Function definitions
struct register_struct *init_register_struct(struct register_struct *registers);
struct cli_struct *parse_cli(struct cli_struct *cl_args, int argc, char **argv);
void print_ram(unsigned char ram[]);
void print_io(unsigned char ram[]);
char get_next_byte(unsigned char *ram, unsigned char pc);

#endif
