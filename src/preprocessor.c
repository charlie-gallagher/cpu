#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "preprocessor.h"


/* Preprocess file and save to a temporary file
 *
 * Strips whitespace, removes empty lines, and removes
 * comment lines and inline comments. This function 
 * itself is responsible for opening and closing the 
 * files. It calls `preprocess_files`, which does the line-
 * by-line transfer. 
 *
 * @param new String, filename to write to
 * @param old String, filename to read from
 *
 * Returns the new file's name, or a NULL pointer on error
 */
char *preprocess(char *new, char *old)
{
	/* Open both files */
	FILE *fp_new;
	FILE *fp_old;

	// Old file
	if ((fp_old = fopen(old, "r")) == NULL) {
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	// New file
	if ((fp_new = fopen(new, "w")) == NULL) {
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	/* Actual line-by-line processing */
	while (preprocess_files(fp_new, fp_old) != 1) {
		;
	}



	fclose(fp_old);
	fclose(fp_new);

	return new;
}


/* Proceeds line-by-line to preprocess files
 *
 * @param fp_new Pointer to new file (write mode)
 * @param fp_old Pointer to old file (read mode)
 *
 * @return Integer, status of the process with 0 for success
 */
int preprocess_files(FILE *fp_new, FILE *fp_old) {
	char line[80], tmp_line[80];

	while (1) {
		fgets(line, 79, fp_old);
		if (feof(fp_old) != 0) {
			printf("EOF reached\n");
			return 1;
		} else {
			strcpy(tmp_line, line);
			stripws(line, tmp_line);
			strip_comment(line);

			if (line[0] == '\0') {
				;
			} else if (line[0] == ';') {
				;
			} else {
				break;
			}
		}
	}

	printf("Writing <%s> to new file\n", line);
	fprintf(fp_new, "%s\n", line);

	return 0;
}



/* Strip whitespace from a line
 *
 * Copies 'from' into 'to' and removes whitespace characters. 
 */
void stripws(char *to , const char *from) 
{
	int i, j;

	for (i = 0, j = 0; i < strlen(from); i++) {
		if (isspace(from[i])) {
			// If the space is surrounded by an apostrophe, add it
			if (from[i - 1] == '\'' && from[i + 1] == '\'') {
				to[j] = from[i];
				j++;
			}
		} else {
			to[j] = from[i];
			j++;
		}
	}

	to[j] = '\0';

	#ifdef DEBUG
	printf("New line after stripping white space: %s\n", to);
	#endif
}


/* Strip comment from a string
 *
 * The comment character is hardcoded as a semi-colon, but 
 * I might make it an argument instead. 
 *
 * Edits a string in place by replacing the first non-quoted
 * semi-colon with a NULL terminator. 
 */
void strip_comment(char *str)
{
	char *pstart_comment;


	if ((pstart_comment = strchr(str, ';')) != NULL) {
		// Check if preceeded by quote
		if (*(pstart_comment - 1) == '\'' && 
				*(pstart_comment + 1) == '\'') {

			#ifdef DEBUG
			printf("Found quoted semi-colon\n");
			#endif 
			// Find next semi-colon if any

			pstart_comment = strchr(pstart_comment + 1, ';');
			if (pstart_comment != NULL) {
				#ifdef DEBUG
				printf("Found comment on semi-colon line\n");
				#endif

				*pstart_comment = '\0';
			}
		} 
		else {
			#ifdef DEBUG
			printf("Comment: %s\n", pstart_comment);
			#endif
			*pstart_comment = '\0';
		}

	}

}
