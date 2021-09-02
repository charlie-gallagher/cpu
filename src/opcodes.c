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
 * Returns 1 for HALT
 * Returns -1 for an instruction error (unknown instruction)
 */
int execute_instruction(unsigned char instruction, unsigned char pc1, 
		struct register_struct *registers, unsigned char ram[])
{
	switch (instruction) {
		case LDA_D: 
		{
			printf("Loading accumulator (direct)\n");
			registers->accum = ram[ram[pc1]];
		    	break;
	    	}
		case LDA_I: 
		{
			printf("Loading accumulator (indirect)\n");
			registers->accum = 
			    ram[ram[pc1] + registers->index];
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
			ram[ram[pc1] + registers->index] = registers->accum;
			break;
		}
		case STA_M:
		{
			ram[pc1] = registers->accum;
			break;
		}
		case LDX_D: 
		{
			registers->index = ram[ram[pc1]];
		    	break;
	    	}
		case LDX_M: 
		{
			printf("Loading accumulator (immediate)\n");
		    	registers->index = ram[pc1];
			break;
		}
		case INCX:
		{
			printf("Incrementing index register\n");
			printf("%d -> %d\n", registers->index, registers->index+1);
			registers->index++;
			break;
		}
		case DECX:
		{
			printf("Decrementing index register\n");
			printf("%d -> %d\n", registers->index, registers->index-1);
			registers->index--;
			break;
		}
		case JMP:
		{
			printf("Unconditional jump to address %d\n", ram[pc1]);
			/* PC automatically incremented after instruction, so -1 */
			registers->pc = ram[pc1] - 1;
			break;
		}
		case HLT:
		{
			printf("Halting execution\n");
			return 1;
		}
		default: 
		{
			fprintf(stderr, "Execution error: Unknown instruction\n");
			return -1;
		}
	}


	return 0;
}
