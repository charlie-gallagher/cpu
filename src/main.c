#include <stdio.h>
#include <stdlib.h>

#define RAM_SIZE 256
#define CLK_FREQ 1
#define IO_START 200


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
    char *input_file;
};

// Function definitions
struct register_struct *init_register_struct(struct register_struct *registers);
struct cli_struct *parse_cli(struct cli_struct *cl_args, int argc, char **argv);
int load_ram(char ram[], struct cli_struct *cli);


int main(int argc, char **argv)
{
    	char ram[RAM_SIZE];
    	struct register_struct regis;
    	struct register_struct *p_regis = &regis;
    	struct cli_struct cli_args;
    	struct cli_struct *p_cli_args = &cli_args;

	// Initialization
	p_regis = init_register_struct(p_regis);
	if ((p_cli_args = parse_cli(p_cli_args, argc, argv)) == NULL) {
		exit(-1);
	}
	load_ram(ram, p_cli_args);

	/* Test */
	int i; 

    	// registers
	printf("Testing registers\n------\n");
	printf("Instruction: %d\n", p_regis->instruction);
	printf("Accumulator: %d\n", p_regis->accum);
	printf("MAR: %d\n", p_regis->mar);
	printf("Index: %d\n", p_regis->index);
	printf("GP: %d\n", p_regis->gp);
	printf("PC: %d\n", p_regis->pc);
	printf("Status: %d\n", p_regis->status);
	printf("SP: %d\n", p_regis->sp);

	printf("\n\n");

    	// CLI
	printf("Testing CLI\n------\n");
	printf("Filename: %s\n", p_cli_args->input_file);

	printf("\n\n");

    	// ram
	printf("Testing RAM\n------\n");
	for (i = 0; i < RAM_SIZE; i++) {
		printf("%d ", ram[i]);
		
		if (i % 10 == 9) {
			putchar('\n');
		}
	}
	putchar('\n');
    
    return 0;
}


struct register_struct *init_register_struct(struct register_struct *registers)
{
	registers->instruction = NULL;
	registers->accum = 0;
	registers->mar = 0;
	registers->index = 0;
	registers->gp = 0;
	registers->pc = 0;
	registers->status = 0;
	registers->sp = 0;

	return registers;
}


struct cli_struct *parse_cli(struct cli_struct *cl_args, int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Incorrect argument\n");
		return NULL;
	}

	cl_args->input_file = argv[1];

	return cl_args;
}


int load_ram(char ram[], struct cli_struct *cli)
{
	/* Temporarily, going to ignore the struct and 
	 * load ram with all zeros
	 */
	int i;

	for (i = 0; i < RAM_SIZE; i++) {
		ram[i] = 0;
	}

	return 0;
}
