#include <stdio.h>
#include <stdlib.h>
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
	unsigned char *paddr;  // Absolute address

	switch (instruction) {
		case LDA_D: 
		{
			printf("Loading accumulator (direct)\n");
			paddr = absolute_address(DIR_ADDR, ram, pc1, registers->index);
			registers->accum = *paddr;

			set_zero_flag(registers->accum, registers);
		    	break;
	    	}
		case LDA_I: 
		{
			printf("Loading accumulator (indirect)\n");
			paddr = absolute_address(INDIR_ADDR, ram, pc1, registers->index);
			registers->accum = *paddr; 

			set_zero_flag(registers->accum, registers);
		    	break;
		}
		case LDA_M: 
		{
			printf("Loading accumulator (immediate)\n");
			paddr = absolute_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Memory value: %d\n", *paddr);
		    	registers->accum = *paddr;

			set_zero_flag(registers->accum, registers);
			break;
		}
		case STA_D: 
		{
			printf("Storing accumulator (direct)\n");
			paddr = absolute_address(DIR_ADDR, ram, pc1, registers->index);
			*paddr = registers->accum;

			set_zero_flag(registers->accum, registers);
			break;
		}
		case STA_I: 
		{
			printf("Storing acccumulator (indirect)\n");
			paddr = absolute_address(INDIR_ADDR, ram, pc1, registers->index);
			*paddr = registers->accum;
			
			set_zero_flag(registers->accum, registers);
			break;
		}
		case STA_M:
		{
			printf("Storing acccumulator (immediate)\n");
			paddr = absolute_address(IMMED_ADDR, ram, pc1, registers->index);
			*paddr = registers->accum;

			set_zero_flag(registers->accum, registers);
			break;
		}
		case LDX_D: 
		{
			printf("Loading index register (direct)\n");
			paddr = absolute_address(DIR_ADDR, ram, pc1, registers->index);
			registers->index = *paddr;

			/* Zero flag */
			set_zero_flag(registers->index, registers);

		    	break;
	    	}
		case LDX_M: 
		{
			printf("Loading index register (immediate)\n");
			paddr = absolute_address(IMMED_ADDR, ram, pc1, registers->index);
		    	registers->index = *paddr;

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
			paddr = absolute_address(DIR_ADDR, ram, pc1, registers->index);
			printf("Unconditional jump to address %d\n", *paddr);
			/* PC automatically incremented after instruction, so -1 */
			registers->pc = *paddr - 1;
			break;
		}
		case JMP_I:
		{
			paddr = absolute_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("Unconditional jump to address %d\n", *paddr);
			/* PC automatically incremented after instruction, so -1 */
			registers->pc = *paddr - 1;
			break;
		}
		case JMP_M:
		{
			paddr = absolute_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Unconditional jump to address %d\n", *paddr);
			/* PC automatically incremented after instruction, so -1 */
			registers->pc = *paddr - 1;
			break;
		}
		case JEQ_D:
		{
			paddr = absolute_address(DIR_ADDR, ram, pc1, registers->index);
			printf("Conditional jump (zero): ");
			if ((registers->status & STATUS_ZERO_MASK) == 1) {
				printf("Jumping to address %d\n", *paddr);
				registers->pc = *paddr - 1;
			} else {
				printf("Passing without jumping\n");
			}
			break;
		}
		case JEQ_I:
		{
			paddr = absolute_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("Conditional jump (zero): ");
			if ((registers->status & STATUS_ZERO_MASK) == 1) {
				printf("Jumping to address %d\n", *paddr);
				registers->pc = *paddr - 1;
			} else {
				printf("Passing without jumping\n");
			}
			break;
		}
		case JEQ_M:
		{
			paddr = absolute_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Conditional jump (zero): ");
			if ((registers->status & STATUS_ZERO_MASK) == 1) {
				printf("Jumping to address %d\n", *paddr);
				registers->pc = *paddr - 1;
			} else {
				printf("Passing without jumping\n");
			}
			break;
		}
		case JNE_D:
		{
			paddr = absolute_address(DIR_ADDR, ram, pc1, registers->index);
			printf("Conditional jump (not zero): ");
			if ((registers->status & STATUS_ZERO_MASK) != 1) {
				printf("Jumping to address %d\n", *paddr);
				registers->pc = *paddr - 1;
			} else {
				printf("Passing without jumping\n");
			}
			break;
		}
		case JNE_I:
		{
			paddr = absolute_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("Conditional jump (not zero): ");
			if ((registers->status & STATUS_ZERO_MASK) != 1) {
				printf("Jumping to address %d\n", *paddr);
				registers->pc = *paddr - 1;
			} else {
				printf("Passing without jumping\n");
			}
			break;
		}
		case JNE_M:
		{
			paddr = absolute_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Conditional jump (not zero): ");
			if ((registers->status & STATUS_ZERO_MASK) != 1) {
				printf("Jumping to address %d\n", *paddr);
				registers->pc = *paddr - 1;
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


/* Returns the absolute address of an instruction
 *
 * Inputs a type (a symbolic constant defined in opcodes.h)
 * Returns a pointer to the requested location in the RAM array
 */
unsigned char *absolute_address(int type, unsigned char ram[], unsigned char pc, int index)
{
	int addr;  // Address in RAM

	switch (type) {
		case DIR_ADDR: 
		{
			addr = ram[pc];
			break;
		}
		case INDIR_ADDR:
		{
			addr = ram[pc] + index;
			break;
		}
		case IMMED_ADDR:
		{
			addr = pc;
			break;
		}
		default:
		{
			fprintf(stderr, "Unknown addressing mode\n");
			exit(1);
		}
	}

	printf("Operand location in RAM: %d\n", addr);
	printf("Operand value in RAM: %d\n", ram[addr]);

	return &ram[addr];
}

