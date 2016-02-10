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

void partitioner(int **fd_p_s, int k)
{
    char *jour;
    jour = (char*)malloc(100 * sizeof(char));
    strcpy(jour, "Start of partitioner's work ");
    time_t timer = time(NULL);
    strcat(jour, ctime(&timer));
    journal(jour);
    char buf[1002];
    int f = open("warandpeace.txt", O_RDONLY);
    int i = 0, c, len = 1, numread, j;
    char *s;
    s = (char *)malloc(len * sizeof(char));
    while ((numread = my_read(f, buf, 1000 * sizeof(char)))> 0){
        for (j = 0; j < numread; ++j){
            if (buf[j] == '\n') {
                s[len - 1] = 0;
                my_write(fd_p_s[i][1], &len, sizeof(int));
                my_write(fd_p_s[i][1], s, len);
                len = 1;
                i = (i + 1) % k;
            }
            else {
                ++len;
                s = (char *)realloc(s, len * sizeof(char));
                s[len - 2] = buf[j];
            }
        }
    }
    close(f);
    free(s);
    timer = time(NULL);
    strcpy(jour, "End of partitioner's work ");
    strcat(jour, ctime(&timer));
    journal(jour);
    free(jour);
    exit(0);
}
