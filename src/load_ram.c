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
	char contents[RAM_SIZE];
	FILE *fp;
	int i, c;

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
		if (read_assembly_line(contents, i, fp) == 1) {
			printf("Aborting file read early -- not 256 lines\n");
			break;
		}
		printf("Byte: %d\n", contents[i]);
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
int read_assembly_line(char byte[], int i, FILE *fp)
{
	char line[80];
	int c, byte_code;

	fgets(line, 80, fp); 
	/* Strip off newline */
	for (c = 0; c < 80; c++) {
		if (line[c] == '\n') {
			line[c] = '\0';
		}
	}

	printf("Line: %s\n", line);

	byte_code = parse_line(line);
	printf("Byte code conversion: %d\n", byte_code);

	if (byte_code == -1) {
		fprintf(stderr, "Error parsing commands\n");
		exit(1);
	}

	if (feof(fp) != 0) {
		printf("End of file reached\n");
		return 1;
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
	} else if (strcmp(line, "STA_M") == 0) {
		return STA_M;
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
	} else if (strcmp(line, "JEQ") == 0) {
		return JEQ;
	} else if (strcmp(line, "JNE") == 0) {
		return JNE;
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
	} else if (atoi(line) != 0 || line[1] == '0') {
		return atoi(line);
	} else {
		return -1;
	}
}


