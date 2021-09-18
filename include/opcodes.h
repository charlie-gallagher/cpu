#ifdef OPCODES
/* Do nothing */

#else
#define OPCODES

int execute_instruction(unsigned char instruction, unsigned char pc1, 
		struct register_struct *registers, unsigned char ram[]);
void set_zero_flag(unsigned char x, unsigned char *pstatus);
void set_negative_flag(unsigned char x, unsigned char *pstatus);
void set_carry_flag(int result, unsigned char *pstatus);
void set_status_flag(int set, int mask, unsigned char *pstatus);
unsigned char *operand_address(int type, unsigned char ram[], unsigned char pc, int index);

#define STA_D 1
#define STA_I 2
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
#define BEQ 17
#define BNE 18
#define ADC_D 19
#define ADC_I 20
#define ADC_M 36
#define SBC_M 37
#define SBC_D 21
#define SBC_I 22
#define AND_D 23
#define AND_I 24
#define OR_D 25
#define OR_I 26
#define JMP 27
#define INCX 30
#define DECX 31
#define AND_M 38
#define OR_M 39
#define TXS 40
#define TXA 41
#define TAX 42
#define TSX 43
#define PHA 44
#define PHP 45
#define PLA 46
#define PLP 47
#define JSR 48
#define RTS 49
#define ASL 50
#define LSR 51
#define ROL 52
#define ROR 53
#define BMI 54
#define BPL 55
#define SEC 56
#define CLC 57
#define BCS 58
#define BCC 59
#define HLT 0



// Address modes
#define DIR_ADDR 100
#define INDIR_ADDR 101
#define IMMED_ADDR 102

// Status register
#define STATUS_REGISTER &(registers->status)
#define STATUS_ZERO_MASK 1
#define STATUS_NEG_MASK 2
#define STATUS_CARRY_MASK 4
#define STATUS_OVERFLOW_MASK 8

#endif
