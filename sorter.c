#define _XOPEN_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include "headers.h"

void sorter(int i, int **fd_p_s, int k, char *key, int mergfd)
{
    char *jour;
    jour = (char*)malloc(100 * sizeof(char));
    sprintf(jour, "Start of work of sorter #%d ", i);
    time_t timer = time(NULL);
    strcat(jour, ctime(&timer));
    journal(jour);
    int len, c, j, strnum = 0;
    char **sorter_s;
    setbuf(stdout, NULL);
    sorter_s = (char **)malloc(1 * sizeof(char*));
    while(my_read(fd_p_s[i][0], &len, sizeof(int)) > 0){
        ++strnum;
        sorter_s = (char **)realloc(sorter_s, strnum * sizeof(char*));
        sorter_s[strnum - 1] = (char*)malloc(len * sizeof(char));
        my_read(fd_p_s[i][0], sorter_s[strnum - 1], len * sizeof(char));
    }
    select_sort(sorter_s, strnum, key);
    my_write(mergfd, &strnum, sizeof(int));
    for (j = 0; j < strnum; ++j){
        len = strlen(sorter_s[j]) + 1;
        my_write(mergfd, &len, sizeof(int));
        my_write(mergfd, sorter_s[j], len* sizeof(char));
    }
    for (j = 0; j < strnum; ++j)
        free(sorter_s[j]);
    free(sorter_s);
    timer = time(NULL);
    sprintf(jour, "End of work of sorter #%d ", i);
    strcat(jour, ctime(&timer));
    journal(jour);
    free(jour);
    exit(0);
}
