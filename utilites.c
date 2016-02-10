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

extern int semid;

void journal(char *s)
{
    FILE *f = fopen("psort.log", "a");
    setbuf(f, NULL);
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
    fprintf(f, "%s", s);
    fclose(f);
    op.sem_op = 1;
    semop(semid, &op, 1);
}

int my_read(int fd, void *buf, int sz)
{
    int cur = 0, t;
    while (cur < sz) {
        t = read(fd, (char *) buf + cur, sz - cur);
        if (t <= 0) {
            return cur;
        }
        cur += t;
    }
    return cur;
}

int my_write(int fd, void *buf, int sz)
{
    int cur = 0, t;
    while (cur < sz) {
        t = write(fd, (char *) buf + cur, sz - cur);
        if (t <= 0) {
            return cur;
        }
        cur += t;
    }
    return cur;
}

int comp(char *a, char *b, char *key)
{
    if (!strcmp(key, "-n"))
        return atoi(a) - atoi(b);
    else if (!strcmp(key, "-r"))
        return strcmp(b, a);
    else
        return strcmp(a, b);
}

void select_sort(char **array, int n, char *key)
{
    int i, min_i, j;
    char *carry;
    for (i = 0; i < n - 1; ++i){
        min_i = i;
        for (j = i + 1; j < n; ++j){
            if (comp(array[j], array[min_i], key) < 0)
            min_i = j;
        }
        carry = array[i];
        array[i] = array[min_i];
        array[min_i] = carry;
    }
}

