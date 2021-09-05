#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "opcodes.h"
#include "load_ram.h"

/* Load ram
 *
 * 	ram		Memory array acting as RAM
 * 	cli		Pointer to command line struct
 */
int load_ram(unsigned char ram[], struct cli_struct *cli)
{
	FILE *fp;
	int i;

	/* Initialize RAM array */
	for (i = 0; i < RAM_SIZE; i++) {
		ram[i] = 0;
	}

	printf("Loading file\n");

	
	/* Open file */
	if ((fp = fopen(cli->input_file, "r")) == NULL) {
		fprintf(stderr, "Error opening file\n");
		return 1;
	}

	/* Copy file contents to array */
	i = 0;
	while (i < RAM_SIZE) {
		printf("READING FILE: line %d\n", i);
		if (read_assembly_line(ram, i, fp) == 1) {
			printf("End of file: read %d lines\n", i);
			break;
		}
		printf("Byte: %d\n", ram[i]);
		i++;
	}


	fclose(fp);
	return 0;
}


/* Read a line and copy it to contents
 *
 * Only takes at most 5 characters from each line
 * Reads a line into the contents array
 */
int read_assembly_line(unsigned char ram[], int i, FILE *fp)
{
	char line[80];
	int c, byte_code;


	while (1) {
		fgets(line, 80, fp);
		if (feof(fp) != 0) {
			printf("End of file reached\n");
			return 1;
		}
		else if (line == NULL) {
			printf("Null pointer found\n");
			return 1;
		}
		else {
			if (line[0] == '\n') {
				printf("Blank line\n");
			} else if (line[0] == ';') {
				printf("Comment line\n");
			} else {
				break;
			}
		}
	}

	/* Strip off newline */
	for (c = 0; c < 80; c++) {
		if (line[c] == '\n') {
			line[c] = '\0';
		} else if (line[c] == '\0') {
			break;
		}
	}

	printf("Line: %s\n", line);

	byte_code = parse_line(line);
	printf("Byte code conversion: %d\n", byte_code);

	if (byte_code == -1) {
		fprintf(stderr, "Error parsing commands\n");
		exit(1);
	} else if (byte_code == -2) {
		printf("Skipping comment\n");
	} else {
		ram[i] = byte_code;
	}



	return 0;
}

int parse_line(char *line)
{
	if (strcmp(line, "LDA_I") == 0) {
		return LDA_I;
	} else if (strcmp(line, "LDA_D") == 0) {
		return LDA_D;
	} else if (strcmp(line, "LDA_M") == 0) {
		return LDA_M;
	} else if (strcmp(line, "STA_D") == 0) {
		return STA_D;
	} else if (strcmp(line, "STA_I") == 0) {
		return STA_I;
	} else if (strcmp(line, "LDX_D") == 0) {
		return LDX_D;
	} else if (strcmp(line, "LDX_M") == 0) {
		return LDX_M;
	} else if (strcmp(line, "INC_D") == 0) {
		return INC_D;
	} else if (strcmp(line, "INC_I") == 0) {
		return INC_I;
	} else if (strcmp(line, "DEC_D") == 0) {
		return DEC_D;
	} else if (strcmp(line, "DEC_I") == 0) {
		return DEC_I;
	} else if (strcmp(line, "CMP_D") == 0) {
		return CMP_D;
	} else if (strcmp(line, "CMP_I") == 0) {
		return CMP_I;
	} else if (strcmp(line, "JEQ_D") == 0) {
		return JEQ_D;
	} else if (strcmp(line, "JEQ_I") == 0) {
		return JEQ_I;
	} else if (strcmp(line, "JEQ_M") == 0) {
		return JEQ_M;
	} else if (strcmp(line, "JNE_D") == 0) {
		return JNE_D;
	} else if (strcmp(line, "JNE_I") == 0) {
		return JNE_I;
	} else if (strcmp(line, "JNE_M") == 0) {
		return JNE_M;
	} else if (strcmp(line, "ADD_D") == 0) {
		return ADD_D;
	} else if (strcmp(line, "ADD_I") == 0) {
		return ADD_I;
	} else if (strcmp(line, "SUB_D") == 0) {
		return SUB_D;
	} else if (strcmp(line, "SUB_I") == 0) {
		return SUB_I;
	} else if (strcmp(line, "AND_D") == 0) {
		return AND_D;
	} else if (strcmp(line, "AND_I") == 0) {
		return AND_I;
	} else if (strcmp(line, "OR_D") == 0) {
		return OR_D;
	} else if (strcmp(line, "OR_I") == 0) {
		return OR_I;
	} else if (strcmp(line, "JMP_D") == 0) {
		return JMP_D;
	} else if (strcmp(line, "JMP_I") == 0) {
		return JMP_I;
	} else if (strcmp(line, "JMP_M") == 0) {
		return JMP_M;
	} else if (strcmp(line, "INCX") == 0) {
		return INCX;
	} else if (strcmp(line, "DECX") == 0) {
		return DECX;
	} else if (strcmp(line, "HLT") == 0) {
		return HLT;
	} else if (strcmp(line, "IO_START") == 0) {
		return IO_START;
	} else if (line[0] == '0' || atoi(line) != 0) {
		return atoi(line);
	} else if (line[0] == ';') {
		return -2;
	} else if (line[0] == '\0') {
		return 0;
	} else {
		return -1;
	}
}


