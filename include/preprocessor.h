#ifdef PREPROCESSOR_H
#else

void stripws(char *to , const char *from);
void strip_comment(char *str);
FILE *preprocess(FILE *new, FILE *old);


#endif
