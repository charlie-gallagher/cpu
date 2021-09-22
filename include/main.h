#ifdef MAIN_CPU
#else 
#define MAIN_CPU
#define RAM_SIZE 0x100
#define CLK_FREQ 5
#define IO_START 0xF0
#define INSTRUCT_PASS 0xFE

#define MAJOR_VERSION 0
#define MINOR_VERSION 3
#define PATCH 5

// Uncomment for exra debugging print statements
// #define DEBUG


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

struct labels {
	char name[20][80];  // (labels.name)[0] should point to first label
	int addr[20];
};


// Function definitions
struct register_struct *init_register_struct(struct register_struct *registers);
struct cli_struct *parse_cli(struct cli_struct *cl_args, int argc, char **argv);
struct labels *init_labels(struct labels *labs);
void print_help(void);
void print_ram(unsigned char ram[]);
void print_io(unsigned char ram[]);
char get_next_byte(unsigned char *ram, unsigned char pc);
void remove_tmpfile(void);
void print_registers(struct register_struct *registers);

#endif
