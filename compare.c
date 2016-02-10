#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){
    FILE *in1 = fopen("out.txt", "r");
    FILE *in2 = fopen("testout.txt", "r");
    char s1[100], s2[100];
    int n = 0;
    while (fgets(s1, 100, in1) > 0) {
        fgets(s2, 100, in2);
        ++n;
        if (strcmp(s1, s2)) {
            printf("%d", n);
            break;
        }
    }
    printf("ALRIGHT\n");
}
