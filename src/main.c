#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "opcodes.h"




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



	/* Load RAM and registers with some phony stuff */
	ram[0] = LDA_M;
	ram[1] = 10;
	ram[2] = STA_D;
	ram[3] = 90;
	ram[4] = LDX_M;
	ram[5] = 0;
	// Unconditional loop to print "Hello" in IO
	ram[6] = LDA_I;
	ram[7] = 20;
	ram[8] = STA_I;
	ram[9] = IO_START;
	ram[10] = INCX;
	ram[11] = 0; // No-op for fixed width instruction with no operand
	ram[12] = JMP;
	ram[13] = 6;
	
	// Hello
	ram[20] = 'B';
	ram[21] = 'r';
	ram[22] = 'e';
	ram[23] = 'l';
	ram[24] = 'y';
	ram[25] = 'n';
	ram[26] = 'n';
	ram[27] = ' ';
	ram[28] = 'H';
	ram[29] = 'e';
	ram[30] = 's';
	ram[31] = 's';


    	// ram
	printf("Loaded RAM\n------\n");
	for (i = 0; i < RAM_SIZE; i++) {
		printf("%d ", ram[i]);
		
		if (i % 16 == 15) {
			putchar('\n');
		}
	}
	putchar('\n');

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


			printf("I/O Print\n------\n");
			print_io(ram);

			if (halt_flag == 1) {
				printf("Halting!\n");
				break;
			} else if (halt_flag == -1) {
				break;
			}

			printf("Sleeping for 1 second...\n");
			sleep(1 / CLK_FREQ);
		}

	}


	printf("Final RAM dump\n------\n");
	for (i = 0; i < RAM_SIZE; i++) {
		printf("%d ", ram[i]);
		
		if (i % 16 == 15) {
			putchar('\n');
		}
	}
	putchar('\n');
    
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


/* Load ram
 *
 * 	ram		Memory array acting as RAM
 * 	cli		Pointer to command line struct
 */
int load_ram(unsigned char ram[], struct cli_struct *cli)
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




