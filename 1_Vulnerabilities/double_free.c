#include<stdlib.h>

int main(){

    char *ptr = (char*) malloc(sizeof(char));

    *ptr = 'a';

    free(ptr);
    free(ptr);
}