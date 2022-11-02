#include<stdlib.h>
#include<string.h>
#include<stdio.h>

void heap_of(const char *x){

    char *y = (char* ) malloc(20);
    strcpy(y,x);

    printf("The input was: %s\n",y);
}

int main(int argc, char **argv){

    heap_of(argv[1]);

    return 0;
}


/*
[afl++ 805609b77f1e] /src/fuzzing_tutorial/1_Vulnerabilities (main) # ./a.out hellohellohellohellohellohe
malloc(): corrupted top size
Aborted
\[afl++ 805609b77f1e] /src/fuzzing_tutorial/1_Vulnerabilities (main) #
*/
