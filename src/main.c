#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"
#include "opcodes.h"
#include "load_ram.h"
#include "labels.h"
#include "preprocessor.h"



char tmp_file[L_tmpnam];

int main(int argc, char **argv)
{
    	unsigned char ram[RAM_SIZE];
	// Temporary filename string
	tmpnam(tmp_file);
	#ifdef DEBUG
	printf("Temporary filename: %s\n", tmp_file);
	#endif

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
	atexit(remove_tmpfile);

	/* Process labels */
	load_labels(p_cli_args, p_labels);
	print_labels(p_labels);

	/* Load RAM with data */
	load_ram(ram, p_cli_args, p_labels);
	print_ram(ram);


	printf("Program initialized! Running program in 2 seconds...\n");
	sleep(2);


	/* Run program */
	cpu_loop(p_regis, ram);


	printf("Final RAM dump\n------\n");
	print_ram(ram);

	remove(tmp_file);
    
	return 0;
}



/* Main loop for CPU operation
 *
 * @param p_regis Pointer to register struct
 * @param ram Pointer to array representing address space
 * 
 * @return Status code, 0 for success and non-zero for failure
 */
int cpu_loop(struct register_struct *p_regis, unsigned char *ram)
{
	int halt_flag = 0;
	// Get clock frequency
	float clk_seconds = 1 / (float) CLK_FREQ;
	double clk_microseconds = clk_seconds * 1000000;

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

			print_registers(p_regis);
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
	registers->mar = 0;	// Not used
	registers->index = 0;
	registers->y = 0;	// Not used
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


/* Print help and exit
 */
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
	printf("RAM\n--------\n");
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


/* Get next byte from ram
 */
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

/* Remove temporary file
 *
 * Removes temporary file by accessing it globally
 */
void remove_tmpfile(void)
{
	remove(tmp_file);
}

/* Print registers and status flags
 */
void print_registers(struct register_struct *registers)
{
	putchar('\n');

	printf("A     X     Status: Z     N     C\n");
	printf("----- -----         ----- ----- -----\n");
	printf("%02Xh   %02Xh           %d     %d     %d\n", registers->accum, registers->index,
					(registers->status & STATUS_ZERO_MASK) > 0,
					(registers->status & STATUS_NEG_MASK) > 0,
                                     	(registers->status & STATUS_CARRY_MASK) > 0);
	putchar('\n');
}


