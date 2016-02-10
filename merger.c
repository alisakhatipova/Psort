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

void merger2(int in1, int in2, int out,  char *key, int gener, int count)
{
    char *jour;
    jour = (char*)malloc(100 * sizeof(char));
    sprintf(jour, "Start of work of merger #%d generation #%d ", count, gener);
    time_t timer = time(NULL);
    strcat(jour, ctime(&timer));
    journal(jour);
    int n1, n2, len1, len2, sumn, exist1 = 0, exist2 = 0;
    char *s1, *s2;
    s1 = (char *)malloc(1 * sizeof(char));
    s2 = (char *)malloc(1 * sizeof(char));
    my_read(in1, &n1, sizeof(int));
    my_read(in2, &n2, sizeof(int));
    sumn = n1 + n2; //summary number of strings from two inputs
    my_write(out, &sumn, sizeof(int));
    while ((n1 != 0) || (n2 != 0) || (exist1) || (exist2)) {
        if ((!exist1) && (n1 != 0)){
            my_read(in1, &len1, sizeof(int));
            s1 = realloc(s1,  len1 * sizeof(char));
            my_read(in1, s1, len1 * sizeof(char));
            n1--;
            exist1++;
        }
        if ((!exist2) && (n2 != 0)){
            my_read(in2, &len2, sizeof(int));
            s2 = realloc(s2,  len2 * sizeof(char));
            my_read(in2, s2, len2 * sizeof(char));
            n2--;
            exist2++;
        }
        if (exist1 && !exist2){
            my_write(out, &len1, sizeof(int));
            my_write(out, s1, len1 * sizeof(char));
            exist1--;
        }
        else if (exist2 && !exist1){
            my_write(out, &len2, sizeof(int));
            my_write(out, s2, len2 * sizeof(char));
            exist2--;
        }
        else if (exist1 && exist2){
            if (comp(s1, s2, key) < 0) {
                my_write(out, &len1, sizeof(int));
                my_write(out, s1, len1 * sizeof(char));
                exist1--;
            }
            else {
                my_write(out, &len2, sizeof(int));
                my_write(out, s2, len2 * sizeof(char));
                exist2--;
            }
        }
    }
    free(s1);
    free(s2);
    close(out);
    close(in1);
    close(in2);
    timer = time(NULL);
    sprintf(jour, "End of work of merger #%d generation #%d ", count, gener);
    strcat(jour, ctime(&timer));
    journal(jour);
    free(jour);
    exit(0);
}

void merger1(int in, int out, char *key, int count, int gener){
    char *jour;
    jour = (char*)malloc(100 * sizeof(char));
    sprintf(jour, "Start of work of merger #%d generation #%d ", count, gener);
    time_t timer = time(NULL);
    strcat(jour, ctime(&timer));
    journal(jour);
    int n, i, len;
    char *s;
    s = (char *)malloc(1 * sizeof(char));
    my_read(in, &n, sizeof(int));
    my_write(out, &n, sizeof(int));
    for (i = 0; i < n; ++i){
        my_read(in, &len, sizeof(int));
        s = realloc(s,  len * sizeof(char));
        my_read(in, s, len * sizeof(char));
        my_write(out, &len, sizeof(int));
        my_write(out, s, len * sizeof(char));
    }
    free(s);
    close(out);
    close(in);
    timer = time(NULL);
    sprintf(jour, "End of work of merger #%d generation #%d ", count, gener);
    strcat(jour, ctime(&timer));
    journal(jour);
    free(jour);
    exit(0);
}
