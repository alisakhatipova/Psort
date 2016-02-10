#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){
    FILE *in = fopen("warandpeace.txt", "r");
    FILE *out = fopen("testout.txt", "w");
    char s[70000][100];
    int n = 0, i;
    while (fgets(s[n], 100, in) > 0)
        ++n;
   qsort(s, n, 100, (int (*)(const void *, const void *))strcmp);
   for(i = 0; i < n; ++i)
     fputs(s[i], out);
}
