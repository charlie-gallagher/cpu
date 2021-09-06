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
	signed char rel_addr = 0; // Relative value for conditional branching

	switch (instruction) {
		case LDA_D: 
		{
			printf("Loading accumulator (direct)\n");
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			registers->accum = *paddr;

			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
		    	break;
	    	}
		case LDA_I: 
		{
			printf("Loading accumulator (indirect)\n");
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			registers->accum = *paddr; 

			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
		    	break;
		}
		case LDA_M: 
		{
			printf("Loading accumulator (immediate)\n");
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Memory value: %d\n", *paddr);
		    	registers->accum = *paddr;

			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			break;
		}
		case STA_D: 
		{
			printf("Storing accumulator (direct)\n");
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			*paddr = registers->accum;

			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			break;
		}
		case STA_I: 
		{
			printf("Storing acccumulator (indirect)\n");
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			*paddr = registers->accum;
			
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			break;
		}
		case LDX_D: 
		{
			printf("Loading index register (direct)\n");
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			registers->index = *paddr;

			/* Zero flag */
			set_zero_flag(registers->index, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

		    	break;
	    	}
		case LDX_M: 
		{
			printf("Loading index register (immediate)\n");
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
		    	registers->index = *paddr;

			/* Zero flag */
			set_zero_flag(registers->index, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case INCX:
		{
			printf("Incrementing index register\n");
			printf("%d -> %d\n", registers->index, registers->index+1);
			registers->index++;

			/* Zero flag */
			set_zero_flag(registers->index, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case DECX:
		{
			printf("Decrementing index register\n");
			printf("%d -> %d\n", registers->index, registers->index-1);
			registers->index--;

			set_zero_flag(registers->index, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case JMP:
		{
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Unconditional jump to address %d\n", *paddr);
			/* PC automatically incremented after instruction, so -1 */
			registers->pc = *paddr - 1;
			break;
		}
		case BEQ:
		{
			/* All conditional branching instructions uses the second 
			 * byte as the number of locations to move. To the PC, the
			 * operand is ADDED. Thus, the value is made into a signed
			 * value.
			 */
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Conditional jump (zero): ");

			// Unsigned converted to signed
			rel_addr = (signed char) *paddr;

			if ((registers->status & STATUS_ZERO_MASK) == 1) {
				printf("Jumping to address %d\n", pc1 + rel_addr);
				registers->pc += rel_addr;
			} else {
				printf("Passing without jumping\n");
			}
			break;
		}
		case BNE:
		{
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Conditional jump (not zero): ");

			// Unsigned converted to signed
			rel_addr = (signed char) *paddr;

			// Address correction
			if (rel_addr < 0) {
				rel_addr -= 2;
			} else {
				rel_addr--;
			}

			if ((registers->status & STATUS_ZERO_MASK) != 1) {
				printf("Jumping to address %d\n", pc1 + rel_addr);
				registers->pc += rel_addr;
			} else {
				printf("Passing without jumping\n");
			}
			break;
		}
		case INC_D:
		{
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			printf("Incrementing value (direct)\n");
			(*paddr)++;

			/* Zero flag */
			set_zero_flag(*paddr, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			break;
		}
		case INC_I:
		{
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("Incrementing value (indirect)\n");
			(*paddr)++;

			/* Zero flag */
			set_zero_flag(*paddr, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			break;
		}
		case DEC_D:
		{
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			printf("Decrementing value (direct)\n");
			(*paddr)--;

			/* Zero flag */
			set_zero_flag(*paddr, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			break;
		}
		case DEC_I:
		{
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("Decrementing value (indirect)\n");
			(*paddr)--;

			/* Zero flag */
			set_zero_flag(*paddr, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			break;
		}
		case CMP_D:
		{
			/* The CMP instruction subtracts the value of the operand
			 * from the accumulator  (accum - *addr). If the value is
			 * zero, the zero flag is set. If it's greater than, 
			 * nothing is set. If it's less than, the negative flag
			 * is set.
			 */
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			printf("Comparing value to accumulator (direct)\n");
			if (registers->accum == *paddr) {
				set_status_flag(1, STATUS_ZERO_MASK, STATUS_REGISTER);
				set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			} else if (registers->accum < *paddr) {
				set_status_flag(0, STATUS_ZERO_MASK, STATUS_REGISTER);
				set_status_flag(1, STATUS_NEG_MASK, STATUS_REGISTER);
			} else {
				set_status_flag(0, STATUS_ZERO_MASK, STATUS_REGISTER);
				set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			}

			break;

		}
		case CMP_I:
		{
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("Comparing value to accumulator (indirect)\n");
			if (registers->accum == *paddr) {
				set_status_flag(1, STATUS_ZERO_MASK, STATUS_REGISTER);
				set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			} else if (registers->accum < *paddr) {
				set_status_flag(0, STATUS_ZERO_MASK, STATUS_REGISTER);
				set_status_flag(1, STATUS_NEG_MASK, STATUS_REGISTER);
			} else {
				set_status_flag(0, STATUS_ZERO_MASK, STATUS_REGISTER);
				set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);
			}
			
			break;
		}
		case ADD_D:
		{
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			printf("Adding %d to accumulator\n", *paddr);
			registers->accum += *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case ADD_I:
		{
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("Adding %d to accumulator\n", *paddr);
			registers->accum += *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case ADD_M:
		{
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Adding %d to accumulator\n", *paddr);
			registers->accum += *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case SUB_D:
		{
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			printf("Substracting %d from accumulator\n", *paddr);
			registers->accum -= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case SUB_I:
		{
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("Subtracting %d from accumulator\n", *paddr);
			registers->accum -= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case SUB_M:
		{
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("Subtracting %d from accumulator\n", *paddr);
			registers->accum -= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case AND_D:
		{
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			printf("ANDing with accumulator (direct)\n");
			registers->accum &= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case AND_I:
		{
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("ANDing with accumulator (indirect)\n");
			registers->accum &= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case AND_M:
		{
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("ANDing with accumulator (immediate)\n");
			registers->accum &= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case OR_D:
		{
			paddr = operand_address(DIR_ADDR, ram, pc1, registers->index);
			printf("ORing with accumulator (direct)\n");
			registers->accum |= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case OR_I:
		{
			paddr = operand_address(INDIR_ADDR, ram, pc1, registers->index);
			printf("ORing with accumulator (indirect)\n");
			registers->accum |= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

			break;
		}
		case OR_M:
		{
			paddr = operand_address(IMMED_ADDR, ram, pc1, registers->index);
			printf("ORing with accumulator (immediate)\n");
			registers->accum |= *paddr;

			/* Status flags */
			set_zero_flag(registers->accum, STATUS_REGISTER);
			set_status_flag(0, STATUS_NEG_MASK, STATUS_REGISTER);

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


/* Automatically sets or clear the zero register based on the first
 * argument. Takes, the argument and a pointer to the status register.
 */
void set_zero_flag(unsigned char x, unsigned char *pstatus) 
{
	/* Zero flag */
	if (x == 0) {
		printf("Setting zero flag\n");
		set_status_flag(1, STATUS_ZERO_MASK, pstatus);
	} else {
		printf("Clearing zero flag\n");
		set_status_flag(0, STATUS_ZERO_MASK, pstatus);
	}
}


/* Set or clear a status flag
 *
 * 	set	Logical indicating whether to set the flag (1) or 
 * 		clear it (0)
 * 	mask	Mask used to set or clear the bit. I've defined the
 * 		masks in opcodes.h
 * 	status	Pointer to the status register
 */
void set_status_flag(int set, int mask, unsigned char *pstatus)
{
	if (set) {
		*pstatus |= mask;
	} else {
		*pstatus &= 255 - mask;
	}
}




/* Returns the absolute address of an instruction's operand
 *
 * 	type		Symbolic constant defined in opcodes.h
 * 	ram		Ram array
 * 	pc		Program counter
 * 	index		Value of the index register
 *
 * Inputs a type (a symbolic constant defined in opcodes.h) and returns a
 * pointer to the requested location in the RAM array. Assign the return 
 * value to a pointer and use that to assign values to ram and registers. 
 */
unsigned char *operand_address(int type, unsigned char ram[], unsigned char pc, int index)
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

