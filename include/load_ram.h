#ifdef LOAD_RAM
#else
#define LOAD_RAM
int load_ram(unsigned char ram[], struct cli_struct *cli);
int read_assembly_line(unsigned char ram[], int i, FILE *fp);
int parse_line(char *line);


#endif
