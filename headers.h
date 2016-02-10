extern void journal(char *s);
extern int my_read(int fd, void *buf, int sz);
extern int my_write(int fd, void *buf, int sz);
extern int comp(char *a, char *b, char *key);
extern void select_sort(char **array, int n, char *key);
extern void partitioner(int **fd_p_s, int k);
extern void sorter(int i, int **fd_p_s, int k, char *key, int mergfd);
extern void merger2(int in1, int in2, int out,  char *key, int gener, int count);
extern void merger1(int in, int out, char *key, int count, int gener);
