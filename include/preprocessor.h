#ifdef PREPROCESSOR_H
#else
#define PREPROCESSOR_H

void stripws(char *to , const char *from);
void strip_comment(char *str);
char *preprocess(char *new, char *old);
int preprocess_files(FILE *fp_new, FILE *fp_old);


#endif
