#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
 */
int read_assembly_line(unsigned char ram[], int i, FILE *fp)
{
	char line[80];
	char tmp_line[80];
	int byte_code;


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


	// Replace first ';' in line with null terminator
	strip_comment(line);

	// Remove whitespace
	strcpy(tmp_line, line);  // Store line in temporary spot
	stripws(line, tmp_line); // Strip whitespace in line

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
	} else if (strcmp(line, "BEQ") == 0) {
		return BEQ;
	} else if (strcmp(line, "BNE") == 0) {
		return BNE;
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
	} else if (strcmp(line, "JMP") == 0) {
		return JMP;
	} else if (strcmp(line, "INCX") == 0) {
		return INCX;
	} else if (strcmp(line, "DECX") == 0) {
		return DECX;
	} else if (strcmp(line, "TAX") == 0) {
		return TAX;
	} else if (strcmp(line, "TXA") == 0) {
		return TXA;
	} else if (strcmp(line, "TSX") == 0) {
		return TSX;
	} else if (strcmp(line, "TXS") == 0) {
		return TXS;
	} else if (strcmp(line, "PHA") == 0) {
		return PHA;
	} else if (strcmp(line, "PHP") == 0) {
		return PHP;
	} else if (strcmp(line, "PLA") == 0) {
		return PLA;
	} else if (strcmp(line, "PLP") == 0) {
		return PLP;
	} else if (strcmp(line, "JSR") == 0) {
		return JSR;
	} else if (strcmp(line, "RTS") == 0) {
		return RTS;
	} else if (strcmp(line, "HLT") == 0) {
		return HLT;
	} else if (strcmp(line, "IO_START") == 0) {
		return IO_START;
	} else if (htoi(line) != -1) {
		return htoi(line);
	} else if (line[0] == '0' || atoi(line) != 0) {
		return atoi(line);
	} else if (line[0] == '\'' && line[2] == '\'') {
		return line[1];
	} else if (line[0] == ';') {
		return -2;
	} else if (line[0] == '\0') {
		return 0;
	} else {
		return -1;
	}
}


/* Converts hex string to integer
 *
 * If the string is longer than 1 byte (2 chars), returns -1
 * Otherwise returns the value of the string as an integer.
 */
int htoi(char *str)
{
	int i, c, rad;
	int value = 0;

	if (strlen(str) != 3) {
		return -1;
	}



	for (i = 0; i < strlen(str); i++) {
		c = tolower(str[i]);

		// Only two characters are allowed, so no need to load math.
		if (i == 0) rad = 16;
		else if (i == 1) rad = 1;

		if (c <= 'f' && c >= 'a') {
			value += rad * (c - 'a' + 10);
		} else if (c <= '9' && c >= '0') {
			value += rad * (c - '0');
		} else if (i == 2 && c == 'h') {
			// ignore terminal h
			;
		} else {
			// Non-hex character
			return -1;
		}
	}

	return value;
}

/* Copies 'from' into 'to' and removes whitespace
 */
void stripws(char *to , const char *from) 
{
	int i, j;

	for (i = 0, j = 0; i < strlen(from); i++) {
		if (from[i] == ' ' || from[i] == '\n' || from[i] == '\t') {
			// If the space is preceded by an apostrophe, add it
			if (from[i - 1] == '\'') {
				to[j] = from[i];
				j++;
			}
		} else {
			to[j] = from[i];
			j++;
		}
	}

	to[j] = '\0';
	printf("New line: %s\n", to);
}


/* Replaces first semi-colon with null string terminator
 */
void strip_comment(char *str)
{
	char *pstart_comment;


	if ((pstart_comment = strchr(str, ';')) != NULL) {
		printf("Comment: %s\n", pstart_comment);
		*pstart_comment = '\0';
	}

}
