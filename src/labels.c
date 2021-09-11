#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "main.h"
#include "load_ram.h"
#include "labels.h"



/* Pulls labels from file in CLI struct
 *
 * Reads the input file and makes a list of labels from 
 * it. Returns a status of 0 (success) or 1 (failure). 
 *
 * The trick here is to properly increment the RAM address
 * counter so that I get the right addresses. 
 */
int load_labels(struct cli_struct *cli, struct labels *labels)
{
	FILE *fp;
	int i;
	int label_status = 0;


	printf("Loading file\n");

	
	/* Open file */
	if ((fp = fopen(cli->input_file, "r")) == NULL) {
		fprintf(stderr, "Error opening file\n");
		return 1;
	}

	/* Find labels and process them */
	i = 0;
	while (i < RAM_SIZE) {
		printf("READING FILE: line %d\n", i);

		label_status = read_labels_line(labels, i, fp);

		if (label_status == 1) {
			printf("End of file: read %d bytes\n", i);
			break;
		} else if (label_status == 2) {
			printf("Previous line was a label, not incrementing i\n");
		} else {
			i++;
		}

	}


	fclose(fp);
	return 0;
}


/* Read next line
 *
 * Reads a line from the input file and optionally copies it
 * to the labels structure. 
 *
 * I'm working this out on the fly, so this might not work out
 * great, but I think a good method is just to read one line at
 * a time until EOF. This will be a simplified version of 
 * load_ram that focuses mostly on: 
 *
 *   (a) Counting memory locations
 *   (b) Removing whitespace and comments
 *   (c) Storing labels with the correct RAM address
 *
 * Returns 1 for EOF, 2 if line contains a label, otherwise 0
 */
int read_labels_line(struct labels *labels, int i, FILE *fp)
{
	char line[80];
	char tmp_line[80];
	int lab_num;

	while (1) {
		fgets(line, 79, fp);
		if (feof(fp) != 0) {
			printf("End of file reached\n");
			return 1;
		} else {
			printf("line: %s\n", line);

			strcpy(tmp_line, line);
			stripws(line, tmp_line);

			if (line[0] == '\0') {
				printf("blank line\n");
			} else if (line[0] == ';') {
				printf("Comment line\n");
			} else {
				break;
			}
		}
	}

	strip_comment(line);

	if (is_label(line)) {
		printf("Found label: %s\n", line);
		strip_colon(line);
		lab_num = store_label(line, i, labels);
	
		if (lab_num == -1) {
			fprintf(stderr, "Error: Unable to store label\n");
			exit(1);
		}

		return 2;
	}

	return 0;
}



/* Search labels for match
 *
 * Searches all labels for a match. 
 *
 * Returns the address of the match, or -1 if no 
 * match was found. 
 */
int search_labels(char *str, struct labels *labels)
{
	int i;

	for (i = 0; i < 20; i++) {
		if (strcmp(str, labels->name[i]) == 0) {
			return labels->addr[i];
		} else {
			continue;
		}
	}

	return -1;
}


/* Store label in labels struct
 *
 * Returns the index (from 0 to 19) of the label in the
 * labels structure. If no place was found, returns -1. 
 */
int store_label(char *name, int addr, struct labels *labels)
{
	int next = find_next_label_loc(labels);

	if (next != -1) {
		strcpy(labels->name[next], name);
		labels->addr[next] = addr;
		printf("Storing label '%s' for address %d in position %d\n", name, addr, next);
	}

	return next;
}



/* Find next available location in labels struct
 *
 * Handles the placement of the labels in the label
 * structure. 
 *
 * Returns the index within the label name array if
 * space was found, otherwise -1. 
 */
int find_next_label_loc(struct labels *labels)
{
	int i = 0;

	while (labels->name[i][1] != '\0' && i < 20) {
		i++;
	}

	if (i >= 20) {
		fprintf(stderr, "Error: all labels already used\n");
		return -1;
	}

	return i;
}


void strip_colon(char *line)
{
	int i = 0;

	while (line[i] != '\0') {
		if (line[i] == ':') {
			line[i] = '\0';
			break;
		}
		i++;
	}
}


/* A label will be defined as at least two characters followed
 * by a colon and (after cleaning whitespace and comments) the
 * end of string NULL terminator. 
 */
int is_label(char *line)
{
	// nchar is number of chars already read
	int i, nchar;
	i = nchar = 0;

	for (i = 0; i < 80; i++) {
		if (isalpha(line[i])) {
			nchar++;
		} else if (isdigit(line[i])) {
			// Digits not allowed in label names
			return 0;
		} else if (line[i] == ':') {
			if (nchar >= 2) {
				if (line[i + 1] == '\0') {
					return nchar;
				} else {
					fprintf(stderr, "Malformed label name: %s\n", line);
					return 0;
				}
			} else {
				fprintf(stderr, 
					"Warning: too few characters in label name\n");
			}
		} else if (line[i] == '\0') {
			return 0;
		} else {
			// What other cases are there... symbols, I suppose
			printf("is_label: Other case (%c)\n", line[i]);
			return 0;
		}
	}

	return 0;
}


void print_labels(struct labels *labels)
{
	int i;

	for (i = 0; i < 20; i++)
	{
		printf("%d: %s (%d)\n", i, labels->name[i], labels->addr[i]);
	}
}
