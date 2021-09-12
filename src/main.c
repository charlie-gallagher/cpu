#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"
#include "opcodes.h"
#include "load_ram.h"
#include "labels.h"
#include "preprocessor.h"




int main(int argc, char **argv)
{
    	unsigned char ram[RAM_SIZE];
	char *tmp_file = tmpnam(NULL);
	printf("Temporary filename: %s\n", tmp_file);
	int halt_flag = 0;
    	struct register_struct regis;
    	struct register_struct *p_regis = &regis;
	struct labels labels;
	struct labels *p_labels = &labels;
    	struct cli_struct cli_args;
    	struct cli_struct *p_cli_args = &cli_args;

	// Initialization
	p_regis = init_register_struct(p_regis);
	p_labels = init_labels(p_labels);

	if ((p_cli_args = parse_cli(p_cli_args, argc, argv)) == NULL) {
		exit(-1);
	}

	/* Run preprocessor */
	p_cli_args->input_file = preprocess(tmp_file, p_cli_args->input_file);


	/* Processing labels */
	load_labels(p_cli_args, p_labels);

	printf("Printing labels\n--------\n");
	print_labels(p_labels);

	/* Loading RAM with data */
	load_ram(ram, p_cli_args, p_labels);
	print_ram(ram);

	// Initialize clock
	float clk_seconds = 1 / (float) CLK_FREQ;
	double clk_microseconds = clk_seconds * 1000000;

	printf("Program initialized! Running program in 2 seconds...\n");
	sleep(2);





	while (1) {

		// Fetch instruction
		if (p_regis->instruction == INSTRUCT_PASS) {
			printf("Program counter (instruction): %Xh\n", p_regis->pc);
			p_regis->instruction = get_next_byte(ram, p_regis->pc);

			#ifdef DEBUG
			printf("Instruction: %Xh\n", p_regis->instruction);
			#endif

			p_regis->pc++;
		} 

		// Fetch operand
		else {
			#ifdef DEBUG
			printf("Execution cycle\n");
			#endif
			halt_flag = execute_instruction(p_regis->instruction, 
					p_regis->pc, 
					p_regis,
					ram);
			p_regis->instruction = INSTRUCT_PASS;
			p_regis->pc++;


			printf("Accumulator: %d\n", p_regis->accum);
			printf("Index: %d\n", p_regis->index);
			printf("Zero flag: %d\n", p_regis->status & STATUS_ZERO_MASK);
			printf("Negative flag: %d\n", p_regis->status & STATUS_NEG_MASK);

			print_ram(ram);

			printf("I/O Print\n------\n");
			print_io(ram);

			if (halt_flag == 1) {
				printf("Halting!\n");
				break;
			} else if (halt_flag == -1) {
				break;
			}


			printf("Sleeping for %0.3f seconds...\n", clk_seconds);
			usleep(clk_microseconds);
		}

	}


	printf("Final RAM dump\n------\n");
	print_ram(ram);

	remove(tmp_file);
    
	return 0;
}







/* Initialize register struct
 *
 * 	registers	Register struct pointer
 *
 * Returns a pointer to the register struct 
 */
struct register_struct *init_register_struct(struct register_struct *registers)
{
	registers->instruction = INSTRUCT_PASS;
	registers->accum = 0;
	registers->mar = 0;
	registers->index = 0;
	registers->gp = 0;
	registers->pc = 0;
	registers->status = 0;
	registers->sp = 0;

	return registers;
}


/* Parse command line
 *
 * 	cl_args		Pointer to command line struct
 * 	argc		argc
 * 	argv		argv
 *
 * Returns a pointer to the command line struct
 */
struct cli_struct *parse_cli(struct cli_struct *cl_args, int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Incorrect number of arguments\n");
		print_help();
		exit(0);
	}

	// Handle help and version arguments
	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		print_help();
		exit(0);
	} else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
		printf("cpu, Version %d.%d.%d\n", MAJOR_VERSION, MINOR_VERSION, PATCH);
		exit(0);
	}


	cl_args->input_file = argv[1];

	return cl_args;
}


void print_help(void)
{
	printf("Usage: cpu [-h|--help] [-v|--version] FILE\n");
	printf("\nA basic 6502-based CPU simulator\n");
	printf("\nReport bugs to charlesjgallagher15@gmail.com\n");
}



/* Print RAM array
 */
void print_ram(unsigned char ram[])
{
	int i;
	for (i = 0; i < RAM_SIZE; i++) {
		printf("%02X ", ram[i]);
		
		if (i % 16 == 15) {
			putchar('\n');
		}
	}
	putchar('\n');
}

/* Print IO array
 */
void print_io(unsigned char ram[])
{
	int i;

	for (i = IO_START; i < RAM_SIZE; i++) {
		printf("%c", ram[i]);
		
		if (i % 20 == 79) {
			putchar('\n');
		}
	}
	putchar('\n');
	putchar('\n');
}


char get_next_byte(unsigned char *ram, unsigned char pc)
{
	return ram[pc];
}


/* Set all 20 arrays of 80 to nulls */
struct labels *init_labels(struct labels *labs)
{
	int i, j;

	for (i = 0; i < 20; i++) {
		// Initialize names
		for (j = 0; j < 80; j++) {
			labs->name[i][j] = '\0';
		}

		// Initialize addresses
		labs->addr[i] = 0;
	}

	return labs;
}
