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

			set_zero_flag(registers->accum, registers);
		    	break;
	    	}
		case LDA_I: 
		{
			printf("Loading accumulator (indirect)\n");
			registers->accum = 
			    ram[ram[pc1] + registers->index];

			set_zero_flag(registers->accum, registers);
		    	break;
		}
		case LDA_M: 
		{
			printf("Loading accumulator (immediate)\n");
			printf("Memory value: %d\n", ram[pc1]);
		    	registers->accum = ram[pc1];

			set_zero_flag(registers->accum, registers);
			break;
		}
		case STA_D: 
		{
			printf("Storing accumulator (direct)\n");
			ram[ram[pc1]] = registers->accum;

			set_zero_flag(registers->accum, registers);
			break;
		}
		case STA_I: 
		{
			printf("Storing acccumulator (indirect)\n");
			ram[ram[pc1] + registers->index] = registers->accum;
			
			set_zero_flag(registers->accum, registers);
			break;
		}
		case STA_M:
		{
			printf("Storing acccumulator (immediate)\n");
			ram[pc1] = registers->accum;

			set_zero_flag(registers->accum, registers);
			break;
		}
		case LDX_D: 
		{
			printf("Loading index register (direct)\n");
			registers->index = ram[ram[pc1]];

			/* Zero flag */
			set_zero_flag(registers->index, registers);

		    	break;
	    	}
		case LDX_M: 
		{
			printf("Loading index register (immediate)\n");
		    	registers->index = ram[pc1];

			/* Zero flag */
			set_zero_flag(registers->index, registers);

			break;
		}
		case INCX:
		{
			printf("Incrementing index register\n");
			printf("%d -> %d\n", registers->index, registers->index+1);
			registers->index++;

			/* Zero flag */
			set_zero_flag(registers->index, registers);

			break;
		}
		case DECX:
		{
			printf("Decrementing index register\n");
			printf("%d -> %d\n", registers->index, registers->index-1);
			registers->index--;

			set_zero_flag(registers->index, registers);

			break;
		}
		case JMP_D:
		{
			printf("Unconditional jump to address %d\n", ram[ram[pc1]]);
			/* PC automatically incremented after instruction, so -1 */
			registers->pc = ram[ram[pc1]] - 1;
			break;
		}
		case JMP_I:
		{
			printf("Unconditional jump to address %d\n", ram[ram[pc1] + registers->index]);
			/* PC automatically incremented after instruction, so -1 */
			registers->pc = ram[ram[pc1] + registers->index] - 1;
			break;
		}
		case JMP_M:
		{
			printf("Unconditional jump to address %d\n", ram[pc1]);
			/* PC automatically incremented after instruction, so -1 */
			registers->pc = ram[pc1] - 1;
			break;
		}
		case JEQ:
		{
			printf("Conditional jump (zero): ");
			if ((registers->status & STATUS_ZERO_MASK) == 1) {
				printf("Jumping to address %d\n", ram[pc1]);
				registers->pc = ram[pc1] - 1;
			} else {
				printf("Passing without jumping\n");
			}
			break;
		}
		case JNE:
		{
			printf("Conditional jump (not zero): ");
			if ((registers->status & STATUS_ZERO_MASK) != 1) {
				printf("Jumping to address %d\n", ram[pc1]);
				registers->pc = ram[pc1] - 1;
			} else {
				printf("Passing without jumping\n");
			}
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


void set_zero_flag(unsigned char x, struct register_struct *registers) 
{
	/* Zero flag */
	if (x == 0) {
		registers->status = registers->status | STATUS_ZERO_MASK;
	} else {
		registers->status = registers->status & 
			(255 - STATUS_ZERO_MASK);
	}
}
