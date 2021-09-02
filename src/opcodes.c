#include <stdio.h>
#include "main.h"
#include "opcodes.h"
/* Opcode execution
 *
 */

/* Execute instruction
 *
 * 	instruction		1 byte opcode
 * 	pc1			Address of operand
 *
 * Returns a status code, 0 for success. 
 */
int execute_instruction(unsigned char instruction, unsigned char pc1, 
		struct register_struct *registers, unsigned char ram[])
{
	switch (instruction) {
		case LDA_D: 
		{
			registers->accum = ram[ram[pc1]];
		    	break;
	    	}
		case LDA_I: 
		{
			registers->accum = 
			    ram[ram[pc1 + registers->index]];
		    	break;
		}
		case LDA_M: 
		{
			printf("Loading accumulator (immediate)\n");
			printf("Memory value: %d\n", ram[pc1]);
		    	registers->accum = ram[pc1];
			break;
		}
		case STA_D: 
		{
			ram[ram[pc1]] = registers->accum;
			break;
		}
		case STA_I: 
		{
			ram[pc1] = registers->accum;
			break;
		}
		case STA_M:
		{
			ram[ram[pc1 + registers->index]] = registers->accum;
			break;
		}
		case HLT:
		{
			printf("Halting execution\n");
			return HLT;
		}
		default: 
		{
			fprintf(stderr, "Execution error: Unknown instruction\n");
			return -1;
		}
	}


	return 0;
}
