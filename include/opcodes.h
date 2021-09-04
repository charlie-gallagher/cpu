#ifdef OPCODES
/* Do nothing */

#else
#define OPCODES

int execute_instruction(unsigned char instruction, unsigned char pc1, 
		struct register_struct *registers, unsigned char ram[]);
void set_zero_flag(unsigned char x, struct register_struct *registers);
unsigned char *absolute_address(int type, unsigned char ram[], unsigned char pc, int index);

#define STA_D 1
#define STA_I 2
#define STA_M 3
#define LDA_D 4
#define LDA_I 5
#define LDA_M 6
#define LDX_D 9
#define LDX_M 10
#define INC_D 11
#define INC_I 12
#define DEC_D 13
#define DEC_I 14
#define CMP_D 15
#define CMP_I 16
#define JEQ_D 17
#define JNE_D 18
#define ADD_D 19
#define ADD_I 20
#define SUB_D 21
#define SUB_I 22
#define AND_D 23
#define AND_I 24
#define OR_D 25
#define OR_I 26
#define JMP_D 27
#define JMP_I 28
#define JMP_M 29
#define INCX 30
#define DECX 31
#define JEQ_I 32
#define JEQ_M 33
#define JNE_I 34
#define JNE_M 35
#define HLT 0


// Address modes
#define DIR_ADDR 100
#define INDIR_ADDR 101
#define IMMED_ADDR 102

// Status register
#define STATUS_ZERO_MASK 1

#endif
