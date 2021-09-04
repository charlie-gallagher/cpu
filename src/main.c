#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "opcodes.h"
#include "load_ram.h"




int main(int argc, char **argv)
{
    	unsigned char ram[RAM_SIZE];
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
	print_ram(ram);


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


	while (1) {
		int halt_flag = 0;

		printf("Program counter: %d\n", p_regis->pc);

		// Fetch instruction
		if (p_regis->instruction == INSTRUCT_PASS) {
			printf("Instruction cycle\n");
			p_regis->instruction = get_next_byte(ram, p_regis->pc);
			printf("Instruction: %d\n", p_regis->instruction);
			p_regis->pc++;
		} 

		// Fetch next two bytes
		else {
			printf("Execution cycle\n");
			halt_flag = execute_instruction(p_regis->instruction, 
					p_regis->pc, 
					p_regis,
					ram);
			p_regis->instruction = INSTRUCT_PASS;
			p_regis->pc++;


			printf("Accumulator: %d\n", p_regis->accum);
			printf("Index: %d\n", p_regis->index);
			printf("Zero flag: %d\n", p_regis->status & STATUS_ZERO_MASK);

			print_ram(ram);

			printf("I/O Print\n------\n");
			print_io(ram);

			if (halt_flag == 1) {
				printf("Halting!\n");
				break;
			} else if (halt_flag == -1) {
				break;
			}

			float clk_seconds = 1 / (float) CLK_FREQ;
			double clk_microseconds = clk_seconds * 1000000;

			printf("Sleeping for %f seconds...\n", clk_seconds);
			usleep(clk_microseconds);
		}

	}


	printf("Final RAM dump\n------\n");
	print_ram(ram);
    
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
		fprintf(stderr, "Incorrect argument\n");
		return NULL;
	}

	cl_args->input_file = argv[1];

	return cl_args;
}




/* Print RAM array
 */
void print_ram(unsigned char ram[])
{
	int i;
	for (i = 0; i < RAM_SIZE; i++) {
		printf("%d ", ram[i]);
		
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

	printf("I/O device printing...\n");
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




