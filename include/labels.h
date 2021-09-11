#ifdef LABELS_H
#else
#define LABELS_H

int load_labels(struct cli_struct *cli, struct labels *labels);
int read_labels_line(FILE *fp, int i, struct labels *labels);
void strip_colon(char *line);
int is_label(char *line);
int search_labels(char *str, struct labels *labels);
int store_label(char *name, int addr, struct labels *labels);
int find_next_label_loc(struct labels *labels);
#endif
