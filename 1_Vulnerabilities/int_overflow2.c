#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char **argv){

    int val, i;
    char *mem;

    if (argc < 2)
        exit(1);

    val = atoi(argv[1]);

    printf("Received input\n");

    if (val > 0){

        int new_alloc = val * sizeof(char *);
        printf("Size of char * %ld\n",sizeof(char *));
        printf("Allocation size: %i\n",new_alloc);
        mem = malloc(new_alloc);
        printf("Created char pointer array\n");
        printf("INT_MAX     :   %d\n", INT_MAX);

        if (mem == NULL){
            printf("Failure\n");
            exit(2);

        }
    }

    printf("Outputting values\n");
    for (i=0; i < val; i++){
        mem[i] = 'A';
        printf("%c", mem[i]);
    }

    printf("\n");

    return 0;
}