#ifdef LOAD_RAM
#else
#define LOAD_RAM
int load_ram(unsigned char ram[], struct cli_struct *cli, struct labels *labels);
int read_assembly_line(unsigned char ram[], struct labels *labels, int i, FILE *fp);
int parse_line(char *line, struct labels *labels);
int htoi(char *str);

#endif
