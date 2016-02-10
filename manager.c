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

int semid;

int main(int argc, char **argv)
{
    int k, i, **fd_p_s, j, mergenow, mergenext, ***mergfd, n, num_to_wait = 0, len, in, out, count, carry, gener = 1;
    char *s, key[3] = "0";
    FILE *f = fopen("psort.log", "w");
    freopen("out.txt", "w", stdout);
    fclose(f);
    setbuf(stdout, NULL);
    semid = semget(IPC_PRIVATE, 1, 0);
    semctl(semid, 0, SETVAL, (int)1);
    k = atoi(argv[1]);// number of processes
    if (argc == 3) // check if inputed key "-n"
        strncpy(key, argv[2], 2);
    mergfd = (int ***)malloc(1 * sizeof(int**)); //two arrays
    mergfd[0] = (int**)malloc(k * sizeof(int*));
    fd_p_s = (int**)malloc(k * sizeof(int*)); // fd from partitioner to sorter
    s = (char*)malloc(1* sizeof(char));
    for (i = 0; i < k; ++i) {
        fd_p_s[i] = (int*)malloc(2 * sizeof(int));
        mergfd[0][i] = (int*)malloc(2 * sizeof(int));
        pipe(fd_p_s[i]);
    }
    if (!fork())
        partitioner(fd_p_s, k);
    for (i = 0; i < k; ++i)
        close(fd_p_s[i][1]);
    for (i = 0; i < k; ++i){
        pipe(mergfd[0][i]);
        if (!fork())
            sorter(i, fd_p_s, k, key, mergfd[0][i][1]); //sort strings and sents it's part to mergfd[0][i][1]
    }
    num_to_wait = k + 1; //partitioner + k sorters
    for (i = 0; i < k; ++i)
        close(fd_p_s[i][0]);
    //start to merge in turn,
    mergenow = k;
    while (mergenow != 1){
        ++gener;
        mergenext = mergenow/2 + mergenow%2;
        mergfd = (int ***)realloc(mergfd, gener * sizeof(int**));
        mergfd[gener - 1] = (int**)malloc(mergenext * sizeof(int*));
        for (i = 0; i < mergenext; ++i){
            mergfd[gener - 1][i] = (int*)malloc(2 * sizeof(int));
            pipe(mergfd[gener - 1][i]);
        }
        count = 0;
        while (count < mergenow) {
            if (count + 1 == mergenow) { // not even number of mergers on this iteration; reached last one without pair
                if (!fork()){
                    merger1(mergfd[gener - 2][count][0], mergfd[gener - 1][count / 2][1], key, count/2, gener);
                    return 0;
                }
            }
            else {
                if (!fork()){
                    merger2(mergfd[gener - 2][count][0], mergfd[gener - 2][count + 1][0], mergfd[gener - 1][count / 2][1], key, count/2, gener);
                    return 0;
                }
            }
            count +=2;
            ++num_to_wait;
        }
        for(i = 0; i < mergenow; ++i)
            close(mergfd[gener - 2][i][0]);
        for(i = 0; i < mergenext; ++i)
            close(mergfd[gener - 1][i][1]);
        mergenow = mergenext;
    }
    my_read(mergfd[gener - 1][0][0], &n, sizeof(int));
    for (i = 0; i < n; ++i){
        my_read(mergfd[gener - 1][0][0], &len, sizeof(len));
        s = (char*)realloc(s, len * sizeof(char));
        my_read(mergfd[gener - 1][0][0], s, len);
        fputs(s, stdout);
        printf("\n");
    }
    for (i = 0; i < num_to_wait; ++i)
        wait(NULL);
    free(s);
    return 0;
}
